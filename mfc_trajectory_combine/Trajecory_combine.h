#pragma once
#include "afxcmn.h"
#include "afxtempl.h"
#include "CoolListCtrl.h"  //�Զ�����Ը�����Ŀ��ɫ����

// CTrajecory_combine �Ի���

class CTrajecory_combine : public CDialogEx
{
	DECLARE_DYNAMIC(CTrajecory_combine)

public:
	CTrajecory_combine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTrajecory_combine();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GPS_COMBINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCoolListCtrl file_name_list;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTrackFile();
	CArray<CString, CString> array_filename;  //����ѡ�еĶ���ļ����ļ���
	afx_msg void OnBnClickedButtonCombine();
};
