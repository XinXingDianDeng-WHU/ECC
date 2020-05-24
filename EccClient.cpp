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

/*选择输入文件*/
void EccClient::inChooseFile()
{	
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("选择文件"), ".", tr("txt(*.txt);;(*.*)"));
	ECC_ui.inputFile_lineEdit->setText(dir);
}
/*选择输出文件*/
void EccClient::outChooseFile()
{
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("选择文件"), ".", tr("txt(*.txt);;(*.*)"));
	ECC_ui.outputFile_lineEdit->setText(dir);
}
/*加密 输入公钥 若为空则默认*/
void EccClient::encryption()
{
	ECC ecc = ECC();
	//PK需要满足格式x,y 
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
			QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("公钥格式错误！"));
		}
	}	
	QString infile = ECC_ui.inputFile_lineEdit->text();
	QString outfile = ECC_ui.outputFile_lineEdit->text();
	std::string str1 = std::string(infile.toLocal8Bit());
	std::string str2 = std::string(outfile.toLocal8Bit());
	if (infile.isEmpty() || outfile.isEmpty()) {
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("输入文件/输出文件不能为空！"));
	}
	else {
		ecc.encodefile(str1, str2); //输入文件,输出文件
	}
	
	
}
/*解密 要有私钥，空则默认*/
void EccClient::decrypt()
{
	QString str = ECC_ui.SK_lineEdit->text();
	ECC ecc = ECC();
	//先判断
	if (!str.isEmpty()) {
		qlonglong RK = str.toLongLong();				
		ecc.setRK(RK);//长整型longlong		
	}	
	QString infile = ECC_ui.inputFile_lineEdit->text();
	QString outfile = ECC_ui.outputFile_lineEdit->text();
	std::string str1 = std::string(infile.toLocal8Bit());
	std::string str2 = std::string(outfile.toLocal8Bit());
	if (infile.isEmpty() || outfile.isEmpty()) {
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("输入文件/输出文件不能为空！"));
	}
	else {
		ecc.decodefile(str1, str2);//输入文件，输出文件
	}
}
/*签名 设置私钥 空则默认*/
void EccClient::signature()
{
	QString msg = ECC_ui.signature_edit->toPlainText();
	if (!msg.isEmpty()) {
		QString signed_msg;	//签名后的消息
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
/*验证签名 要公钥，空则默认*/
void EccClient::check()
{
	QString signed_msg = ECC_ui.signature_edit->toPlainText();
	int flag = 2;
	if (!signed_msg.isEmpty()) {
		QString msg;	//签名的消息
		//调用验证签名
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
				QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("公钥格式错误！"));
			}
		}		
		std::string str = ecc.verify(std::string(signed_msg.toLocal8Bit()));//需要验证的消息
		msg = QString::fromLocal8Bit(str.data());
		//首字符是0表示失败，1表示成功
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
