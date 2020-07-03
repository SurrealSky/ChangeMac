
// ChangeMacDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChangeMac.h"
#include "ChangeMacDlg.h"
#include "afxdialogex.h"

//#pragma comment(

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeMacDlg 对话框




CChangeMacDlg::CChangeMacDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeMacDlg::IDD, pParent)
	, ip_Address(0)
	, ip_Mask(0)
	, ip_Gate(0)
	, str_Mac(_T(""))
	, m_Seconds(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeMacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_AdapterLists);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, ip_Address);
	DDX_IPAddress(pDX, IDC_IPADDRESS2, ip_Mask);
	DDX_IPAddress(pDX, IDC_IPADDRESS3, ip_Gate);
	DDX_Text(pDX, IDC_EDIT1, str_Mac);
	DDX_Text(pDX, IDC_EDIT2, m_Seconds);
}

BEGIN_MESSAGE_MAP(CChangeMacDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CChangeMacDlg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CChangeMacDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CChangeMacDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CChangeMacDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CChangeMacDlg 消息处理程序

BOOL CChangeMacDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	if(!InitNetInfo())
	{
		AfxMessageBox("网卡信息加载失败!");
		return TRUE;
	}
	this->m_Seconds="5";
	this->UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChangeMacDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeMacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChangeMacDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CChangeMacDlg::InitNetInfo()
{
	if(!cMac.Init())
	{
		return false;
	}
	this->m_AdapterLists.ResetContent();
	map<DWORD ,string >::iterator m1_Iter;
	for ( m1_Iter = cMac.m_AdapterInfo.csConnectName.begin( ); m1_Iter != cMac.m_AdapterInfo.csConnectName.end( ); m1_Iter++ )
	{
		this->m_AdapterLists.AddString(m1_Iter->second.c_str());
	}
	this->m_AdapterLists.SetCurSel(0);
	this->UpdateData(FALSE);
	CChangeMacDlg::OnSelchangeCombo1();
	return TRUE;
}


void CChangeMacDlg::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int cur=this->m_AdapterLists.GetCurSel();
	char csConnectName[256]={0};
	this->m_AdapterLists.GetLBText(m_AdapterLists.GetCurSel(),csConnectName);
	//取出对应的网卡名
	DWORD index=0;
	map<DWORD ,string >::iterator m1_Iter;
	for ( m1_Iter = cMac.m_AdapterInfo.csConnectName.begin( ); m1_Iter != cMac.m_AdapterInfo.csConnectName.end( ); m1_Iter++ )
	{
		if(strcmp(m1_Iter->second.c_str(),csConnectName)==0)
		{
			index=m1_Iter->first;
			break;
		}
	}

	PIP_ADAPTER_INFO pIpAdapterInfo=cMac.m_AdapterInfo.pIpAdapterInfo;

	while(pIpAdapterInfo)
	{
		if(index==pIpAdapterInfo->Index)
		{
			this->ip_Address=htonl(inet_addr(pIpAdapterInfo->IpAddressList.IpAddress.String));
			this->ip_Gate=htonl(inet_addr(pIpAdapterInfo->GatewayList.IpAddress.String));
			this->ip_Mask=htonl(inet_addr(pIpAdapterInfo->IpAddressList.IpMask.String));
			char mac[0x128]={0};
			sprintf(mac,"%02X%02X%02X%02X%02X%02X",pIpAdapterInfo->Address[0],pIpAdapterInfo->Address[1],pIpAdapterInfo->Address[2],
				pIpAdapterInfo->Address[3],pIpAdapterInfo->Address[4],pIpAdapterInfo->Address[5],pIpAdapterInfo->Address[6]);
			this->str_Mac=mac;
			break;
		}
		pIpAdapterInfo=pIpAdapterInfo->Next;
	}
	this->UpdateData(FALSE);
}


void CChangeMacDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//取的适配器名
	this->UpdateData(TRUE);
	int cur=this->m_AdapterLists.GetCurSel();
	char csConnectName[256]={0};
	this->m_AdapterLists.GetLBText(m_AdapterLists.GetCurSel(),csConnectName);
	cMac.ChangeMac(csConnectName,this->str_Mac.GetBuffer(0),str_Mac.GetLength());
	cMac.Destory();
	this->InitNetInfo();
}


void CChangeMacDlg::OnEnChangeEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString oldmac=this->str_Mac;
	this->UpdateData(TRUE);
	if(this->str_Mac.GetLength()>0xC)
	{
		this->str_Mac=oldmac;
		this->UpdateData(FALSE);
	}
	//暂时先不判断是否合法
}


void CChangeMacDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	int cur=this->m_AdapterLists.GetCurSel();
	char csConnectName[256]={0};
	this->m_AdapterLists.GetLBText(m_AdapterLists.GetCurSel(),csConnectName);
	cMac.ResetMac(csConnectName,0);
	//重新加载
	cMac.Destory();
	this->InitNetInfo();
}
