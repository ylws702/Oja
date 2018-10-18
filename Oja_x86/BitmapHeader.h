#pragma once
#include <Windows.h>
typedef struct tagBITMAPHEADER
{
	//位图文件头
	BITMAPFILEHEADER fileHeader;
	//位图信息头
	BITMAPINFOHEADER infoHeader;
	//彩色表
	RGBQUAD rgbQuad;
} BITMAPHEADER;
