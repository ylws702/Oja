#include "stdafx.h"
#include "Node.h"


Node::Node(unsigned connectionCount, float alpha)
    :connectionCount(connectionCount), alpha(alpha)
{
    this->weights = new float[connectionCount];
    for (unsigned i = 0; i < connectionCount; i++)
    {
        this->weights[i] = 1.0f / connectionCount;
    }
}


Node::~Node()
{
    delete[] this->weights;
}


void Node::Train(float* inputs, float* deltas)
{
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        output += weights[i] * inputs[i];
    }
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        //OjaÑ§Ï°·½·¨
         weights[i] += this->alpha *output*(weights[i] - deltas[i]);
    }
}

float Node::GetOutput() const
{
    return output;
}

float Node::GetWeight(int i) const
{
    return this->weights[i];
}
