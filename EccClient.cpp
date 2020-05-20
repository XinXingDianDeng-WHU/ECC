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
		QString::fromLocal8Bit("选择文件"), ".", tr("txt(*.txt)"));
	ECC_ui.inputFile_lineEdit->setText(dir);
}
/*选择输出文件*/
void EccClient::outChooseFile()
{
	QString dir = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("选择文件"), ".", tr("txt(*.txt)"));
	ECC_ui.outputFile_lineEdit->setText(dir);
}
/*加密 输入公钥 若为空则默认*/
void EccClient::encryption()
{
	//x,y 不满足弹框
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
				QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("输入文件/输出文件不能为空！"));
			}
			else {
				ecc.encodefile(infile.toStdString(), outfile.toStdString()); //输入文件,输出文件
			}
		}
		else {
			QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("公钥格式错误！"));
		}
	}
	else {
		ECC ecc = ECC();
		Point p = Point();
		ecc.setPK(p);
		QString infile = ECC_ui.inputFile_lineEdit->text();
		QString outfile = ECC_ui.outputFile_lineEdit->text();
		if (infile.isEmpty() || outfile.isEmpty()) {
			QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("输入文件/输出文件不能为空！"));
		}
		else {
			ecc.encodefile(infile.toStdString(), outfile.toStdString()); //输入文件,输出文件
		}
	}

	
	
}
/*解密 要有私钥，空则默认*/
void EccClient::decrypt()
{
	//先判断
	ECC ecc = ECC();
	Point p = Point();
	ecc.setRK(1);//长整型longlong
	ecc.decodefile("", "");//输入文件，输出文件
}
/*签名 设置私钥 空则默认*/
void EccClient::signature()
{
	QString signed_msg;	//签名后的消息
	//调用签名函数
	ECC ecc = ECC();
	ecc.setRK(1);
	std::string str = ecc.sign("");//string


	SigDialog *dialog = new SigDialog(this);
	connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
	emit sendData(signed_msg, 1);
	dialog->exec();
	delete dialog;
	
}
/*验证签名 要公钥，空则默认*/
void EccClient::check()
{
	QString msg;	//签名的消息
	//调用验证签名
	ECC ecc = ECC();
	Point p;
	ecc.setPK(p);
	std::string str = ecc.verify("");//需要验证的消息
	//首字符成功为0，失败为1

	SigDialog *dialog = new SigDialog(this);
	connect(this, SIGNAL(sendData(QString, int)), dialog, SLOT(receiveData(QString, int)));
	emit sendData(msg, 2);
	dialog->exec();
	delete dialog;
}
