#ifndef SVASSISTMAINWINDOW_H
#define SVASSISTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SVAssistMainWindow;
}

class SVAssistMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SVAssistMainWindow(QWidget *parent = 0);
    ~SVAssistMainWindow();
private:
    void InitWindow();
private:
    Ui::SVAssistMainWindow *ui;
};

#endif // SVASSISTMAINWINDOW_H
