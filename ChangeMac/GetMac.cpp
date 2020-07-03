#include "StdAfx.h"
#include "GetMac.h"
#include<Mprapi.h>
#include<IPHlpApi.h>
#include "Setupapi.h"


#pragma comment(lib,"Setupapi.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Mprapi.lib")


CGetMac::CGetMac(void)
{
	this->m_AdapterInfo.pIpAdapterInfo=NULL;
	this->m_AdapterInfo.iCount=0;
	this->m_AdapterInfo.csAdapterName.clear();
	this->m_AdapterInfo.csConnectName.clear();
}

BOOL CGetMac::Init()
{
	if(!GetIpAddrsInfo())
	{
		return false;
	}
	return true;
}

void CGetMac::Destory()
{
	if(this->m_AdapterInfo.pIpAdapterInfo!=NULL)
	{
		delete this->m_AdapterInfo.pIpAdapterInfo;
		this->m_AdapterInfo.pIpAdapterInfo=NULL;
	}
	this->m_AdapterInfo.iCount=0;
	this->m_AdapterInfo.csAdapterName.clear();
	this->m_AdapterInfo.csConnectName.clear();
}


CGetMac::~CGetMac(void)
{
	if(this->m_AdapterInfo.pIpAdapterInfo!=NULL)
	{
		delete this->m_AdapterInfo.pIpAdapterInfo;
		this->m_AdapterInfo.pIpAdapterInfo=NULL;
	}
	this->m_AdapterInfo.iCount=0;
	this->m_AdapterInfo.csAdapterName.clear();
	this->m_AdapterInfo.csConnectName.clear();
}

BOOL CGetMac::GetIpAddrsInfo()
{
	/******************************************* 
    *通过Iphlpapi库获取网卡信息和个数 
    ********************************************/  
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();  
    ULONG stSize = sizeof(IP_ADAPTER_INFO);  
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //获得其大小  
  
    if (ERROR_BUFFER_OVERFLOW == nRel)                      //重新申请所需要的空间  
    {  
        delete pIpAdapterInfo;  
        pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];  
        nRel=GetAdaptersInfo(pIpAdapterInfo, &stSize);   
    }  
    if (ERROR_SUCCESS == nRel)                              //获取信息成功  
    {  
        m_AdapterInfo.pIpAdapterInfo = pIpAdapterInfo;  
        m_AdapterInfo.iCount = 0;  
        while (pIpAdapterInfo)                          //获取网卡个数 
        { 
			m_AdapterInfo.iCount++;
			pIpAdapterInfo=pIpAdapterInfo->Next;
        }  
    }	 
	GetAdapterNames();  //获取网卡名
	GetConnectNames();  //获取网络连接名

	return true;
}

BOOL CGetMac::GetAdapterNames()
{
	PIP_ADAPTER_INFO pIpAdapterInfo=m_AdapterInfo.pIpAdapterInfo; 
	while(pIpAdapterInfo)
	{
		m_AdapterInfo.csAdapterName.insert(pair<int,string>(pIpAdapterInfo->Index,pIpAdapterInfo->Description));
		pIpAdapterInfo=pIpAdapterInfo->Next;
	 }
	return true;
}

BOOL CGetMac::GetConnectNames()
{
	/******************************************* 
    *通过mprapi库获取连接名称 
    *并通过index将网卡信息和连接名称相关联 
    ********************************************/  
    HANDLE   hMprConfig;                    //连接信息的句柄  
    DWORD   dwRet=0;                        //返回值  
    PIP_INTERFACE_INFO   plfTable = NULL;   //接口信息表  
    DWORD   dwBufferSize=0;                 //接口信息表空间大小  
  
    dwRet = MprConfigServerConnect(NULL, &hMprConfig);  //获得句柄  
    dwRet = GetInterfaceInfo(NULL, &dwBufferSize);      //获得接口信息表大小  
  
    if(dwRet == ERROR_INSUFFICIENT_BUFFER)              //获得接口信息  
    {   
        plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(),   
                                                  HEAP_ZERO_MEMORY, dwBufferSize);   
        GetInterfaceInfo(plfTable, &dwBufferSize);   
    }   
  
  
    TCHAR   szFriendName[256];                   //接口名称  
    DWORD   tchSize = sizeof(TCHAR) * 256;   
    ZeroMemory(&szFriendName, tchSize);    
  
    for (int i = 0; i < plfTable-> NumAdapters; i++)   
    {   
        IP_ADAPTER_INDEX_MAP   AdaptMap;         //接口信息  
        AdaptMap = plfTable->Adapter[i];   
  
        dwRet = MprConfigGetFriendlyName(hMprConfig, AdaptMap.Name,  
            (PWCHAR)szFriendName, tchSize);      //获得连接名称unicode   
        USES_CONVERSION;  
        char* pName = W2A((PWCHAR)szFriendName);                           //转换为ansi  
		m_AdapterInfo.csConnectName.insert(pair<int,string>(AdaptMap.Index,pName));
    }   
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plfTable); 

	return true;
}

BOOL CGetMac::ChangeMac(char *ConnectName,char *mac,int len)
{
	//获得适配器instance
	//获得当前适配器连接名称
	CString csConnectName(ConnectName);
	//找到相应的适配器信息
	DWORD index=0;
	map<DWORD ,string >::iterator m1_Iter;
	for ( m1_Iter = m_AdapterInfo.csConnectName.begin( ); m1_Iter != m_AdapterInfo.csConnectName.end( ); m1_Iter++ )
	{
		if(strcmp(m1_Iter->second.c_str(),csConnectName.GetBuffer(0))==0)
		{
			index=m1_Iter->first;
			break;
		}
	}
	//m_AdapterInfo.csConnectName[index].c_str();
	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	while(pIpAdapterInfo)
	{
		if(pIpAdapterInfo->Index==index)
		{
			break;
		}
		pIpAdapterInfo=pIpAdapterInfo->Next;
	}
	//获得适配器instance
	char *strNetCfgInstanceId = pIpAdapterInfo->AdapterName;
	//获得MAC
	CString csMac(mac);
	if (csMac.GetLength()==0)
	{
		//为空时删除对应的键值
		//AfxMessageBox("mac地址不能为空");
		//return TRUE;
	}
	char *lpszMacID = csMac.GetBuffer(0);


	//修改注册表修改mAC
	BOOL bRet = FALSE;
	HKEY hKey = NULL;			

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}"),
		0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		TCHAR Name[1024];
		DWORD cName = 1024;

		while( RegEnumKeyEx(hKey, dwIndex, Name, &cName, 
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS )
		{

			HKEY hSubKey = NULL;
			if(RegOpenKeyEx(hKey,Name,0,KEY_ALL_ACCESS,&hSubKey) == ERROR_SUCCESS)
			{
				
				BYTE Data[1204];
				DWORD cbData = 1024;
				if(RegQueryValueEx(hSubKey,_T("NetCfgInstanceId"),NULL,NULL,Data,&cbData) == ERROR_SUCCESS)
				{
					if(_tcscmp((TCHAR*)Data,strNetCfgInstanceId) == 0)
					{
						if(lpszMacID == NULL)
						{
							//Delete the NetCfgInstanceId entry
							LONG nErr = RegDeleteValue(hSubKey, _T("NetworkAddress"));
							if( (nErr == ERROR_SUCCESS) || (nErr == ERROR_FILE_NOT_FOUND) )
							{
								bRet = TRUE;
								break;
							}							
						}
						else
						{
							//Add the NetCfgInstanceId entry
							if(RegSetValueEx(hSubKey, _T("NetworkAddress"),0,REG_SZ,(const BYTE*)lpszMacID,sizeof(TCHAR) * ((DWORD)_tcslen(lpszMacID) + 1)) == ERROR_SUCCESS)
							{
								bRet = TRUE;
								break;
							}							
						}												
					}
				}
				RegCloseKey(hSubKey);
			}					
			cName = 1024;
			dwIndex++;
		}
		RegCloseKey(hKey);
	}

	//因为修改MAC后需要重新启动来使用新的MAC

	//判断操作系统类型
	bool isXP=false;
	OSVERSIONINFOEX osinfo;
osinfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
if(GetVersionEx((OSVERSIONINFO*)&osinfo))
{
		if(osinfo.dwMajorVersion == 5 && osinfo.dwMinorVersion == 1) 
        { 
			isXP=true;
        } 
        else if(osinfo.dwMajorVersion == 5 && osinfo.dwMinorVersion == 2) 
        { 
			isXP=true;
        } 
        else if(osinfo.dwMajorVersion ==  6 && osinfo.dwMinorVersion == 0) 
        { 
			isXP=false;
        } 
        else if(osinfo.dwMajorVersion ==  6 && osinfo.dwMinorVersion == 1) 
        { 
			isXP=false;
        }else
		{
			isXP=false;
		}
}
	//禁用网卡
	if(isXP)
	{
		this->ReStartNetcard(FALSE);
		this->ReStartNetcard(TRUE);
	}else
	{
		CString csValue(csConnectName);
		CString CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" DISABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));

		//启动网卡
		CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" ENABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));
	}

	return bRet;
}

BOOL CGetMac::ResetMac(char *ConnectName,int len)
{
	//获得适配器instance
	//获得当前适配器连接名称
	CString csConnectName(ConnectName);
	//找到相应的适配器信息
	DWORD index=0;
	map<DWORD ,string >::iterator m1_Iter;
	for ( m1_Iter = m_AdapterInfo.csConnectName.begin( ); m1_Iter != m_AdapterInfo.csConnectName.end( ); m1_Iter++ )
	{
		if(strcmp(m1_Iter->second.c_str(),csConnectName.GetBuffer(0))==0)
		{
			index=m1_Iter->first;
			break;
		}
	}
	//m_AdapterInfo.csConnectName[index].c_str();
	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	while(pIpAdapterInfo)
	{
		if(pIpAdapterInfo->Index==index)
		{
			break;
		}
		pIpAdapterInfo=pIpAdapterInfo->Next;
	}
	//获得适配器instance
	char *strNetCfgInstanceId = pIpAdapterInfo->AdapterName;
	//删除注册表mAC
	BOOL bRet = FALSE;
	HKEY hKey = NULL;			

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}"),
		0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		TCHAR Name[1024];
		DWORD cName = 1024;

		while( RegEnumKeyEx(hKey, dwIndex, Name, &cName, 
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS )
		{

			HKEY hSubKey = NULL;
			if(RegOpenKeyEx(hKey,Name,0,KEY_ALL_ACCESS,&hSubKey) == ERROR_SUCCESS)
			{
				
				BYTE Data[1204];
				DWORD cbData = 1024;
				if(RegQueryValueEx(hSubKey,_T("NetCfgInstanceId"),NULL,NULL,Data,&cbData) == ERROR_SUCCESS)
				{
					if(_tcscmp((TCHAR*)Data,strNetCfgInstanceId) == 0)
					{
						//Delete the NetCfgInstanceId entry
						LONG nErr=RegDeleteValue(hSubKey,_T("NetworkAddress"));
						//LONG nErr = RegDeleteValue(hSubKey, _T("NetworkAddress"));
						if( (nErr == ERROR_SUCCESS) || (nErr == ERROR_FILE_NOT_FOUND) )
						{
							bRet = TRUE;
							break;
						}																		
					}
				}
				RegCloseKey(hSubKey);
			}					
			cName = 1024;
			dwIndex++;
		}
		RegCloseKey(hKey);
	}

	//因为修改MAC后需要重新启动来使用新的MAC

	//判断操作系统类型
	bool isXP=false;
	OSVERSIONINFOEX osinfo;
osinfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
if(GetVersionEx((OSVERSIONINFO*)&osinfo))
{
		if(osinfo.dwMajorVersion == 5 && osinfo.dwMinorVersion == 1) 
        { 
			isXP=true;
        } 
        else if(osinfo.dwMajorVersion == 5 && osinfo.dwMinorVersion == 2) 
        { 
			isXP=true;
        } 
        else if(osinfo.dwMajorVersion ==  6 && osinfo.dwMinorVersion == 0) 
        {  
			isXP=false;
        } 
        else if(osinfo.dwMajorVersion ==  6 && osinfo.dwMinorVersion == 1) 
        { 
			isXP=false;
        }else
		{
			isXP=false;
		}
}
	//禁用网卡
	if(isXP)
	{
		this->ReStartNetcard(FALSE);
		this->ReStartNetcard(TRUE);
	}else
	{
		CString csValue(csConnectName);
		CString CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" DISABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));

		//启动网卡
		CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" ENABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));
	}

	return bRet;
}

BOOL CGetMac::ReStartNetcard(bool bStatus)
{
	LPCTSTR HardwareId = TEXT("{4D36E972-E325-11CE-BFC1-08002BE10318}"); 
       GUID g = {0X4D36E972, 0XE325, 0X11CE, {0XBF, 0XC1, 0X08, 0X00, 0X2B, 0XE1, 0X03, 0X18}}; 
       DWORD NewState = 0; 
       //调用ddk函数，来禁用网卡 
       DWORD i = 0, err = 0; 
      
       HDEVINFO hDevInfo = NULL; 
       SP_DEVINFO_DATA spDevInfoData = {0}; 
      
       NewState = bStatus ? DICS_ENABLE : DICS_DISABLE; 
      
       //访问系统的硬件库 
       hDevInfo = SetupDiGetClassDevs(NULL, TEXT("PCI"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT); 
       if(INVALID_HANDLE_VALUE == hDevInfo) 
       { 
              _tprintf(TEXT("访问系统硬件出错!")); 
              return GetLastError(); 
       } 
      
       //枚举硬件，获得需要的接口 
       spDevInfoData.cbSize=sizeof(SP_DEVINFO_DATA); 
       memcmp(&(spDevInfoData.ClassGuid), &g, sizeof(GUID)); 
      
       for(i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) 
       { 
             
              PBYTE PropertyBuffer = NULL; 
              DWORD PropertyBufferSize = 0; 
             
              //获得硬件的属性值 
             
              while(!SetupDiGetDeviceRegistryProperty( 
                     hDevInfo, 
                     &spDevInfoData, 
                     SPDRP_CLASSGUID, 
                     NULL, 
                     PropertyBuffer, 
                     PropertyBufferSize, 
                     &PropertyBufferSize)) 
              { 
                     if(ERROR_INSUFFICIENT_BUFFER == GetLastError()) 
                     { 
                            if(PropertyBuffer) 
                            { 
                                   free(PropertyBuffer); 
                            } 
                            PropertyBuffer = (PBYTE)malloc(PropertyBufferSize); 
                     } 
                     else 
                     { 
                            break; 
                     }
              } 
             
              if(!PropertyBuffer) 
              { 
                     continue; 
              } 
             
              if(!_tcsicmp(HardwareId, (LPTSTR)PropertyBuffer)) 
              { 
                     SP_PROPCHANGE_PARAMS spPropChangeParams ; 
                    
                     free(PropertyBuffer); 
                    
                     spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER); 
                     spPropChangeParams.ClassInstallHeader.InstallFunction=DIF_PROPERTYCHANGE; 
                     spPropChangeParams.Scope = DICS_FLAG_GLOBAL; 
                     spPropChangeParams.StateChange = NewState; 
                    
                     if(!SetupDiSetClassInstallParams(hDevInfo,&spDevInfoData,(SP_CLASSINSTALL_HEADER*)&spPropChangeParams,sizeof(spPropChangeParams))) 
                     { 
                            err = GetLastError(); 
                            goto cleanup_DeviceInfo; 
                     } 
                    
                     if(!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,hDevInfo,&spDevInfoData)) 
                     { 
                            err = GetLastError(); 
                            goto cleanup_DeviceInfo; 
                     } 
                    
              } 
              else 
              { 
                     free(PropertyBuffer); 
              } 
       } 
      
cleanup_DeviceInfo : 
      
       SetupDiDestroyDeviceInfoList(hDevInfo); 
      
       return err; 
}

BOOL CGetMac::ExcuteCommand(char* pCommandParam)
{
	//初始化shellexe信息  
    SHELLEXECUTEINFO   ExeInfo;   
    ZeroMemory(&ExeInfo, sizeof(SHELLEXECUTEINFO));   
    ExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);   
    ExeInfo.lpFile = "cmd.exe";   
    ExeInfo.lpParameters = pCommandParam;  
    ExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;   
    ExeInfo.nShow = SW_HIDE;   
    ExeInfo.hwnd = NULL;  
    ExeInfo.lpVerb = NULL;  
    ExeInfo.lpDirectory = NULL;  
    ExeInfo.hInstApp = NULL;      
  
    //执行命令  
    ShellExecuteEx(&ExeInfo);  
  
    //等待进程结束  
    WaitForSingleObject(ExeInfo.hProcess, INFINITE); 
	
	return true;
}