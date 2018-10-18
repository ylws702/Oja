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
    //����4�ֽں�ͼ�����ݵ��ֽ���
    this->byteCount = size - offset;
    char* byteContent = new char[this->byteCount];
    ifs.read(byteContent, this->byteCount);
    ifs.close();
    //ÿ��ʵ��δ����洢ͼ�����ݵı�����
    unsigned actualRowBitCount = (int)header.infoHeader.biWidth*(unsigned)header.infoHeader.biBitCount;
    //����4�ֽں�ÿ��ͼ�����ݵı�����
    unsigned rowBitCount = (actualRowBitCount + 31) >> 5 << 5;
    //����4�ֽں�ͼ�����ݵı�����
    unsigned bitCount = (int)header.infoHeader.biHeight*rowBitCount;
    //��ȡ��������
    unsigned readCount = 0;
    //ʵ��δ����洢ͼ�����ݵı�����
    this->actualBitCount = actualRowBitCount * (int)header.infoHeader.biHeight;
    //��ȡ�ռ�
    this->bitContent = new char[this->actualBitCount];
    for (unsigned i = 0; i < bitCount; i++)
    {
        //�������
        if (i%rowBitCount >= actualRowBitCount)
        {
            //��С��i����СrowBits�ı�����-1
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