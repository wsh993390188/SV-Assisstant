#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>

class closebutton : public QToolButton
{
    Q_OBJECT
public:
    explicit closebutton(QWidget *parent = nullptr);

signals:
private:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // CLOSEBUTTON_H
