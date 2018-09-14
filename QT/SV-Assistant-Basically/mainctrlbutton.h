#ifndef MAINCTRLBUTTON_H
#define MAINCTRLBUTTON_H

#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

class mainctrlbutton : public QToolButton
{
    Q_OBJECT
public:
    explicit mainctrlbutton(QWidget *parent = nullptr);
private:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINCTRLBUTTON_H
