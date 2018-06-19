#ifndef DISKSELECTBUTTON_H
#define DISKSELECTBUTTON_H

#include <QToolButton>
#include <QString>

class diskselectbutton : public QToolButton
{
    Q_OBJECT
public:
    explicit diskselectbutton(QWidget *parent = nullptr);

signals:
    void SendButtonName(QString x);
public slots:
    void ReciveClicked(bool state);
};

#endif // DISKSELECTBUTTON_H
