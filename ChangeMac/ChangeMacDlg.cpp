
// ChangeMacDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChangeMac.h"
#include "ChangeMacDlg.h"
#include "afxdialogex.h"

//#pragma comment(

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChangeMacDlg �Ի���




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


// CChangeMacDlg ��Ϣ�������

BOOL CChangeMacDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	if(!InitNetInfo())
	{
		AfxMessageBox("������Ϣ����ʧ��!");
		return TRUE;
	}
	this->m_Seconds="5";
	this->UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChangeMacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int cur=this->m_AdapterLists.GetCurSel();
	char csConnectName[256]={0};
	this->m_AdapterLists.GetLBText(m_AdapterLists.GetCurSel(),csConnectName);
	//ȡ����Ӧ��������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ȡ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString oldmac=this->str_Mac;
	this->UpdateData(TRUE);
	if(this->str_Mac.GetLength()>0xC)
	{
		this->str_Mac=oldmac;
		this->UpdateData(FALSE);
	}
	//��ʱ�Ȳ��ж��Ƿ�Ϸ�
}


void CChangeMacDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE);
	int cur=this->m_AdapterLists.GetCurSel();
	char csConnectName[256]={0};
	this->m_AdapterLists.GetLBText(m_AdapterLists.GetCurSel(),csConnectName);
	cMac.ResetMac(csConnectName,0);
	//���¼���
	cMac.Destory();
	this->InitNetInfo();
}
