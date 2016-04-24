
// mfc_trajectory_combineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_trajectory_combine.h"
#include "mfc_trajectory_combineDlg.h"
#include "afxdialogex.h"
#include "Export_gps_point.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmfc_trajectory_combineDlg �Ի���



Cmfc_trajectory_combineDlg::Cmfc_trajectory_combineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_TRAJECTORY_COMBINE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmfc_trajectory_combineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_FUN, m_tab);
}

BEGIN_MESSAGE_MAP(Cmfc_trajectory_combineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FUN, &Cmfc_trajectory_combineDlg::OnSelchangeTabFun)
END_MESSAGE_MAP()


// Cmfc_trajectory_combineDlg ��Ϣ�������

BOOL Cmfc_trajectory_combineDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MINIMIZE);---->������ÿ���,�����м���ȥ��
	//  ���³���һ����,���Զ���С������������,Ϊ�˽���������о�����
	//����......

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//tab�ؼ����ѡ�
	m_tab.InsertItem(0, "�ٶȵ�ͼGPS�㵼������");
	m_tab.InsertItem(1, "�ٶȵ�ͼ�켣�ļ��ϲ�����");
	m_tab.InsertItem(2, "���ڹ���");
	//ѡ������Ի���
	exp_gps_dlg.Create(IDD_DIALOG_GPS_POINT, &m_tab);
	traje_combine_dlg.Create(IDD_DIALOG_GPS_COMBINE, &m_tab);
	about_tool_dlg.Create(IDD_DIALOG_ABOUT_TOOL, &m_tab);
	CRect rc;
	m_tab.GetClientRect(&rc);
	//  �����ӶԻ����ڸ������е�λ��
	rc.top += 21;
	rc.bottom += 20;
	rc.left += 1;
	rc.right += 20;

	//  �ƶ��Ի���tab�ͻ���
	exp_gps_dlg.MoveWindow(&rc);
	traje_combine_dlg.MoveWindow(&rc);
	about_tool_dlg.MoveWindow(&rc);
	//�������غ���ʾ
	exp_gps_dlg.ShowWindow(true);
	traje_combine_dlg.ShowWindow(false);
	about_tool_dlg.ShowWindow(false);
	//����Ĭ��ѡ�
	m_tab.SetCurSel(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cmfc_trajectory_combineDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmfc_trajectory_combineDlg::OnPaint()
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
		CPaintDC  dc(this);
		CRect  rect;
		GetClientRect(&rect);
		CDC  dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap  bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP_BACK);
		BITMAP  bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap  *pbmpOld = dcMem.SelectObject(&bmpBackground); dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0,
		bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cmfc_trajectory_combineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmfc_trajectory_combineDlg::OnSelchangeTabFun(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	int cur_sel = m_tab.GetCurSel();
	switch (cur_sel)
	{
	case 0:
		exp_gps_dlg.ShowWindow(true);
		traje_combine_dlg.ShowWindow(false);
		about_tool_dlg.ShowWindow(false);
		break;
	case 1:
		traje_combine_dlg.ShowWindow(true);
		exp_gps_dlg.ShowWindow(false);
		about_tool_dlg.ShowWindow(false);
		break;
	case 2:
		about_tool_dlg.ShowWindow(true);
		traje_combine_dlg.ShowWindow(false);
		exp_gps_dlg.ShowWindow(false);
		break;
	default:
		break;
	}
}
