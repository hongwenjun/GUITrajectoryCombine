// AboutToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_trajectory_combine.h"
#include "AboutToolDlg.h"
#include "afxdialogex.h"


// CAboutToolDlg �Ի���

IMPLEMENT_DYNAMIC(CAboutToolDlg, CDialogEx)

CAboutToolDlg::CAboutToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ABOUT_TOOL, pParent)
{

}

CAboutToolDlg::~CAboutToolDlg()
{
}

void CAboutToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAboutToolDlg ��Ϣ�������


void CAboutToolDlg::OnPaint()
{
	CPaintDC  dc(this);
	CRect  rect;
	GetClientRect(&rect);
	CDC  dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap  bmpBackground;
	bmpBackground.LoadBitmap(IDB_BITMAP_GPS_BACK);
	BITMAP  bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap  *pbmpOld = dcMem.SelectObject(&bmpBackground); dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0,
	bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
}


HBRUSH CAboutToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//�����еľ�̬�ı���͸������ʾ����ɫ
	if (nCtlColor == CTLCOLOR_DLG)
	{
		//���ضԻ������ɫ   
		hbr = (HBRUSH)::CreateSolidBrush(RGB(146, 161, 202));
	}
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		//�þ�̬�ı���͸��   
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 255));   //����������ɫ��ɫ   
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
