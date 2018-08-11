#include "diskselectbutton.h"

diskselectbutton::diskselectbutton(QWidget *parent) : QToolButton(parent)
{
    this->setStyleSheet("QToolButton {border:none;}");
    connect(this, &QToolButton::clicked, this, &diskselectbutton::ReciveClicked);
}

void diskselectbutton::ReciveClicked(bool state)
{
    Q_UNUSED(state)
    emit SendButtonName(this->text());
}
