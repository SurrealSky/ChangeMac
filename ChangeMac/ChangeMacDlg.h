
// ChangeMacDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include"GetMac.h"


// CChangeMacDlg �Ի���
class CChangeMacDlg : public CDialogEx
{
// ����
public:
	CChangeMacDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHANGEMAC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �����б�
	CComboBox m_AdapterLists;
	// ip��ַ
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
