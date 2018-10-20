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
    void Train(const float *inputs);
    //��ȡ���
    //index: Ҫ��ȡ����Ľڵ�λ��(��0��ʼ)
    //inputs: ���������,��СΪinputCount
    //����ֵ: ���
    float GetOutputs(unsigned index, const float* inputs)const;
    //��ȡȨ������
    //index: Ҫ��ȡȨ�صĽڵ�λ��(��0��ʼ)
    //weights: ָ����Ȩ�ص������ָ��
    void GetWeights(unsigned index, float* weights)const;
    ~Net();
private:
    //�ڵ���
    unsigned nodeCount;
    //������
    unsigned inputCount;
    //ָ��ڵ�ָ�������ָ��
    Node** nodes;
};

