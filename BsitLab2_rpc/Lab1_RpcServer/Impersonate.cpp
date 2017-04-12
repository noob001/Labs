#include "Impersonate.h"

std::list<RPC_CLIENT> clients;

RPC_STATUS get_client_handle(RPC_BINDING_HANDLE *hBinding)
{
	RPC_STATUS status;
	RPC_ASYNC_STATE rpcAsync;
	status = RpcAsyncInitializeHandle(&rpcAsync, sizeof(rpcAsync));
	if (status)
	{
		printf("error in RpcAsyncInitializeHandle errcode=%d\n", status);
		return status;
	}
	*hBinding = RpcAsyncGetCallHandle(&rpcAsync);
	return RPC_S_OK;
}

RPC_CLIENT* get_client()
{
	RPC_BINDING_HANDLE Binding;
	RPC_STATUS status = get_client_handle(&Binding);
	if (status)
	{
		printf("error in RpcServerInqBindingHandle errcode=%d\n", status);
		return NULL;
	}
	std::list<RPC_CLIENT>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
		if ((*it).BindingHandle == Binding)
			return &(*it);
	return NULL;
}

RPC_CLIENT* add_client()
{
	RPC_BINDING_HANDLE Binding;
	RPC_STATUS status = get_client_handle(&Binding);
	if (status)
	{
		printf("error in RpcServerInqBindingHandle errcode=%d\n", status);
		return NULL;
	}
	RPC_CLIENT client;
	std::list<RPC_CLIENT>::iterator it = clients.end();
	clients.push_back(client);
	it--;
	(*it).BindingHandle = Binding;
	(*it).Token = NULL;
	return &(*it);
}

int hypocrite_mode()
{
	RPC_CLIENT* client;
	client = get_client();
	if (!ImpersonateLoggedOnUser(client->Token))
	{
		printf("ImpersonateLoggedOnUser  - %d\n", GetLastError());
		return 0;
	}
	return 1;
}

int remove_the_mask()
{
	if (!RevertToSelf())
	{
		printf("RevertToSelf  - %d/n", GetLastError());
		return 0;
	}
	return 1;
}