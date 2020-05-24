#include "SigDialog.h"

SigDialog::SigDialog(QWidget* parent)
	: QDialog(parent)
{
	sig_ui.setupUi(this);

	//connect(parent, SIGNAL(sendData(QString, int)), this, SLOT(receiveData(QString, int)));
}

SigDialog::~SigDialog()
{
	//delete sig_ui;
}

void SigDialog::receiveData(QString msg, int flag)
{
	sig_ui.show_edit->setPlainText(msg);
	if (flag == 1)
		sig_ui.sig_label->setText(QString::fromLocal8Bit("ǩ���ɹ���"));
	else if (flag == 2)
		sig_ui.sig_label->setText(QString::fromLocal8Bit("��֤ǩ���ɹ���"));
	else if(flag==3)
		sig_ui.sig_label->setText(QString::fromLocal8Bit("��֤ǩ��ʧ�ܣ�"));
}