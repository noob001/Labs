// ClientLab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <initguid.h>
#include "../BSITLab3/ServerDcom.h"
#include <iostream>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

void main(int argc, char **argv)
{
	//�������������� ���������
	printf("Welcome to Client\n");
	//���������� ���������
	HRESULT hRes = S_OK;
	long x;
	//������������� COM
	hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//assert �������� ���������, ���� � hRes �� ��������� ������
	assert(SUCCEEDED(hRes));
	// �������� ���������� ������� Test,
	// ��������� ��������� �� ���������
	ITest *pTest = 0;
	hRes = CoCreateInstance(CLSID_Test, 0, CLSCTX_SERVER,
		IID_ITest, reinterpret_cast<void**>(&pTest));

	
	//� ������ ������
	if (SUCCEEDED(hRes)) {
		wchar_t *pOutText = NULL;
		printf("hRes SUCCEEDED\nWaiting for text from server:\n");

		//�������� �����
		hRes = pTest->GetText(&pOutText);

		//��� �������� ������ ������
		if (SUCCEEDED(hRes)) {
			printf("Success!\n");
			//������ ������
			wprintf(pOutText);

			//����������� ������
			CoTaskMemFree(pOutText);
		}
		else
		{
			//�������� �� ������
			printf("Error GetText\n");
		}
		//�������� �������
		pTest->Release();
	}
	else {
		//�������� �� ������
		printf("Failed CoCreateInstance\n");

	}
	//��������������� COM
	CoUninitialize();
	printf("\nShutdown...");
}

