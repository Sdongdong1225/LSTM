#pragma once
#include "iostream"
#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "vector"
#include "assert.h"
using namespace std;

#define innode  2       //����������������2������
#define hidenode  26    //���ؽ�������洢��Я��λ��
#define outnode  1      //���������������һ��Ԥ������
#define alpha  0.1      //ѧϰ����
#define binary_dim 8    //������������󳤶�

#define randval(high) ( (double)rand() / RAND_MAX * high )
#define uniform_plus_minus_one ( (double)( 2.0 * rand() ) / ((double)RAND_MAX + 1.0) - 1.0 )  //��������ֲ�


double sigmoid(double x);				//�����
double dsigmoid(double y);				//������ĵ�����yΪ�����ֵ
double dtanh(double y);					//tanh�ĵ�����yΪtanhֵ
void int2binary(int n, int *arr);		//��һ��10��������ת��Ϊ2������
void winit(double w[], int n);			//Ȩֵ��ʼ��
class RNN
{
public:
	RNN();
	virtual ~RNN();
	void train();

public:
	double W_I[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
	double U_I[hidenode][hidenode];   //������һ��������뱾�����㵥Ԫ�������ŵ�Ȩֵ����
	double W_F[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
	double U_F[hidenode][hidenode];   //������һ�������뱾�����㵥Ԫ�������ŵ�Ȩֵ����
	double W_O[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
	double U_O[hidenode][hidenode];   //������һ������������ʱ�̵��������Ȩֵ����
	double W_G[innode][hidenode];     //���ڲ����¼����Ȩֵ����
	double U_G[hidenode][hidenode];   //���ڲ����¼����Ȩֵ����
	double W_out[hidenode][outnode];  //����������������Ȩֵ����

	double *x;             //layer 0 ���ֵ������������ֱ���趨
	//double *layer_1;     //layer 1 ���ֵ
	double *y;             //layer 2 ���ֵ
};


