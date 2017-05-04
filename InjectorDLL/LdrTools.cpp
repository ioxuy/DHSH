#include "stdafx.h"
#include "LdrHeader.h"
#include <stdlib.h>

PTEB_ACTIVE_FRAME FindThreadFrameByContext(ULONG_PTR Context)
{
	PTEB_ACTIVE_FRAME Frame;

	Frame = pRtlGetFrame();
	while (Frame != NULL && Frame->Context != Context)
		Frame = Frame->Previous;

	return Frame;
}

LOAD_MEM_DLL_INFO* GetLoadMemDllInfo()
{
	return (LOAD_MEM_DLL_INFO *)FindThreadFrameByContext(LOAD_MEM_DLL_INFO_MAGIC);
}




VOID XorKey(PVOID Data, PVOID Key, INT KeySize, INT DataSize)
{
	PBYTE pKey = (PBYTE)Key;
	PBYTE lpData = (PBYTE)Data;
	for (int i = 0; i < DataSize; i++)
	{
		lpData[i] = lpData[i] ^ i^pKey[i%KeySize];
	}
}
BOOL Ldr2PeFile(PVOID inBuffer, PVOID *outBuffer, UINT inSize, UINT *outSize)
{
	PVOID pBuffer = NULL;
	int nRealSize = 0;
	PLDR_FILE pLdr = NULL;
	pLdr = (PLDR_FILE)inBuffer;
	if (pLdr->HeadTag == LDR_TAG)
	{
		nRealSize = pLdr->DosHeaderSize + pLdr->OtherDataSize + pLdr->PeHeaderSize;
		pBuffer = malloc(nRealSize);
		if (pBuffer)
		{
			//////////////////////////////////////////////////////////////////////////
			//开始了！！
			//////////////////////////////////////////////////////////////////////////
			XorKey((PVOID)((ULONG_PTR)inBuffer + sizeof(LDR_FILE)), pLdr->Key, 8, inSize - sizeof(LDR_FILE));

			RtlCopyMemory(pBuffer, (PVOID)((ULONG_PTR)inBuffer + pLdr->DosHeaderOffset), pLdr->DosHeaderSize);
			RtlCopyMemory((PVOID)((ULONG_PTR)pBuffer + pLdr->DosHeaderSize), (PVOID)((ULONG_PTR)inBuffer + pLdr->PeHeaderOffset), pLdr->PeHeaderSize);
			RtlCopyMemory((PVOID)((ULONG_PTR)pBuffer + pLdr->DosHeaderSize + pLdr->PeHeaderSize), (PVOID)((ULONG_PTR)inBuffer + pLdr->OtherDataOffset), pLdr->OtherDataSize);

			*outSize = nRealSize;
			*outBuffer = pBuffer;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL Pe2LdrFile(PVOID inBuffer, PVOID *outBuffer, UINT inSize, UINT *outSize)
{
	//////////////////////////////////////////////////////////////////////////
	//计算出OutSize
	//////////////////////////////////////////////////////////////////////////
	//先扩大inSize

	int n = 0;
	int nRealSize = 0;
	PLDR_FILE pLdr = NULL;
	PVOID pBuffer = NULL;
	PIMAGE_DOS_HEADER dosheader;
	PIMAGE_NT_HEADERS ntheader;

	srand(GetTickCount());
	n = rand() % 3 + 1;
	nRealSize = inSize + sizeof(LDR_FILE);
	pBuffer = malloc(nRealSize);
	if (pBuffer)
	{
		//////////////////////////////////////////////////////////////////////////
		//获得key和填充各种码
		//////////////////////////////////////////////////////////////////////////
		pLdr = (PLDR_FILE)pBuffer;

		RtlZeroMemory(pBuffer, nRealSize);

		pLdr->HeadTag = LDR_TAG;

		dosheader = (PIMAGE_DOS_HEADER)inBuffer;
		ntheader = (PIMAGE_NT_HEADERS)((ULONG_PTR)inBuffer + dosheader->e_lfanew);

		pLdr->DosHeaderSize = dosheader->e_lfanew;
		pLdr->PeHeaderSize = ntheader->FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_NT_HEADERS32) - sizeof(IMAGE_OPTIONAL_HEADER32);
		pLdr->OtherDataSize = inSize - pLdr->DosHeaderSize - pLdr->PeHeaderSize;
		if (ntheader->FileHeader.Characteristics&IMAGE_FILE_DLL)
		{
			pLdr->LdrFlags = LDR_FLG_DLLFILE;
		}
		else
		{
			pLdr->LdrFlags = LDR_FLG_EXEFILE;
		}
		//计算一下Offset 把DOSHEADER放在文件尾
		//LDR_FILE
		//OTHERDATA
		//垃圾数据
		//PEHEADER
		//DOSHEADER
		pLdr->DosHeaderOffset = nRealSize - pLdr->DosHeaderSize;
		pLdr->PeHeaderOffset = nRealSize - pLdr->DosHeaderSize - pLdr->PeHeaderSize;
		pLdr->OtherDataOffset = sizeof(LDR_FILE);

		//按部就班的复制数据

		RtlCopyMemory((PVOID)((ULONG_PTR)pBuffer + pLdr->DosHeaderOffset), dosheader, pLdr->DosHeaderSize);

		RtlCopyMemory((PVOID)((ULONG_PTR)pBuffer + pLdr->PeHeaderOffset), ntheader, pLdr->PeHeaderSize);

		RtlCopyMemory((PVOID)((ULONG_PTR)pBuffer + pLdr->OtherDataOffset), (PVOID)((ULONG_PTR)inBuffer + pLdr->DosHeaderSize + pLdr->PeHeaderSize), pLdr->OtherDataSize);

		//数据复制ok，生成Key鸟
		for (int i = 0; i < 8; i++)
		{
			pLdr->Key[i] = rand() % 0xFF;
		}
		XorKey((PVOID)((ULONG_PTR)pBuffer + sizeof(LDR_FILE)), pLdr->Key, 8, nRealSize - sizeof(LDR_FILE));
		*outSize = nRealSize;
		*outBuffer = pBuffer;
		return TRUE;
	}
	return FALSE;
}

BOOL ReadInFileW(LPCWSTR lpszFile, PVOID *outBuffer, UINT *outFileSize)
{
	PVOID m_FileData;
	UINT m_FileSize;
	BOOL bRet = FALSE;
	HANDLE hFile = CreateFileW(lpszFile, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//把文件读出来
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		DWORD dwRet = 0;
		m_FileData = (PVOID)new BYTE[dwFileSize];
		m_FileSize = dwFileSize;
		if (ReadFile(hFile, m_FileData, dwFileSize, &dwRet, NULL))
		{
			bRet = TRUE;
			*outFileSize = m_FileSize;
			*outBuffer = m_FileData;
		}
		CloseHandle(hFile);
	}
	return bRet;
}

BOOL ReadInFileA(LPCSTR lpszFile, PVOID *outBuffer, UINT *outFileSize)
{
	PVOID m_FileData;
	UINT m_FileSize;
	BOOL bRet = FALSE;
	HANDLE hFile = CreateFileA(lpszFile, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//把文件读出来
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		DWORD dwRet = 0;
		m_FileData = (PVOID)new BYTE[dwFileSize];
		m_FileSize = dwFileSize;
		if (ReadFile(hFile, m_FileData, dwFileSize, &dwRet, NULL))
		{
			bRet = TRUE;
			*outFileSize = m_FileSize;
			*outBuffer = m_FileData;
		}
		CloseHandle(hFile);
	}
	return bRet;
}

BOOL WriteToFileW(LPCWSTR lpszFileName, PVOID Buffer, UINT nBufferSize)
{
	BOOL bRet = FALSE;
	HANDLE hFile = CreateFileW(lpszFileName, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRet = 0;
		if (WriteFile(hFile, Buffer, nBufferSize, &dwRet, NULL))
		{
			bRet = TRUE;
		}
		CloseHandle(hFile);
	}
	return bRet;
}

BOOL WriteToFileA(LPCSTR lpszFileName, PVOID Buffer, UINT nBufferSize)
{
	BOOL bRet = FALSE;
	HANDLE hFile = CreateFileA(lpszFileName, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRet = 0;
		if (WriteFile(hFile, Buffer, nBufferSize, &dwRet, NULL))
		{
			bRet = TRUE;
		}
		CloseHandle(hFile);
	}
	return bRet;
}