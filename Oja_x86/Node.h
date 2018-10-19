#pragma once
#include <cmath>
class Node
{
public:
    //connectionCount: �ڵ�������������
    //alpha: ѧϰ�ٶ�
    Node(unsigned connectionCount, float alpha);
    ~Node();
    //ѵ���ڵ�
    //inputs: ��������,Ԫ�ظ���Ϊ�ڵ�������������(this->connectionCount)
    //delta: ��ȥǰ������ֵ
    void Train(float* inputs, float delta);
    //��ȡ���
    //����ֵ: ���ֵ
    float GetOutput()const;
    //��ȡȨֵ
    //����ֵ: ��i�����ӵ�Ȩֵ
    float GetWeight(int i)const;
private:
    //����Ȩ������ָ��
    float* weights;
    //���
    float output = 0.0f;
    //�ڵ�������������
    unsigned connectionCount;
    //ѧϰ�ٶ�
    float alpha;
};

