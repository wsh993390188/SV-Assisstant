#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPalette>
#include <QString>
#include <QPushButton>

struct HomebaseInfo
{
    QHBoxLayout* layout;
	QPushButton* BaseLabel;
    QLabel* BaseInfo;
    QSpacerItem *horizontalSpace;
    QSpacerItem *BeginhorizontalSpace;
    QSpacerItem *EndhorizontalSpace;
    HomebaseInfo(QWidget* father);
};

class Homewidget : public QWidget
{
    Q_OBJECT
public:
    explicit Homewidget(QWidget *parent = nullptr);
	~Homewidget();
signals:
	void switchPage(int i);
public slots:

private:
    void Init();
private:
    HomebaseInfo *cpubaseinfo;
    HomebaseInfo *motherbroadbaseinfo;
    HomebaseInfo *memorybaseinfo;
    HomebaseInfo *gpubaseinfo;
    HomebaseInfo *diskbaseinfo;
    HomebaseInfo *monitorbaseinfo;
    HomebaseInfo *networkbaseinfo;
    HomebaseInfo *audiobaseinfo;
    QVBoxLayout *m_verticalLayout;
};

#endif // HOMEWIDGET_H
