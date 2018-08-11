#include "cpuusageqcharts.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QDebug>

CCPUUseRate* CCPUUseRate::self = nullptr;
CCPUUseRate* CCPUUseRate::Instance()
{
    if(!self)
        self = new CCPUUseRate();
    return self;
}

cpuusageQcharts::cpuusageQcharts(QWidget *parent) : QWidget(parent)
{
    data = { 0 };
    maxSize = 21; // 只存储最新的 21 个数据
    maxX = 200;
    maxY = 100;
    splineSeries = new QSplineSeries();
    scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);
    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis; //定义X轴
    QPen Xpen(Qt::blue);
    Xpen.setStyle(Qt::SolidLine);
    axisX->setRange(0, maxX); //设置范围
    axisX->setGridLinePen(Xpen);
    axisX->setGridLineVisible(true); //设置是否显示网格线
    axisX->setMinorTickCount(4); //设置小刻度线的数目

    QValueAxis *axisY = new QValueAxis;
    QPen Ypen(Qt::blue);
    Ypen.setStyle(Qt::SolidLine);
    axisY->setRange(0, maxY);
    axisY->setGridLinePen(Ypen);
    axisY->setGridLineVisible(true);

    chart->setAxisX(axisX, splineSeries);
    chart->setAxisY(axisY, splineSeries);
    chart->setAxisX(axisX, scatterSeries);
    chart->setAxisY(axisY, scatterSeries);
    chart->axisX()->hide();
//    chart->axisY()->hide();
    //chart->setMargins(QMargins(0,0,0,0));
    splineSeries->append(0, 0);
    scatterSeries->append(0, 0);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    layout->setSpacing(0);
    setLayout(layout);
    CCPUUseRate::Instance()->Initialize();
    timerId = startTimer(1000);
}
void cpuusageQcharts::timerEvent(QTimerEvent *event) {
    // 产生一个数据，不停的接收到新数据
    if (event->timerId() == timerId && this->isVisible()) {

//        static FILETIME preidleTime;
//        static FILETIME prekernelTime;
//        static FILETIME preuserTime;

//        FILETIME idleTime;
//        FILETIME kernelTime;
//        FILETIME userTime;

//        GetSystemTimes(&idleTime, &kernelTime, &userTime);

//        quint64 a, b;
//        int idle, kernel, user;

//        a = (preidleTime.dwHighDateTime << 31) | preidleTime.dwLowDateTime;
//        b = (idleTime.dwHighDateTime << 31) | idleTime.dwLowDateTime;
//        idle = b - a;

//        a = (prekernelTime.dwHighDateTime << 31) | prekernelTime.dwLowDateTime;
//        b = (kernelTime.dwHighDateTime << 31) | kernelTime.dwLowDateTime;
//        kernel = b - a;

//        a = (preuserTime.dwHighDateTime << 31) | preuserTime.dwLowDateTime;
//        b = (userTime.dwHighDateTime << 31) | userTime.dwLowDateTime;
//        user = b - a;

//        int newData = (kernel + user - idle) * 100 / (kernel + user);
//        qDebug() <<newData;
//        newData = (newData < 0 ? 0 : newData);
//        preidleTime = idleTime;
//        prekernelTime = kernelTime;
//        preuserTime = userTime;
        dataReceived(CCPUUseRate::Instance()->GetCPUUseRate());
    }
    else
    {
        data.clear();
        data << 0;
        splineSeries->clear();
        scatterSeries->clear();
    }
}

void cpuusageQcharts::dataReceived(__int64 value) {
    data << value;
    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }
    // 界面被隐藏后就没有必要绘制数据的曲线了
    splineSeries->clear();
    scatterSeries->clear();
    int dx = maxX / (maxSize-1);
    int less = maxSize - data.size();
    for (int i = 0; i < data.size(); ++i) {
        splineSeries->append(less*dx+i*dx, data.at(i));
        scatterSeries->append(less*dx+i*dx, data.at(i));
    }
}

bool CCPUUseRate::Initialize()
{
    FILETIME ftIdle, ftKernel, ftUser;
    bool flag = FALSE;
    if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        m_fOldCPUIdleTime = FileTimeToDouble(ftIdle);
        m_fOldCPUKernelTime = FileTimeToDouble(ftKernel);
        m_fOldCPUUserTime = FileTimeToDouble(ftUser);

    }
    return flag;
}

__int64 CCPUUseRate::GetCPUUseRate()
{
    int nCPUUseRate = -1;
    FILETIME ftIdle, ftKernel, ftUser;
    if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        double fCPUIdleTime = FileTimeToDouble(ftIdle);
        double fCPUKernelTime = FileTimeToDouble(ftKernel);
        double fCPUUserTime = FileTimeToDouble(ftUser);
        nCPUUseRate = (int)(100.0 - (fCPUIdleTime - m_fOldCPUIdleTime)
            / (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime)
            *100.0);
        m_fOldCPUIdleTime = fCPUIdleTime;
        m_fOldCPUKernelTime = fCPUKernelTime;
        m_fOldCPUUserTime = fCPUUserTime;
    }
    return nCPUUseRate;
}

double CCPUUseRate::FileTimeToDouble(FILETIME &filetime)
{
    return (double)(filetime.dwHighDateTime * 4.294967296E9) + (double)filetime.dwLowDateTime;
}
