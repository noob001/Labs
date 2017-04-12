

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Mar 06 13:25:44 2017
 */
/* Compiler settings for ServerDcom.idl:
    Oi, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "ServerDcom.h"

#define TYPE_FORMAT_STRING_SIZE   11                                
#define PROC_FORMAT_STRING_SIZE   17                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _ServerDcom_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } ServerDcom_MIDL_TYPE_FORMAT_STRING;

typedef struct _ServerDcom_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } ServerDcom_MIDL_PROC_FORMAT_STRING;

typedef struct _ServerDcom_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } ServerDcom_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const ServerDcom_MIDL_TYPE_FORMAT_STRING ServerDcom__MIDL_TypeFormatString;
extern const ServerDcom_MIDL_PROC_FORMAT_STRING ServerDcom__MIDL_ProcFormatString;
extern const ServerDcom_MIDL_EXPR_FORMAT_STRING ServerDcom__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITest_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITest_ProxyInfo;

HRESULT STDMETHODCALLTYPE ITest_GetText_Proxy( 
    ITest * This,
    /* [string][out] */ wchar_t **pszMessage)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall(
                 ( PMIDL_STUB_DESC  )&Object_StubDesc,
                 (PFORMAT_STRING) &ServerDcom__MIDL_ProcFormatString.Format[0],
                 ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const ServerDcom_MIDL_PROC_FORMAT_STRING ServerDcom__MIDL_ProcFormatString =
    {
        0,
        {
			0x33,		/* FC_AUTO_HANDLE */
			0x4c,		/* Old Flags:  object, */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* x86 stack size = 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 14 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */

			0x0
        }
    };

static const ServerDcom_MIDL_TYPE_FORMAT_STRING ServerDcom__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/*  8 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITest, ver. 0.0,
   GUID={0x0FFBDAAD,0xFCA7,0x11D2,{0x8F,0xF4,0x00,0x10,0x5A,0xA4,0x5B,0xDC}} */

#pragma code_seg(".orpc")
static const unsigned short ITest_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ITest_ProxyInfo =
    {
    &Object_StubDesc,
    ServerDcom__MIDL_ProcFormatString.Format,
    &ITest_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITest_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ServerDcom__MIDL_ProcFormatString.Format,
    &ITest_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
const CINTERFACE_PROXY_VTABLE(4) _ITestProxyVtbl = 
{
    &IID_ITest,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    ITest_GetText_Proxy
};

const CInterfaceStubVtbl _ITestStubVtbl =
{
    &IID_ITest,
    &ITest_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_ServerDcom_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    ServerDcom__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _ServerDcom_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITestProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _ServerDcom_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITestStubVtbl,
    0
};

PCInterfaceName const _ServerDcom_InterfaceNamesList[] = 
{
    "ITest",
    0
};


#define _ServerDcom_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ServerDcom, pIID, n)

int __stdcall _ServerDcom_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_ServerDcom_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo ServerDcom_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ServerDcom_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ServerDcom_StubVtblList,
    (const PCInterfaceName * ) & _ServerDcom_InterfaceNamesList,
    0, /* no delegation */
    & _ServerDcom_IID_Lookup, 
    1,
    1,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

