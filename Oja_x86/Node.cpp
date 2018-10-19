#include "stdafx.h"
#include "Node.h"


Node::Node(unsigned connectionCount, float alpha)
    :connectionCount(connectionCount), alpha(alpha)
{
    this->weights = new float[connectionCount];
    for (unsigned i = 0; i < connectionCount; i++)
    {
        this->weights[i] = 0.0f;
    }
}


Node::~Node()
{
    delete[] this->weights;
}


void Node::Train(float* inputs, float delta)
{
    output = 0.0f;
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        output += weights[i] * inputs[i];
    }
    for (unsigned i = 0; i < this->connectionCount; i++)
    {
        //OjaÑ§Ï°·½·¨
        weights[i] += this->alpha *output*(weights[i] - delta);
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
