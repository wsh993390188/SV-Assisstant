#include "mainctrlbutton.h"

mainctrlbutton::mainctrlbutton(QWidget *parent) : QPushButton(parent)
{
    QPixmap pixmap(tr(":/resources/Main/minsize_grey.png"));
    pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
    this->setIcon(pixmap);
    this->setIconSize(QSize(20,20));
    this->setStyleSheet("QPushButton {border:none;}");
    this->setMouseTracking(true);
    this->installEventFilter(this);
}

bool mainctrlbutton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QPushButton")) {
        if (event->type() == QEvent::Enter) {
            QPixmap pixmap(tr(":/resources/Main/minsize_black.png"));
            pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
            this->setIcon(pixmap);
            this->setIconSize(QSize(20,20));
            this->setStyleSheet("QPushButton {border-style:none;background-color: #81BEF7;}");
        } else if (event->type() == QEvent::Leave) {
            QPixmap pixmap(tr(":/resources/Main/minsize_grey.png"));
            pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
            this->setIcon(pixmap);
            this->setIconSize(QSize(20,20));
            this->setStyleSheet("QPushButton {border-style:none;}");
        }
    }
    return QObject::eventFilter(watched, event);
}
