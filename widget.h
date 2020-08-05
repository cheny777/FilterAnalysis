#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegendMarker>
#include <list>
#include <fstream>
#include <vector>
#include <QSettings>
#include <QWheelEvent>
#include <QMouseEvent>
#include <string>

using namespace std;
using namespace QtCharts;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Widget *ui;

    vector<vector<double>> m_dataList;
    vector<vector<double>> m_dataListoriginal;
    vector<string> m_strList;
    /*创建表*/
    QChart *m_charts;
    /*创建数据源*/
    QLineSeries *series[8];
    /*初始化曲线*/
    void initChart();
    void updataD();
    double m_max ;
    double m_min ;

    int comboxindex;
    int filterindex;

protected:
    void wheelEvent(QWheelEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // WIDGET_H
