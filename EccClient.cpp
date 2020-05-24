#include "EccClient.h"
#include "SigDialog.h"
#include "ECC.h"

EccClient::EccClient(QWidget *parent)
	: QMainWindow(parent)
{
	ECC_ui.setupUi(this);

	connect(ECC_ui.input_toolButton, SIGNAL(clicked()), this, SLOT(inChooseFile()));
	connect(ECC_ui.output_toolButton, SIGNAL(clicked()), this, SLOT(outChooseFile()));
	connect(ECC_ui.encryption_button, SIGNAL(clicked()), this, SLOT(encryption()));
	connect(ECC_ui.decrypt_button, SIGNAL(clicked()), this, SLOT(decrypt()));
	connect(ECC_ui.signature_button, SIGNAL(clicked()), this, SLOT(signature()));
	connect(ECC_ui.check_button, SIGNAL(clicked()), this, SLOT(check()));
}

/*ѡ�������ļ�*/
void EccClient::inChooseFile()
{	
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("ѡ���ļ�"), ".", tr("txt(*.txt);;(*.*)"));
	ECC_ui.inputFile_lineEdit->setText(dir);
}
/*ѡ������ļ�*/
void EccClient::outChooseFile()
{
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("ѡ���ļ�"), ".", tr("txt(*.txt);;(*.*)"));
	ECC_ui.outputFile_lineEdit->setText(dir);
}
/*���� ���빫Կ ��Ϊ����Ĭ��*/
void EccClient::encryption()
{
	ECC ecc = ECC();
	//PK��Ҫ�����ʽx,y 
	QRegExp rex("\\d+,\\d+");
	QString PK = ECC_ui.PK_lineEdit->text();
	if (!ECC_ui.PK_lineEdit->text().isEmpty()) {
		if (rex.exactMatch(PK))
		{
			QStringList list = ECC_ui.PK_lineEdit->text().split(",");
			qlonglong x = list.at(0).toLongLong();
			qlonglong y = list.at(1).toLongLong();			
			Point p = Point(x, y);
			ecc.setPK(p);			
		}
		else {
			QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��Կ��ʽ����"));
		}
	}	
	QString infile = ECC_ui.inputFile_lineEdit->text();
	QString outfile = ECC_ui.outputFile_lineEdit->text();
	std::string str1 = std::string(infile.toLocal8Bit());
	std::string str2 = std::string(outfile.toLocal8Bit());
	if (infile.isEmpty() || outfile.isEmpty()) {
		QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ļ�/����ļ�����Ϊ�գ�"));
	}
	else {
		ecc.encodefile(str1, str2); //�����ļ�,����ļ�
	}
	
	
}
/*���� Ҫ��˽Կ������Ĭ��*/
void EccClient::decrypt()
{
	QString str = ECC_ui.SK_lineEdit->text();
	ECC ecc = ECC();
	//���ж�
	if (!str.isEmpty()) {
		qlonglong RK = str.toLongLong();				
		ecc.setRK(RK);//������longlong		
	}	
	QString infile = ECC_ui.inputFile_lineEdit->text();
	QString outfile = ECC_ui.outputFile_lineEdit->text();
	std::string str1 = std::string(infile.toLocal8Bit());
	std::string str2 = std::string(outfile.toLocal8Bit());
	if (infile.isEmpty() || outfile.isEmpty()) {
		QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ļ�/����ļ�����Ϊ�գ�"));
	}
	else {
		ecc.decodefile(str1, str2);//�����ļ�������ļ�
	}
}
/*ǩ�� ����˽Կ ����Ĭ��*/
void EccClient::signature()
{
	QString msg = ECC_ui.signature_edit->toPlainText();
	if (!msg.isEmpty()) {
		QString signed_msg;	//ǩ�������Ϣ
		ECC ecc = ECC();
		QString str1 = ECC_ui.SK_lineEdit->text();
		if (!str1.isEmpty()) {
			qlonglong RK = str1.toLongLong();
			ecc.setRK(RK);
		}		
		std::string str2 = ecc.sign(std::string(msg.toLocal8Bit()));//string
		signed_msg = QString::fromLocal8Bit(str2.data());

		SigDialog* dialog = new SigDialog(this);
		connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
		emit sendData(signed_msg, 1);
		dialog->exec();
		delete dialog;
	}	
}
/*��֤ǩ�� Ҫ��Կ������Ĭ��*/
void EccClient::check()
{
	QString signed_msg = ECC_ui.signature_edit->toPlainText();
	int flag = 2;
	if (!signed_msg.isEmpty()) {
		QString msg;	//ǩ������Ϣ
		//������֤ǩ��
		ECC ecc = ECC();
		QRegExp rex("\\d+,\\d+");
		QString PK = ECC_ui.PK_lineEdit->text();
		if (!ECC_ui.PK_lineEdit->text().isEmpty()) {
			if (rex.exactMatch(PK))
			{
				QStringList list = ECC_ui.PK_lineEdit->text().split(",");
				qlonglong x = list.at(0).toLongLong();
				qlonglong y = list.at(1).toLongLong();
				Point p = Point(x, y);
				ecc.setPK(p);
			}
			else {
				QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��Կ��ʽ����"));
			}
		}		
		std::string str = ecc.verify(std::string(signed_msg.toLocal8Bit()));//��Ҫ��֤����Ϣ
		msg = QString::fromLocal8Bit(str.data());
		//���ַ���0��ʾʧ�ܣ�1��ʾ�ɹ�
		if (msg.startsWith("1"))
			flag = 2;
		else if (msg.startsWith("0"))
			flag = 3;
		msg.remove(0, 2);

		SigDialog* dialog = new SigDialog(this);
		connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
		emit sendData(msg, flag);
		dialog->exec();
		delete dialog;
	}
	
}
