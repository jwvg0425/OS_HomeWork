// sort.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

int compare(const void* a, const void* b)
{
	return _tcscmp((TCHAR*)a, (TCHAR*)b);
}


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR String[1000][256];

	int strNum=4;

	for (strNum = 0;; strNum++)
	{
		if (_fgetts(String[strNum], 256, stdin) == NULL)
		{
			break;
		}
	}

	qsort(String, strNum, sizeof(String[0]), compare);

	for (int i = 0; i < strNum; i++)
	{
		_fputts(String[i],stdout);
	}
	return 0;
}

