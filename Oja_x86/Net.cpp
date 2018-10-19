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
    float delta = 0.0f;
    float* deltas = new float[this->inputCount];
    for (unsigned i = 0; i < this->inputCount; i++)
    {
        deltas[i] = 0.0f;
    }
    //依次训练每个节点
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        for (unsigned j = 0; j < this->inputCount; j++)
        {
            deltas[i] += this->nodes[i]->GetWeight(j) * this->nodes[i]->GetOutput();
        }
        delta += this->nodes[i]->GetOutput();
        this->nodes[i]->Train(inputs, delta);
    }
}

void Net::GetOutputs(float *inputs, float *outputs) const
{
    //获取每个节点的输出
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        outputs[i] = this->nodes[i]->GetOutput();
    }
}

Net::~Net()
{
    for (unsigned i = 0; i < this->nodeCount; i++)
    {
        delete nodes[i];
    }
    delete[] nodes;
}
