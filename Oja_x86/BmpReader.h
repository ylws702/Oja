#pragma once
#include <fstream>
#include "BitmapHeader.h"

class BmpReader
{
public:
    //bmppath:文件路径
    BmpReader(const char *bmppath);
    virtual ~BmpReader();
    //实际未对齐存储图像数据的比特数
    unsigned actualBitCount;
    //对齐4字节后图像数据的字节数
    unsigned byteCount;
    //获取bmp图像数据
    //i: 图像数据第i比特(从0起)
    //返回值: 比特为0返回-1.0f,比特为1返回1.0f
    float Get(int i)const;
    //获取bmp头
    //返回值: bmp头
    BITMAPHEADER GetHeader()const;
    //bmp头与图像信息间的数据
    char* headerRest;
    //headerRest元素个数
    unsigned headerRestCount;
private:
    //图像比特数据
    char* bitContent;
    //bmp头
    BITMAPHEADER header;
};

