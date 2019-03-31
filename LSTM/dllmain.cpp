// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "RNN.h"

class KalmanFilter;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
	//调用LSTM
	srand(time(NULL));
	RNN rnn;
	rnn.train();

	//调用卡尔曼滤波法
    return TRUE;
}

