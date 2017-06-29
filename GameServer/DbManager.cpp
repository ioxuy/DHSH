#include "DbManager.h"
#include <future>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLAsync.h>
#include "Account.h"

#define _SELF L"DbManager.cpp"

BOOL CDbManager::ExcuteSQL_Ret_Single_Text(_In_ CONST std::wstring& wsSQL, _Out_ std::wstring& wsText) CONST
{
	return ExcuteSQL(wsSQL, [&wsText](SQLHSTMT& hStmt)
	{
		WCHAR wszText[MAX_PATH] = { 0 };
		::SQLGetData(hStmt, 1, SQL_C_WCHAR, wszText, MAX_PATH, NULL);
		wsText = wszText;
	});
}

CDbManager::CDbManager()
{

}

BOOL CDbManager::GetAccount(_Out_ std::vector<CAccount>& VecAccount)
{
	static CONST UINT uMaxTextSize = 32;
	WCHAR Text1[uMaxTextSize];
	WCHAR Text2[uMaxTextSize];
	WCHAR Text3[uMaxTextSize];
	WCHAR Text4[uMaxTextSize];
	WCHAR Text5[uMaxTextSize];
	return ExcuteSQL(L"exec [GetAccount]", [&](SQLHSTMT& hStmt) 
	{
		ZeroMemory(Text1, _countof(Text1));
		ZeroMemory(Text2, _countof(Text2));
		ZeroMemory(Text3, _countof(Text3));
		ZeroMemory(Text4, _countof(Text4));
		ZeroMemory(Text5, _countof(Text5));

		::SQLGetData(hStmt, 1, SQL_C_WCHAR, Text1, uMaxTextSize, NULL);
		::SQLGetData(hStmt, 2, SQL_C_WCHAR, Text2, uMaxTextSize, NULL);
		::SQLGetData(hStmt, 3, SQL_C_WCHAR, Text3, uMaxTextSize, NULL);
		::SQLGetData(hStmt, 4, SQL_C_WCHAR, Text4, uMaxTextSize, NULL);
		::SQLGetData(hStmt, 5, SQL_C_WCHAR, Text5, uMaxTextSize, NULL);

		CAccount Account(static_cast<DWORD>(_wtoi(Text1)), std::wstring(Text2), std::wstring(Text3), _wtoi(Text4) != 0 ? TRUE : FALSE, static_cast<DWORD>(_wtoi(Text5)));
		VecAccount.push_back(std::move(Account));
	});
}

BOOL CDbManager::GetAccountCardLog(_In_ DWORD dwAccountId, _Out_ std::vector<std::wstring>& VecText) CONST
{
	WCHAR wszText[MAX_PATH] = { 0 };

	std::wstring wsSQL;
	return ExcuteSQL(MyTools::CCharacter::FormatText(wsSQL,L"exec [proc_GetAccountCardLog] %d", dwAccountId), [&wszText, &VecText](SQLHSTMT& hStmt)
	{
		ZeroMemory(wszText, _countof(wszText));
		::SQLGetData(hStmt, 1, SQL_C_WCHAR, wszText, MAX_PATH, NULL);
		VecText.push_back(std::wstring(wszText));
	});
}

BOOL CDbManager::ExcuteSQL(_In_ CONST std::wstring& wsSQL, std::function<VOID(SQLHSTMT&)> GetDataPtr) CONST
{
	SQLEnvParam Env;
	if (!InitializeSQLEnv(Env))
	{
		FreeMem(Env);
		return FALSE;
	}

	auto Ret = SQLAllocHandle(SQL_HANDLE_STMT, Env.hDbc, &Env.hStmt);
	if (Ret != SQL_SUCCESS)
	{
		LOG_CF_E(L"SQLAllocHandle(SQL_HANDLE_STMT) Faild, Err=%d", Ret);
		FreeMem(Env);
		return FALSE;
	}

	std::shared_ptr<WCHAR> Buffer(new WCHAR[wsSQL.length() + 1], [](WCHAR* p) { delete[] p; });
	MyTools::CCharacter::wstrcpy_my(Buffer.get(), wsSQL.c_str(), wsSQL.length());

	Ret = SQLExecDirectW(Env.hStmt, Buffer.get(), wsSQL.length());
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF_E(L"SQLExecDirectW Faild. Ret=%d,SQL=%s", Ret, wsSQL.c_str());
		PrintSQLErrText(SQL_HANDLE_STMT, Env.hStmt, TRUE);
		FreeMem(Env);
		return FALSE;
	}


	for (int i = 0; GetDataPtr != nullptr && i < 100 && ::SQLFetch(Env.hStmt) == SQL_SUCCESS; ++i)
		GetDataPtr(Env.hStmt);

	FreeMem(Env);
	return TRUE;
}

BOOL CDbManager::ExcuteSQL_No_Result(_In_ CONST std::wstring& wsSQL) CONST
{
	return ExcuteSQL(wsSQL, nullptr);
}

BOOL CDbManager::GetVecAccountConfog(_Out_ std::vector<DbAccountConfig>& Vec) CONST
{
	WCHAR Text1[MAX_PATH];
	WCHAR Text2[MAX_PATH];
	WCHAR Text3[MAX_PATH];
	return ExcuteSQL(L"exec [GetAccountConfig]", [&](SQLHSTMT& hStmt)
	{
		ZeroMemory(Text1, _countof(Text1));
		ZeroMemory(Text2, _countof(Text2));
		ZeroMemory(Text3, _countof(Text3));

		::SQLGetData(hStmt, 1, SQL_C_WCHAR, Text1, MAX_PATH, NULL);
		::SQLGetData(hStmt, 2, SQL_C_WCHAR, Text2, MAX_PATH, NULL);
		::SQLGetData(hStmt, 3, SQL_C_WCHAR, Text3, MAX_PATH, NULL);

		DbAccountConfig Config;
		Config.dwAccountId = _wtoi(Text1);
		Config.wsConfigName = Text2;
		Config.wsConfigValue = Text3;
		Vec.push_back(std::move(Config));
	});
}

BOOL CDbManager::RechargeCard(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsCardNo, _Out_ std::wstring& wsRetMsg) CONST
{
	std::wstring wsSQL;
	return ExcuteSQL_Ret_Single_Text(MyTools::CCharacter::FormatText(wsSQL, L"exec [proc_ActiveCard] %d,N'%s'", dwAccountId, wsCardNo.c_str()), wsRetMsg);
}

BOOL CDbManager::RegisterAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ CONST std::wstring& wsClientIp, _Out_ std::wstring& wsRetMsg) CONST
{
	std::wstring wsSQL;
	return ExcuteSQL_Ret_Single_Text(MyTools::CCharacter::FormatText(wsSQL, L"exec [proc_CreateAccount] N'%s',N'%s',N'%s'", wsAccountName.c_str(), wsAccountPass.c_str(), wsClientIp.c_str()), wsRetMsg);
}

BOOL CDbManager::SetAccountConfig(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsConfigName, _In_ CONST std::wstring& wsConfigValue) CONST
{
	std::wstring wsSQL;
	return ExcuteSQL_No_Result(MyTools::CCharacter::FormatText(wsSQL, L"exec [proc_SetAccountConfig] %d,N'%s',N'%s'", dwAccountId, wsConfigName.c_str(), wsConfigValue.c_str()));
}

BOOL CDbManager::SetAccountLoginRecord(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsClientIp) CONST
{
	std::wstring wsSQL;
	return ExcuteSQL_No_Result(MyTools::CCharacter::FormatText(wsSQL, L"exec [SetAccountIP] %d,N'%s'", dwAccountId, wsClientIp.c_str()));
}


VOID CDbManager::AsyncExcuteSQL(_In_ CONST std::wstring& wsSQL)
{
	auto fnMethodPtr = [this](_In_ CONST std::wstring& wsSQL)
	{
		if(!CDbManager::ExcuteSQL_No_Result(wsSQL))
			LOG_CF_E(L"ExcuteSQL_No_Result Faild! SQL=%s", wsSQL.c_str());
	};

	MyTools::CLAsync::GetInstance().ExcuteAsync(fnMethodPtr, wsSQL);
}

BOOL CDbManager::GetTime_By_AccountId(_In_ DWORD dwAccountId, _Out_ DWORD& dwTime) CONST
{
	std::wstring wsText, wsSQL;
	if (!ExcuteSQL_Ret_Single_Text(MyTools::CCharacter::FormatText(wsSQL, L"exec [GetAccountById] %d", dwAccountId), wsText))
		return FALSE;

	dwTime = _wtoi(wsText.c_str());
	return TRUE;
}

BOOL CDbManager::InitializeSQLEnv(_Out_ SQLEnvParam& Env) CONST
{
	WCHAR wsDns[] = { L"DHSH" };
	WCHAR wsDbUser[] = { L"GameUser" };
	WCHAR wsDbPass[] = { L"db_lg_648881240" };

	auto Ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &Env.hEnv);
	if (Ret != SQL_SUCCESS)
	{
		LOG_CF_E(L"SQLAllocHandle(SQL_HANDLE_ENV) = %d!", Ret);
		return FALSE;
	}

	Ret = SQLSetEnvAttr(Env.hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), SQL_IS_INTEGER);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF_E(L"SQLSetEnvAttr = %d", Ret);
		return FALSE;
	}

	Ret = SQLAllocHandle(SQL_HANDLE_DBC, Env.hEnv, &Env.hDbc);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF_E(L"SQLAllocHandle(SQL_HANDLE_DBC) = %d", Ret);
		return FALSE;
	}

	Ret = SQLConnectW(Env.hDbc, wsDns, SQL_NTS, wsDbUser, SQL_NTS, wsDbPass, SQL_NTS);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF_E(L"SQLConnectW Faild. Err=%d", Ret);
		PrintSQLErrText(SQL_HANDLE_DBC, Env.hDbc, 0);
		return FALSE;
	}

	return TRUE;
}

VOID CDbManager::PrintSQLErrText(_In_ SQLSMALLINT fHandleType, _In_ SQLHANDLE handle, _In_ int ConnInd) CONST
{
	RETCODE plm_retcode = SQL_SUCCESS;
	SQLWCHAR plm_szSqlState[MAX_PATH] = L"", plm_szErrorMsg[MAX_PATH] = L"";
	SDWORD plm_pfNativeError = 0L;
	SWORD plm_pcbErrorMsg = 0;
	SQLSMALLINT plm_cRecNmbr = 1;
	SDWORD plm_SS_MsgState = 0, plm_SS_Severity = 0;
	SQLINTEGER plm_Rownumber = 0;
	USHORT plm_SS_Line;
	SQLSMALLINT plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];

	while (plm_retcode != SQL_NO_DATA_FOUND)
	{
		plm_retcode = SQLGetDiagRec(fHandleType, handle, plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError, plm_szErrorMsg, MAX_PATH - 1, &plm_pcbErrorMsg);
		if (plm_retcode != SQL_NO_DATA_FOUND)
		{
			if (ConnInd)
			{
				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_ROW_NUMBER, &plm_Rownumber, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_LINE, &plm_SS_Line, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_SEVERITY, &plm_SS_Severity, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_PROCNAME, &plm_SS_Procname, sizeof(plm_SS_Procname), &plm_cbSS_Procname);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname, sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
			}

			LOG_CF_E(L"SqlState=%s", plm_szSqlState);
			LOG_CF_E(L"NativeError=%d", plm_pfNativeError);
			LOG_CF_E(L"ErrorMsg=%s", plm_szErrorMsg);
			LOG_CF_E(L"pcbErrorMsg=%d", plm_pcbErrorMsg);

			if (ConnInd)
			{
				LOG_CF_E(L"ODBCRowNumber=%d", plm_Rownumber);
				LOG_CF_E(L"SSrvrLine=%d", plm_Rownumber);
				LOG_CF_E(L"SSrvrMsgState=%d", plm_SS_MsgState);
				LOG_CF_E(L"SSrvrSeverity=%d", plm_SS_Severity);
				LOG_CF_E(L"SSrvrProcname=%s", plm_SS_Procname);
				LOG_CF_E(L"SSrvrSrvname=%s", plm_SS_Srvname);
			}
		}

		plm_cRecNmbr++;   // Increment to next diagnostic record.
	}
}

VOID CDbManager::FreeMem(_In_ SQLEnvParam& Env) CONST
{
	if (Env.hStmt != SQL_NULL_HSTMT)
	{
		::SQLCloseCursor(Env.hStmt);
		::SQLFreeHandle(SQL_HANDLE_STMT, Env.hStmt);
		Env.hStmt = SQL_NULL_HSTMT;
	}

	if (Env.hDbc != SQL_NULL_HDBC)
	{
		::SQLDisconnect(Env.hDbc);
		::SQLFreeHandle(SQL_HANDLE_DBC, Env.hDbc);
		Env.hDbc = SQL_NULL_HDBC;
	}

	if (Env.hEnv != SQL_NULL_HENV)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, Env.hEnv);
		Env.hEnv = SQL_NULL_HENV;
	}
}

