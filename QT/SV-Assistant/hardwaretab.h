#ifndef HARDWARETAB_H
#define HARDWARETAB_H

#include <QWidget>
#include <QString>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>

class HardwareButton : public QToolButton
{
    Q_OBJECT
public:
    explicit HardwareButton(QWidget *parent = nullptr);

signals:

public slots:

private:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    volatile bool Changecolor;
};

class HardwareTab : public QWidget
{
    Q_OBJECT
public:
    explicit HardwareTab(QWidget *parent = nullptr);

signals:
	void SendHardwareTabIndex(int index);
public slots:

private:
    void Init();
private:
    HardwareButton* CPUButton;
    HardwareButton* MotherBroadButton;
    HardwareButton* MemoryButton;
    HardwareButton* GPUButton;
    HardwareButton* DiskButton;
    HardwareButton* MonitorButton;
    HardwareButton* AudioButton;
    HardwareButton* NetworkButton;
    HardwareButton* OtherButton;
    QVBoxLayout *layout;
    QSpacerItem *verticalSpace;
};

#endif // HARDWARETAB_H
