#ifndef RWBASEBUTTON_H
#define RWBASEBUTTON_H

#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QValidator>
#include <QDebug>

class RWBaseButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RWBaseButton(QWidget *parent = nullptr);

signals:
    void SendBaseAddr(unsigned int x);
public slots:
    void OnClickedInit();
};

#endif // RWBASEBUTTON_H
