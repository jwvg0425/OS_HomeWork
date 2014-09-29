// windows.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
#include "stdafx.h"
#include <windows.h>
#include <tlHelp32.h>

#define MAX_STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] 
= _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

int CmdProcessing();
TCHAR * StrLower(TCHAR * str);
bool Tokenize();
void printFileData(WIN32_FIND_DATA data);
void StartProcess(TCHAR* processName);
bool PrintProcessList();
bool KillProcess();
bool PrintFileList();
bool MakeDirectory();
bool RemoveDirectory();
bool DeleteFile();
bool RenameFile();
void EchoString();
void ChangeCurrentDirectory();
void TypeTextFile();
void SortString();
void PrintHistory();

TCHAR cmdString[MAX_STR_LEN];
TCHAR cmdCopy[MAX_STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][MAX_STR_LEN];
TCHAR cmdHistory[1000][MAX_STR_LEN];
int historyNum = 0;
int tokenNum = 0;
TCHAR seps[] = _T(" ,\t\n");

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("korean"));

	DWORD isExit;

	if (argc >= 2)
	{
		tokenNum = argc - 1;
		for (int i = 1; i < argc; i++)
		{
			_tcscpy(cmdTokenList[i - 1], argv[i]);
		}
		CmdProcessing();
	}


	while (1)
	{
		if (!Tokenize())
		{
			continue;
		}
		isExit = CmdProcessing();
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
		tokenNum = 0;
	}

	return 0;
}

int SearchToken()
{
	TCHAR * token = _tcstok(cmdString, _T("!"));

	for (int i = historyNum - 2; i >= 0; i--)
	{
		TCHAR string[MAX_STR_LEN];
		TCHAR* token2;

		_tcscpy(string, cmdHistory[i]);
		token2 = _tcstok(string, seps);

		if (!_tcscmp(token, token2))
		{
			return i;
		}
	}

	return -1;
}



bool Tokenize()
{
	TCHAR cDIR[MAX_STR_LEN];
	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	_tprintf(_T("%s>>"), cDIR);
	_getts_s(cmdString);
	_tcscpy_s(cmdHistory[historyNum++], cmdString);

	int nowNum = historyNum - 1;
	while (cmdString[0] == '!')
	{
		if (cmdString[1] == '!')
		{
			if (_tcslen(cmdString) == 2 && nowNum >= 1)
			{
				_tcscpy(cmdString, cmdHistory[nowNum - 1]);
				nowNum = nowNum - 1;
			}
			else
			{
				_tprintf(_T("���� ��ɾ �������� �ʽ��ϴ�.\n"));
				return false;
			}
		}
		else
		{
			int num = SearchToken();
			if (num != -1)
			{
				_tcscpy(cmdString, cmdHistory[num]);
				nowNum = num;
			}
			else
			{
				_tprintf(_T("���� ��ɾ �������� �ʽ��ϴ�.\n"));
				return false;
			}
		}
	}
	_tcscpy_s(cmdCopy, cmdString);
	



	TCHAR * token = _tcstok(cmdString, seps);


	while (token != NULL){
		_tcscpy(
			cmdTokenList[tokenNum++], StrLower(token)
			);
		token = _tcstok(NULL, seps);
	}

	return true;
}

int CmdProcessing()
{

	if (!_tcscmp(cmdTokenList[0], _T("exit"))){
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("pwd")))
	{
		TCHAR cDIR[MAX_STR_LEN];
		GetCurrentDirectory(MAX_STR_LEN, cDIR);

		_tprintf(_T("%s\n"), cDIR);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("cls")))
	{
		_tsystem(_T("cls"));
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		StartProcess(_T("windows"));
	}
	else if (!_tcscmp(cmdTokenList[0], _T("list")))
	{
		if (!PrintProcessList())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("cd")))
	{
		ChangeCurrentDirectory();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kill")))
	{
		if (!KillProcess())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("dir")))
	{
		if (!PrintFileList())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("mkdir")))
	{
		if (!MakeDirectory())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("rmdir")))
	{
		if (!RemoveDirectory())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("del")))
	{
		if (!DeleteFile())
		{
			return -1;
		}
	}
	else if (!_tcscmp (cmdTokenList[0], _T("ren")))
	{
		if (!RenameFile())
		{
			return -1;
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("help")))
	{
		_tprintf(_T("pwd\t���� ���丮�� �����ݴϴ�.\n"));
		_tprintf(_T("cls\tȭ���� ����ϴ�.\n"));
		_tprintf(_T("help\t��ɾ� ����� �����ݴϴ�.\n"));
		_tprintf(_T("exit\t���α׷��� �����մϴ�.\n"));
		_tprintf(_T("echo\t���ڿ��� ����մϴ�.\n"));
		_tprintf(_T("start\t���α׷��� �����մϴ�.\n"));
		_tprintf(_T("list\t�������� ���μ��� ����� �����ݴϴ�.\n"));
		_tprintf(_T("kill\t���μ����� ������ �����ŵ�ϴ�. \n"));
		_tprintf(_T("dir\t���� ���丮 ������ ���� ����� �����ݴϴ�. \n"));
		_tprintf(_T("mkdir\t���� ���丮 ������ �� ���丮�� �����մϴ�. \n"));
		_tprintf(_T("rmdir\t���� ���丮 ������ ������ ���丮�� �����մϴ�. \n"));
		_tprintf(_T("del\t���� ���丮 ������ ������ ������ �����մϴ�. \n"));
		_tprintf(_T("ren\t���� ���丮 ������ ������ ���� �̸��� �����մϴ�.\n"));
		_tprintf(_T("cd\t���� ���丮�� �����մϴ�.\n"));
		_tprintf(_T("history\t���ݱ��� �Է��� ��ɾ� ����� �����ݴϴ�.\n"));
		_tprintf(_T("type\t������ ������ ȭ�鿡 ����մϴ�.\n"));
		_tprintf(_T("sort\t�Է��� ���ڿ��� �����Ͽ� ����մϴ�.(Ctrl+Z�� ����)\n"));
		_tprintf(_T("!(��ɾ�)\t���� �ֱٿ� ������ (��ɾ�)�� �����մϴ�.\n"));
		_tprintf(_T("!!\t�ٷ� ���� ������ ��ɾ �����մϴ�.\n"));
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		EchoString();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("type")))
	{
		TypeTextFile();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("sort")))
	{
		SortString();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("history")))
	{
		PrintHistory();
	}
	else
	{
		StartProcess(cmdTokenList[0]);
	}
	return 0;
}

TCHAR * StrLower(TCHAR * str)
{
	TCHAR *ret = str;

	while (*str)
	{
		if (_istupper(*str))
		{
			*str = _totlower(*str);
		}
		str++;
	}
	return ret;
}


void printFileData(WIN32_FIND_DATA data)
{
	SYSTEMTIME fileTime, localTime;

	FileTimeToSystemTime(&data.ftLastWriteTime, &fileTime);
	SystemTimeToTzSpecificLocalTime(NULL, &fileTime, &localTime);
	TCHAR fileTimeStr[MAX_PATH];

	_stprintf(fileTimeStr, _T("%02d�� %02d �� %02d�� %02d��%02d��"), localTime.wYear, localTime.wMonth, localTime.wDay,
		localTime.wHour, localTime.wMinute);

	_tprintf(_T("%15s"), fileTimeStr);

	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		_tprintf(_T("  DIR  "));
	}
	else
	{
		_tprintf(_T("       "));
	}

	_tprintf(_T("%15s\n"), data.cFileName);
}

void StartProcess(TCHAR* processName)
{
	bool isRun;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR optString[MAX_STR_LEN] = { 0, };

	_tcscpy(optString, processName);

	for (int i = 1; i < tokenNum; i++)
	{
		_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
	}

	isRun = CreateProcess(NULL, optString, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (isRun == false){
		_tprintf(ERROR_CMD, processName);
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

bool PrintProcessList()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolhelp32Snapshot error! \n"));
		return false;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32Fist error! \n"));
		CloseHandle(hProcessSnap);
		return false;
	}

	do
	{
		_tprintf(_T("%40s %5d\n"), pe32.szExeFile, pe32.th32ProcessID);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return true;
}

bool KillProcess()
{
	if (tokenNum < 2)
	{
		return false;
	}

	TCHAR * token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolhelp32Snapshot error! \n"));
		return false;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	bool isTerminate = false;

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32Fist error! \n"));
		CloseHandle(hProcessSnap);
		return false;
	}

	do
	{
		if (!_tcscmp(pe32.szExeFile, token))
		{
			HANDLE hProcess;

			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, -1);
				isTerminate = true;
			}

			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	if (isTerminate == false)
	{
		_tprintf(_T("Kill Error!\n"));
		return false;
	}

	return true;
}

bool PrintFileList()
{
	TCHAR cDIR[MAX_STR_LEN];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR DirSpec[MAX_PATH];

	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	_stprintf(DirSpec, _T("%s%s"), cDIR, _T("\\*"));

	hFind = FindFirstFile(DirSpec, &FindFileData);


	if (hFind == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Invalid file handle \n"));
		return false;
	}

	printFileData(FindFileData);

	while (FindNextFile(hFind, &FindFileData) != 0)
	{
		printFileData(FindFileData);
	}

	FindClose(hFind);

	return true;
}

bool MakeDirectory()
{
	TCHAR cDIR[MAX_STR_LEN];
	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	if (tokenNum < 2)
	{
		return false;
	}
	TCHAR path[MAX_PATH];
	TCHAR * token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	_stprintf(path, _T("%s%s%s"), cDIR, _T("\\"), token);

	if (!CreateDirectory(path, NULL) && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(_T("%s��(��) �̹� �����ϴ� ���丮�Դϴ�.\n"), token);
		return false;
	}

	return true;
}

bool RemoveDirectory()
{
	TCHAR cDIR[MAX_STR_LEN];
	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	if (tokenNum < 2)
	{
		return false;
	}
	TCHAR path[MAX_PATH];
	TCHAR * token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	_stprintf(path, _T("%s%s%s"), cDIR, _T("\\"), token);

	if (!RemoveDirectory(path))
	{
		_tprintf(_T("%s��(��) �������� �ʴ� ���丮�Դϴ�.\n"), token);
		return false;
	}

	return true;
}

bool DeleteFile()
{
	TCHAR cDIR[MAX_STR_LEN];
	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	if (tokenNum < 2)
	{
		return false;
	}
	TCHAR path[MAX_PATH];
	TCHAR * token = _tcstok(NULL, seps); token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	_stprintf(path, _T("%s%s%s"), cDIR, _T("\\"), token);

	if (!DeleteFile(path) && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		_tprintf(_T("%s��(��) �������� �ʴ� �����Դϴ�.\n"), token);
		return false;
	}

	return true;
}

bool RenameFile()
{
	TCHAR cDIR[MAX_STR_LEN];
	GetCurrentDirectory(MAX_STR_LEN, cDIR);

	if (tokenNum < 3)
	{
		return false;
	}
	TCHAR oldPath[MAX_PATH], newPath[MAX_PATH];
	_stprintf(oldPath, _T("%s%s%s"), cDIR, _T("\\"), cmdTokenList[1]);
	_stprintf(newPath, _T("%s%s%s"), cDIR, _T("\\"), cmdTokenList[2]);

	if (!MoveFile(oldPath, newPath))
	{
		_tprintf(_T("���� �̸� ���濡 �����߽��ϴ�."));
		return false;
	}

	return true;
}

void EchoString()
{
	if (tokenNum < 2)
		return;

	TCHAR * token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	_tprintf(_T("%s\n"), token);
}

void ChangeCurrentDirectory()
{
	if (tokenNum < 2)
		return;
	TCHAR * token = _tcstok(cmdCopy, seps);
	token = _tcstok(NULL, _T("\0"));

	SetCurrentDirectory(token);
}

void TypeTextFile()
{
	TCHAR cmdStringWithOptions[MAX_STR_LEN] = { 0, };
	BOOL isRun;

	if (!_tcscmp(cmdTokenList[2], _T("|")))
	{
		HANDLE hReadPipe, hWritePipe;

		SECURITY_ATTRIBUTES pipeSA = {
			sizeof(SECURITY_ATTRIBUTES), NULL, TRUE
		};

		CreatePipe(&hReadPipe, &hWritePipe, &pipeSA, 0);

		STARTUPINFO siType = { 0, };
		PROCESS_INFORMATION piType;
		siType.cb = sizeof(siType);

		siType.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		siType.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		siType.hStdOutput = hWritePipe;
		siType.dwFlags |= STARTF_USESTDHANDLES;

		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);
		_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[1]);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &siType, &piType);

		CloseHandle(piType.hThread);
		CloseHandle(hWritePipe);

		STARTUPINFO siSort = { 0, };
		PROCESS_INFORMATION piSort;

		siSort.cb = sizeof(siSort);

		siSort.hStdInput = hReadPipe;
		siSort.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		siSort.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		siSort.dwFlags |= STARTF_USESTDHANDLES;

		isRun = CreateProcess(NULL, cmdTokenList[3], NULL, NULL, TRUE, 0, NULL, NULL, &siSort, &piSort);

		CloseHandle(piSort.hThread);
		CloseHandle(hReadPipe);

		WaitForSingleObject(piType.hProcess, INFINITE);
		WaitForSingleObject(piSort.hProcess, INFINITE);

		CloseHandle(piType.hProcess);
		CloseHandle(piSort.hProcess);

	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);
		_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[1]);

		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void SortString()
{

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	bool isRun;

	isRun = CreateProcess(NULL, cmdCopy , NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void PrintHistory()
{
	for (int i = 0; i < historyNum; i++)
	{
		_tprintf(_T("%s\n"), cmdHistory[i]);
	}
}
