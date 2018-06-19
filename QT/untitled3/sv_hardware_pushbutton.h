#ifndef SV_HARDWARE_PUSHBUTTON_H
#define SV_HARDWARE_PUSHBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QSize>
#include <QDebug>
class SV_HARDWARE_PUSHBUTTON : public QToolButton
{
    Q_OBJECT
public:
    explicit SV_HARDWARE_PUSHBUTTON(QWidget *parent = nullptr);

signals:

public slots:
private:
    void updatestyle();
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // SV_HARDWARE_PUSHBUTTON_H
