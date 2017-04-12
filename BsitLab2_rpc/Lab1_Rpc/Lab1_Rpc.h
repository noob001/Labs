

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Feb 27 14:16:39 2017
 */
/* Compiler settings for Lab1_Rpc.idl:
    Oicf, W4, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __Lab1_Rpc_h__
#define __Lab1_Rpc_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __Lab1_Rpc_INTERFACE_DEFINED__
#define __Lab1_Rpc_INTERFACE_DEFINED__

/* interface Lab1_Rpc */
/* [implicit_handle][version][uuid] */ 

int Log_in( 
    /* [string][in] */ char *login,
    /* [string][in] */ char *password,
    /* [string][in] */ char *domain);

int Read_file( 
    /* [string][in] */ char *filename,
    /* [in] */ long offset,
    /* [out] */ char data[ 1024 ]);

int Write_file( 
    /* [string][in] */ char *filename,
    /* [in] */ char data[ 1024 ],
    /* [in] */ long count);

int Create_file( 
    /* [string][in] */ char *filename);

int Delete_file( 
    /* [string][in] */ char *filename);


extern handle_t hLab1_rpcHandle;


extern RPC_IF_HANDLE Lab1_Rpc_v1_0_c_ifspec;
extern RPC_IF_HANDLE Lab1_Rpc_v1_0_s_ifspec;
#endif /* __Lab1_Rpc_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


