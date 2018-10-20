// Oja_x86.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Net.h"
#include <cstring>
#include <fstream>

#define EXPORT __declspec(dllexport)
#define INPUT_LEN 64
#define OUTPUT_LEN 4
#define BLOCK_COUNT 4096
#define TRAINNING_TIMES 100
#define ALPHA 1.0e-06F
char* zipPath = nullptr;
float* pOffset = nullptr;
Net* pNet = nullptr;

extern "C" EXPORT void init(const char* filename)
{
    pNet = new Net(INPUT_LEN, OUTPUT_LEN, ALPHA);
    if (zipPath != nullptr)
    {
        delete[] zipPath;
        //zipPath = nullptr;
    }
    int length = std::strlen(filename) + 1;
    zipPath = new char[length];
    std::memcpy(zipPath, filename, length);
}

extern "C" EXPORT void dispose()
{
    if (zipPath != nullptr)
    {
        delete[] zipPath;
        zipPath = nullptr;
    }
    if (pOffset != nullptr)
    {
        delete[] pOffset;
        pOffset = nullptr;
    }
    if (pNet != nullptr)
    {
        delete pNet;
        pNet = nullptr;
    }
}


extern "C" EXPORT void setOffset(const float* inputs)
{
    if (pOffset != nullptr)
    {
        delete[] pOffset;
        //pOffset = nullptr;
    }
    pOffset = new float[INPUT_LEN];
    std::memcpy(pOffset, inputs, INPUT_LEN * sizeof(float));
}

extern "C" EXPORT void train(const float* inputs)
{
    pNet->Train(inputs);
}

typedef struct tagZGHEADER
{
    const char z = 'Z';
    const char g = 'G';
    uint32_t offsetSize;
    uint32_t zipSize;
}zgHeader, *pZgHeader;

extern "C" EXPORT void writeHeaderAndOffset()
{
    zgHeader header;
    header.offsetSize = INPUT_LEN * sizeof(float);
    header.zipSize = OUTPUT_LEN * sizeof(float) * 4096;
    std::ofstream ofs(zipPath, std::ofstream::binary);
    if (!ofs.fail())
    {
        ofs.write((char*)&header, sizeof(header));
        ofs.write((char*)pOffset, header.offsetSize);
    }
    ofs.close();
}

extern "C" EXPORT void write(const float* inputs)
{
    std::ofstream ofs(zipPath, std::ofstream::binary | std::ofstream::app);
    if (!ofs.fail())
    {
        float outputs[OUTPUT_LEN];
        for (unsigned i = 0; i < OUTPUT_LEN; ++i)
        {
            outputs[i] = pNet->GetOutputs(i, inputs);
        }
        ofs.write((char*)outputs, sizeof(outputs));
    }
    ofs.close();
}

extern "C" EXPORT void writeWeights()
{
    std::ofstream ofs(zipPath, std::ofstream::binary | std::ofstream::app);
    if (!ofs.fail())
    {
        float weights[INPUT_LEN];
        for (unsigned i = 0; i < OUTPUT_LEN; ++i)
        {
            pNet->GetWeights(i, weights);
            ofs.write((char*)weights, sizeof(weights));
        }
    }
    ofs.close();
}

extern "C" EXPORT void unzip(const char* path, float*& outputs)
{
    zgHeader header;
    std::ifstream ifs(path, std::ofstream::binary);
    if (ifs.fail())
    {
        ifs.close();
        return;
    }
    ifs.read((char*)&header, sizeof(header));
    float offset[INPUT_LEN];
    ifs.read((char*)&offset, header.offsetSize);
    float weightsT[OUTPUT_LEN][INPUT_LEN];
    for (unsigned i = 0; i < OUTPUT_LEN; ++i)
    {
        ifs.read((char*)weightsT[i], sizeof(weightsT[i]));
    }
    float block[OUTPUT_LEN];
    unsigned count = 0;
    for (unsigned i = 0; i < BLOCK_COUNT; ++i)
    {
        ifs.read((char*)&block, sizeof(block));
        for (unsigned j = 0; j < INPUT_LEN; ++j)
        {
            outputs[count] = weightsT[0][j] * block[0];
            for (unsigned k = 1; k < OUTPUT_LEN; ++k)
            {
                outputs[count] += weightsT[k][j] * block[k];
            }
            ++count;
        }
    }
}
