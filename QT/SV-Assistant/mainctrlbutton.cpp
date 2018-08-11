#include "mainctrlbutton.h"

mainctrlbutton::mainctrlbutton(QWidget *parent) : QToolButton(parent)
{
    this->setStyleSheet("QToolButton {border:none;}");
    this->setMouseTracking(true);
    this->installEventFilter(this);
}

bool mainctrlbutton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QToolButton")) {
        if (event->type() == QEvent::Enter) {
            this->setStyleSheet("QToolButton {border-style:none;background-color: #81BEF7;}");
        } else if (event->type() == QEvent::Leave) {
            this->setStyleSheet("QToolButton {border-style:none;}");
        }
    }
    return QObject::eventFilter(watched, event);
}
