// type.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

