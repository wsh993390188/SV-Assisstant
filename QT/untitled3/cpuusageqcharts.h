#ifndef CPUUSAGEQCHARTS_H
#define CPUUSAGEQCHARTS_H

#include <QChartView>
#include <QTimer>
#include <QList>
#include <QLineSeries>
#include <QHBoxLayout>
#include "Windows.h"

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QScatterSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class cpuusageQcharts : public QWidget
{
    Q_OBJECT
public:
    explicit cpuusageQcharts(QWidget *parent = nullptr);
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(__int64 value);
    int timerId;
    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    QList<double> data; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;
};

class CCPUUseRate
{
public:
    bool Initialize();
    //调用Initialize后要等待1秒再调用此函数
    __int64 GetCPUUseRate();
    static CCPUUseRate* Instance();
private:
    double FileTimeToDouble(FILETIME &filetime);
private:
    double m_fOldCPUIdleTime;
    double m_fOldCPUKernelTime;
    double m_fOldCPUUserTime;
    static CCPUUseRate *self;
};

#endif // CPUUSAGEQCHARTS_H
