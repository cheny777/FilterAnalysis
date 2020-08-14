#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "fmath.h"
#include <QMessageBox>
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QStringList FList;
    FList<<QString::fromLocal8Bit("无")<<QString::fromLocal8Bit("一阶滞后低通滤波")<<QString::fromLocal8Bit("限幅滤波")<<QString::fromLocal8Bit("中值滤波")<<QString::fromLocal8Bit("均值滤波")<<QString::fromLocal8Bit("低通滤波带方向")<<QString::fromLocal8Bit("低通+均值");
    ui->comboBox_2->addItems(FList);

    m_dataList.clear();

    m_charts = new QChart;
    ui->widget->setChart(m_charts);
    ui->widget->setRenderHint(QPainter::Antialiasing);
    m_charts->setAnimationOptions(QChart::SeriesAnimations);//设置曲线动画模式
    m_charts->createDefaultAxes(); //建立默认坐标轴，不需要QValueAxis
    m_charts->legend()->hide(); //隐藏图例
    m_charts->setMargins(QMargins(3,3,3,3));


    m_charts->setTheme(QChart::ChartThemeLight);//设置白色主题
    m_charts->setDropShadowEnabled(true);//背景阴影
    m_charts->setAutoFillBackground(true);  //设置背景自动填充
    m_charts->setTitleBrush(QBrush(QColor(0,0,255)));//设置标题Brush
    m_charts->setTitleFont(QFont("微软雅黑"));//设置标题字体
    m_charts->setTitle(QString::fromLocal8Bit("曲线图"));

    m_max = -100000;
    m_min = +1000000;

    for(int i =0;i<8;i++)
    {
        series[i] = new QLineSeries;
    }
    series[0]->setPen(QPen(QColor("#FF0000"),1,Qt::SolidLine));
    series[1]->setPen(QPen(QColor("#FFFF00"),1,Qt::SolidLine));
    series[2]->setPen(QPen(QColor("#00FF00"),1,Qt::SolidLine));
    series[3]->setPen(QPen(QColor("#00FFCC"),1,Qt::SolidLine));
    series[4]->setPen(QPen(QColor("#0000FF"),1,Qt::SolidLine));
    series[5]->setPen(QPen(QColor("#CC00FF"),1,Qt::SolidLine));
    series[6]->setPen(QPen(QColor("#A3BB44"),1,Qt::SolidLine));
    series[7]->setPen(QPen(QColor("#33AECC"),1,Qt::SolidLine));

    QStringList numlist;
    numlist<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    ui->comboBox->addItems(numlist);

    ui->widget->setRubberBand(QChartView::RectangleRubberBand);
    comboxindex = 0;

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(100);

    filterindex = 0;
    SETLowPassFilter_AverageS(10);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString str = QFileDialog::getOpenFileName(NULL,"open","./");
    ifstream infile;
    infile.open(str.toStdString(),ios::in);

    m_max = -100000;
    m_min = +1000000;


    if(infile.is_open())
    {
        m_dataList.clear();
        m_dataListoriginal.clear();

        char buffer[256];
        while (!infile.eof())
        {
            infile.getline(buffer,256);
            string str = string(buffer);
            m_strList.push_back(str);
            // qDebug()<<QString(buffer);
            if(buffer[0] == '[')
            {
                if(buffer[1] == 'd' && buffer[2] =='a'&&buffer[3] == 't'&&buffer[4] == 'a' )
                {
                    char per;
                    while (!infile.eof())
                    {
                        infile.getline(buffer,256);

                        istringstream ss(buffer);

                        vector<double> dou;
                        double d;
                        for(int i =0;i<8;i++)
                        {
                            ss>>d>>per;

                            if(d>m_max)
                            {
                                m_max = d;
                            }
                            if(d<m_min)
                            {
                                m_min = d;
                            }
                            dou.push_back(d);
                        }
                        m_dataList.push_back(dou);
                        m_dataListoriginal.push_back(dou);
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox::information(NULL,"提示","打开失败");
    }

    qDebug()<<m_dataList.size();


    //   for(int i =0;i<m_dataList.size();i++)
    //   {
    //       for(int j =0;j<m_dataList[i].size();j++)
    //       {
    //           cout<<m_dataList[i][j];
    //       }
    //       cout<<endl;
    //   }
    initChart();

}

void Widget::initChart()
{

    //m_charts->axisY()->setRange(m_min,m_max);//默认坐标轴限定范围
    //m_charts->axisX()->setRange(0, m_dataList.size());//默认坐标轴限定范围

    updataD();
}

void Widget::updataD()
{

    for(int i =0;i<8;i++)
    {
        series[i]->clear();
    }

    for(int i =0;i<m_dataList.size();i++)
    {
        for(int j =0;j<m_dataList[i].size();j++)
        {
            series[j]->append(i,m_dataList[i][j]);
        }
    }

    for(int i =0;i<8;i++)
    {
        m_charts->addSeries(series[i]);//输入数据
    }
    // m_charts->axisY()->setRange(m_min, m_max);//默认坐标轴限定范围
}

void Widget::wheelEvent(QWheelEvent *e)
{
    QRect rect = ui->widget->geometry();

    if(rect.contains(e->x(),e->y()))
    {
        if(e->delta()>0)
        {
            m_charts->zoom(0.9);
        }else
        {
            m_charts->zoom(1.1);
        }
    }

}

void Widget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QRect rect = ui->widget->geometry();

    if(rect.contains(e->x(),e->y()))
    {
        m_charts->zoomReset();
    }
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    for(int i =0;i<8;i++)
    {
        if(i != index)
        {
            series[i]->setVisible(false);
        }
        else
        {
            series[i]->setVisible(true);
        }
    }
    comboxindex = index;
    ui->comboBox_2->setCurrentIndex(0);
}

void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    filterindex = index;
    switch (index) {
    case 1:/*低通1*/
        for(int i =0;i<m_dataList.size();i++)
        {
            m_dataList[i][comboxindex] = Filter1jieS(m_dataListoriginal[i][comboxindex]);
        }
        break;
    case 2:
        break;
    case 3:/*中值*/
        for(int i =0;i<m_dataList.size();i++)
        {
            m_dataList[i][comboxindex] = FfilterS(m_dataListoriginal[i][comboxindex]);
        }
        break;
    case 4:/*均值*/
        for(int i =0;i<m_dataList.size();i++)
        {
            m_dataList[i][comboxindex] = LowPassFilter_AverageS(m_dataListoriginal[i][comboxindex]);
        }
        break;
    case 5:
        /*低通*/
        for(int i =0;i<m_dataList.size();i++)
        {
            m_dataList[i][comboxindex] = FilterDitong2S(m_dataListoriginal[i][comboxindex]);
        }
        break;
    case 6:/*低通加均值*/
        for(int i =0;i<m_dataList.size();i++)
        {
            m_dataList[i][comboxindex] = LowPassFilter_AverageS(Filter1jieS(m_dataListoriginal[i][comboxindex]));
        }
        break;
    default:
        break;
    }
    if(index>0 && index <8)
    {
        series[comboxindex]->clear();
        for(int i =0;i<m_dataList.size();i++)
        {
            series[comboxindex]->append(i,m_dataList[i][comboxindex]);
        }
    }
}

void Widget::on_pushButton_2_clicked()
{

    QString sss = QFileDialog::getSaveFileName(NULL,"save","./test.dat","*");
    ofstream outfile;
    outfile.open(sss.toStdString(),ios::out|ios::trunc);
    if(outfile.is_open())
    {
        for(int i =0;i<m_strList.size();i++)
        {
            outfile<<m_strList[i]<<endl;
        }
        for(int i =0;i<m_dataList.size();i++)
        {
            for(int j=0;j<m_dataList[i].size();j++)
            {
                outfile<<std::setiosflags(std::ios::fixed)<< std::setprecision(4)<<m_dataList[i][j]<<',';
            }
            outfile<<endl;
        }
    }
    else
    {
        QMessageBox::information(NULL,"error","打开失败");
    }

}


void Widget::on_doubleSpinBox_valueChanged(double arg1)
{
    //ui->horizontalSlider->setValue(arg1);
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    qDebug()<<value;
    switch (filterindex) {
    case 1:
        setFilter1jieS(value/100.0);
        ui->doubleSpinBox->setValue(value/100.0);
        break;
    case 4:
        SETLowPassFilter_AverageS(value);
        ui->doubleSpinBox->setValue(value);
        break;

    default:
        break;
    }
    on_comboBox_2_currentIndexChanged(filterindex);
}
