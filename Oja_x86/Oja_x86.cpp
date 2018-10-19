// Oja_x86.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Net.h"

#define EXPORT __declspec(dllexport)
#define INPUT_LEN 64
#define OUTPUT_LEN 4
#define TRAINNING_TIMES 100

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

extern "C" EXPORT bool zip(const char* filename, float*inputs, float*& outputs)
{
    Net net(INPUT_LEN, OUTPUT_LEN, 0.05f);
    for (unsigned i = 0; i < TRAINNING_TIMES; i++)
    {
        net.Train(inputs);
    }
    for (unsigned i = 0; i < OUTPUT_LEN; i++)
    {
        outputs[i] = net.GetOutputs(i);
    }
    return true;
}
