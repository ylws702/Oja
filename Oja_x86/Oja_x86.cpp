// Oja_x86.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BmpReader.h"
#include <charconv>
#define EXPORT __declspec(dllexport)
#define INPUT_LEN 64
#define OUTPUT_LEN 4

extern "C" EXPORT void makeLog(const char* str)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    FILE* fp;
    fopen_s(&fp, "log;", "a+");
    if (fp != NULL)
    {
        setlocale(0, "zh-CN");
        fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n",
            time.wYear, time.wMonth, time.wDay,
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
            str);
        fclose(fp);
    }
}

extern "C" EXPORT bool zip(const char* filename, int* input, int*& output)
{
    char s[10] = {0};
    for (int i = 0; i < 4; ++i)
    {
        output[i] = i;
    }
    return true;
}
