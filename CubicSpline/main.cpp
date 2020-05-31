/*
	���������EasyXͼ��⣬����������������
*/
#include<iostream>
#include<graphics.h>
#include<math.h>
using namespace std;
#define  MAXNUM  50   //����������������������Ϊ50��
typedef struct SPLINE    //���������ṹ�壬���ڴ洢һ��������������Ϣ
{ //��ʼ����������
	float x[MAXNUM + 1];    //�洢�����ϵĵ��x���꣬���51����
	float y[MAXNUM + 1];    //�洢�����ϵĵ��y���꣬���51����
	unsigned int point_num;   //�洢�����ϵ�ʵ�ʵ� �� �ĸ���
	float begin_k1;     //��ʼ���һ�׵�����Ϣ
	float end_k1;     //��ֹ���һ�׵�����Ϣ
					  //float begin_k2;    //��ʼ��Ķ��׵�����Ϣ
					  //float end_k2;     //��ֹ��Ķ��׵�����Ϣ
					  //�������õ���������S(x)
	float k1[MAXNUM + 1];    //���е��һ�׵�����Ϣ
	float k2[MAXNUM + 1];    //���е�Ķ��׵�����Ϣ
							 //51����֮����50���Σ�func[]�洢ÿ�εĺ���ϵ��
	float a3[MAXNUM], a1[MAXNUM];
	float b3[MAXNUM], b1[MAXNUM];
	//�ֶκ�������ʽΪ Si(x) = a3[i] * {x(i+1) - x}^3  + a1[i] * {x(i+1) - x} +
	//        b3[i] * {x - x(i)}^3 + b1[i] * {x - x(i)}
	//xiΪx[i]��ֵ��xi_1Ϊx[i+1]��ֵ      
}SPLINE, *pSPLINE;
typedef int RESULT;      //���غ���ִ�еĽ��״̬������Ϊ����ķ���ѡ��
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE -1
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef ERR
#define ERR  -2
#endif
						 //////////////////////////////////////////////////////////////////////////////////
						 /*===============================================================================
						 *** �������ƣ� Spline3()
						 *** ����˵���� �������������ֵ������ʹ��׷�Ϸ����M����
						 *** ��ڲ����� (pSPLINE)pLine  �����ṹ��ָ��pLine�е�x[],y[],num,begin_k1,end_k1
						 *** ���ڲ����� (pSPLINE)pLine  �����ṹ��ָ��pLine�еĺ�������
						 *** ���ز����� ���س���ִ�н����״̬TRUE or FALSE
						 ================================================================================*/
RESULT Spline3(pSPLINE pLine)
{
	float H[MAXNUM] = { 0 };     //С����Ĳ���
	float Fi[MAXNUM] = { 0 };     //�м���
	float U[MAXNUM + 1] = { 0 };    //�м���
	float A[MAXNUM + 1] = { 0 };    //�м���
	float D[MAXNUM + 1] = { 0 };    //�м���
	float M[MAXNUM + 1] = { 0 };    //M����
	float B[MAXNUM + 1] = { 0 };    //׷�Ϸ��м���
	float Y[MAXNUM + 1] = { 0 };    //׷�Ϸ��м����
	int i = 0;
	////////////////////////////////////////�����м����
	if ((pLine->point_num < 3) || (pLine->point_num > MAXNUM + 1))
	{
		return ERR;       //�������ݵ����̫�ٻ�̫��
	}
	for (i = 0; i <= pLine->point_num - 2; i++)
	{          //��H[i]
		H[i] = pLine->x[i + 1] - pLine->x[i];
		Fi[i] = (pLine->y[i + 1] - pLine->y[i]) / H[i]; //��F[x(i),x(i+1)]
	}
	for (i = 1; i <= pLine->point_num - 2; i++)
	{          //��U[i]��A[i]��D[i]
		U[i] = H[i - 1] / (H[i - 1] + H[i]);
		A[i] = H[i] / (H[i - 1] + H[i]);
		D[i] = 6 * (Fi[i] - Fi[i - 1]) / (H[i - 1] + H[i]);
	}
	//���߽�����Ϊ1����������
	U[i] = 1;
	A[0] = 1;
	D[0] = 6 * (Fi[0] - pLine->begin_k1) / H[0];
	D[i] = 6 * (pLine->end_k1 - Fi[i - 1]) / H[i - 1];
	//���߽�����Ϊ2����������
	//U[i] = 0;
	//A[0] = 0;
	//D[0] = 2 * begin_k2;
	//D[i] = 2 * end_k2;
	/////////////////////////////////////////׷�Ϸ����M����
	B[0] = A[0] / 2;
	for (i = 1; i <= pLine->point_num - 2; i++)
	{
		B[i] = A[i] / (2 - U[i] * B[i - 1]);
	}
	Y[0] = D[0] / 2;
	for (i = 1; i <= pLine->point_num - 1; i++)
	{
		Y[i] = (D[i] - U[i] * Y[i - 1]) / (2 - U[i] * B[i - 1]);
	}
	M[pLine->point_num - 1] = Y[pLine->point_num - 1];
	for (i = pLine->point_num - 1; i > 0; i--)
	{
		M[i - 1] = Y[i - 1] - B[i - 1] * M[i];
	}
	//���㷽�������ս��
	for (i = 0; i <= pLine->point_num - 2; i++)
	{
		pLine->a3[i] = M[i] / (6 * H[i]);
		pLine->a1[i] = (pLine->y[i] - M[i] * H[i] * H[i] / 6) / H[i];
		pLine->b3[i] = M[i + 1] / (6 * H[i]);
		pLine->b1[i] = (pLine->y[i + 1] - M[i + 1] * H[i] * H[i] / 6) / H[i];
	}
	return TRUE;
}

void CoordinateTransform(double &x, double &y)
{//����ת��������X���27-31��Y���2.5��4.5
	x = x*160 - 4320;
	y = -240 * y + 1080;
}

void DrawCubicSpline(pSPLINE line)
{//�������������ĺ�����ȡ��ɢ��ԣ�������Ӧ�����ص����
	int num = line->point_num;
	double y,z;
	int rate = 2000;
	for (int i = 0; i < num - 1; i++)
	{
		for (double  j= line->x[i];j < line->x[i + 1];j += 1.0 / rate)
		{
			//�������������ķֶν���ʽ���õ�����ֵ
			y = line->a3[i] * pow((line->x[i + 1] - j), 3) + line->a1[i] * (line->x[i + 1] - j) + line->b3[i] * pow((j - line->x[i]), 3) + line->b1[i] * (j - line->x[i]);
			z = j;
			CoordinateTransform(z, y);
			putpixel(z, y,GREEN);
		}
	}
}

SPLINE line1;								//����ȫ�ֱ���
pSPLINE pLine1 = &line1;

int main()
{
	line1.x[0] = 27.7;				//�Թ��������Ĳ������г�ʼ��
	line1.x[1] = 28;
	line1.x[2] = 29;
	line1.x[3] = 30;
	line1.y[0] = 4.1;
	line1.y[1] = 4.3;
	line1.y[2] = 4.1;
	line1.y[3] = 3.0;
	line1.point_num = 4;
	line1.begin_k1 = 3.0;			//��ʼ�����һ�׵���
	line1.end_k1 = -4.0;				//��β���һ�׵���
	Spline3(pLine1);					//������������Ĳ�������
	initgraph(640, 480);				//��ʼ����ͼ����
	DrawCubicSpline(pLine1);	//����������������
	system("pause");
	return 0;
}