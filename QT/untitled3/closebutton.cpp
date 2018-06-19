#include "closebutton.h"

closebutton::closebutton(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet("QPushButton {border-style:none;}");
    QPixmap pixmap(tr(":/resources/Main/close.png"));
    pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
    this->setIcon(pixmap);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setIconSize(QSize(20,20));
    this->setMouseTracking(true);
    this->installEventFilter(this);
    connect(this, &QPushButton::pressed, this, &closebutton::OnClicked);
}

bool closebutton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QPushButton")) {
        if (event->type() == QEvent::Enter) {
            this->setStyleSheet("QPushButton {border-style:none;background-color: #d81e06;}");
        } else if (event->type() == QEvent::Leave) {
            this->setStyleSheet("QPushButton {border-style:none;}");
        }
    }
    return QObject::eventFilter(watched, event);
}

bool closebutton::OnClicked()
{
    exit(1);
}
