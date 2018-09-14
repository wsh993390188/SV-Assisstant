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

class IOButton : public QToolButton
{
	Q_OBJECT
public:
	explicit IOButton(QWidget *parent = nullptr);
	~IOButton();
signals:
	void SendBaseAddr(unsigned int x);
public slots:
	void OnClickedInit();
};

class IOIndexButton : public QToolButton
{
	Q_OBJECT
public:
	explicit IOIndexButton(QWidget *parent = nullptr);
	~IOIndexButton();
signals:
	void SendBaseAddr(uint Index, uint data);
public slots:
	void OnClickedInit();
};