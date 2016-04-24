// CoolListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_trajectory_combine.h"
#include "CoolListCtrl.h"

BEGIN_MESSAGE_MAP(CCoolListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	//    ON_WM_SIZE()  
END_MESSAGE_MAP()
CCoolListCtrl::CCoolListCtrl(void)
{
}

CCoolListCtrl::~CCoolListCtrl(void)
{
}

void CCoolListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr = (NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	switch (lplvdr->nmcd.dwDrawStage)//�ж�״̬  
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		COLORREF ItemColor;
		//������SetItemColor(DWORD iItem, COLORREFcolor) ���õ�//ITEM�ź�COLORREF �������в��ң�Ȼ�������ɫ��ֵ��  
		if (MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
		{
			//lplvdr->clrText = RGB(0,0,0)//ItemColor  
			lplvdr->clrTextBk = ItemColor;
			*pResult = CDRF_DODEFAULT;
		}
		break;
	}
}

void CCoolListCtrl::SetItemColor(DWORD item, COLORREF color)
{
	// m_iCurrentItem = iItem;  
	// m_CurrentColor = color;  
	MapItemColor.SetAt(item, color);//����ĳ�е���ɫ  
	this->RedrawItems(item, item);//����Ⱦɫ  

								  //this->SetCheck(item,1);  
	this->SetFocus();//���ý���  
	UpdateWindow();
}


