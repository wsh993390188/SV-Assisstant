#ifndef SETTINGBUTTON_H
#define SETTINGBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QPointF>
#include <QSize>
#include <QDebug>

class settingbutton : public QToolButton
{
    Q_OBJECT
public:
    explicit settingbutton(QWidget *parent = nullptr);

signals:

public slots:
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QMenu *m_Menu;
};

#endif // SETTINGBUTTON_H
