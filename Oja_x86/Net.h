#pragma once
#include "Node.h"
class Net
{
public:
    Net();
    //inputCount: ������
    //nodeCount: �ڵ���
    //alpha: ѧϰ�ٶ�
    Net(unsigned inputCount,unsigned nodeCount, float alpha);
    //ѵ��
    //inputs: ��������,Ԫ�ظ���Ϊ�ڵ����(this->nodeCount)
    void Train(float *inputs);
    //��ȡ���
    //index: Ҫ��ȡ����Ľڵ�λ��(��0��ʼ)
    float GetOutputs(unsigned index)const;
    ~Net();
private:
    //�ڵ���
    unsigned nodeCount;
    //������
    unsigned inputCount;
    //ָ��ڵ�ָ�������ָ��
    Node** nodes;
};

