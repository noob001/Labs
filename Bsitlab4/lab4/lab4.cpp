// lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock2.h>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

SOCKET ClientSocket;

	void os()
		{
			char buf[2];
			int res;
			char answer[1024];
			buf[0] = '1';
			send(ClientSocket, (char *)&buf, sizeof(buf), 0);
			ZeroMemory(&answer, sizeof(answer));
			res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
			cout << "OS version: " << endl;
			cout << answer << endl;
			if (res <= 0)
			{
				cout << "Recv error \n" << endl;
			}
		}

	void time()
		{
			char buf[2];
			int res;
			char answer[1024];
			buf[0] = '2';
			send(ClientSocket, (char *)&buf, sizeof(buf), 0);
			ZeroMemory(&answer, sizeof(answer));
			res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
			cout << "time: ";
			cout << answer << endl;
			if (res <= 0)
			{
				cout << "Recv error \n" << endl;
			}
			ZeroMemory(&answer, sizeof(answer));
		}

	void uptime()
	{
		char buf[2];
		int res;
		char answer[1024];
		buf[0] = '3';
		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << "UpTime: ";
		cout << answer << endl;
		if (res <= 0)
		{
			cout << "Recv error \n" << endl;
		}
		ZeroMemory(&answer, sizeof(answer));
	}

	void memorystatus()
	{
		char buf[2];
		int res;
		char answer[1024];
		buf[0] = '4';
		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << "Memory: ";
		cout << answer << endl;
		if (res <= 0)
		{
			cout << "Recv error \n" << endl;
		}
		ZeroMemory(&answer, sizeof(answer));
	}

	void freespace()
	{
		char buf[2];
		int res;
		char answer[1024];
		buf[0] = '5';
		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << "Free space: ";
		cout << answer << endl;
		if (res <= 0)
		{
			cout << "Recv error \n" << endl;
		}
		ZeroMemory(&answer, sizeof(answer));
	}

	void rightfile()
	{
		char buf[2];
		int res;
		char answer[1024];
		char path[255];
		buf[0] = '6';

		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		
		cout << answer << endl;
		cin >> path;

		send(ClientSocket, (char *)&path, sizeof(path), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);

		if (strcmp(answer, "access denied") == 0)
		{
			cout << answer << "!!!" << endl;
			return;
		}
		cout << answer << endl;
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		int num;
		num = atoi(answer);
		for (int i = 0; i < num; i++)
		{
			res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);

			cout << answer << endl;
		}

		ZeroMemory(&answer, sizeof(answer));
	}

	void rightkey()
	{
		char buf[2];
		int res;
		char answer[1024];
		char path[255];
		buf[0] = '7';
		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << answer << endl;
		cout << "The names of registry keys must use the following literal strings to" << endl << " identify the predefined registry keys :" << endl << " CLASSES_ROOT, CURRENT_USER, MACHINE, USERS." << endl;
		cin >> path;
		send(ClientSocket, (char *)&path, sizeof(path), 0);
		ZeroMemory(&answer, sizeof(answer));
		recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		if (strcmp(answer, "access denied") == 0)
		{
			cout << answer << "!!!" << endl;
			return;
		}
		cout << answer << endl;
		ZeroMemory(&answer, sizeof(answer));
		recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		int num;
		num = atoi(answer);
		for (int i = 0; i < num; i++)
		{
			res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
			cout << answer << endl;
		}
		ZeroMemory(&answer, sizeof(answer));
	}

	void ownerfile()
	{
		char buf[2];
		int res;
		char answer[1024];
		char path[255];
		buf[0] = '8';
		send(ClientSocket, (char *)&buf, sizeof(buf), 0);
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << answer << endl;
		cin >> path;
		send(ClientSocket, (char *)&path, sizeof(path), 0);
		ZeroMemory(&answer, sizeof(answer));
		recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		if (strcmp(answer, "access denied") == 0)
		{
			cout << answer << "!!!" << endl;
			return;
		} 
		cout << answer << endl;
		ZeroMemory(&answer, sizeof(answer));
		res = recv(ClientSocket, (char *)&answer, sizeof(answer), 0);
		cout << answer << endl;
		if (res <= 0)
		{
			cout << "Recv error \n" << endl;
		}
		ZeroMemory(&answer, sizeof(answer));
	}


int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA ws;

	if (WSAStartup(MAKEWORD(1, 1), &ws) == SOCKET_ERROR)
		cout << "Error while WSAStartup: " << WSAGetLastError() << endl;

	if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		cout << "Error while create socket: " << WSAGetLastError() << endl;
	char addr[20];


	cout << "Enter IP: "; // Просим ввести ip-адрес сервера
	cin >> addr; // Получаемего
	sockaddr_in sin;
	sin.sin_family = AF_INET; // TCP-IP соедение
	sin.sin_port = htons(666); // Порт
	sin.sin_addr.s_addr = inet_addr(addr); //Преобразуем адрес в нужный формат

	if (connect(ClientSocket, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "Socket_Error" << endl;
		return 1;
	}

	cout << "1 - OS" << endl << "2 - Time" << endl << "3 - UpTime" << endl << "4 - Memory Status" << endl << "5 - Free space" << endl;
	cout << "6 - Right's file" <<endl<< "7 - Right's key" << endl<< "8 - Owner of file" <<endl<< "9 - Exit" <<endl;

	int oper = 0;

	while (true)
	{
		cout << "> ";
		cin >>oper;

		switch (oper)
		{
		case 1:
			os();
			break;
		case 2:
			time();
			break;
		case 3:
			uptime();
			break;
		case 4:
			memorystatus();
			break;
		case 5:
			freespace();
			break;
		case 6:
			rightfile();
			break;
		case 7:
			rightkey();
			break;
		case 8:
			ownerfile();
			break;
		case 9:

			char buf[2];
			buf[0] = '9';
			send(ClientSocket, (char *)&buf, sizeof(buf), 0);
			cout << "End";
			Sleep(1000);
			closesocket(ClientSocket);
			WSACleanup();
			return 0;
		}
	}
}