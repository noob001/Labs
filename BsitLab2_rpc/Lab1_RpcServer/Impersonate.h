#include <lmcons.h>
#include <rpc.h>
#include <list>

struct RPC_CLIENT
{
	HANDLE Token;
	RPC_BINDING_HANDLE BindingHandle;
};

RPC_CLIENT* get_client();
RPC_CLIENT* add_client();

int hypocrite_mode();
int remove_the_mask();