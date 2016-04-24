// Trajecory_combine.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_trajectory_combine.h"
#include "Trajecory_combine.h"
#include "afxdialogex.h"
#include "bdgps_api.h"
#include <map>  //ʹ��map�������GPS��
#pragma warning(disable:4996)
//   �����̲߳����ṹ��
typedef struct
{
	const char *dest_filename;   //�켣�ļ���
	CListCtrl *p_list_ctrl;  //ָ��list ctrol�ؼ�
	CArray<CString, CString> src_filename;  //���д������ļ���
} PARAMETER;

GPS_FILEHEAD  first_file_gps_filehead;  //GPS�ļ�ͷ
// CTrajecory_combine �Ի���

IMPLEMENT_DYNAMIC(CTrajecory_combine, CDialogEx)

CTrajecory_combine::CTrajecory_combine(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_GPS_COMBINE, pParent)
{

}

CTrajecory_combine::~CTrajecory_combine()
{
}

void CTrajecory_combine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, file_name_list);
}


BEGIN_MESSAGE_MAP(CTrajecory_combine, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TRACK_FILE, &CTrajecory_combine::OnBnClickedButtonTrackFile)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE, &CTrajecory_combine::OnBnClickedButtonCombine)
END_MESSAGE_MAP()


// CTrajecory_combine ��Ϣ�������


BOOL CTrajecory_combine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	file_name_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	file_name_list.InsertColumn(0, "�ļ���", LVCFMT_LEFT, 450);
	file_name_list.InsertColumn(1, "�ļ�����״̬", LVCFMT_LEFT, 178);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CTrajecory_combine::OnPaint()
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


void CTrajecory_combine::OnBnClickedButtonTrackFile()
{
	//����һ������ѡ�����ļ��ĶԻ���  "gz"������ʾĬ���ļ���׺��
	CFileDialog file_dlg(TRUE, "gz", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT); 
	file_dlg.m_ofn.lpstrFilter = "gz or bin files(*.gz)|*.gz||";
	file_dlg.m_ofn.lpstrTitle = "ѡ��켣�ļ�";

	//������Դ�500���ļ�
	file_dlg.m_ofn.nMaxFile = 500 * MAX_PATH;
	char *ch = new TCHAR[file_dlg.m_ofn.nMaxFile];
	file_dlg.m_ofn.lpstrFile = ch;

	//  �ڴ������
	ZeroMemory(file_dlg.m_ofn.lpstrFile, sizeof(TCHAR) * file_dlg.m_ofn.nMaxFile);

	if (file_name_list.GetItemCount() != 0)  //�б�Ϊ���������
	{
		file_name_list.DeleteAllItems();
		array_filename.RemoveAll();
	}
	
	if (file_dlg.DoModal() == IDOK)
	{
		POSITION file_pos = file_dlg.GetStartPosition();
		CString  list_item("");
		while (file_pos != NULL)
		{
			list_item = file_dlg.GetNextPathName(file_pos);
			array_filename.Add(list_item);
		}
	}

	int total_file = array_filename.GetSize();
	if (total_file == 0)
	{
		AfxMessageBox("û��ѡ��Ҫ������ļ�!");
		return;
	}

	for (int i = 0; i < total_file; i++)
	{
		file_name_list.SetItemColor(i, RGB(167, 253, 255));
		file_name_list.InsertItem(i, array_filename.GetAt(i));
		file_name_list.SetItemText(i, 1, "������......");
	}
	delete [] ch;
}


//////////////////////////////////////////////////////////////////////////
///  ������ѡ�ļ�GPS�㵽map����
///  para1:filename-----��ѡ�ļ���
///  para2:n------------list�ؼ���������
///  para3:list_ctrl----list�ؼ�
///  parq4:map_gps_point---GPS��map����
//////////////////////////////////////////////////////////////////////////
void SaveGPSPointToMap(const char* filename, const int n, CCoolListCtrl &list_ctrl, std::map<time_t, GPS_POINT>& map_gps_point)
{
	FILE* pFile = fopen(filename, "rb");
	if (!pFile)
	{
		list_ctrl.SetItemColor(n, RGB(255, 0, 0));
		list_ctrl.SetItemText(n, 1, "���ļ��޷���,�Ѻ���...");
		return;
	}

	size_t data_size = get_fileSize(filename);
	int32_t gz_header_3byte = GZ_HEADER_3BYTE;
	fread(&gz_header_3byte, 1, sizeof(gz_header_3byte), pFile);
	rewind(pFile);

	// �ж��Ƿ���gz�ļ�
	bool is_gzfile = (GZ_HEADER_3BYTE == (gz_header_3byte & 0xFFFFFF));

	char* buffer = NULL;
	if (is_gzfile)
	{
		// ��ȡgz�ļ�����ѹbdgps�켣�ļ����ڴ�
		data_size = get_gzbinSize(filename);

		gzFile gzf = gzopen(filename, "rb");
		buffer = new char[data_size + 1];
		buffer[data_size] = 0;

		if (gzread(gzf, buffer, data_size) < 0)
			return;
		gzclose(gzf);
	}
	else
	{
		buffer = new char[data_size];
		fread(buffer, 1, data_size, pFile);
	}

	// �����ڴ��е�����
	char *gps_buffer = buffer;
	GPS_FILEHEAD* gps_filehead = (GPS_FILEHEAD*)gps_buffer;   // bin�ļ�ͷ
	if (!((gps_filehead->empty_1 == 0x00) && (gps_filehead->data_pos == 0x18)))  //�ж��Ƿ���bin�켣�ļ�
	{
		list_ctrl.SetItemColor(n, RGB(255, 0, 0));
		list_ctrl.SetItemText(n, 1, "���ļ����ǹ켣�ļ�,�Ѻ���...");
		return;
	}

	if (n == 0)  // ��ȡ��һ���ļ����ļ�ͷ
	{
		memcpy(&first_file_gps_filehead, gps_filehead, sizeof(GPS_FILEHEAD));
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

	GPS_POINT* gps_point = (GPS_POINT*)(gps_buffer + gps_filehead->data_pos);  // ��һ��GPS��¼
	int gps_point_total = (data_size - gps_filehead->data_pos) / (sizeof(GPS_POINT) - ver_offset); // GPS��¼��Ŀ��

	int fraction = 10;    // Ĭ�����10��GPS����
	int count = fraction;
	while (gps_point_total--)
	{
		if (count % fraction == 0)
		{
			map_gps_point.insert(std::make_pair(gps_point->timestamp, *gps_point));
			count--;
		}
		else if ((--count) == 0)
		{
			count = fraction;
		}
			
		gps_point++;
		gps_point = (GPS_POINT*)((char*)gps_point - ver_offset);    // ���ݾɰ汾 02 04 05 ���� 06�汾���ݶ�����������ָ�����
	}
	gps_point = (GPS_POINT*)((char*)gps_point + ver_offset);
	--gps_point;
	map_gps_point.insert(std::make_pair(gps_point->timestamp, *gps_point));
	list_ctrl.SetItemText(n, 1, "���ļ��ϲ����!!!");
	
	fclose(pFile);
	delete[] buffer;
	return;
}

//////////////////////////////////////////////////////////////////////////
///     �ϲ����й켣�ļ�:������ѡ��켣�ļ���GPS�㱣�浽.bin.gz�ļ�
///      para1:bin_file:��ʱ�ļ�
///      para2:filename:�ϲ����ļ���
//////////////////////////////////////////////////////////////////////////
bool MakeGpsFile(FILE* bin_file, const char* filename)
{
	// ���bin���ݴ�С
	rewind(bin_file);
	fseek(bin_file, 0, SEEK_END);
	size_t bin_size = ftell(bin_file);
	rewind(bin_file);

	// ����gps�ļ�ͷ
	GPS_FILEHEAD* gps_filehead = new GPS_FILEHEAD;
	size_t gps_head_size = sizeof(GPS_FILEHEAD);

	memcpy(gps_filehead, &first_file_gps_filehead, sizeof(GPS_FILEHEAD));
	gps_filehead->data_pos = 0x18;
	gps_filehead->data_ver = 0x06;    // 06 ��06��켣�ļ���֮ǰ��04  05

	// ����gps�ļ�ͷ ��  bin���ݴ�С �ڴ�
	size_t buf_size = gps_head_size + bin_size;
	char* buffer = new char[buf_size];
	memset(buffer, 0, buf_size);
	// ���ص�����
	memcpy(buffer, gps_filehead, gps_head_size);
	fread(buffer + gps_head_size, bin_size, 1, bin_file);

	// д��gzѹ���İٶȵ����켣�ļ�
	gzFile gzf = gzopen(filename, "rw");
	if (gzwrite(gzf, buffer, buf_size) < 0)
	{
		return false;
	}
	
	gzclose(gzf);
	delete [] buffer;
	delete gps_filehead;
	return true;
}

//////////////////////////////////////////////////////////////////////////
///                      �̴߳�����:
///      �����ڸ�list�ؼ��������Ŀ̫��ʱ,��ɳ�����濨��,Ϊ�˽���������,�ڴ�
///      ʹ���̴߳�������������list�ؼ��в����ȡ����VIP������,�������̴߳���
///      �Ͳ��Ῠ����.
//////////////////////////////////////////////////////////////////////////
//void ThreadCombine(LPVOID pParam)
//{
//	PARAMETER *p_parameter = (PARAMETER *)pParam;
//	std::map<time_t, GPS_POINT> map_gps_point;    //ʹ��map�������GPS��
//	size_t file_num = p_parameter->src_filename.GetSize();
//	CString file_name("");
//	for (size_t i = 0; i < file_num; i++)
//	{
//		//   ��������GPS�㵽map����
//		file_name = p_parameter->src_filename.GetAt(i);
//		SaveGPSPointToMap(file_name.GetBuffer(file_name.GetLength()), i, *(p_parameter->p_list_ctrl), map_gps_point);
//	}
//
//	FILE* bin_file = tmpfile();
//	for (auto it = map_gps_point.begin(); it != map_gps_point.end(); ++it)
//	{
//		fwrite(&it->second, sizeof(GPS_POINT), 1, bin_file);   // д�������gps�ڵ㵽bin�ļ�
//	}
//	if (MakeGpsFile(bin_file, p_parameter->dest_filename) == false)
//	{
//		AfxMessageBox("δ֪����:�ϲ��켣�ļ�ʧ��!");
//	}
//	else
//	{
//		AfxMessageBox("�ϲ��켣�ļ��ɹ�!");
//	}
//	fclose(bin_file);
//}

void CTrajecory_combine::OnBnClickedButtonCombine()
{
	if (array_filename.GetSize() == 0)
	{
		AfxMessageBox("��û��ѡ��켣�ļ�,��ѡ�к��ٺϲ�");
		return;
	}

	CFileDialog combine_dlg(FALSE, "bin.gz", "combined_trajectory");  //���������ļ��Ի���  "bin.gz"������ʾĬ���ļ���׺��
	combine_dlg.m_ofn.lpstrFilter = "html files(*.bin.gz)|*.bin.gz||";
	combine_dlg.m_ofn.lpstrTitle = "�ϲ�ѡ��Ĺ켣�ļ�";
	std::map<time_t, GPS_POINT> map_gps_point;    //ʹ��map�������GPS��
	if (combine_dlg.DoModal() == IDOK)
	{
		CString file_path = combine_dlg.GetPathName();

		size_t file_num = array_filename.GetSize();
		CString file_name("");
		for (size_t i = 0; i < file_num; i++)
		{
			//   ��������GPS�㵽map����
			file_name = array_filename.GetAt(i);
			char *file = file_name.GetBuffer(file_name.GetLength());
			SaveGPSPointToMap(file, i, file_name_list, map_gps_point);
		}

		FILE* bin_file = tmpfile();
		for (auto it = map_gps_point.begin(); it != map_gps_point.end(); ++it)
		{
			fwrite(&it->second, sizeof(GPS_POINT), 1, bin_file);   // д�������gps�ڵ㵽bin�ļ�
		}

		if (map_gps_point.size() == 0)
		{
			AfxMessageBox("��ѡ�ļ������ǹ켣�ļ����޷��ϲ�!");
			return;
		}
		if (MakeGpsFile(bin_file, file_path.GetBuffer(file_path.GetLength())))
		{
			AfxMessageBox("�ϲ��켣�ļ��ɹ�!");
		}
		else
		{
			AfxMessageBox("�ϲ��켣�ļ�ʧ��!");
		}
	}
}
