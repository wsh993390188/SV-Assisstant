#ifndef SV_MASTER_MAINWINDOWS_H
#define SV_MASTER_MAINWINDOWS_H

#include <QMainWindow>

namespace Ui {
class SV_Master_Mainwindows;
}

class SV_Master_Mainwindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit SV_Master_Mainwindows(QWidget *parent = 0);
    ~SV_Master_Mainwindows();

private:
    Ui::SV_Master_Mainwindows *ui;
};

#endif // SV_MASTER_MAINWINDOWS_H
