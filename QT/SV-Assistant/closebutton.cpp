#include "closebutton.h"

closebutton::closebutton(QWidget *parent) : QToolButton(parent)
{
    this->setStyleSheet("QToolButton {border:none;}");
    this->setMouseTracking(true);
    this->installEventFilter(this);
    connect(this, &QToolButton::clicked, this, [&](bool state){ if(!state) exit(1);});
}

bool closebutton::eventFilter(QObject *watched, QEvent *event)
{
    QToolButton *btn = (QToolButton*)watched;
    if (watched->inherits("QToolButton")) {
        if (event->type() == QEvent::Enter) {
            this->setStyleSheet("QToolButton {border-style:none;background-color: #d81e06;}");
        } else if (event->type() == QEvent::Leave) {
            if(!btn->isChecked())
                this->setStyleSheet("QToolButton {border-style:none;}");
            else
                this->setStyleSheet("QToolButton {border-style:none;background-color: #d81e06;}");
        }
    }
    return QObject::eventFilter(watched, event);
}
