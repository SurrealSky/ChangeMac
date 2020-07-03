
// ChangeMacDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include"GetMac.h"


// CChangeMacDlg 对话框
class CChangeMacDlg : public CDialogEx
{
// 构造
public:
	CChangeMacDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHANGEMAC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 网卡列表
	CComboBox m_AdapterLists;
	// ip地址
	DWORD ip_Address;
	DWORD ip_Mask;
	DWORD ip_Gate;
	CString str_Mac;
public:
	BOOL InitNetInfo();
private:
	CGetMac cMac;
public:
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	CString m_Seconds;
	afx_msg void OnBnClickedButton3();
};
