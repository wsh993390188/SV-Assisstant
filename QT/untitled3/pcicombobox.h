#ifndef PCICOMBOBOX_H
#define PCICOMBOBOX_H

#include <QComboBox>
#include <QString>
class pcicombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit pcicombobox(QWidget *parent = nullptr);

signals:
    void SendPCIName(QString PCIName);
private:

};

#endif // PCICOMBOBOX_H
