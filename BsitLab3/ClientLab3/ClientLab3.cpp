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
	//информационное сообщение
	printf("Welcome to Client\n");
	//переменная состояния
	HRESULT hRes = S_OK;
	long x;
	//инициализация COM
	hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//assert завершит программу, если в hRes не состояние успеха
	assert(SUCCEEDED(hRes));
	// Создание экземпляра объекта Test,
	// получение указателя на интерфейс
	ITest *pTest = 0;
	hRes = CoCreateInstance(CLSID_Test, 0, CLSCTX_SERVER,
		IID_ITest, reinterpret_cast<void**>(&pTest));

	
	//в случае успеха
	if (SUCCEEDED(hRes)) {
		wchar_t *pOutText = NULL;
		printf("hRes SUCCEEDED\nWaiting for text from server:\n");

		//вызываем метод
		hRes = pTest->GetText(&pOutText);

		//при успешном вызове метода
		if (SUCCEEDED(hRes)) {
			printf("Success!\n");
			//печать строки
			wprintf(pOutText);

			//особождение памяти
			CoTaskMemFree(pOutText);
		}
		else
		{
			//собщение об ошибке
			printf("Error GetText\n");
		}
		//удаление объекта
		pTest->Release();
	}
	else {
		//собщение об ошибке
		printf("Failed CoCreateInstance\n");

	}
	//деинициализация COM
	CoUninitialize();
	printf("\nShutdown...");
}

