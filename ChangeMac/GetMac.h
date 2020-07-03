#pragma once
#include<map>
using namespace std;
typedef struct _CAdapterInfo
{
	PIP_ADAPTER_INFO pIpAdapterInfo;
	unsigned int iCount;
	//char (*csConnectName)[256];
	//char (*csAdapterName)[256];
	//IP_ADDRESS_STRING *pIp;
	//IP_ADDRESS_STRING *pMask;
	//IP_ADDRESS_STRING Gata;
	map<DWORD ,std::string> csConnectName;
	map<DWORD ,std::string> csAdapterName;
}CAdapterInfo,*PCAdapterInfo;

class CGetMac
{
public:
	CGetMac(void);
	~CGetMac(void);
private:
	BOOL GetIpAddrsInfo();
	BOOL GetConnectNames();
	BOOL GetAdapterNames();
	BOOL ExcuteCommand(char* pCommandParam);
	BOOL ReStartNetcard(bool);
public:
	BOOL Init();
	void Destory();
	BOOL ChangeMac(char *,char *,int);
	BOOL ResetMac(char *,int);
public:
	CAdapterInfo m_AdapterInfo;
};

