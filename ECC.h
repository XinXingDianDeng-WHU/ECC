#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <string.h>

#define LL long long
#define INF 0x7FFFFFFFFFFFFFFF
#define BYTE_PER_POINT 1

struct Point
{
	LL x, y;
	LL offset;
	Point()
	{
		this->x = 0;
		this->y = 0;
		this->offset = 0;
	}
	Point(LL x, LL y)
	{
		this->x = x;
		this->y = y;
		this->offset = 0;
	}
	bool operator == (const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
	bool O()
	{
		return this->x == INF && this->y == INF;
	}
};

struct SignedMessage
{
	std::string message;
	std::string hash;
	LL numbers[16 / BYTE_PER_POINT];
	LL Sn[16 / BYTE_PER_POINT];//MD5��HASH���Ϊ16�ֽ�
};

struct VerifyResult
{
	bool success;//�Ƿ�ǩ���ɹ�
	std::string message;
};

struct PointPair
{
	Point first, second;
};

class ECC
{
private:// ��Ա����
	
	LL r;//��Կ

	//��Բ���� y^2 = x^3 + ax + b  mod p
	LL a;
	LL b;
	LL p;
	LL n;// G �Ľ�

	LL randomK = 2;//����� -- ���ܲ����õ�

	Point G;//����
	Point P;//��Կ P = rG
public:// ��Ա����
	Point add(Point p1, Point p2);// ����p1 + p2 �Ľ��
	Point minus(Point p1, Point p2);// ���� p1 - p2 �Ľ��
	Point mul(LL k, Point p);// ���� k*p �Ľ��
	Point subMul(LL k, Point p, Point ps[], bool flags[]);// ��mul��������k*p
	
	PointPair encode(Point Pm); // ����ϢPm���ܳ�{kG,Pm+kP}������ֵ�ĵ�һ������kG���ڶ�����Pm+kP
	Point decode(PointPair Cm); // ������Cm = {kG,Pm+kP}���ܳ���ϢM��Ӧ�ĵ�Pm

	//��Ϣ�����Ϳ����޸ģ���һ����char*  ,stringʲô��Ҳ��
	Point encodeMessage(unsigned char message);//����Ϣת��Ϊ�����ϵĵ�Pm
	unsigned char decodeMessage(Point Pm);//�������ϵĵ�Pmת��Ϊ��Ϣmessage����encodeMessage��Ϊ�����

	LL getRandom(); // ��ȡ�����k , 1 <= k < p
	bool judgePoint(Point point);//�жϵ�P�Ƿ�����Բ������

private:// ��̬���ߺ�����
	static LL mod(LL a, LL n);// ���� a mod n����Ҫ�Ǵ���aΪ���������
	static LL inv(LL a, LL n);// ���� a����ģn����Ԫ������������򷵻�0
	static LL gcd(LL a, LL b);//��a��b�Ĵ�Сû������
	static Point extend_gcd(LL a, LL b);//��չ��ŷ������㷨��(x,y)ʹ��ax+by = gcd(a,b)
	static int quadraticresidue(LL p, LL c);//����ģp ����c�Ƿ�Ϊp�Ķ���ʣ��  1Ϊ�� -1Ϊ���� ���õ¼Ǻ�
	static LL powerMod(LL b, LL n, LL m);//��b^n (mod) m  bΪ����  nΪָ��  mΪģ�� 
	
public:// ��ǰ�˵��õĽӿ�
	ECC(); // Ĭ��ʹ��Ĭ�ϵ�ֵ���й���

	void setRK(LL key); // ������Կ
	Point getPK(); // ��ù�ԿP
	void setPK(Point pk); // ���ù�ԿP,������֮��ֻ�������߽��м��ܺ�ǩ����֤������ʹ����Ҫ��Կr�Ĺ���

	//���¹��캯��Ҫͨ����ʽ P=rG �����P��ֵ
	ECC(LL r,LL a,LL b,LL p,Point G); // �����Զ����ֵ���й��죬���Ǳ�������G�Ľף�ͨ��O=nG����
	ECC(LL r, LL a, LL b, LL p, Point G, LL n);
	void setEllipticParameter(LL r, LL a, LL b, LL p, Point G);
	void setEllipticParameter(LL r, LL a, LL b, LL p, Point G, LL n);
	/*
	//��ʱ���ܷ���ֵ�Ͳ�������  ---  ÿ���һ���ٽ��ע��һ��
	void encode(); // �����ļ���Ϊ����
	void decode(); // �����ļ���Ϊ����
	void sign(); // ����Ϣ��������ǩ��
	void verify(); // ����ǩ����֤
	*/

	bool encodefile(std::string inputFilePath,std::string outputFilePath = ""); //���ļ����м��� 
	bool decodefile(std::string inputFilePath, std::string outputFilePath = ""); //���ļ����н���
	std::string sign(std::string message); // ����Ϣ��������ǩ��
	std::string verify(std::string signedMessage); // ����ǩ����֤
};

