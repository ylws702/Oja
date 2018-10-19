#include "stdafx.h"
#include "Net.h"

Net::Net() :inputCount(1), nodeCount(1)
{
    this->nodes = new Node*[this->nodeCount];
    this->nodes[0] = new Node(this->inputCount, 1);
}

Net::Net(unsigned inputCount, unsigned nodeCount, float alpha)
    : inputCount(inputCount), nodeCount(nodeCount)
{
    this->nodes = new Node*[nodeCount];
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        this->nodes[i] = new Node(this->inputCount, alpha);
    }
}

void Net::Train(float *inputs)
{
    float* deltas = new float[this->inputCount];
    for (unsigned i = 0; i < this->inputCount; i++)
    {
        deltas[i] = 0.0f;
    }
    //����ѵ��ÿ���ڵ�
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        //�ۻ�,w_i(k)[j]*y_i(k)
        for (unsigned j = 0; j < this->inputCount; j++)
        {
            deltas[j] += this->nodes[i]->GetWeight(j) * this->nodes[i]->GetOutput();
        }
        this->nodes[i]->Train(inputs, deltas);
    }
}

float Net::GetOutputs(unsigned index) const
{
    //��ȡ��index���ڵ�����(��0��ʼ)
    return this->nodes[index]->GetOutput();
}

Net::~Net()
{
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        delete nodes[i];
    }
    delete[] nodes;
}
