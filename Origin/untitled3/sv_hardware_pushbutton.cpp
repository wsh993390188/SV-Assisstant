#include "sv_hardware_pushbutton.h"

SV_HARDWARE_PUSHBUTTON::SV_HARDWARE_PUSHBUTTON(QWidget *parent) : QToolButton(parent)
{
    this->updatestyle();
}

void SV_HARDWARE_PUSHBUTTON::updatestyle()
{
    this->setStyleSheet("QToolButton {border-style:none;}");
    QPixmap pixmap(":/resources/Main/selectbutton.png");
    pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
    this->setIcon(pixmap);
    this->setIconSize(QSize(20,20));
    this->setMouseTracking(true);
    this->installEventFilter(this);
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

bool SV_HARDWARE_PUSHBUTTON::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QToolButton")) {
        //QToolButton *btn = (QToolButton *)watched;
        if (event->type() == QEvent::Enter) {
            this->setStyleSheet("QToolButton {border-style:none;background-color: #81BEF7;}");
        } else if (event->type() == QEvent::Leave) {
            this->setStyleSheet("QToolButton {border-style:none;}");
        }
    }

    return QObject::eventFilter(watched, event);
}
