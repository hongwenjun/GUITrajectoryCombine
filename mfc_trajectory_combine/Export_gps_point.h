#pragma once
#include "afxcmn.h"
#include "CoolListCtrl.h"  //�Զ�����Ը�����ɫ��CListCtrl�ؼ�

// CExport_gps_point �Ի���

class CExport_gps_point : public CDialogEx
{
	DECLARE_DYNAMIC(CExport_gps_point)

public:
	CExport_gps_point(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExport_gps_point();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GPS_POINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CCoolListCtrl m_gps_point;
	CString m_trajectory_file;  // �켣�ļ�
	int m_edit_fraction;  // �켣�ļ��ֶ���

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGpsFile();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonTxt();
	afx_msg void OnBnClickedButtonMap();
	
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
