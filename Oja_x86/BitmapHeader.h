#pragma once
#include <Windows.h>
typedef struct tagBITMAPHEADER
{
	//λͼ�ļ�ͷ
	BITMAPFILEHEADER fileHeader;
	//λͼ��Ϣͷ
	BITMAPINFOHEADER infoHeader;
	//��ɫ��
	RGBQUAD rgbQuad;
} BITMAPHEADER;
