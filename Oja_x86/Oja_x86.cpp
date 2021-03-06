// Oja_x86.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Net.h"
#include <cstring>
#include <fstream>

//声明导出函数
#define EXPORT __declspec(dllexport)
//输入长度
#define INPUT_LEN 64
//输出长度
#define OUTPUT_LEN 4
//分块数量
#define BLOCK_COUNT 4096
//学习常数
#define ALPHA 0.01F
//压缩文件路径
char* zipPath = nullptr;
//指向偏移量数组的指针
float* pOffset = nullptr;
//指向训练网络的指针
Net* pNet = nullptr;

//初始化压缩
//filename: 要保存的压缩文件路径
extern "C" EXPORT void init(const char* filename)
{
    //新建神经网络
    if (pNet != nullptr)
    {
        delete pNet;
        //pNet = nullptr;
    }
    pNet = new Net(INPUT_LEN, OUTPUT_LEN, ALPHA);
    //保存压缩文件路径至zipPath
    if (zipPath != nullptr)
    {
        delete[] zipPath;
        //zipPath = nullptr;
    }
    int length = std::strlen(filename) + 1;
    zipPath = new char[length];
    std::memcpy(zipPath, filename, length);
}

//压缩完成后清理内存
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

//训练
//inputs: 训练输入数据
extern "C" EXPORT void train(const float* inputs)
{
    pNet->Train(inputs);
}

//设置偏移量(用于零均值化)
//offsets: 偏移量
extern "C" EXPORT void setOffset(const float* offsets)
{
    if (pOffset != nullptr)
    {
        delete[] pOffset;
        //pOffset = nullptr;
    }
    pOffset = new float[INPUT_LEN];
    std::memcpy(pOffset, offsets, INPUT_LEN * sizeof(float));
}

//压缩文件头
typedef struct tagZGHEADER
{
    //压缩文件头标识
    const char z = 'Z';
    const char g = 'G';
    //偏移量数组大小(字节)
    uint32_t offsetSize = 0;
    //压缩数据大小(字节)
    uint32_t zipSize = 0;
}zgHeader, *pZgHeader;

//写入文件头和偏移量
extern "C" EXPORT void writeHeaderAndOffset()
{
    //初始化文件头
    zgHeader header;
    header.offsetSize = INPUT_LEN * sizeof(float);
    header.zipSize = OUTPUT_LEN * sizeof(float) * 4096;
    //写入文件头和偏移量
    std::ofstream ofs(zipPath, std::ofstream::binary);
    if (!ofs.fail())
    {
        ofs.write((char*)&header, sizeof(header));
        ofs.write((char*)pOffset, header.offsetSize);
    }
    ofs.close();
}

//写入权值
extern "C" EXPORT void writeWeights()
{
    std::ofstream ofs(zipPath, std::ofstream::binary | std::ofstream::app);
    if (!ofs.fail())
    {
        //依次获取各节点权值数组
        float weights[INPUT_LEN];
        for (unsigned i = 0; i < OUTPUT_LEN; ++i)
        {
            pNet->GetWeights(i, weights);
            //写入权值数组
            ofs.write((char*)weights, sizeof(weights));
        }
    }
    ofs.close();
}

//写入数据
//inputs: 输入数据,大小为OUTPUT_LEN
extern "C" EXPORT void write(const float* inputs)
{
    std::ofstream ofs(zipPath, std::ofstream::binary | std::ofstream::app);
    if (!ofs.fail())
    {
        //依次根据inputs获取各节点输出
        float outputs[OUTPUT_LEN];
        for (unsigned i = 0; i < OUTPUT_LEN; ++i)
        {
            outputs[i] = pNet->GetOutputs(i, inputs);
        }
        //写入输出数据
        ofs.write((char*)outputs, sizeof(outputs));
    }
    ofs.close();
}

//解压path文件为float数组至outputs,每INPUT_LEN元素为一个块的数据
//path: 压缩文件路径
//outputs: 解压出的数据
extern "C" EXPORT void unzip(const char* path, float*& outputs)
{
    zgHeader header;
    std::ifstream ifs(path, std::ofstream::binary);
    if (ifs.fail())
    {
        ifs.close();
        return;
    }
    //读取文件头
    ifs.read((char*)&header, sizeof(header));
    //验证是否为本算法产生的压缩文件
    if (header.z!='Z'||header.g!='G')
    {
        ifs.close();
        return;
    }
    //读取偏移量
    float offset[INPUT_LEN];
    ifs.read((char*)&offset, header.offsetSize);
    //转置的权重矩阵(OUTPUT_LEN*INPUT_LEN)
    float weightsT[OUTPUT_LEN][INPUT_LEN];
    for (unsigned i = 0; i < OUTPUT_LEN; ++i)
    {
        ifs.read((char*)weightsT[i], sizeof(weightsT[i]));
    }
    //压缩矩阵列向量(OUTPUT_LEN*1)
    float block[OUTPUT_LEN];
    unsigned count = 0;
    for (unsigned i = 0; i < BLOCK_COUNT; ++i)
    {
        ifs.read((char*)&block, sizeof(block));
        for (unsigned j = 0; j < INPUT_LEN; ++j)
        {
            //初始化为偏移量
            outputs[count] = offset[j];
            //加上矩阵乘法得出的值
            for (unsigned k = 0; k < OUTPUT_LEN; ++k)
            {
                outputs[count] += weightsT[k][j] * block[k];
            }
            ++count;
        }
    }
}
