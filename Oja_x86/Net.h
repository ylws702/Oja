#pragma once
#include "Node.h"
class Net
{
public:
    Net();
    //inputCount: 输入数
    //nodeCount: 节点数
    //alpha: 学习速度
    Net(unsigned inputCount,unsigned nodeCount, float alpha);
    //训练
    //inputs: 输入数组,元素个数为节点个数(this->nodeCount)
    void Train(const float *inputs);
    //获取输出
    //index: 要获取输出的节点位置(从0开始)
    //inputs: 输入的数据,大小为inputCount
    float GetOutputs(unsigned index, const float* inputs)const;
    ~Net();
private:
    //节点数
    unsigned nodeCount;
    //输入数
    unsigned inputCount;
    //指向节点指针数组的指针
    Node** nodes;
};

