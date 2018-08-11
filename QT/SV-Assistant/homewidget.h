#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPalette>
#include <QString>

struct HomebaseInfo
{
    QHBoxLayout* layout;
    QLabel* BaseLabel;
    QLabel* BaseInfo;
    QSpacerItem *horizontalSpace;
    QSpacerItem *BeginhorizontalSpace;
    HomebaseInfo(QWidget* father);
};

class Homewidget : public QWidget
{
    Q_OBJECT
public:
    explicit Homewidget(QWidget *parent = nullptr);
	~Homewidget();
signals:

public slots:

private:
    void Init();
private:
    HomebaseInfo *cpubaseinfo;
    HomebaseInfo *motherbroadbaseinfo;
    HomebaseInfo *memorycpubaseinfo;
    HomebaseInfo *gpubaseinfo;
    HomebaseInfo *diskbaseinfo;
    HomebaseInfo *monitorbaseinfo;
    HomebaseInfo *networkbaseinfo;
    HomebaseInfo *audiobaseinfo;
    QVBoxLayout *m_verticalLayout;
};

#endif // HOMEWIDGET_H
