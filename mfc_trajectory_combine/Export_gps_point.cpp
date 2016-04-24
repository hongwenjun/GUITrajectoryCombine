// Export_gps_point.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_trajectory_combine.h"
#include "Export_gps_point.h"
#include "afxdialogex.h"
#include "bdgps_api.h"
#include <fstream>
#pragma warning(disable:4996)
//   �����̲߳����ṹ��
typedef struct
{
	const char *filename;   //�켣�ļ���
	CCoolListCtrl *p_list_ctrl; //ָ��list ctrol�ؼ�
	int fraction;    //�켣�ļ��ֶ���
} PARAMETER;

// CExport_gps_point �Ի���

IMPLEMENT_DYNAMIC(CExport_gps_point, CDialogEx)

CExport_gps_point::CExport_gps_point(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_GPS_POINT, pParent)
	, m_trajectory_file(_T(""))
	, m_edit_fraction(10)
{

}

CExport_gps_point::~CExport_gps_point()
{
}

void CExport_gps_point::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GPS_POINT, m_gps_point);
	DDX_Text(pDX, IDC_EDIT_TRAJ_FILE, m_trajectory_file);
	DDX_Text(pDX, IDC_EDIT_FRACTION, m_edit_fraction);
	DDV_MinMaxInt(pDX, m_edit_fraction, 0, 3600);
}


BEGIN_MESSAGE_MAP(CExport_gps_point, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GPS_FILE, &CExport_gps_point::OnBnClickedButtonGpsFile)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CExport_gps_point::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_TXT, &CExport_gps_point::OnBnClickedButtonTxt)
	ON_BN_CLICKED(IDC_BUTTON_MAP, &CExport_gps_point::OnBnClickedButtonMap)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CExport_gps_point ��Ϣ�������


BOOL CExport_gps_point::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_gps_point.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_gps_point.InsertColumn(0, "γ��", LVCFMT_LEFT, 150);
	m_gps_point.InsertColumn(1, "����", LVCFMT_LEFT, 150);
	m_gps_point.InsertColumn(2, "ʱ��(Km/H)", LVCFMT_LEFT, 120);
	m_gps_point.InsertColumn(3, "ʱ���", LVCFMT_LEFT, 210);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CExport_gps_point::OnBnClickedButtonGpsFile()
{
	CFileDialog file_dlg(TRUE);  //����ѡ���ļ��Ի���
	file_dlg.m_ofn.lpstrFilter = "gz or bin files(*.gz)|*.gz||";
	file_dlg.m_ofn.lpstrTitle = "ѡ��켣�ļ�";
	m_trajectory_file = "";
	UpdateData(FALSE);
	if (file_dlg.DoModal() == IDOK)
	{
		    m_trajectory_file = file_dlg.GetPathName();  //��ȡȫ·��
			UpdateData(FALSE);  //��Edit��ʾѡ�е��ļ�ȫ·��
			return;
	}
}

//////////////////////////////////////////////////////////////////////////
///       ���б�ؼ�����GPS��
//////////////////////////////////////////////////////////////////////////
void InsertGPSPoint(CCoolListCtrl *ptr_list, GPS_POINT *ptr_point, int pos)
{
	if ((ptr_list == NULL) || (ptr_point == NULL))   return;
	CString cstr("");
	ptr_list->SetItemColor(pos, RGB(167, 253, 255));
	//���뾭��
	cstr.Format("%.5f", (float)ptr_point->latitude / 100000);
	ptr_list->InsertItem(pos, cstr);

	//����γ��
	cstr.Format("%.5f", (float)ptr_point->longitude / 100000);
	ptr_list->SetItemText(pos, 1, cstr);

	//����ʱ��
	cstr.Format("%.1f", (float)ptr_point->speed / 100 * 3600 / 1000);
	ptr_list->SetItemText(pos, 2, cstr);

	//����ʱ���
	CTime time(ptr_point->timestamp);
	ptr_list->SetItemText(pos, 3, time.Format("%Y-%m-%d %H:%M:%S"));
}

//////////////////////////////////////////////////////////////////////////
///                      �̴߳�����:
///      �����ڸ�list�ؼ��������Ŀ̫��ʱ,��ɳ�����濨��,Ϊ�˽���������,�ڴ�
///      ʹ���̴߳�������������list�ؼ��в����ȡ����VIP������,�������̴߳���
///      �Ͳ��Ῠ����.
//////////////////////////////////////////////////////////////////////////
void ThreadFunc(LPVOID pParam)
{
	PARAMETER *p_parameter = (PARAMETER *)pParam;
	FILE* ptr_file = fopen(p_parameter->filename, "rb"); // ���������ļ�
	if (NULL == ptr_file)
	{
		AfxMessageBox("�ļ�������,���ļ�ʧ��!");
		return;
	}

	int32_t gz_header_3byte = GZ_HEADER_3BYTE;  //gz�ļ�ͷ
	fread(&gz_header_3byte, 1, sizeof(gz_header_3byte), ptr_file);
	rewind(ptr_file);

	// �ж��Ƿ���gz�ļ�
	bool is_gzfile = (GZ_HEADER_3BYTE == (gz_header_3byte & 0xFFFFFF));

	char* buffer = NULL;
	long data_size = get_fileSize(p_parameter->filename);
	if (is_gzfile)
	{
		// ��ȡgz�ļ����ڴ�
		data_size = get_gzbinSize(p_parameter->filename); //gz�ļ���ѹ��Ĵ�С

		gzFile gzf = gzopen(p_parameter->filename, "rb");
		buffer = new char[data_size + 1];
		buffer[data_size] = 0;

		if (gzread(gzf, buffer, data_size) < 0)
		{
			AfxMessageBox("��ȡgz�ļ����ִ���!");
			return;
		}
		gzclose(gzf);
	}
	else
	{
		// ���Ѿ���ѹ��bin�ļ����ڴ�
		buffer = new char[data_size];
		fread(buffer, 1, data_size, ptr_file);
	}

	// �����ڴ��е�����
	char* gps_buffer = buffer;
	GPS_FILEHEAD* gps_filehead = (GPS_FILEHEAD*)gps_buffer;   // bin�ļ�ͷ
	if (!((gps_filehead->empty_1 == 0x00) && (gps_filehead->data_pos == 0x18)))
	{
		AfxMessageBox("����:��ѡ�Ĳ��ǹ켣�ļ�!");
		return;
	}

    // ���ݾɰ汾 02 04 05 �͵�ǰ 06�汾���ݣ�ʹ��ָ����ˣ��������ݽṹ��һ����
	int ver_offset = 0;
	if (gps_filehead->data_ver == 5)
	{
		ver_offset = sizeof(int32_t);
	}
	else if (gps_filehead->data_ver <= 4)
	{
		ver_offset = 2 * sizeof(int32_t);
	}

	size_t GPS_POINT_SIZE = sizeof(GPS_POINT);
	GPS_POINT* gps_point = (GPS_POINT*)(gps_buffer + gps_filehead->data_pos);  // ��һ��GPS��

	int gps_point_total = (data_size - gps_filehead->data_pos) / (GPS_POINT_SIZE - ver_offset); // GPS�����
	if (gps_point_total == 0)  //�жϹ켣�ļ����Ƿ���GPS��
	{
		AfxMessageBox("���ļ�û��GPS��!");
		return;
	}

	int item_count = p_parameter->p_list_ctrl->GetItemCount();
	if (item_count)  //����б�ǿ�,����ɾ��list������Ŀ,Ȼ������µ���Ŀ
	{
		p_parameter->p_list_ctrl->DeleteAllItems();
		item_count = 0;
	}

   try
   {
	  const int tmp = gps_point_total;  //��ʱ��������GPS������
	  if (p_parameter->fraction == 0)  //�������Ϊ0,���ʾ��ӡ����GPS��
	  {
		 while (gps_point_total--)
		 {
			InsertGPSPoint(p_parameter->p_list_ctrl, gps_point++, item_count++);
			gps_point = (GPS_POINT*)((char*)gps_point - ver_offset);    // ���ݾɰ汾 02 04 05 ���� 06�汾���ݶ�����������ָ�����
		 }
	  }
	  else
	  {
		 int count = tmp / p_parameter->fraction;  //��fraction����ȡ��
		 if (count == 0)   //�ж��Ƿ�ֶ�������
		 {
			 CString err_str;
			 err_str.Format("%d", tmp);
			 AfxMessageBox("�ֶ�������,������ֶ�������" + err_str + "������!");
			 return;
		 }
		 while (gps_point_total--)
		 {
			if ((count % (tmp / p_parameter->fraction)) == 0)
			{
				InsertGPSPoint(p_parameter->p_list_ctrl, gps_point, item_count++);
				count--;
			}
			else
			{
				if (--count == 0)
				{
					count = tmp / p_parameter->fraction;
				}
			}

			gps_point++;
			gps_point = (GPS_POINT*)((char*)gps_point - ver_offset);
		  }

		  gps_point = (GPS_POINT*)((char*)gps_point + ver_offset);
		  InsertGPSPoint(p_parameter->p_list_ctrl, --gps_point, item_count++); // �յ�ڵ�
	  }
    }
	catch (...)  //    ����δ֪�쳣
	{
		AfxMessageBox("δ֪����:�����ļ���!");
	}
	fclose(ptr_file);
	delete [] buffer;
}

void CExport_gps_point::OnBnClickedButtonExport()
{
	UpdateData(TRUE); //���ݴ��͵��ؼ�����
	if (0 == m_trajectory_file.GetLength())
	{
		AfxMessageBox("����ѡ��켣�ļ�!");
		return;
	}
	PARAMETER *p_parameter = new PARAMETER;  //�Զ����̲߳����ṹ
	p_parameter->filename = m_trajectory_file.GetBuffer(m_trajectory_file.GetLength());
	p_parameter->p_list_ctrl = &m_gps_point;
	p_parameter->fraction = m_edit_fraction;

	//�����̴߳�����gps��
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, p_parameter, 0, NULL);
}


void CExport_gps_point::OnBnClickedButtonTxt()
{
	size_t item_count = m_gps_point.GetItemCount();
	if (item_count == 0)
	{
		AfxMessageBox("�б�Ϊ��,�뵼��GPS����ٱ���");
		return;
	}
	
	CString def_filename = m_trajectory_file;
	int n = def_filename.Find('.');
	def_filename = def_filename.Left(n) + ".gps";  //��ȡĬ���ļ���

	CFileDialog export_dlg(FALSE, "txt", def_filename);  //���������ļ��Ի���  "txt"������ʾĬ���ļ���׺��
	export_dlg.m_ofn.lpstrFilter = "txt files(*.txt)|*.txt||";
	export_dlg.m_ofn.lpstrTitle = "����GPS���ı��ļ�";
	if (export_dlg.DoModal() == IDOK)
	{
		std::ofstream gps_file(export_dlg.GetPathName(), std::ios::out);
		if (!gps_file.is_open())
		{
			AfxMessageBox("�����ļ�ʧ��,δ֪����!");
			return;
		}
		// �����ļ�����
		gps_file << "����" << "\t" << "γ��" << "\t" << "ʱ��(Km/H)" << "\t" << "ʱ���" << std::endl;

		for (size_t i = 0; i < item_count; i++)
		{
			gps_file << m_gps_point.GetItemText(i, 0) << "\t"       //д�뾭��
				     << m_gps_point.GetItemText(i, 1) << "\t"       //д��γ��
				     << m_gps_point.GetItemText(i, 2) << "\t"       //д��ʱ��
				     << m_gps_point.GetItemText(i, 3) << std::endl; //д��ʱ���
		}
		gps_file.close();
		AfxMessageBox("�����ı��ļ��ɹ�!");
	}
}


void CExport_gps_point::OnBnClickedButtonMap()
{
	size_t item_count = m_gps_point.GetItemCount();
	if (0 == item_count)
	{
		AfxMessageBox("�б�Ϊ��,�뵼��GPS����ٱ���");
		return;
	}

	CString def_filename = m_trajectory_file;
	int n = def_filename.Find('.');
	def_filename = def_filename.Left(n);  //��ȡĬ���ļ���

	CFileDialog export_dlg(FALSE, "html", def_filename);  //���������ļ��Ի���  "html"������ʾĬ���ļ���׺��
	export_dlg.m_ofn.lpstrFilter = "html files(*.html)|*.html||";
	export_dlg.m_ofn.lpstrTitle = "���ɵ�ͼ�ļ�";

	if (export_dlg.DoModal() == IDOK)
	{
		std::ofstream map_file(export_dlg.GetPathName(), std::ios::out);
		if (!map_file.is_open())
		{
			AfxMessageBox("���ɵ�ͼ�ļ�ʧ��,δ֪����!");
			return;
		}

		//   ����html��ͼ�ļ�
		map_file << "<html><head>\n<meta http-equiv=\"refresh\" content=\"2; url=http://www.google.cn/maps/dir/";
		for (size_t i = 0; i < item_count; i++)
		{
			map_file << m_gps_point.GetItemText(i, 0) << "%09"       //д�뾭��
				     << m_gps_point.GetItemText(i, 1) << "/";         //д��γ��
		}
		map_file << "\" />\n</head><body>\n�Զ���ת�� ��google��ͼ <p>\n http://www.google.cn/maps/dir/ gps����/ gps���� / gps���� <p>\n";
		map_file << "��ǰ�켣�����ļ�:" << m_trajectory_file << "<p>\n</body></html>";
		map_file.close();
		//AfxMessageBox("�����ĵ�ͼ���ɹ�!");
		
		ShellExecute(NULL, "open", export_dlg.GetPathName(), "", "", SW_SHOWNORMAL);
	}
}


void CExport_gps_point::OnPaint()
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


HBRUSH CExport_gps_point::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		//pDC->SetTextColor(RGB(0, 0, 255));   //����������ɫ��ɫ   
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
