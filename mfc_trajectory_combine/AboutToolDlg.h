#pragma once


// CAboutToolDlg �Ի���

class CAboutToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutToolDlg)

public:
	CAboutToolDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutToolDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ABOUT_TOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
