
// mfc_trajectory_combineDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Export_gps_point.h"
#include "Trajecory_combine.h"
#include "AboutToolDlg.h"

// Cmfc_trajectory_combineDlg �Ի���
class Cmfc_trajectory_combineDlg : public CDialogEx
{
// ����
public:
	Cmfc_trajectory_combineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_TRAJECTORY_COMBINE_DIALOG };
#endif
	CExport_gps_point exp_gps_dlg;  //����gps��Ի���
	CTrajecory_combine traje_combine_dlg;  //�켣�ϲ��Ի���
	CAboutToolDlg about_tool_dlg;   //��������Ի���
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
	CTabCtrl m_tab;  //tab�ؼ�����
	afx_msg void OnSelchangeTabFun(NMHDR *pNMHDR, LRESULT *pResult);
};
