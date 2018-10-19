#pragma once
#include <cmath>
class Node
{
public:
    //connectionCount: 节点输入连接数量
    //alpha: 学习速度
    Node(unsigned connectionCount, float alpha);
    ~Node();
    //训练节点
    //inputs: 输入数组,元素个数为节点输入连接数量(this->connectionCount)
    //delta: 减去前面特征值,w_1(k)y_1(k)+w_2(k)y_2(k)+...
    void Train(float* inputs, float* deltas);
    //获取输出
    //返回值: 输出值
    float GetOutput()const;
    //获取权值
    //返回值: 第i个连接的权值
    float GetWeight(int i)const;
private:
    //连接权重数组指针
    float* weights;
    //输出
    float output = 0.0f;
    //节点输入连接数量
    unsigned connectionCount;
    //学习速度
    float alpha;
};

