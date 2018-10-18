#include "stdafx.h"
#include "BmpReader.h"


BmpReader::BmpReader(const char *bmppath)
{
    std::ifstream ifs(bmppath, std::ifstream::binary);
    if (ifs.fail())
    {
        throw std::exception("open file failed.");
    }
    ifs.read((char*)&this->header, sizeof(this->header));
    unsigned size = this->header.fileHeader.bfSize;
    unsigned offset = this->header.fileHeader.bfOffBits;

    this->headerRestCount = offset - sizeof(this->header);
    this->headerRest = new char[this->headerRestCount];
    ifs.read(this->headerRest, this->headerRestCount);
    //对齐4字节后图像数据的字节数
    this->byteCount = size - offset;
    char* byteContent = new char[this->byteCount];
    ifs.read(byteContent, this->byteCount);
    ifs.close();
    //每行实际未对齐存储图像数据的比特数
    unsigned actualRowBitCount = (int)header.infoHeader.biWidth*(unsigned)header.infoHeader.biBitCount;
    //对齐4字节后每行图像数据的比特数
    unsigned rowBitCount = (actualRowBitCount + 31) >> 5 << 5;
    //对齐4字节后图像数据的比特数
    unsigned bitCount = (int)header.infoHeader.biHeight*rowBitCount;
    //读取计数变量
    unsigned readCount = 0;
    //实际未对齐存储图像数据的比特数
    this->actualBitCount = actualRowBitCount * (int)header.infoHeader.biHeight;
    //获取空间
    this->bitContent = new char[this->actualBitCount];
    for (unsigned i = 0; i < bitCount; i++)
    {
        //跳过填充
        if (i%rowBitCount >= actualRowBitCount)
        {
            //不小于i的最小rowBits的倍数再-1
            i = (i + rowBitCount - 1) / rowBitCount * rowBitCount - 1;
            continue;
        }
        this->bitContent[readCount++] = byteContent[i >> 3] >> (7 - i % 8) & 1;
    }
    delete[] byteContent;
}


BmpReader::~BmpReader()
{
    delete[] this->bitContent;
    delete[] this->headerRest;
}

float BmpReader::Get(int i)const
{
    return this->bitContent[i] == 1 ? 1.0f : -1.0f;
}

BITMAPHEADER BmpReader::GetHeader() const
{
    return this->header;
}