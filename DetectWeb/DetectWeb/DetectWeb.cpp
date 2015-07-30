// DetectWeb.cpp : Defines the entry point for the console application.
//

//#define _CRTDBG_MAP_ALLOC

#include "stdafx.h"
#include "DetectWeb.h"
#include "afxinet.h"
#include "Psapi.h"

#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


#pragma comment(lib, "Wininet.lib")

void ProcessAll(void);
void CreateOwnProcess(void);
void KillOwnProcess(void);
BOOL CheckWebConnection(void);
DWORD FindProcess(WCHAR *strProcessName);
int UnicodeToChar(CString &strIn, char *pchOut, int nCharLen);
int UTF8ToUnicode(LPCSTR strIn, LPWSTR strOut, int nLenOut);


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			ProcessAll();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


void ProcessAll(void)
{
	while (true)
	{
		if (!CheckWebConnection())
		{
			KillOwnProcess();
			Sleep(5000);
			CreateOwnProcess();
		}
		Sleep(1000 * 30);
	}
}


void CreateOwnProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	TCHAR cmdline[] = L"C:\\Drcom\\DrUpdateClient\\DrMain.exe";

	if (!CreateProcess(NULL,   // No module name (use command line)
		cmdline,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,              // New console
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	printf("Create DrMain Process!\n");
}


void KillOwnProcess(void)
{
	DWORD exitCode;
	HANDLE hProcess;
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, FindProcess(L"DrMain.exe"));
	if (hProcess == NULL)
	{
		return;
	}
	GetExitCodeProcess(hProcess, &exitCode);
	TerminateProcess(hProcess, exitCode);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, FindProcess(L"DrClient.exe"));
	if (hProcess == NULL)
	{
		return;
	}
	GetExitCodeProcess(hProcess, &exitCode);
	TerminateProcess(hProcess, exitCode);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, FindProcess(L"DrUpdate.exe"));
	if (hProcess == NULL)
	{
		return;
	}
	GetExitCodeProcess(hProcess, &exitCode);
	TerminateProcess(hProcess, exitCode);
	return;
}

int UnicodeToChar(CString &strIn, char *pchOut, int nCharLen)
{
	if (pchOut == NULL)
	{
		return 0;
	}

	int nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(2048), -1, NULL, 0, NULL, NULL);
	nLen = min(nLen, nCharLen);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(2048), -1, pchOut,
		nLen, NULL, NULL);

	if (nLen < nCharLen)
	{
		pchOut[nLen] = 0;
	}

	return nLen;
}


int UTF8ToUnicode(LPCSTR strIn, LPWSTR strOut, int nLenOut)
{
	if (strOut == NULL)
	{
		return 0;
	}

	int nLen = MultiByteToWideChar(CP_UTF8, 0, strIn, -1, NULL, 0);
	nLen = min(nLen, nLenOut);
	MultiByteToWideChar(CP_UTF8, 0, strIn, -1, strOut, nLen);

	if (nLen < nLenOut)
	{
		strOut[nLen] = 0;
	}

	return nLen;
}

BOOL CheckWebConnection(void)
{
	BOOL BFlag = FALSE;

	CString strFileURLInServer(L"http://www.baidu.com");
	CInternetSession session;
	CHttpFile* pHttpFile = NULL;
	DWORD dwStatus;
	const int nTimeOut = 1000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
	const int BUFFER_LENGTH = 1024;
	WCHAR pszBuffer[BUFFER_LENGTH];
	CString str;

	pHttpFile = (CHttpFile*)session.OpenURL(strFileURLInServer, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, 0);
	pHttpFile->QueryInfoStatusCode(dwStatus);
	if (dwStatus == HTTP_STATUS_OK)
	{
		pHttpFile->SetReadBufferSize(BUFFER_LENGTH);
		pHttpFile->ReadString(str);

		UTF8ToUnicode((LPCSTR)(str.GetBuffer()), pszBuffer, BUFFER_LENGTH);

		WCHAR* result = wcsstr(pszBuffer, L"百度一下");

		if (result != NULL)
		{
			BFlag = TRUE;
		}
		
	}
	pHttpFile->Close();
	delete pHttpFile;
	session.Close();

	return BFlag;
}

/*
BOOL CheckWebConnection(void)
{
	//_CrtSetBreakAlloc(225);
	CString strFileURLInServer(L"http://www.microsoft.com/en-us/default.aspx");
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;
	DWORD dwType;
	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
	const int BUFFER_LENGTH = 1024 * 2;
	char pszBuffer[BUFFER_LENGTH];
	BOOL BFlag = FALSE;

	try
	{
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, INTERNET_FLAG_RELOAD, wPort);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_RELOAD);
		if (pHttpFile->SendRequest() == FALSE)
			return FALSE;
		DWORD dwStateCode;

		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode == HTTP_STATUS_OK)
		{
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

			char* str1 = strstr(pszBuffer, "<title>");
			char* str2 = strstr(str1, "</title>");
			str2[0] = '\0';
			str2 = strstr(str1, "Microsoft");

			BFlag = (str2 != NULL);
		}
		else
		{
			if (pHttpFile != NULL)
			{
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			if (pHttpConnection != NULL)
			{
				pHttpConnection->Close();
				delete pHttpConnection;
				pHttpConnection = NULL;
			}
			session.Close();
			return FALSE;
		}
	}
	catch (...)
	{
		if (pHttpFile != NULL)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if (pHttpConnection != NULL)
		{
			pHttpConnection->Close();
			delete pHttpConnection;
			pHttpConnection = NULL;
		}
		session.Close();
		return FALSE;
	}

	if (pHttpFile != NULL)
	{
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
	}
	if (pHttpConnection != NULL)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
		pHttpConnection = NULL;
	}
	session.Close();
	return BFlag;

}
*/

DWORD FindProcess(WCHAR *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	WCHAR szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return 0;
	for (int i = 0; i< (int)(cbNeeded / sizeof(DWORD)); i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameEx(hProcess, hMods[0], szProcessName, sizeof(szProcessName));

		if (wcsstr(szProcessName, strProcessName))
		{
			return(aProcesses[i]);
		}
	}
	return 0;
}
