#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <locale.h>
#include "../Lab1_Rpc/Lab1_Rpc.h"
#include "Impersonate.h"



using namespace std;

int Log_in(char * login, char * password, char * domain)
{
	RPC_CLIENT* new_client;

	new_client = get_client();
	if (!new_client)
	{
		new_client = add_client();
		if (!new_client)
		{
			cout << "error while adding new client" << endl;
			return 0;
		}
	}

	if (new_client->Token != NULL)
	{
		CloseHandle(new_client->Token);
		new_client->Token = NULL;
	}

	if (!LogonUserA((char*)login, (char*)domain, (char*)password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &new_client->Token))
	{
		cout << "error in logon user errcode=" << GetLastError() << endl;
		return 0;
	}
}

int Create_file(char* filename)
{
	if (!hypocrite_mode())
		return 1;
	FILE *file=fopen(filename, "w");
	if(!file)
	{
		remove_the_mask();
		return 1;
	}
	fclose(file);
	remove_the_mask();
	return 0;
}

int Delete_file(char* filename)
{

	if (!hypocrite_mode())
		return 1;
	if (!DeleteFileA(filename))
	{
		remove_the_mask();
		return 1;
	}
	remove_the_mask();
	return 0;
}

int Write_file(char* filename, char data[1024], long count)
{
	if (!hypocrite_mode())
		return 1;
	FILE *f = fopen(filename, "ab");
	if (fwrite(data, 1, count, f) != count)
	{
		fclose(f);
		remove_the_mask();
		return 1;
	}
	fclose(f);
	remove_the_mask();
	return 0;
}


int Read_file(char* filename, long offset, char readed_data[1024])
{
	if (!hypocrite_mode())
		return -1;
	FILE *f = fopen(filename, "rb");
	if (!f)
	{
		remove_the_mask();
		return -1;
	}
	fseek(f, offset, SEEK_SET);
	int readed = fread(readed_data, 1, 512, f);
	fclose(f);
	remove_the_mask();
	return readed;
}

int RPCServer()
{
	RPC_STATUS status;

	status = RpcServerUseProtseqEp(
		reinterpret_cast<unsigned char*>("ncacn_ip_tcp"),
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
		reinterpret_cast<unsigned char*>("2222"),
		NULL);

	if (status)
	{
		printf("RpcServerUseProtseqEp - %d", status);
		return 0;
	}

	status = RpcServerRegisterIf2( // The difference is the ability to register an auto-listen interface and to specify a security-callback function.
		Lab1_Rpc_v1_0_s_ifspec, // Interface to register.
		NULL, // Use the MIDL generated entry-point vector.
		NULL, // Use the MIDL generated entry-point vector.
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Use default number of concurrent calls.
		(unsigned)-1, // Infinite max size of incoming data blocks.
		NULL); // Naive security callback.	
	if (status)
	{
		printf("RpcServerRegisterIf - %d\n", status);
		return 0;
	}

	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
	if (status)
	{
		printf("RpcServerListen - %d\n", status);
		return 0;
	}

	return NO_ERROR;
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER midl_user_free(void __RPC_FAR * p)
{
	free(p);
}