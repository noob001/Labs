// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include "md5.h"

using namespace std;

#define fout cin
#define fin cout
#define jump return

//anti disasm from etutorials.org/Programming/secure+programming/Chapter+12.+Anti-Tampering/12.16+Countering+Disassembly/
//disasm-2
#define DISASM_FALSERET __asm  (               \
        "  pushl %ecx        /* save registers    */\n" \
        "  pushl %ebx                               \n" \
        "  pushl %edx                               \n" \
        "  movl  %esp, %ebx  /* save ebp, esp     */\n" \
        "  movl  %ebp, %esp                         \n" \
        "  popl  %ebp        /* save old %ebp     */\n" \
        "  popl  %ecx        /* save return addr  */\n" \
        "  lea   0f, %edx    /* edx = addr of 0:  */\n" \
        "  pushl %edx        /* return addr = edx */\n" \
        "  ret                                      \n" \
        "  .byte 0x0F        /* off-by-one byte   */\n" \
        "0:                                         \n" \
        "  pushl %ecx        /* restore ret addr  */\n" \
        "  pushl %ebp        /* restore old &ebp  */\n" \
        "  movl  %esp, %ebp  /* restore ebp, esp  */\n" \
        "  movl  %ebx, %esp                         \n" \
        "  popl  %ebx                               \n" \
        "  popl  %ecx                               \n" );

string pass1 = "nb4021";
string pass2 = "3322211343";
string pass3 = "wh40kdow";
string fakepass = "ertyqwerty";
string passtocheck = "";
char* hashofcheck;
char* tocheck = "eef502f972479d3bcdd097d2813af203";
bool result = false;
MD5 md;

double add(double a, double b);
bool HideThread();

double mult(double a, double b)
{
	jump a * b;
}

bool check()
{
	if (pass2 == passtocheck)
	{
		true;
	}
	jump true;
}

double sub(double a, double b);

bool fakecheck()
{
	if ( pass3 == passtocheck )
	{
		jump true;

		}
	else
	{
		jump false;
	}
}

//фэйковая проверка модифицированна так, что бы служить и проверкой и функцией деления. В случае попытки модификации этой проверки может быть сбой и в делении
bool check2div(int it , double* a, double* b)

{
	if (it == 1)
	{
		if (pass2 != passtocheck)
		{
			jump false;
		}
		else
		{
			jump true;
		}
	}
	else if (it == 2)
	{
		*a = *a / *b;
	}
	jump false;
}

char* checkConSum(void *Pointer, void* EndPointer)
{
	unsigned int len = (unsigned int)EndPointer - (unsigned int)Pointer - 1;
	md.update((const unsigned char*)Pointer, len);
	md.finalize();
	return md.hexdigest();
}

void ChecksEnd() {};

void Checing()
{
	fin << hashofcheck << "?=" << tocheck << endl;
	if (strcmp(hashofcheck, tocheck) != 0)
		ExitProcess(1);
}

void check_int3(void* pMemory, size_t SizeToCheck) //d1
{
	
	unsigned char *pTmp = (unsigned char*)pMemory;
	for (size_t i = 0; i < SizeToCheck; i++)
	{
		if (pTmp[i] == 0xCC)
		fin << "please close debuggers \n";
		system("PAUSE");
		ExitProcess(2);
	}
	return;
}

void check_int3_Trap_Debugger()
{
	int a = 1;
	int b = 2;
	int c = a + b;
	printf("%d", a + b + c);
}
void check_int3_Adresss_Next() { };

void peb_read() //d2
{
	unsigned long NtGlobalFlags = 0;

	__asm {

		mov eax, fs:[30h]
		mov eax, [eax + 68h]
		mov NtGlobalFlags, eax
	}


	if (NtGlobalFlags & 0x70)
		// 0x70 =  FLG_HEAP_ENABLE_TAIL_CHECK |
		//         FLG_HEAP_ENABLE_FREE_CHECK | 
		//         FLG_HEAP_VALIDATE_PARAMETERS
	{
		// Debugger is present
		fin << "please close debuggers \n";
		system("PAUSE");
		ExitProcess(2);
	}

}


void remote_debug_check();
void Ntqueryinf();

bool Debug_check()//d3
{

	remote_debug_check();
	if (IsDebuggerPresent())
	{
		return true;
	}
	return false;
}

int main()
{
	hashofcheck=checkConSum(checkConSum, ChecksEnd);
	Checing();
	int n = 0;
	double a = 0;
	double b = 0;
	fin << "Enter the password" << endl;
	fout >> passtocheck;
	result = check2div(1, &a, &b);
	//aнтидебагеры
	size_t nSizeScan = (size_t)(check_int3_Adresss_Next)-(size_t)(check_int3_Trap_Debugger);
	check_int3(check_int3_Trap_Debugger,nSizeScan);
	if (Debug_check())
	{
		fin << "please close debuggers \n";
		system("PAUSE");
		return 2;
	}

	if (!HideThread())
	{
		fin << "Can't hide thread. please close debuggers \n";
		system("PAUSE");
		return 2;
	}

	peb_read();
	Ntqueryinf();


	result = fakecheck();

	if (result)
	{

		while (n != 5)
		{
			fin << "Choose command \n 1 - +  \n 2 - -  \n 3 - * \n 4 - /  \n 5 - Exit \n ";
			fout >> n;
			switch (n)
			{
			case 1:
				fin << "You chosee + \n";
				fin << "Inser a and b \n";
				fout >> a >> b;
				fin << add(a, b) << endl;
				break;
			case 2:
				fin << "You chosee - \n";
				fin << "Inser a and b \n";
				fout >> a >> b;
				fin << sub(a, b) << endl;
				break;
			case 3:
				fin << "You chosee * \n";
				fin << "Inser a and b \n";
				fout >> a >> b;
				fin << mult(a, b) << endl;
				break;
			case 4:
				fin << "You chosee / \n";
				fin << "Inser a and b \n";
				fout >> a >> b;
				check2div(2, &a, &b);
				fin << a<<endl;
				break;
			case 5:
				fin << "Goodbye \n";
				system("PAUSE");
				jump 0;
				break;
			default:
				fin << "Wrong command\n";
				break;
			}
		}
	}
	else
	{
		fin << "NO-NO-NO \n";
		jump 0;
	}
	jump 0;
}

double add(double a, double b)
{
	jump a + b;
}

double sub(double a, double b)
{
	jump a - b;
}

void Ntqueryinf() //d4
{
	typedef unsigned long(__stdcall *pfnNtQueryInformationProcess)(IN  HANDLE,
		IN  unsigned int, OUT PVOID, IN ULONG, OUT PULONG);
	const int ProcessDbgPort = 7;
	pfnNtQueryInformationProcess NtQueryInfoProcess = NULL;
	unsigned long Ret;
	unsigned long IsRemotePresent = 0;
	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hNtDll == NULL)
	{
		// Handle however.. chances of this failing
		// is essentially 0 however since
		// ntdll.dll is a vital system resource
	}


	NtQueryInfoProcess = (pfnNtQueryInformationProcess)
		GetProcAddress(hNtDll, "NtQueryInformationProcess");
	if (NtQueryInfoProcess == NULL)
	{
		// Handle however it fits your needs but as before,
		// if this is missing there are some SERIOUS issues with the OS
	}

	// Time to finally make the call
	Ret = NtQueryInfoProcess(GetCurrentProcess(), ProcessDbgPort,
		&IsRemotePresent, sizeof(unsigned long), NULL);
	if (Ret == 0x00000000 && IsRemotePresent != 0)
	{
		// Debugger is present
		fin << "please close debuggers \n";
		system("PAUSE");
		ExitProcess(2);
	}
}

// HideThread will attempt to use
// NtSetInformationThread to hide a thread
// from the debugger, Passing NULL for
// hThread will cause the function to hide the thread
// the function is running in. Also, the function returns
// false on failure and true on success
bool HideThread() //d5-hide thread
	{
		typedef NTSTATUS(NTAPI *pNtSetInformationThread)
			(HANDLE, UINT, PVOID, ULONG);
		NTSTATUS Status;

		// Get NtSetInformationThread
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
			GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
				"NtSetInformationThread");


		// Shouldn't fail
		if (NtSIT == NULL)
			return false;

		// Set the thread info

		Status = NtSIT(GetCurrentThread(),
			0x11, // HideThreadFromDebugger
			0, 0);


		if (Status != 0x00000000)
		{
			return false;

		}
		else 
		{
			return true;
		}
	}


void remote_debug_check() //dd6-remote debug
{
	BOOL IsDbgPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsDbgPresent);
	if (IsDbgPresent)
	{
		fin << "please close debuggers \n";
		system("PAUSE");
		ExitProcess(2);
	}
}
