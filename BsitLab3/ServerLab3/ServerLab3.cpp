#include "stdafx.h"
#include <windows.h>
#include <initguid.h>
#include <assert.h>
#include <stdio.h>
#include "../BSITLab3/ServerDcom.h"
#include <ctime>
#include <iostream>
#include <string.h>
#include <locale.h>



using namespace std;
HANDLE g_hExitEvent;
//

inline ULONG ComponentAddRef()
{
	ULONG ul = CoAddRefServerProcess();
	wprintf(TEXT("ComponentAddRef(%ld)\n"), ul);
	return ul;
}

inline ULONG ComponentRelease()
{
	ULONG ul = CoReleaseServerProcess();

	wprintf(TEXT("ComponentRelease(%ld)\n"), ul);
	if (ul == 0) {
		SetEvent(g_hExitEvent);
	}
	return ul;
}
inline long ByteLen(wchar_t *pwsz)
{
	return (sizeof(wchar_t)*(wcslen(pwsz) + 1));
}

// Определение класса компонента на основе интерфейса ITest
class Server : public ITest {
private:
	LONG m_lRefCount;

public:

	Server() : m_lRefCount(0) { ComponentAddRef(); }
	~Server() { ComponentRelease(); }

	static HRESULT CreateObject(LPUNKNOWN pUnkOuter, REFIID riid,
		void** ppv);
public:

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	// Реализация метода AddRef
	STDMETHODIMP_(ULONG) AddRef(void)
	{
		return InterlockedIncrement(&m_lRefCount);
	}

	// Реализация метода Release
	STDMETHODIMP_(ULONG) Release(void)
	{
		long lCount = InterlockedDecrement(&m_lRefCount);
		if (lCount == 0)
		{
			delete this;
		}
		return lCount;
	}
	

	STDMETHODIMP GetText(wchar_t **pszText)
	{
		*pszText = static_cast<wchar_t *>(TEXT("Wake up, neo!\n"));
		return S_OK;
	}


};

// Реализация метода CreateObject
HRESULT Server::CreateObject(LPUNKNOWN pUnkOuter,
	REFIID riid,
	void** ppv)
{
	*ppv = NULL;

	if (pUnkOuter != NULL) { return CLASS_E_NOAGGREGATION; }

	Server * pCOM = new Server;
	if (pCOM == NULL) { return E_OUTOFMEMORY; }

	HRESULT hr = pCOM->QueryInterface(riid, ppv);
	if (FAILED(hr)) { delete pCOM; }

	return hr;
}
// Реализация метода QueryInterface
STDMETHODIMP Server::QueryInterface(REFIID riid, void** ppv)
{
	if (ppv == NULL) { return E_INVALIDARG; }

	if (riid == IID_IUnknown) {
		*ppv = static_cast<ITest *>(this);
	}
	else if (riid == IID_ITest) {
		*ppv = static_cast<ITest *>(this);
	}
	else {
		*ppv = NULL; return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return S_OK;
}
// Фабрика класса
class ServerFactory : public IClassFactory
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef(void)
	{
		return 1;
	}
	STDMETHODIMP_(ULONG) Release(void)
	{
		return 1;
	}
	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter,
		REFIID riid,
		void **ppv)
	{
		return Server::CreateObject(pUnkOuter, riid, ppv);
	}

	STDMETHODIMP LockServer(BOOL fLock)
	{
		if (fLock) {
			ComponentAddRef();
		}
		else {
			ComponentRelease();
		}
		return S_OK;
	}
};
// Реализация метода QueryInterface
STDMETHODIMP ServerFactory::QueryInterface(REFIID riid, void** ppv)
{
	if (ppv == NULL) { return E_INVALIDARG; }

	if (riid == IID_IUnknown) {
		*ppv = static_cast<IClassFactory *>(this);
	}
	else if (riid == IID_IClassFactory) {
		*ppv = static_cast<IClassFactory *>(this);
	}
	else {
		*ppv = NULL; return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return S_OK;
}
void RegisterServer()
{
	wchar_t szKey[MAX_PATH];
	wchar_t szValue[MAX_PATH];
	HKEY hKey = 0;

	LPOLESTR lpCLSID;
	LPOLESTR lpAPPID;

	StringFromCLSID(CLSID_Test, &lpCLSID);
	StringFromCLSID(APPID_Test, &lpAPPID);

	// HKEY_CLASSES_ROOT\CLSID\{3CC346E1-D3CC-11d1-915C-006008052F2D}


	wcscpy(szKey, TEXT("CLSID\\"));
	wcscat_s(szKey, lpCLSID);

	RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hKey);
	wcscpy(szValue, TEXT("ServerDcom"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)szValue, ByteLen(szValue));

	//  "AppID"="{B475EB51-D3CD-11d1-915C-006008052F2D}"

	wcscpy(szValue, lpAPPID);
	RegSetValueEx(hKey, TEXT("AppID"), 0, REG_SZ,(BYTE*)szValue, ByteLen(szValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\CLSID\{3CC346E1-D3CC-11d1-915C-006008052F2D}\LocalServer32

	wcscat_s(szKey, TEXT("\\LocalServer32"));

	RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hKey);
	GetModuleFileName(0, szValue, MAX_PATH);
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)szValue, ByteLen(szValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\AppID\ServerDcom.exe
	//      "AppID"="{B475EB51-D3CD-11d1-915C-006008052F2D}"

	wcscpy(szKey, TEXT("AppID\\ServerDcom.exe"));
	RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hKey);

	wcscpy(szValue, lpAPPID);
	RegSetValueEx(hKey, TEXT("AppID"), 0, REG_SZ,(BYTE*)szValue, ByteLen(szValue));
	RegCloseKey(hKey);



	wcscpy(szKey, TEXT("AppID\\"));
	wcscat_s(szKey, lpAPPID);
	RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hKey);
	wcscpy(szValue, TEXT("ServerDcom"));
	RegSetValueEx(hKey, 0, 0, REG_SZ,(BYTE*)szValue, ByteLen(szValue));
	RegCloseKey(hKey);

	CoTaskMemFree(lpCLSID);
	CoTaskMemFree(lpAPPID);

}


void UnregisterServer()
{
	LPOLESTR lpCLSID;
	LPOLESTR lpAPPID;

	StringFromCLSID(CLSID_Test, &lpCLSID);
	StringFromCLSID(APPID_Test, &lpAPPID);

	long error = 0;

	wchar_t szKey[MAX_PATH];
	wchar_t szKey2[MAX_PATH];

	wcscpy(szKey, TEXT("CLSID\\"));
	wcscat_s(szKey, lpCLSID);

	wcscpy(szKey2, szKey);

	wcscat_s(szKey, TEXT("LocalServer32"));

	error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey2);


	error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);


	wcscpy(szKey, TEXT("AppID\\"));
	wcscat_s(szKey, lpAPPID);

	error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);


	error = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("AppID\\ServerDcom.exe"));

}
void main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	HRESULT hr;
	ServerFactory g_ClassFactory;
	BOOL fRegisterServer = FALSE;
	BOOL fUnregisterServer = FALSE;
	BOOL fEmbedding = FALSE;
	DWORD dwRegister;
	// Информация о сервере
	printf("Server ready:\n");
	g_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(g_hExitEvent);
	// registration if required

	if (argc > 1) {
		printf("argc > 1! \n");
		if (_stricmp(argv[1], "/REGSERVER") == 0)
		{
			printf("REGSERVER! \n");
			fRegisterServer = TRUE;
			RegisterServer();
			printf("Registered server!\n");
			return;
		}
		else  if (_stricmp(argv[1], "/UNREGSERVER") == 0)
		{
			printf("UNREGSERVER! \n");
			fUnregisterServer = TRUE;
			UnregisterServer();
			printf("UnRegistered server!\n");
			return;
		}

	}

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));



	hr = CoRegisterClassObject(CLSID_Test,
		&g_ClassFactory,
		CLSCTX_LOCAL_SERVER,
		REGCLS_MULTIPLEUSE,
		&dwRegister);
	assert(SUCCEEDED(hr));
	g_ClassFactory.Release();

	WaitForSingleObject(g_hExitEvent, INFINITE);

	CoRevokeClassObject(dwRegister);

	CoUninitialize();

	Sleep(5000);



}
