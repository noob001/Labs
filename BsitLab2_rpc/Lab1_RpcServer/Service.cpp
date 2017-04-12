// WinServiceLab2.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <iostream> 
#include <tchar.h>


using namespace std;

SERVICE_STATUS        g_ServiceStatus;
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = NULL;

int RPCServer();

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
void InstallService();
void UninstallService();
void StartService();
void StopService();


#define SERVICE_NAME  _T("Lab2_rpc_fileService")		
// _T -  ������, ������� ������� ����������� �������������� ������� �� ������ ��� ��������� ��������������� define.
// ��� ��������, ��� � ��������� ����� �������� ������� "L"

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "install") == 0)
			InstallService();

		else if (strcmp(argv[1], "uninstall") == 0)
			UninstallService();

		else if (strcmp(argv[1], "start") == 0)
			StartService();

		else if (strcmp(argv[1], "stop") == 0)
			StopService();
		else printf("incorrect command");
	}

	SERVICE_TABLE_ENTRY ServiceTable[] =
	{
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
	{
		return GetLastError();
	}

	return 0;
}


VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{

	// ����������� ����������� ������� �������
	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
	//������� RegisterServiceCtrlHandler() ������������ �������, ������� ����� ������������ ����������� ������� � ������� 
	//(�����, ��������, ��� ��������� �������). � ������ ��������� ���������� ������� ���������� ���������� ������� �������
	//(service status handle). ��� ��������� ���������� ������� ���������� ������� ��������.
	DWORD Status = E_FAIL;
	if (g_StatusHandle == NULL)
	{
		cout << "Error while RegisterServiceCtrlHandler" << endl;
		goto EXIT;
	}

	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;  //����� �������� �� LocalSystem account
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;     //��� ������. ������ ��������
	g_ServiceStatus.dwControlsAccepted = 0;						//����������� ���� 
	g_ServiceStatus.dwWin32ExitCode = 0;						//��� ������, ������� ���������� ������ ��� ��������� �� ������ ��� ������� ��� ���������.
	g_ServiceStatus.dwServiceSpecificExitCode = 0;				//����������� ���, ������� ������ �� ������, ���� �� dwWin32ExitCode=ERROR_SERVICE_SPECIFIC_ERROR
	g_ServiceStatus.dwCheckPoint = 0;							//��� ������������ ���������� ���������. ������ ���� 0 ����� �� start, stop, pause, or continue operation pending
	g_ServiceStatus.dwWaitHint = 0; //��������� ����� , ����������� ��� ����������� �������, ���������, �����, ��� ���������� ������, � �������������. 
									//����� ��� ��� ��������� ���������� ������� ������, ������ ������ ������� ���� ��������� ����� � SetServiceStatus ������� ���� � ���������������� 
									//dwCheckPoint �������� ��� ��������� � dwCurrentState . ���� ���������� ������� , ��������� dwWaitHint ��������, � dwCheckPoint �� ������������� ��� 
									//dwCurrentState �� ����������, ��������� ���������� ��������� ���������� �������� ��� ������ ����� ������������ , ��� ��������� ������ � ������ ������ 
									//���� �����������.

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		cout << "ServiceMain: SetServiceStatus returned error" << endl;
	}


	//������ ������� ��������� �������
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		// ���� �������� ������ �������� �������, ������� ����������� �������� ������������ � �����
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			cout << "ServiceMain: SetServiceStatus returned error. Create event failed." << endl;
		}
		goto EXIT;
	}

	// ��������
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		cout << "ServiceMain: SetServiceStatus returned error. While starting" << endl;
	}
	//�������� �������� �������
	RPCServer();
	
	CloseHandle(g_ServiceStopEvent);

	// �������� ����������� �� ���������
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		cout << "ServiceMain: SetServiceStatus returned error" << endl;
	}

EXIT:
	return;
}


VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
	// ������� �� ��������� ��������
	// ServiceCtrlHandler ���������� ������ ���, ��� SCM ���� ������� �� ��������� ��������� �������. 
	// � �������� �� ���������� ��� �������� ���������� ���������� ������ �������.
	switch (CtrlCode)
	{
		//���������� ������ , ��� ��� ������ �������� ���� ������� ���������� � ��������� ���������� ���������� ��������.
	case SERVICE_CONTROL_INTERROGATE:
		break;
		//
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
		//������������� ��������� RPC
		RpcMgmtStopServerListening(NULL);
		SetEvent(g_ServiceStopEvent);
		return;
	case SERVICE_CONTROL_PAUSE:
		break;
		//���������� ���������������� ������, ��� ��� ������ �������������. 
	case SERVICE_CONTROL_CONTINUE:
		break;
	default:
		break;
	}
	// ������� SetServiceStatus ��������� ���������� � ������� ���������� ���������� �������� ��� ���������� ������.
	SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

}

void InstallService()
{
	//��������� SCM
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);
	if (serviceControlManager)
	{
		TCHAR path[_MAX_PATH + 1]; // _MAX_PATH	������������ ����� ������� ����
		//GetModuleFileName �������� ������ ���� � �����, ����������� ��������� ������. ������ ������ ���� �������� ������� ���������. 
		if (GetModuleFileName(0, path, sizeof(path) / sizeof(path[0])))
		{
			//������� ������
			SC_HANDLE service = CreateService(serviceControlManager, SERVICE_NAME, SERVICE_NAME,			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path, 0, 0, 0, 0, 0);
			if (service)
			{
				cout << "Service installed!" << endl;
				if (StartService(service, NULL, NULL)) cout << "Service is started" << endl;
				else cout << "Error while starting" << endl;
				CloseServiceHandle(service);
			}
			else cout << "Error! Can't open SCM" << endl;
		}
		CloseServiceHandle(serviceControlManager);
	}
}

void UninstallService()
{
	StopService();
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (serviceControlManager)
	{
		SC_HANDLE service = OpenService(serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS | DELETE);
		if (service)
		{
			SERVICE_STATUS status;
			// ������� QueryServiceStatus ��������� ������� ��������� ��������� ������.
			if (QueryServiceStatus(service, &status))
			{
				if (status.dwCurrentState == SERVICE_STOPPED)
				{
					if (DeleteService(service)) cout << "Service uninstalled!" << endl;

					else
					{
						cout << "Error! Can't open SCM" << endl;
						return;
					}
				}
			}

			CloseServiceHandle(service);
		}
		CloseServiceHandle(serviceControlManager);
	}
}

void StartService()
{
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (serviceControlManager)
	{
		SC_HANDLE hService = OpenService(serviceControlManager, SERVICE_NAME, SERVICE_ALL_ACCESS);
		if (hService)
		{
			if (StartService(hService, NULL, NULL)) cout << "Service is started" << endl;
			else
			{
				cout << "Error! Can't start service" << endl;
				return;
			}
		}
	}
	CloseServiceHandle(serviceControlManager);
}

void StopService()
{
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (serviceControlManager)
	{
		SC_HANDLE hService = OpenService(serviceControlManager, SERVICE_NAME, SERVICE_STOP);
		if (hService)
		{
			SERVICE_STATUS status;
			if (ControlService(hService, SERVICE_CONTROL_STOP, &status)) cout << "Service stopped" << endl;
			else
			{
				cout << "Error! Can't stop service" << endl;
				return;
			}
		}
	}
	CloseServiceHandle(serviceControlManager);
}
