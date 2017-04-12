#include "../Lab1_Rpc/Lab1_Rpc.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <locale.h>
#include <vector>

#define UPLOAD 1
#define DOWNLOAD 2
#define REMOVE 3
#define HELP 4
#define EXIT 5

using namespace std;

class ClientLogic
{
private:

	static vector<char*> separate_arguments(char* cmdline)
	{
		vector<char*> separation;
		int k = 0, length = strlen(cmdline);
		for (int i = 0; i <= length; i++)
		{
			if (cmdline[i] == ' ' || i == length)
			{
				char *t = new char[i - k + 1];
				strncpy(t, (cmdline + k), i - k);
				t[i - k] = 0;
				separation.push_back(t);
				k = i + 1;
			}
		}
		return separation;
	}

	static int Command(vector<char*> argv)
	{
		if (argv.size() == 0)
			return -1;
		char *cmd = argv[0];

		
		if (!strcmp(cmd, "download"))
		{
			if (argv.size()<3)
				return -1;
			return DOWNLOAD;
		}
		else if (!strcmp(cmd, "upload"))
		{
			if (argv.size()<3)
				return -1;
			return UPLOAD;
		}
		else if (!strcmp(cmd, "remove"))
		{
			if (argv.size()<2)
				return -1;
			return REMOVE;
		}
		else if (!strcmp(cmd, "exit"))
		{
			return EXIT;
		}
		else if (!strcmp(cmd, "help"))
		{
			return HELP;
		}
		return -1;
	}

	static void UploadFile(char* src, char* dest)
	{
		FILE *f;
		char data[1024];
		long readed;
		f = fopen(src, "rb");
		if (!f)
		{
			cout << "error while opening " << src << "!" << endl;
			return;
		}
		
		Create_file(dest);
		while (!feof(f))
		{
			readed = fread(data, 1, 512, f);
			Write_file(dest, data, readed);
		}
		fclose(f);
	}

	static void DownloadFile(char* src, char* dest)
	{
		FILE *f;
		FILE *f_src;
		char data[1024];
		long length, position = 0, readed;
		int bytes_to_transfer = 512;

		f = fopen(dest, "wb");
		if (!f)
		{
			cout << "Destination  " << dest << " is not exist!" << endl;
			return;
		}
		f_src = fopen(src, "rb");
		if (!f_src)
		{
			cout << "Error while opening " << src << "! Maybe you have not permission to read or file is not exist" << endl;
			return;
		}
		while ((readed = Read_file(src, position, data)))
		{
			if (readed == -1)

				break;
			fwrite(data, 1, readed, f);
			position += readed;
		}
		fclose(f);
	}

public:

	static void Help()
	{
		cout << "Download file from server -  download [source] [destination]\n"
			 << "Upload file to server - upload [source] [destination]\n"
			 << "Remove file - remove [filename]\n"
			 << "Exit program - exit\n"
			 << "Show functions - help\n\n\n";
	}

	static int LogIn()
	{
		char username[32], password[32], domain[32] = "";
		cout << "username:";
		cin >> username;
		cout << "password:";
		cin >> password;
		if (Log_in(username, password, domain))
		{
			return 1;
		}
		return 0;
	}

	static int Handle(char* cmdline)
	{
		int code;
		vector<char*> argv = separate_arguments(cmdline);
		if ((code = Command(argv)) == -1)
		{
			cout << "Invalid input!" << endl;
			return 0;
		}
		switch (code)
		{
		case REMOVE:
			Delete_file(argv[1]);
			break;
		case UPLOAD:
			UploadFile(argv[1], argv[2]);
			break;
		case DOWNLOAD:
			DownloadFile(argv[1], argv[2]);
			break;
		case EXIT:
			return -1;
		case HELP:
			Help();
		}
		return 0;
	}
};


int main()
{
	setlocale(LC_ALL, "Russian");

	char cmdline[256], address[16];
	RPC_STATUS status;
	unsigned char* szStringBinding = NULL;

	cout << "address:";
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.getline(address, 16, '\n');

	status = RpcStringBindingCompose(
		NULL,
		reinterpret_cast<unsigned char*>("ncacn_ip_tcp"),
		reinterpret_cast<unsigned char*>(address),
		reinterpret_cast<unsigned char*>("2222"), 
		NULL,
		&szStringBinding);
	if (status)
	{
		printf("ERROR: RpcStringBindingCompose - %d\n", status);
		return 0;
	}

	status = RpcBindingFromStringBinding(
		szStringBinding,
		&hLab1_rpcHandle);

	if (status)
	{
		printf("ERROR: RpcBindingFromStringBinding - %d\n", status);
		return 0;
	}

	while (!ClientLogic::LogIn())
	{
		cout << "Error while logging in!" << endl;
	}
	cout << "Welcome!" << endl;
	Sleep(1000);
	system("cls");
	ClientLogic::Help();
	RpcTryExcept
	{
		do {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << ">";
			cin.getline(cmdline, 256, '\n');
		} while (ClientLogic::Handle(cmdline) != -1);
	}
		RpcExcept(1)
	{
		printf("RPC exception %d\n", RpcExceptionCode());
	}
	RpcEndExcept

		status = RpcStringFree(&szStringBinding);
	if (status)
	{
		printf("ERROR: RpcStringFree - %d\n", status);
		return 0;
	}

	status = RpcBindingFree(&hLab1_rpcHandle);
	if (status)
	{
		printf("ERROR: RpcBindingFree - %d", status);
		return 0;
	}
	getchar();
	return 0;
}

void* __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER midl_user_free(void* p)
{
	free(p);
}