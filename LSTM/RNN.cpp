#include "stdafx.h"
#include "RNN.h"
int largest_number = (pow(2, binary_dim));  //����������󳤶ȶ�Ӧ�Ŀ��Ա�ʾ�����ʮ������
double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

double dsigmoid(double y)
{
	return y * (1.0 - y);
}

double dtanh(double y)
{
	y = tanh(y);
	return 1.0 - y * y;
}

void int2binary(int n, int *arr)
{
	int i = 0;
	while (n)
	{
		arr[i++] = n % 2;
		n /= 2;
	}
	while (i < binary_dim)
		arr[i++] = 0;
}
void winit(double w[], int n) //Ȩֵ��ʼ��
{
	for (int i = 0; i < n; i++)
		w[i] = uniform_plus_minus_one;  //��������ֲ�
}

RNN::RNN()
{
	x = new double[innode];
	y = new double[outnode];
	winit((double*)W_I, innode * hidenode);
	winit((double*)U_I, hidenode * hidenode);
	winit((double*)W_F, innode * hidenode);
	winit((double*)U_F, hidenode * hidenode);
	winit((double*)W_O, innode * hidenode);
	winit((double*)U_O, hidenode * hidenode);
	winit((double*)W_G, innode * hidenode);
	winit((double*)U_G, hidenode * hidenode);
	winit((double*)W_out, hidenode * outnode);
}

RNN::~RNN()
{
	delete x;
	delete y;
}

void RNN::train()
{
	int epoch, i, j, k, m, p;
	std::vector<double*> I_vector;      //������
	std::vector<double*> F_vector;      //������
	std::vector<double*> O_vector;      //�����
	std::vector<double*> G_vector;      //�¼���
	std::vector<double*> S_vector;      //״ֵ̬
	std::vector<double*> h_vector;      //���ֵ
	std::vector<double> y_delta;        //����������������ƫ��

	for (epoch = 0; epoch < 11000; epoch++)  //ѵ������
	{
		double e = 0.0;  //���

		int predict[binary_dim];               //����ÿ�����ɵ�Ԥ��ֵ
		memset(predict, 0, sizeof(predict));

		int a_int = (int)randval(largest_number / 2.0);  //�������һ������ a
		int a[binary_dim];
		int2binary(a_int, a);                 //תΪ��������

		int b_int = (int)randval(largest_number / 2.0);  //���������һ������ b
		int b[binary_dim];
		int2binary(b_int, b);                 //תΪ��������

		int c_int = a_int + b_int;            //��ʵ�ĺ� c
		int c[binary_dim];
		int2binary(c_int, c);                 //תΪ��������

		//��0ʱ����û��֮ǰ��������ģ����Գ�ʼ��һ��ȫΪ0��
		double *S = new double[hidenode];     //״ֵ̬
		double *h = new double[hidenode];     //���ֵ

		for (i = 0; i < hidenode; i++)
		{
			S[i] = 0;
			h[i] = 0;
		}
		S_vector.push_back(S);
		h_vector.push_back(h);

		//���򴫲�
		for (p = 0; p < binary_dim; p++)           //ѭ���������������飬�����λ��ʼ
		{
			x[0] = a[p];
			x[1] = b[p];
			double t = (double)c[p];          //ʵ��ֵ
			double *in_gate = new double[hidenode];     //������
			double *out_gate = new double[hidenode];    //�����
			double *forget_gate = new double[hidenode]; //������
			double *g_gate = new double[hidenode];      //�¼���
			double *state = new double[hidenode];       //״ֵ̬
			double *h = new double[hidenode];           //�������ֵ

			for (j = 0; j < hidenode; j++)
			{
				//�����ת��������
				double inGate = 0.0;
				double outGate = 0.0;
				double forgetGate = 0.0;
				double gGate = 0.0;
				double s = 0.0;

				for (m = 0; m < innode; m++)
				{
					inGate += x[m] * W_I[m][j];
					outGate += x[m] * W_O[m][j];
					forgetGate += x[m] * W_F[m][j];
					gGate += x[m] * W_G[m][j];
				}

				double *h_pre = h_vector.back();
				double *state_pre = S_vector.back();
				for (m = 0; m < hidenode; m++)
				{
					inGate += h_pre[m] * U_I[m][j];
					outGate += h_pre[m] * U_O[m][j];
					forgetGate += h_pre[m] * U_F[m][j];
					gGate += h_pre[m] * U_G[m][j];
				}

				in_gate[j] = sigmoid(inGate);
				out_gate[j] = sigmoid(outGate);
				forget_gate[j] = sigmoid(forgetGate);
				g_gate[j] = sigmoid(gGate);

				double s_pre = state_pre[j];
				state[j] = forget_gate[j] * s_pre + g_gate[j] * in_gate[j];
				h[j] = in_gate[j] * tanh(state[j]);
			}


			for (k = 0; k < outnode; k++)
			{
				//���ز㴫���������
				double out = 0.0;
				for (j = 0; j < hidenode; j++)
					out += h[j] * W_out[j][k];
				y[k] = sigmoid(out);               //��������Ԫ���
			}

			predict[p] = (int)floor(y[0] + 0.5);   //��¼Ԥ��ֵ

			//�������ز㣬�Ա��´μ���
			I_vector.push_back(in_gate);
			F_vector.push_back(forget_gate);
			O_vector.push_back(out_gate);
			S_vector.push_back(state);
			G_vector.push_back(g_gate);
			h_vector.push_back(h);

			//�����׼������������ƫ��
			y_delta.push_back((t - y[0]) * dsigmoid(y[0]));
			e += fabs(t - y[0]);          //���
		}

		//���򴫲�

		//������ƫ�ͨ����ǰ֮��һ��ʱ�������������͵�ǰ������������
		double h_delta[hidenode];
		double *O_delta = new double[hidenode];
		double *I_delta = new double[hidenode];
		double *F_delta = new double[hidenode];
		double *G_delta = new double[hidenode];
		double *state_delta = new double[hidenode];
		//��ǰʱ��֮���һ�����ز����
		double *O_future_delta = new double[hidenode];
		double *I_future_delta = new double[hidenode];
		double *F_future_delta = new double[hidenode];
		double *G_future_delta = new double[hidenode];
		double *state_future_delta = new double[hidenode];
		double *forget_gate_future = new double[hidenode];
		for (j = 0; j < hidenode; j++)
		{
			O_future_delta[j] = 0;
			I_future_delta[j] = 0;
			F_future_delta[j] = 0;
			G_future_delta[j] = 0;
			state_future_delta[j] = 0;
			forget_gate_future[j] = 0;
		}
		for (p = binary_dim - 1; p >= 0; p--)
		{
			x[0] = a[p];
			x[1] = b[p];

			//��ǰ���ز�
			double *in_gate = I_vector[p];     //������
			double *out_gate = O_vector[p];    //�����
			double *forget_gate = F_vector[p]; //������
			double *g_gate = G_vector[p];      //�¼���
			double *state = S_vector[p + 1];     //״ֵ̬
			double *h = h_vector[p + 1];         //�������ֵ

			//ǰһ�����ز�
			double *h_pre = h_vector[p];
			double *state_pre = S_vector[p];

			for (k = 0; k < outnode; k++)  //����������ÿ�������Ԫ������Ȩֵ
			{
				//����������������֮�������Ȩ
				for (j = 0; j < hidenode; j++)
					W_out[j][k] += alpha * y_delta[p] * h[j];
			}

			//����������ÿ�����ص�Ԫ����������������Ȩֵ
			for (j = 0; j < hidenode; j++)
			{
				h_delta[j] = 0.0;
				for (k = 0; k < outnode; k++)
				{
					h_delta[j] += y_delta[p] * W_out[j][k];
				}
				for (k = 0; k < hidenode; k++)
				{
					h_delta[j] += I_future_delta[k] * U_I[j][k];
					h_delta[j] += F_future_delta[k] * U_F[j][k];
					h_delta[j] += O_future_delta[k] * U_O[j][k];
					h_delta[j] += G_future_delta[k] * U_G[j][k];
				}

				O_delta[j] = 0.0;
				I_delta[j] = 0.0;
				F_delta[j] = 0.0;
				G_delta[j] = 0.0;
				state_delta[j] = 0.0;

				//�������У�����
				O_delta[j] = h_delta[j] * tanh(state[j]) * dsigmoid(out_gate[j]);
				state_delta[j] = h_delta[j] * out_gate[j] * dtanh(state[j]) +
					state_future_delta[j] * forget_gate_future[j];
				F_delta[j] = state_delta[j] * state_pre[j] * dsigmoid(forget_gate[j]);
				I_delta[j] = state_delta[j] * g_gate[j] * dsigmoid(in_gate[j]);
				G_delta[j] = state_delta[j] * in_gate[j] * dsigmoid(g_gate[j]);

				//����ǰһ�������������������֮���Ȩֵ
				for (k = 0; k < hidenode; k++)
				{
					U_I[k][j] += alpha * I_delta[j] * h_pre[k];
					U_F[k][j] += alpha * F_delta[j] * h_pre[k];
					U_O[k][j] += alpha * O_delta[j] * h_pre[k];
					U_G[k][j] += alpha * G_delta[j] * h_pre[k];
				}

				//����������������֮�������Ȩ
				for (k = 0; k < innode; k++)
				{
					W_I[k][j] += alpha * I_delta[j] * x[k];
					W_F[k][j] += alpha * F_delta[j] * x[k];
					W_O[k][j] += alpha * O_delta[j] * x[k];
					W_G[k][j] += alpha * G_delta[j] * x[k];
				}

			}

			if (p == binary_dim - 1)
			{
				delete  O_future_delta;
				delete  F_future_delta;
				delete  I_future_delta;
				delete  G_future_delta;
				delete  state_future_delta;
				delete  forget_gate_future;
			}

			O_future_delta = O_delta;
			F_future_delta = F_delta;
			I_future_delta = I_delta;
			G_future_delta = G_delta;
			state_future_delta = state_delta;
			forget_gate_future = forget_gate;
		}
		delete  O_future_delta;
		delete  F_future_delta;
		delete  I_future_delta;
		delete  G_future_delta;
		delete  state_future_delta;

		if (epoch % 1000 == 0)
		{
			cout << "error��" << e << endl;
			cout << "pred��";
			for (k = binary_dim - 1; k >= 0; k--)
				cout << predict[k];
			cout << endl;

			cout << "true��";
			for (k = binary_dim - 1; k >= 0; k--)
				cout << c[k];
			cout << endl;

			int out = 0;
			for (k = binary_dim - 1; k >= 0; k--)
				out += predict[k] * pow(2, k);
			cout << a_int << " + " << b_int << " = " << out << endl << endl;
		}

		for (i = 0; i < I_vector.size(); i++)
			delete I_vector[i];
		for (i = 0; i < F_vector.size(); i++)
			delete F_vector[i];
		for (i = 0; i < O_vector.size(); i++)
			delete O_vector[i];
		for (i = 0; i < G_vector.size(); i++)
			delete G_vector[i];
		for (i = 0; i < S_vector.size(); i++)
			delete S_vector[i];
		for (i = 0; i < h_vector.size(); i++)
			delete h_vector[i];

		I_vector.clear();
		F_vector.clear();
		O_vector.clear();
		G_vector.clear();
		S_vector.clear();
		h_vector.clear();
		y_delta.clear();
	}
}
