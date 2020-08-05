#ifndef FMATH_H
#define FMATH_H
#include <vector>
using namespace std;
//1���޸��˲�
/*  Aֵ�ɸ���ʵ���������
valueΪ��Чֵ��new_valueΪ��ǰ����ֵ
�˲����򷵻���Ч��ʵ��ֵ  */
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

//2����λֵ�˲���
/*  Nֵ�ɸ���ʵ���������
�������ð�ݷ�*/
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

/*��ֵ�˲�*/
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

// һ���ͺ��˲���
/*
ȡa=0-1�������˲����=(1-a)���β���ֵ+a�ϴ��˲������
B���ŵ㣺
�������Ը��ž������õ��������ã�
�����ڲ���Ƶ�ʽϸߵĳ��ϡ�
C��ȱ�㣺
��λ�ͺ������ȵͣ�
�ͺ�̶�ȡ����aֵ��С��
���������˲�Ƶ�ʸ��ڲ���Ƶ��1/2�ĸ����źš�*/

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

/*��ͨ�˲�2*/
int num_x = 0;      //һ������ֵ������ͬ����仯��������ݣ�num_xԽ��
float k_x = 0.3;    //��ʾ���¶�������ݵ����ζȣ�ȡֵ��Χ0-1
int old_flag = 0;   //��ʾ��n-2�����ݵ���n-1�����ݵı仯���ƣ���Ϊ1����Ϊ0
int new_flag = 0;   //��ʾ��n-1�����ݵ���n�����ݵı仯���ƣ���Ϊ1����Ϊ0
float old_data = 0;     //��n-1�εĴ�
float new_data = 0;     //��n�ε�����ֵ
#define Threshold_1 0.5    //�ٽ��1�������ݱ仯ֵ�������ֵʱ��ʼ�ı�num_x
#define Threshold_2 1   //�ٽ��2�������ݱ仯ֵ�������ֵʱ��ʼ�ı�k_x

double FilterDitong2S(double new_data1)
{
        new_data = new_data1;
        if (new_data - old_data > 0 )       //���㷽��
            new_flag = 1;
        else new_flag = 0;
        if (new_flag == old_flag)           //�仯ͬ��
        {
            if (abs (new_data - old_data) > Threshold_1)    //�仯�ܴ���num����
                num_x += 5;
            if (num_x >= Threshold_2)       //�仯����ͬ����һֱ�仯�ܴ���k_x����
                k_x += 0.1;
        }
        else                //�仯����
        {
            num_x = 0;
            k_x = 0.1;
            old_flag = new_flag;
        }
        if (k_x > 0.95)  k_x = 0.95;    //ϵ���޷�
        new_data = (1-k_x) * old_data + k_x * new_data;   //�����
        old_data = new_data;        //����old_data
        return old_data;
}

#endif // FMATH_H
