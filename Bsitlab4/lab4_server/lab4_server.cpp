// lab4_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<winsock2.h>
#include<iostream>
#include<string.h>
#include<Aclapi.h>
#include<Sddl.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

SOCKET client;

	void os()
	{
		cout << " - OS version" << endl;
		OSVERSIONINFOEX osvi;
		//ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((LPOSVERSIONINFOW)&osvi);
		char OSVer[32];
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
			sprintf(OSVer, "Windows 8 or higher");
		else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
			sprintf(OSVer, "Windows 7");
		else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
			sprintf(OSVer, "Windows Vista");
		else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			sprintf(OSVer, "Windows XP");
		else sprintf(OSVer, "Can't recognize :(");
		send(client, OSVer, sizeof(OSVer), 0);
	}

	void time()
	{
		cout << " - Time" << endl;
		char tmp[100];
		SYSTEMTIME sm;
		GetLocalTime(&sm);
		sprintf(tmp, "%02d:%02d:%02d", sm.wHour, sm.wMinute, sm.wSecond);
		send(client, tmp, sizeof(tmp), 0);
	}

	void uptime()
	{
		cout << " - UpTime" << endl;
		char time[100];
		int hour, min, sec, msec = GetTickCount();
		hour = msec / (1000 * 60 * 60);
		min = msec / (1000 * 60) - hour * 60;
		sec = (msec / 1000) - (hour * 60 * 60) - min * 60;
		sprintf(time, "%02d:%02d:%02d", hour, min, sec);
		send(client, time, sizeof(time), 0);
	}

	void memorystatus()
	{
		cout << " - Memory status" << endl;
		char bytes[1024];
		char bytestmp[100];
		MEMORYSTATUS stat;
		GlobalMemoryStatus(&stat);
		sprintf(bytes, "In use %u MBytes \n", ((stat.dwTotalPhys - stat.dwAvailPhys) / 1048576));
		sprintf(bytestmp, "AvailPageFile %u MBytes \n", (stat.dwAvailPageFile / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "TotalPageFile %u MBytes \n", (stat.dwTotalPageFile / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "AvailPhys %u MBytes \n", (stat.dwAvailPhys / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "TotalPhys %u MBytes \n", (stat.dwTotalPhys / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "AvailVirtual %u MBytes \n", (stat.dwAvailVirtual / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "TotalVirtual %u MBytes \n", (stat.dwTotalVirtual / 1048576));
		strcat(bytes, bytestmp);
		sprintf(bytestmp, "MemoryLoad %u percents \n", stat.dwMemoryLoad);
		strcat(bytes, bytestmp);
		send(client, bytes, sizeof(bytes), 0);
	}

	void freespace()
	{
		char tmp[100];
		cout << " - Free space" << endl;
		char disks[26][3] = { 0 };
		DWORD dr = GetLogicalDrives();
		int i, n, count = 0;
		for (i = 0; i < 26; i++)
		{
			n = ((dr >> i) & 0x00000001);
			if (n == 1)
			{
				disks[count][0] = char(65 + i);
				disks[count][1] = ':';
				disks[count][2] = '/';
				LPCSTR localdisk = disks[count];
				count++;
				double freeSpace;
				DWORD s, b, f, c;
				if (GetDriveTypeA(localdisk) == DRIVE_FIXED)
				{
					if (GetDiskFreeSpaceA(localdisk, &s, &b, &f, &c))
					{
						freeSpace = (double)f * (double)s *
							(double)b / 1024.0 / 1024.0 / 1024.0;
						sprintf(tmp, "%s %f GBytes", localdisk,
							freeSpace);
						send(client, tmp, sizeof(tmp), 0);
					}
				}
			}
		}
	}

	void rightfile()
	{
		cout << " - See access for file" << endl;
		char ObjectType = 0;
		const int BUF_SIZE = 256;
		char DirName[BUF_SIZE] = "";
		char tmp[30] = "enter path";

		send(client, tmp, sizeof(tmp), 0);
		recv(client, (char *)&DirName, sizeof(DirName), 0);

		WCHAR out_buf[1024];
		PSECURITY_DESCRIPTOR psd = 0;
		PACL dacl;
		ACL_SIZE_INFORMATION acl_size;
		ACCESS_ALLOWED_ACE* pACE;
		int i = 0, j = 0;
		unsigned long mask;
		wchar_t wtext[50];
		mbstowcs(wtext, DirName, strlen(DirName) + 1);//Plus null
		LPWSTR ptr = wtext;
		if (GetNamedSecurityInfo(ptr, SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, &dacl, NULL, &psd) != ERROR_SUCCESS)
		{
			char tmp[] = "access denied";
			cout << tmp << endl;
			send(client, tmp, sizeof(tmp), 0);

		}
		else
		{
			char tmp[] = "Wait";
			cout << tmp << endl;
			send(client, tmp, sizeof(tmp), 0);

			GetAclInformation(dacl, &acl_size, sizeof(acl_size), AclSizeInformation);
			int num = acl_size.AceCount;
			char str[2];
			sprintf(str, "%d", num);
			cout << str << endl;
			send(client, str, sizeof(str), 0);
			for (i = 0; i < acl_size.AceCount; i++)
			{
				cout << "i = " << i;
				WCHAR user[256] = { 0 };
				WCHAR domain[256] = { 0 };
				ZeroMemory(&out_buf, sizeof(out_buf));
				DWORD user_len = sizeof(user);
				DWORD domain_len = sizeof(domain);
				SID_NAME_USE SidName;
				PSID pSID;
				GetAce(dacl, i, (PVOID*)&pACE);
				pSID = (PSID)(&(pACE->SidStart));
				// The LookupAccountSid accepts a security identifier (SID) as input. 
				// It retrieves the name of the account for this SID 
				// and the name of the first domain on which this SID is found. 
				if (LookupAccountSid(NULL, pSID, user, &user_len, domain, &domain_len, &SidName))
				{
					wcscat(out_buf, L"\nUser: ");
					wcscat(out_buf, user);
					cout << out_buf << endl;

					mask = ((ACCESS_ALLOWED_ACE *)pACE)->Mask;
					wcscat(out_buf, L"\n General accesses from ACL: \n");
					if (mask & DELETE)
						wcscat(out_buf, L"\t - delete access\n");
					if (mask & READ_CONTROL)
						wcscat(out_buf, L"\t - read security descriptor and owner.\n");
					if (mask & WRITE_DAC)
						wcscat(out_buf, L"\t - write DACL.\n");
					if (mask & WRITE_OWNER)
						wcscat(out_buf, L"\t - assign the write owner.\n");
					if (mask & SYNCHRONIZE)
						wcscat(out_buf, L"\t - allows a process to wait for an object to enter the signaled state.\n");
					if (mask & GENERIC_ALL)
						wcscat(out_buf, L"\t - all possible access rights.\n");
					if (mask & GENERIC_EXECUTE)
						wcscat(out_buf, L"\t - execute.\n");
					if (mask & GENERIC_WRITE)
						wcscat(out_buf, L"\t - write.\n");
					if (mask & GENERIC_READ)
						wcscat(out_buf, L"\t - read.\n");
					// SPECIFIC RIGHTS FOR FILES & FOLDERS 
					int length = strlen(DirName);
					// If Directory 
					if (DirName[length - 1] == '\\')
					{
						wcscat(out_buf, L"\n Directory rights: \n");
						if (mask & FILE_LIST_DIRECTORY)
							wcscat(out_buf, L"\t - list the contents of the directory;\n");
						if (mask & FILE_ADD_FILE)
							wcscat(out_buf, L"\t - create a file in the directory.\n");
						if (mask & FILE_ADD_SUBDIRECTORY)
							wcscat(out_buf, L"\t - create a subdirectory.\n");
						if (mask & FILE_DELETE_CHILD)
							wcscat(out_buf, L"\t - delete a directory.\n");
						if (mask & FILE_TRAVERSE)
							wcscat(out_buf, L"\t - the directory can be relocated.\n");
					}
					// else if File 
					else
					{
						wcscat(out_buf, L"\n File rights: \n");
						if (mask & FILE_READ_DATA)
							wcscat(out_buf, L"\t - read file.\n");
						if (mask & FILE_WRITE_DATA)
							wcscat(out_buf, L"\t - write into a file.\n");
						if (mask &
							FILE_APPEND_DATA)
							wcscat(out_buf, L"\t - append data to the file.\n");
						if (mask & FILE_EXECUTE)
							wcscat(out_buf, L"\t - execute a file.\n");
						if (mask & FILE_DELETE_CHILD)
							wcscat(out_buf, L"\t - delete file.\n");
					}
					char str[1024];
					sprintf(str, "%S", out_buf);
					cout << str << endl;
					send(client, str, sizeof(str), 0);
				}

			}
		}
	}

	void rightkey()
	{
		cout << " - See access for key" << endl;
		char ObjectType = 0;
		const int BUF_SIZE = 256;
		char DirName[BUF_SIZE] = "";
		char tmp[30] = "enter path";
		send(client, tmp, sizeof(tmp), 0);
		recv(client, (char *)&DirName, sizeof(DirName), 0);
		WCHAR out_buf[1024];
		PSECURITY_DESCRIPTOR psd = 0;
		PACL dacl;
		ACL_SIZE_INFORMATION acl_size;
		ACCESS_ALLOWED_ACE* pACE;
		unsigned long mask;
		int i = 0, j = 0;
		wchar_t wtext[256];
		mbstowcs(wtext, DirName, strlen(DirName) + 1); //Plus null
		LPWSTR ptr = wtext;

		if (GetNamedSecurityInfo(ptr, SE_REGISTRY_KEY,
			DACL_SECURITY_INFORMATION, NULL, NULL, &dacl,
			NULL, &psd) != ERROR_SUCCESS)
		{
			char tmp[20] = "access denied";
			send(client, tmp, sizeof(tmp), 0);
		}
		else
		{

			char tmp[] = "Wait";
			send(client, tmp, sizeof(tmp), 0);
			GetAclInformation(dacl, &acl_size, sizeof(acl_size),
				AclSizeInformation);
			int num = acl_size.AceCount;
			char str[2];
			sprintf(str, "%d", num);
			send(client, str, sizeof(str), 0);

			for (i = 0; i < acl_size.AceCount; i++)
			{
				WCHAR user[256] = { 0 };
				WCHAR domain[256] = { 0 };
				ZeroMemory(&out_buf, sizeof(out_buf));
				DWORD user_len = sizeof(user);
				DWORD domain_len = sizeof(domain);
				SID_NAME_USE SidName;
				PSID pSID;
				GetAce(dacl, i, (PVOID*)&pACE);
				pSID = (PSID)(&(pACE->SidStart));
				if (LookupAccountSid(NULL, pSID, (LPWSTR)user,
					&user_len, (LPWSTR)domain, &domain_len, &SidName))
				{
					wcscat(out_buf, L"\nUser: ");
					wcscat(out_buf, user);
					mask = ((ACCESS_ALLOWED_ACE *)pACE)->Mask;
					wcscat(out_buf, L"\n General accesses from ACL:	\n");

					if (mask & DELETE)
						wcscat(out_buf, L"\t - delete access\n");

					if (mask & READ_CONTROL)
						wcscat(out_buf, L"\t - read security descriptor and owner.\n");

					if (mask & WRITE_DAC)
						wcscat(out_buf, L"\t - write DACL.\n");

					if (mask & WRITE_OWNER)
						wcscat(out_buf, L"\t - assign the write owner.\n");

					if (mask & SYNCHRONIZE)
						wcscat(out_buf, L"\t - allows a process to wait for an object to enter the signaled state.\n");

					if (mask & GENERIC_ALL)
						wcscat(out_buf, L"\t - all possible access rights.\n");

					if (mask & GENERIC_EXECUTE)
						wcscat(out_buf, L"\t - execute.\n");

					if (mask & GENERIC_WRITE)
						wcscat(out_buf, L"\t - write.\n");

					if (mask & GENERIC_READ)
						wcscat(out_buf, L"\t - read.\n");

					// SPECIFIC RIGHTS FOR FILES & FOLDERS

					wcscat(out_buf, L"\n Registry Key rights: \n");
					if (mask & KEY_ALL_ACCESS)
						wcscat(out_buf, L"\t - all access right.\n");

					if (mask & KEY_CREATE_SUB_KEY)
						wcscat(out_buf, L"\t - create a subkey ofregistry key.\n");

					if (mask & KEY_ENUMERATE_SUB_KEYS)
						wcscat(out_buf, L"\t - enumerate the subkeys of registry key.\n");

					if (mask & KEY_NOTIFY)
						wcscat(out_buf, L"\t - change notifications for registry key / subkeys.\n");

					if (mask & KEY_QUERY_VALUE)
						wcscat(out_buf, L"\t - ask values of registrykey.\n");

					if (mask & KEY_SET_VALUE)
						wcscat(out_buf, L"\t - create/delete/set registry value.\n");
				}

				char str[1024];
				sprintf(str, "%S", out_buf);
				send(client, str, sizeof(str), 0);
			}
		}
	}

	void ownerfile()
	{
		cout << " - Owner of file" << endl;
		WCHAR out_buf[500];
		PSECURITY_DESCRIPTOR pSD = 0;
		PSID pOwnerSid;
		ACCESS_ALLOWED_ACE* pACE;
		int i = 0, j = 0;
		PACL dacl;
		ACL_SIZE_INFORMATION acl_size;
		const int BUF_SIZE = 256;
		char DirName[BUF_SIZE] = "";
		char tmp[] = "enter path";
		send(client, tmp, sizeof(tmp), 0);
		recv(client, (char *)&DirName, sizeof(DirName), 0);
		ZeroMemory(&out_buf, sizeof(out_buf));
		wchar_t wtext[50];
		mbstowcs(wtext, DirName, strlen(DirName) + 1);//Plus null
		LPWSTR ptr = wtext;

		if (GetNamedSecurityInfo(ptr, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &dacl, NULL, &pSD) != ERROR_SUCCESS)
		{
			char tmp[20] = "access denied";
			send(client, tmp, sizeof(tmp), 0);
		}

		else
		{
			if (GetNamedSecurityInfo(ptr, SE_FILE_OBJECT,
				OWNER_SECURITY_INFORMATION, &pOwnerSid, NULL, NULL, NULL, &pSD) !=
				ERROR_SUCCESS)
			{
				char tmp[20] = "access denied";
				send(client, tmp, sizeof(tmp), 0);
			}

			else
			{
				char tmp[] = "Wait";
				send(client, tmp, sizeof(tmp), 0);
				WCHAR user[30];
				WCHAR domain[30];
				DWORD user_len = sizeof(user);
				DWORD domain_len = sizeof(domain);
				GetAclInformation(dacl, &acl_size, sizeof(acl_size), AclSizeInformation);
				ZeroMemory(&user, sizeof(user));
				ZeroMemory(&out_buf, sizeof(out_buf));
				SID_NAME_USE SidName;
				PSID pSID;

				if (LookupAccountSid(NULL, pOwnerSid, (LPWSTR)user, &user_len, (LPWSTR)domain, &domain_len, &SidName))
				{
					wcscat(out_buf, user);
				}

				char str[1024];
				sprintf(str, "%S", out_buf);
				send(client, str, sizeof(str), 0);
			}
		}
	}



int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA ws;
	
	if (WSAStartup(MAKEWORD(1, 1), &ws) == SOCKET_ERROR)
		cout << "Error while WSAStartup: " << WSAGetLastError() << endl;

	SOCKET ServerSocket;
	if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "Error while ñreating socket: " << WSAGetLastError() << endl;
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(666); 
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind(ServerSocket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "Error binding: " << WSAGetLastError() << endl;
	}

	if (listen(ServerSocket, 5) == SOCKET_ERROR) // 5-äëèíà î÷åðåäè
	{
		cout << "Error listening: " << WSAGetLastError() << endl;
	}

	client = accept(ServerSocket, (LPSOCKADDR)&addr, 0);

	if (client == INVALID_SOCKET)
	{
		cout << "Socket_Error" << endl;
		return 1;
	} 

	while (true)
	{
		char buff[2];
		recv(client, &buff[0], sizeof(buff), 0);
		switch (buff[0])
		{
		case '1':
			os();
			break;
		case '2':
			time();
			break;
		case '3':
			uptime();
			break;
		case '4':
			memorystatus();
			break;
		case '5':
			freespace();
			break;
		case '6':
			rightfile();
			break;
		case '7':
			rightkey();
			break;
		case '8':
			ownerfile();
			break;
		case '9':
			cout << "End" << endl;
			Sleep(2000);
			closesocket(ServerSocket);
			WSACleanup();
			return 0;
		}
	}

}