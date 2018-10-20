#include "stdafx.h"
#include "Node.h"
#define sqrt(x) std::sqrtf(x);

Node::Node(unsigned connectionCount, float alpha)
    :connectionCount(connectionCount), alpha(alpha)
{
    this->weights = new float[connectionCount];
    float initialValue = sqrt(1.0f / connectionCount);
    for (unsigned i = 0; i < connectionCount; i++)
    {
        this->weights[i] = initialValue;
    }
}


Node::~Node()
{
    delete[] this->weights;
}


void Node::Train(const float* inputs, const float* deltas)
{
    this->output = 0.0f;
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        this->output += this->weights[i] * inputs[i];
    }
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        //OjaÑ§Ï°·½·¨
        this->weights[i] += this->alpha * this->output * (inputs[i] - deltas[i]);
    }
}

float Node::GetOutput() const
{
    return this->output;
}

float Node::GetOutput(const float* inputs) const
{
    float output = 0.0f;
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        output += this->weights[i] * inputs[i];
    }
    return output;
}

float Node::GetWeight(int i) const
{
    return this->weights[i];
}
