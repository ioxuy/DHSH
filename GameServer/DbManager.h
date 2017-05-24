#ifndef __DHSH_GAMESERVER_DB_DBMANAGER_H__
#define __DHSH_GAMESERVER_DB_DBMANAGER_H__

#include <MyTools/ClassInstance.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include <queue>



class CAccount;
class CDbManager : public MyTools::CClassInstance<CDbManager>
{
private:
	struct SQLEnvParam
	{
		SQLHENV hEnv;
		SQLHDBC hDbc;
		SQLHSTMT hStmt;
		SQLEnvParam()
		{
			hEnv = SQL_NULL_HENV;
			hDbc = SQL_NULL_HDBC;
			hStmt = SQL_NULL_HSTMT;
		}
	};

public:
	struct DbAccountConfig
	{
		DWORD        dwAccountId;
		std::wstring wsConfigName;
		std::wstring wsConfigValue;
	};

public:
	CDbManager();
	~CDbManager() = default;

	// 获取所有的帐号
	BOOL GetAccount(_Out_ std::vector<CAccount>& VecAccount);


	// 获取帐号下的充值记录
	BOOL GetAccountCardLog(_In_ DWORD dwAccountId, _Out_ std::vector<std::wstring>& VecText) CONST;
	

	// 获取所有帐号的配置文件
	BOOL GetVecAccountConfog(_Out_ std::vector<DbAccountConfig>& Vec) CONST;


	// 充值
	BOOL RechargeCard(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsCardNo, _Out_ std::wstring& wsRetMsg) CONST;


	// 注册
	BOOL RegisterAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ CONST std::wstring& wsClientIp, _Out_ std::wstring& wsRetMsg) CONST;


	// 更新配置
	BOOL SetAccountConfig(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsConfigName, _In_ CONST std::wstring& wsConfigValue) CONST;


	// 登录记录
	BOOL SetAccountLoginRecord(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsClientIp) CONST;


	// 运行异步执行SQL线程
	BOOL RunThread();


	// Stop 异步执行SQL线程
	VOID Stop();


	// 异步执行SQL
	VOID AsyncExcuteSQL(_In_ CONST std::wstring& wsSQL);
private:

	// 
	BOOL ExcuteSQL(_In_ CONST std::wstring& wsSQL) CONST;

	//
	BOOL ExcuteSQL_Ret_Single_Text(_In_ CONST std::wstring& wsSQL, _Out_ std::wstring& wsText) CONST;

	//
	BOOL ExcuteSQL(_In_ CONST std::wstring& wsSQL, std::function<VOID(SQLHSTMT&)> GetDataPtr) CONST;

	//
	BOOL InitializeSQLEnv(_Out_ SQLEnvParam& Env) CONST;

	//
	VOID PrintSQLErrText(_In_ SQLSMALLINT fHandleType, _In_ SQLHANDLE handle, _In_ int ConnInd) CONST;

	//
	VOID FreeMem(_In_ SQLEnvParam& Env) CONST;

	// Thread
	static DWORD WINAPI _WorkThread(LPVOID lpParam);
private:
	HANDLE                   _hThread;
	BOOL                     _bRun;
	std::queue<std::wstring> _QueueSQL;
	MyTools::CLLock			 _Lock;
};


#endif // !__DHSH_GAMESERVER_DB_DBMANAGER_H__
