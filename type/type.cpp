// type.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>

void Type(TCHAR* fileName)
{
	TCHAR StringBuff[1024];

	FILE* filePtr = _tfopen(fileName, _T("rt"));

	while (_fgetts(StringBuff, 1024, filePtr))
	{
		_fputts(StringBuff, stdout);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//if (argc < 2)
	//	return -1;
	Type(argv[1]);
	return 0;
}

