#include "settingbutton.h"

settingbutton::settingbutton(QWidget *parent) : QToolButton(parent), m_Menu(new QMenu(this))
{
    QPixmap pixmap(tr(":/resources/Main/setting_grey.png"));
    pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
    this->setIcon(pixmap);
    this->setIconSize(QSize(20,20));
    this->setStyleSheet("QToolButton {border:none;} QToolButton::menu-indicator { image: None;}");
    this->setMouseTracking(true);
    this->installEventFilter(this);
    m_Menu->addMenu(tr("0"));
    m_Menu->addMenu(tr("1"));
    m_Menu->addMenu(tr("3"));
    m_Menu->setStyleSheet("QMenu {border:none;} QMenu::menu-indicator { image: None;}");
    this->setPopupMode(QToolButton::InstantPopup);
    this->setMenu(m_Menu);
}

bool settingbutton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QToolButton")) {
        //QToolButton *btn = (QToolButton *)watched;
        if (event->type() == QEvent::Enter) {
            QPixmap pixmap(tr(":/resources/Main/setting_black.png"));
            pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
            this->setIcon(pixmap);
            this->setIconSize(QSize(20,20));
            this->setStyleSheet("QToolButton {border-style:none;background-color: #81BEF7;} QToolButton::menu-indicator { image: None;}");
        } else if (event->type() == QEvent::Leave) {
            QPixmap pixmap(tr(":/resources/Main/setting_grey.png"));
            pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio);
            this->setIcon(pixmap);
            this->setIconSize(QSize(20,20));
            this->setStyleSheet("QToolButton {border-style:none;} QToolButton::menu-indicator { image: None;}");
        }
    }

    return QObject::eventFilter(watched, event);
}
