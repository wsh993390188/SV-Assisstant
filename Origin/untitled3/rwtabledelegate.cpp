#include "rwtabledelegate.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

RwTableDelegate::RwTableDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

//! [1]
QWidget *RwTableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QDialog *editor = new QDialog(parent);
    QPushButton *OK = new QPushButton(editor);
    QPushButton *Cancel = new QPushButton(editor);
    QLineEdit *InputData = new QLineEdit(editor);
    QHBoxLayout *editorHBoxLayput = new QHBoxLayout();
    QVBoxLayout *selectlayout = new QVBoxLayout(editor);
    InputData->setObjectName(tr("editorInputLineEdit"));
    OK->setText(tr("Done"));
    OK->setObjectName(tr("editorDoneButton"));
    Cancel->setObjectName(tr("editorCancelButton"));
    Cancel->setText(tr("Cancel"));
    selectlayout->addWidget(OK);
    selectlayout->addWidget(Cancel);
    editorHBoxLayput->addWidget(InputData);
    editorHBoxLayput->addLayout(selectlayout);
    editor->setLayout(editorHBoxLayput);
    connect(OK, &QPushButton::clicked, editor, &QDialog::accept);
    connect(Cancel, &QPushButton::clicked, editor, &QDialog::reject);
//    editor->setFrame(false);
//    editor->setMinimum(0);
//    editor->setMaximum(100);

    return editor;
}
//! [1]

//! [2]
void RwTableDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QDialog *dialog = static_cast<QDialog*>(editor);
    QObjectList ch = dialog->children();
    for(auto& a : ch)
    {
        a->objectName() == tr("editorInputLineEdit");
        QLineEdit *InputData = static_cast<QLineEdit*>(a);
        InputData->setText(QString::number(value,16).toUpper());
    }
}
//! [2]
//!
//! //! [3]
void RwTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDialog *dialog = static_cast<QDialog*>(editor);
    QObjectList ch = dialog->children();
    QLineEdit *InputData = nullptr;
    for(auto& a : ch)
    {
        a->objectName() == tr("editorInputLineEdit");
        InputData = static_cast<QLineEdit*>(a);
    }

    if(InputData)
    {
        uint value = InputData->text().toUInt(nullptr, 16);
        model->setData(index, value, Qt::EditRole);
    }
}
//! [3]

//! [4]
void RwTableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//! [4]
