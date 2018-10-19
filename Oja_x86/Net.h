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
    //inputs: ��������,Ԫ�ظ���Ϊ�ڵ����(this->nodeCount)
    //outputs: ������,Ԫ�ظ���Ϊ�ڵ����(this->nodeCount)
    void GetOutputs(float *inputs, float *outputs)const;
    ~Net();
private:
    //�ڵ���
    unsigned nodeCount;
    //������
    unsigned inputCount;
    //ָ��ڵ�ָ�������ָ��
    Node** nodes;
};

