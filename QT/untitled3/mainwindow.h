#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStringList>
#include <QTextStream>
#include <QToolButton>
#include <QThread>
#include <QMutex>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QMap>
#include <QDebug>
#include "cpulib.h"
#include "diskdefination.h"
#include "gpu.h"
#include "Driverdll.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void Init();
    void keyPressEvent(QKeyEvent *event);
    void diskupdate();
    void updateMemory();
    void gpuupdate();
	void OtherUpdateData();
    bool cleardir(const QString &dirName);
private slots:
    void send();
    void on_disktoolButton_clicked(QString x);

private:
    int gputimeID;
    int timeID;
    Ui::MainWindow *ui;
    QMap<QString, QToolButton*> DiskButtonSelect;
};

#endif // MAINWINDOW_H
