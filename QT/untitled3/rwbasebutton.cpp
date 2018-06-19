#include "rwbasebutton.h"

RWBaseButton::RWBaseButton(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet(QString(
                        "QPushButton {border-style:none;\
                        border:1px solid #B6B6B6;\
                        color:#57595B;\
                        padding:5px;\
                        min-height:15px;\
                        border-radius:5px;\
                        background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #E4E4E4,stop:1 #E4E4E4);\
                    }"));

    connect(this, &QPushButton::clicked, this, &RWBaseButton::OnClickedInit);
}

void RWBaseButton::OnClickedInit()
{
    QDialog diag;
    QVBoxLayout* dialogvlayout = new QVBoxLayout();
    QVBoxLayout* dialogvlayout_2 = new QVBoxLayout();
    QHBoxLayout* dialoghlayout = new QHBoxLayout(&diag);
    QHBoxLayout* dialoghlayout_2 = new QHBoxLayout();

    QPushButton* Done = new QPushButton(&diag);
    QPushButton* Close = new QPushButton(&diag);
    QLabel* Addr = new QLabel(&diag);
    QLineEdit* InputAddr = new QLineEdit(&diag);
    QCheckBox *check4GB = new QCheckBox(&diag);
    check4GB->setText("Open 4GB Gate");
    Addr->setText(tr("Start Address: "));
    Done->setText(tr("Done"));
    Close->setText(tr("Cancel"));
    QRegExp regx("^[a-fA-F0-9]{0,8}$");
    InputAddr->setValidator(new QRegExpValidator(regx, &diag));
    dialogvlayout->addWidget(Done);
    dialogvlayout->addWidget(Close);
    dialoghlayout_2->addWidget(Addr);
    dialoghlayout_2->addWidget(InputAddr);
    dialogvlayout_2->addWidget(check4GB);
    dialogvlayout_2->addLayout(dialoghlayout_2);
    dialoghlayout->addLayout(dialogvlayout_2);
    dialoghlayout->addLayout(dialogvlayout);
    connect(Done, &QPushButton::clicked, &diag, &QDialog::accept);
    connect(Close, &QPushButton::clicked, &diag, &QDialog::reject);
    connect(check4GB, &QCheckBox::stateChanged, [&](int state)
    {
        if (state == Qt::Checked) // "选中"
        {
            QRegExp regx("^[a-fA-F0-9]*$");
            InputAddr->setValidator(new QRegExpValidator(regx, &diag));
        }
        else // 未选中 - Qt::Unchecked
        {
            QRegExp regx("^[a-fA-F0-9]{0,8}$");
            InputAddr->setValidator(new QRegExpValidator(regx, &diag));
        }
    });
    diag.setLayout(dialoghlayout);

    if(diag.exec() == QDialog::Accepted && !InputAddr->text().isEmpty())
    {
        uint baseaddr = InputAddr->text().toUInt(nullptr, 16);
        this->setText(tr("Base Address: ") + QString::number(baseaddr, 16).toUpper());
        qDebug() << "Dialog Accepted";
        emit SendBaseAddr(baseaddr);
    }
    else
    {
        qDebug() << "Dialog Reject";
    }
}



