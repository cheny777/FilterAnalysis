#ifndef FMATH_H
#define FMATH_H
#include <vector>
using namespace std;
//1、限副滤波
/*  A值可根据实际情况调整
value为有效值，new_value为当前采样值
滤波程序返回有效的实际值  */
#define A 10
double value;
double filterS(double newd)
{
    double  new_value;
    new_value = newd;
    if ( ( new_value - value > A ) || ( value - new_value > A ))
        return value;
    return new_value;

}

//2、中位值滤波法
/*  N值可根据实际情况调整
排序采用冒泡法*/
#define N  5
double value_buf[N];
int jishu = 0;
double FfilterS(double newd)
{
    if(jishu<N)
    {
        value_buf[jishu] = newd;
        jishu++;
    }
    else
    {
        for(int i = 0;i<N-1;i++)
        {
            value_buf[i] = value_buf[i+1];
        }
        value_buf[N-1] = newd;
    }

    int i,j;
    double temp;
    for (j=0;j<=jishu;j++)
    {
        for (i=0;i<=jishu;i++)
        {
            if ( value_buf[i]>value_buf[i+1] )
            {
                temp = value_buf[i];
                value_buf[i] = value_buf[i+1];
                value_buf[i+1] = temp;
            }
        }
    }
    return value_buf[(jishu-1)/2];
}

/*均值滤波*/
int AV = 10;
vector<double> Average_buf;
int junzhi = 0;
void SETLowPassFilter_AverageS(int num)
{
    AV = num;
    Average_buf.clear();
    for(int i =0;i<num;i++)
    {
        Average_buf.push_back(0);
    }
    junzhi = 0;
}

double LowPassFilter_AverageS(double data)
{
    double add=0;
    double  result;

    if(junzhi<AV)
    {
        Average_buf[junzhi] = data;
        junzhi++;
    }
    else
    {
        for(int i =0 ;i<AV ;i++)
        {
            Average_buf[i] = Average_buf[i+1];
        }
        Average_buf[AV-1] = data;
    }

    for(int i=0;i<junzhi+1;i++)
    {
        add += Average_buf[i];
    }
    result=add/double(junzhi+1);
    return result;
}

// 一阶滞后滤波法
/*
取a=0-1，本次滤波结果=(1-a)本次采样值+a上次滤波结果。
B、优点：
对周期性干扰具有良好的抑制作用；
适用于波动频率较高的场合。
C、缺点：
相位滞后，灵敏度低；
滞后程度取决于a值大小；
不能消除滤波频率高于采样频率1/2的干扰信号。*/

double FILTER_A = 0.1;
void setFilter1jieS(double d)
{
    FILTER_A = d;
}

double Value;
double Filter1jieS(double newdata)
{
    Value = (int)((float)newdata * FILTER_A + (1.0 - FILTER_A) * (float)Value);
    return Value;
}

/*低通滤波2*/
int num_x = 0;      //一个过渡值，对于同方向变化量大的数据，num_x越大
float k_x = 0.3;    //表示对新读入的数据的信任度，取值范围0-1
int old_flag = 0;   //表示第n-2个数据到第n-1个数据的变化趋势，加为1，减为0
int new_flag = 0;   //表示第n-1个数据到第n个数据的变化趋势，加为1，减为0
float old_data = 0;     //第n-1次的答案
float new_data = 0;     //第n次的输入值
#define Threshold_1 0.5    //临界点1，当数据变化值超过这个值时开始改变num_x
#define Threshold_2 1   //临界点2，当数据变化值超过这个值时开始改变k_x

double FilterDitong2S(double new_data1)
{
        new_data = new_data1;
        if (new_data - old_data > 0 )       //计算方向
            new_flag = 1;
        else new_flag = 0;
        if (new_flag == old_flag)           //变化同向
        {
            if (abs (new_data - old_data) > Threshold_1)    //变化很大则num增大
                num_x += 5;
            if (num_x >= Threshold_2)       //变化持续同向且一直变化很大则k_x增大
                k_x += 0.1;
        }
        else                //变化反向
        {
            num_x = 0;
            k_x = 0.1;
            old_flag = new_flag;
        }
        if (k_x > 0.95)  k_x = 0.95;    //系数限幅
        new_data = (1-k_x) * old_data + k_x * new_data;   //计算答案
        old_data = new_data;        //更新old_data
        return old_data;
}

#endif // FMATH_H
