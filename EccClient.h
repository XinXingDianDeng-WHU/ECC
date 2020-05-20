#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_ECC.h"


class EccClient : public QMainWindow
{
	Q_OBJECT

public:
	EccClient(QWidget *parent = Q_NULLPTR);

private:
	Ui::ECCClass ECC_ui;
	
signals:
	void sendData(QString msg, int flag);	//传数据的信号

private slots:
	void inChooseFile();
	void outChooseFile();
	void encryption();
	void decrypt();
	void signature();
	void check();
};
