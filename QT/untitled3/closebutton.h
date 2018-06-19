#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>

class closebutton : public QPushButton
{
    Q_OBJECT
public:
    explicit closebutton(QWidget *parent = nullptr);

signals:

public slots:
    bool OnClicked();
private:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // CLOSEBUTTON_H
