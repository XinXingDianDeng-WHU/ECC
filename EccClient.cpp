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
		QString::fromLocal8Bit("ѡ���ļ�"), ".", tr("txt(*.txt)"));
	ECC_ui.inputFile_lineEdit->setText(dir);
}
/*ѡ������ļ�*/
void EccClient::outChooseFile()
{
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("ѡ���ļ�"), ".", tr("txt(*.txt)"));
	ECC_ui.outputFile_lineEdit->setText(dir);
}
/*���� ���빫Կ ��Ϊ����Ĭ��*/
void EccClient::encryption()
{
	//x,y �����㵯��
	QRegExp rex("\d+,\d+");
	QString PK = ECC_ui.PK_lineEdit->text();
	if (!ECC_ui.PK_lineEdit->text().isEmpty()) {
		if (rex.exactMatch(PK))
		{
			QStringList list = ECC_ui.PK_lineEdit->text().split(",");
			qlonglong x = list.at(0).toLongLong();
			qlonglong y = list.at(1).toLongLong();
			ECC ecc = ECC();
			Point p = Point(x, y);
			ecc.setPK(p);
			QString infile = ECC_ui.inputFile_lineEdit->text();
			QString outfile = ECC_ui.outputFile_lineEdit->text();
			if (infile.isEmpty() || outfile.isEmpty()) {
				QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ļ�/����ļ�����Ϊ�գ�"));
			}
			else {
				ecc.encodefile(infile.toStdString(), outfile.toStdString()); //�����ļ�,����ļ�
			}
		}
		else {
			QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��Կ��ʽ����"));
		}
	}
	else {
		ECC ecc = ECC();
		Point p = Point();
		ecc.setPK(p);
		QString infile = ECC_ui.inputFile_lineEdit->text();
		QString outfile = ECC_ui.outputFile_lineEdit->text();
		if (infile.isEmpty() || outfile.isEmpty()) {
			QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ļ�/����ļ�����Ϊ�գ�"));
		}
		else {
			ecc.encodefile(infile.toStdString(), outfile.toStdString()); //�����ļ�,����ļ�
		}
	}

	
	
}
/*���� Ҫ��˽Կ������Ĭ��*/
void EccClient::decrypt()
{
	//���ж�
	ECC ecc = ECC();
	Point p = Point();
	ecc.setRK(1);//������longlong
	ecc.decodefile("", "");//�����ļ�������ļ�
}
/*ǩ�� ����˽Կ ����Ĭ��*/
void EccClient::signature()
{
	QString signed_msg;	//ǩ�������Ϣ
	//����ǩ������
	ECC ecc = ECC();
	ecc.setRK(1);
	std::string str = ecc.sign("");//string


	SigDialog *dialog = new SigDialog(this);
	connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
	emit sendData(signed_msg, 1);
	dialog->exec();
	delete dialog;
	
}
/*��֤ǩ�� Ҫ��Կ������Ĭ��*/
void EccClient::check()
{
	QString msg;	//ǩ������Ϣ
	//������֤ǩ��
	ECC ecc = ECC();
	Point p;
	ecc.setPK(p);
	std::string str = ecc.verify("");//��Ҫ��֤����Ϣ
	//���ַ��ɹ�Ϊ0��ʧ��Ϊ1

	SigDialog *dialog = new SigDialog(this);
	connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
	emit sendData(msg, 2);
	dialog->exec();
	delete dialog;
}
