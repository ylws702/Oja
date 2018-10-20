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

void Net::Train(const float *inputs)
{
    float* deltas = new float[this->inputCount];
    for (unsigned i = 0; i < this->inputCount; i++)
    {
        deltas[i] = 0.0f;
    }
    //依次训练每个节点
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        //累积,w_i(k)[j]*y_i(k)
        float iOutput = this->nodes[i]->GetOutput();
        for (unsigned j = 0; j < this->inputCount; j++)
        {
            deltas[j] += this->nodes[i]->GetWeight(j) * iOutput;
        }
        this->nodes[i]->Train(inputs, deltas);
    }
    delete[] deltas;
}

float Net::GetOutputs(unsigned index, const float* inputs) const
{
    //获取第index个节点的输出(从0开始)
    return this->nodes[index]->GetOutput(inputs);
}

Net::~Net()
{
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        delete nodes[i];
    }
    delete[] nodes;
}
