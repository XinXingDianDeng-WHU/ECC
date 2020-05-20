#pragma once
#include <QtWidgets>
#include <qdialog.h>
#include "ui_sigDialog.h"
class SigDialog :
	public QDialog
{
	Q_OBJECT;

public:
	SigDialog(QWidget* parent = Q_NULLPTR);
	~SigDialog();

private:
	Ui::sigDialog sig_ui;

private slots:
	void receiveData(QString msg, int flag);

};

