#ifndef SVASSISTMAINWINDOW_H
#define SVASSISTMAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>

namespace Ui {
class SVAssistMainWindow;
}

class SVAssistMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SVAssistMainWindow(QWidget *parent = 0);
    ~SVAssistMainWindow();
protected:
	void timerEvent(QTimerEvent * event);
private:
	void UpdateUsages();
    void InitWindow();
private:
	int CPUUsageID;
    Ui::SVAssistMainWindow *ui;
};

#endif // SVASSISTMAINWINDOW_H
