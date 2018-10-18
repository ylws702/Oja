#pragma once
#include <fstream>
#include "BitmapHeader.h"

class BmpReader
{
public:
    //bmppath:�ļ�·��
    BmpReader(const char *bmppath);
    virtual ~BmpReader();
    //ʵ��δ����洢ͼ�����ݵı�����
    unsigned actualBitCount;
    //����4�ֽں�ͼ�����ݵ��ֽ���
    unsigned byteCount;
    //��ȡbmpͼ������
    //i: ͼ�����ݵ�i����(��0��)
    //����ֵ: ����Ϊ0����-1.0f,����Ϊ1����1.0f
    float Get(int i)const;
    //��ȡbmpͷ
    //����ֵ: bmpͷ
    BITMAPHEADER GetHeader()const;
    //bmpͷ��ͼ����Ϣ�������
    char* headerRest;
    //headerRestԪ�ظ���
    unsigned headerRestCount;
private:
    //ͼ���������
    char* bitContent;
    //bmpͷ
    BITMAPHEADER header;
};

