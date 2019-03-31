#pragma once
#include "iostream"
#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "vector"
#include "assert.h"
using namespace std;

#define innode  2       //输入结点数，将输入2个加数
#define hidenode  26    //隐藏结点数，存储“携带位”
#define outnode  1      //输出结点数，将输出一个预测数字
#define alpha  0.1      //学习速率
#define binary_dim 8    //二进制数的最大长度

#define randval(high) ( (double)rand() / RAND_MAX * high )
#define uniform_plus_minus_one ( (double)( 2.0 * rand() ) / ((double)RAND_MAX + 1.0) - 1.0 )  //均匀随机分布


double sigmoid(double x);				//激活函数
double dsigmoid(double y);				//激活函数的导数，y为激活函数值
double dtanh(double y);					//tanh的导数，y为tanh值
void int2binary(int n, int *arr);		//将一个10进制整数转换为2进制数
void winit(double w[], int n);			//权值初始化
class RNN
{
public:
	RNN();
	virtual ~RNN();
	void train();

public:
	double W_I[innode][hidenode];     //连接输入与隐含层单元中输入门的权值矩阵
	double U_I[hidenode][hidenode];   //连接上一隐层输出与本隐含层单元中输入门的权值矩阵
	double W_F[innode][hidenode];     //连接输入与隐含层单元中遗忘门的权值矩阵
	double U_F[hidenode][hidenode];   //连接上一隐含层与本隐含层单元中遗忘门的权值矩阵
	double W_O[innode][hidenode];     //连接输入与隐含层单元中遗忘门的权值矩阵
	double U_O[hidenode][hidenode];   //连接上一隐含层与现在时刻的隐含层的权值矩阵
	double W_G[innode][hidenode];     //用于产生新记忆的权值矩阵
	double U_G[hidenode][hidenode];   //用于产生新记忆的权值矩阵
	double W_out[hidenode][outnode];  //连接隐层与输出层的权值矩阵

	double *x;             //layer 0 输出值，由输入向量直接设定
	//double *layer_1;     //layer 1 输出值
	double *y;             //layer 2 输出值
};


