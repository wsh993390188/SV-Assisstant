#pragma once

#include <QToolButton>
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
#include <windows.h>

class MemoryButton : public QToolButton
{
	Q_OBJECT
public:
	MemoryButton(QWidget *parent = nullptr);
	~MemoryButton();
signals:
	void SendBaseAddr(ULARGE_INTEGER x);
public slots:
	void OnClickedInit();
};
