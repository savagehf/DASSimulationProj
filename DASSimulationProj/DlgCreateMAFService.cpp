

#include "stdafx.h"

#include "DlgCreateMAFService.h"
#include "MyTools.h"
#include "MainFrm.h"
#include "DlgCreateRequest.h"
#include "DASSimulationView.h"

#include "DASRender.h"
#include "BussinessData.h"
#include "DlgMAFPhase2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CArray<CTime, CTime&> g_arrMafEmptyWin;

CDlgCreateMAFService::CDlgCreateMAFService(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateMAFService::IDD, pParent)
{

	
	m_strTdrs = _T("1");
	m_ctimeStartTime = 0;
	m_ctimeEndTime = 0;
	m_ctimeEndDate = 0;
	m_ctimeStartDate = 0;

}


void CDlgCreateMAFService::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TIMEPICKER_END, m_ctlEndTime);
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_ctlStartTime);
	DDX_Control(pDX, IDC_DATEPICKER_END, m_ctlEndDate);
	DDX_Control(pDX, IDC_DATEPICKER_START, m_ctlStartDate);
	
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_START, m_ctimeStartTime);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_END, m_ctimeEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_END, m_ctimeEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_START, m_ctimeStartDate);
	
	
	DDX_CBString(pDX, IDC_COMBO_TDRS_MAF, m_strTdrs);

	
	

	DDX_Control(pDX, IDC_MAF_PHASE_2_SHOW, m_BtnShowPhase2);
}


BEGIN_MESSAGE_MAP(CDlgCreateMAFService, CDialog)
	ON_BN_CLICKED(ID_DAS_VIS_QUERY, OnProduceUPUsableTime)
	//ON_BN_CLICKED(ID_CREATE_TASK, OnCreateTaskWithUpusableTime)
	//ON_BN_CLICKED(IDC_BTN_CREATE_MAF_WITHTUT,OnBnClickedBtnCreateMafWithtut)
	ON_BN_CLICKED(IDC_BTN_US_TDRS_VISIBLE_TIME, &CDlgCreateMAFService::OnBnClickedBtnUsTdrsVisibleTime)
	
	ON_BN_CLICKED(IDC_MAF_PHASE_2_SHOW, &CDlgCreateMAFService::OnBnClickedMafPhase2Show)
END_MESSAGE_MAP()

//ģ��涨��ʱ��Σ���ȴ����0.5-1-2Сʱ���ƻ�����8��ʱ������ҡ�
//2013��4��8��21:20:13
void CDlgCreateMAFService::OnProduceUPUsableTime() 
{
	//UpdateData(TRUE);
	//if(m_ctimeStartDate >= m_ctimeEndDate)
	//{
	//	AfxMessageBox(_T("����'����ʱ��'����'��ʼʱ��'!"));
	//	return;
	//}

	////1.����tutʱ��Ρ�
	//CBussinessData* pData = CBussinessData::Instance();
	//if (!pData->SimulateTUT(m_ctimeStartDate, m_ctimeEndDate))
	//{
	//	return;
	//}
	//
	////2.�����ݲ��뵽ListCtrl
	//int nTDRS = _tcstoul(m_strTdrs.GetBuffer(), NULL, 10);
	//int nSIC  = _tcstoul(m_strSic.GetBuffer(), NULL, 10);

	//CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	//CString strEnd   = MyTools::GetFormatedDate(m_ctimeEndDate);
	//CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	//CString strTimeEnd = MyTools::GetFormatedTime(m_ctimeEndTime);
	//CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	//CString strNewEndDateTime = strEnd + _T(" ")+strTimeEnd;

	//CTime timeNewStartDateTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	//CTime timeNewEndDateTime = MyTools::GetCTimeFromStringEx(strNewEndDateTime);

	//// tut ��
	//BOOL bNotTut = TRUE;
	//m_listTUT.DeleteAllItems();
	//int nIndex = 0;
	//for (ListTimeTable::iterator it = pData->m_listTUT.begin();
	//	it != pData->m_listTUT.end(); ++it)
	//{
	//	CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
	//	CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);
	//	int nTD = _tcstoul(it->strTDRS.GetBuffer(), NULL, 10);
	//	int nSI = _tcstoul(it->strSIC.GetBuffer(), NULL, 10);

	//	if (nTD == nTDRS
	//		&& nSI == nSIC
	//		&& timeStart>= timeNewStartDateTime
	//		&& timeEnd <= timeNewEndDateTime)
	//	{
	//		bNotTut = FALSE;
	//		// Insert TuT
	//		m_listTUT.InsertItem(nIndex, it->strTDRS);
	//		m_listTUT.SetItemText(nIndex, 1, it->strSIC);
	//		m_listTUT.SetItemText(nIndex, 2, it->strStartTime);
	//		m_listTUT.SetItemText(nIndex, 3, it->strEndTime);
	//		m_listTUT.SetItemText(nIndex, 4, it->strLastTime);

	//		nIndex++;
	//	}

	//}

	//if (bNotTut)
	//{
	//	AfxMessageBox(_T("��ǰû�������ѯ������TUT��"));
	//}

	////UpdateList();
}

//void CDlgCreateMAFService::UpdateList()
//{
//	CBussinessData* pData = CBussinessData::Instance();
//
//	int nTDRS = _tcstoul(m_strTdrs.GetBuffer(), NULL, 10);
//	int nSIC  = _tcstoul(m_strSic.GetBuffer(), NULL, 10);
//
//	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
//	CString strEnd   = MyTools::GetFormatedDate(m_ctimeEndDate);
//	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
//	CString strTimeEnd = MyTools::GetFormatedTime(m_ctimeEndTime);
//	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
//	CString strNewEndDateTime = strEnd + _T(" ")+strTimeEnd;
//	
//	CTime timeNewStartDateTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
//	CTime timeNewEndDateTime = MyTools::GetCTimeFromStringEx(strNewEndDateTime);
//
//	// tut ��
//	BOOL bNotTut = TRUE;
//	m_listTUT.DeleteAllItems();
//	int nIndex = 0;
//	for (ListTimeTable::iterator it = pData->m_listTUT.begin();
//		it != pData->m_listTUT.end(); ++it)
//	{
//		CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
//		CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);
//		int nTD = _tcstoul(it->strTDRS.GetBuffer(), NULL, 10);
//		int nSI = _tcstoul(it->strSIC.GetBuffer(), NULL, 10);
//
//		if (nTD == nTDRS
//		   && nSI == nSIC
//		   && timeStart>= timeNewStartDateTime
//		   && timeEnd <= timeNewEndDateTime)
//		{
//			bNotTut = FALSE;
//			// Insert TuT
//			m_listTUT.InsertItem(nIndex, it->strTDRS);
//			m_listTUT.SetItemText(nIndex, 1, it->strSIC);
//			m_listTUT.SetItemText(nIndex, 2, it->strStartTime);
//			m_listTUT.SetItemText(nIndex, 3, it->strEndTime);
//			m_listTUT.SetItemText(nIndex, 4, it->strLastTime);
//
//			nIndex++;
//		}
//		
//	}
//
//	// UP ʱ���
//	BOOL bNoUpTable = TRUE;
//	m_listUPVisibletable.DeleteAllItems();
//	int nIndx=0;
//	for (ListTimeTable::iterator itor = pData->m_listVisbleTime.begin();
//		itor != pData->m_listVisbleTime.end(); ++itor)
//	{
//		CTime timeStart = MyTools::GetCTimeFromStringEx(itor->strStartTime);
//		CTime timeEnd = MyTools::GetCTimeFromStringEx(itor->strEndTime);
//		int nTD = _tcstoul(itor->strTDRS.GetBuffer(), NULL, 10);
//		int nSI = _tcstoul(itor->strSIC.GetBuffer(), NULL, 10);
//
//		if (nTD == nTDRS
//			&& nSI == nSIC
//			&& timeStart >= timeNewStartDateTime
//			&& timeEnd <= timeNewEndDateTime)
//		{
//			bNoUpTable = FALSE;
//			// Insert usable table control.
//			m_listUPVisibletable.InsertItem(nIndx, itor->strTDRS);
//			m_listUPVisibletable.SetItemText(nIndx,1, itor->strSIC);
//			m_listUPVisibletable.SetItemText(nIndx,2, itor->strStartTime);
//			m_listUPVisibletable.SetItemText(nIndx,3, itor->strEndTime);
//			m_listUPVisibletable.SetItemText(nIndx,4, itor->strLastTime);
//
//			nIndx++;
//		}	
//	}
//
//	if (bNotTut)
//	{
//		AfxMessageBox(_T("��ǰû�������ѯ������TUT��"));
//	}
//	if (bNoUpTable)
//	{
//		AfxMessageBox(_T("��ǰû�������ѯ�����Ŀɼ�ʱ�䴰��"));
//	}
//}
//

BOOL CDlgCreateMAFService::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//DWORD dwStyle = m_listUPVisibletable.GetExtendedStyle();
	//dwStyle |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	//dwStyle |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	//m_listUPVisibletable.SetExtendedStyle(dwStyle);

	//DWORD dwSty = m_listTUT.GetExtendedStyle();
	//dwSty |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	//dwSty |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	//m_listTUT.SetExtendedStyle(dwSty);

	m_ctlStartDate.SetFormat(_T("yyyy-MM-dd"));
	m_ctlEndDate.SetFormat(_T("yyyy-MM-dd"));

	m_ctimeStartDate = CTime::GetCurrentTime();
	m_ctimeEndDate = CTime::GetCurrentTime();
	m_ctimeStartTime = CTime::GetCurrentTime();
	m_ctimeEndTime = CTime::GetCurrentTime();

	UpdateData(0);
	

	
	
	((CComboBox*)(GetDlgItem(IDC_COMBO_TDRS_MAF)))->InsertString(0, _T("1"));
	((CComboBox*)(GetDlgItem(IDC_COMBO_TDRS_MAF)))->SetCurSel(0);

	//m_listTUT.InsertColumn(0, _T("TDRS"), LVCFMT_RIGHT, 50);
	//m_listTUT.InsertColumn(1, _T("SIC"), LVCFMT_RIGHT, 50);
	//m_listTUT.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, 150);
	//m_listTUT.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, 150);
	//m_listTUT.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 150);

	//m_listUPVisibletable.InsertColumn(0, _T("TDRS"), LVCFMT_RIGHT, 50);
	//m_listUPVisibletable.InsertColumn(1, _T("SIC"), LVCFMT_RIGHT, 50);
	//m_listUPVisibletable.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, 150);
	//m_listUPVisibletable.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, 150);
	//m_listUPVisibletable.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 150);
//	m_listUPVisibletable.InsertColumn(5, "ID", LVCFMT_CENTER, 0);

		//				BTNST_COLOR_BK_IN		Background color when mouse is over the button
		//				BTNST_COLOR_FG_IN		Text color when mouse is over the button
		//				BTNST_COLOR_BK_OUT		Background color when z is outside the button
		//				BTNST_COLOR_FG_OUT		Text color when mouse is outside the button
		//				BTNST_COLOR_BK_FOCUS	Background color when the button is focused
		//				BTNST_COLOR_FG_FOCUS	Text color when the button is focused
	m_BtnShowPhase2.SetTooltipText(_T("��ʾ�ý׶�ģ�����"));
	m_BtnShowPhase2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,0,0));
	m_BtnShowPhase2.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
	m_BtnShowPhase2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255,0,0));
	m_BtnShowPhase2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));
	m_BtnShowPhase2.DrawBorder();
	
	return TRUE; 
}

//ȥ��������2013��4��8��21:07:40
//void CDlgCreateMAFService::OnBnClickedBtnCreateMafWithtut()
//{
//	int nTutIndex = m_listTUT.GetSelectionMark();
//	if (nTutIndex == -1) 
//	{
//		AfxMessageBox(_T("��ѡ��һ����¼��"));
//		return;
//	}	
//
//	CComboBox* pComBo = (CComboBox*)GetDlgItem(IDC_COMBO_TDRS_MAF);
//	CString strTDRS;
//	pComBo->GetWindowText(strTDRS);
//
//	int nSelectedItem = -1;
//	POSITION pos = m_listTUT.GetFirstSelectedItemPosition();
//	if (pos == NULL)
//	{
//		AfxMessageBox(_T("No items were selected!\n"));
//		return;
//	}
//	else
//	{
//		while (pos)
//		{
//			nSelectedItem = m_listTUT.GetNextSelectedItem(pos);
//		}
//	}
//
//	CString strStartTime = m_listTUT.GetItemText(nSelectedItem, 2);
//	CString strEndTime = m_listTUT.GetItemText(nSelectedItem, 3);
//
//	CDlgCreateRequest dlg(this);
//	dlg.SetControlValue(strTDRS, m_strSic, strStartTime, strEndTime);
//	dlg.SetServiceType(enmSrvType_MAF);
//	dlg.DoModal();
//}

//ȥ��������2013��4��8��21:07:51
// void CDlgCreateMAFService::OnCreateTaskWithUpusableTime() 
// {
// 	int nUPIndex  = m_listUPVisibletable.GetSelectionMark(); 
// 	if (nUPIndex == -1) 
// 	{
// 		AfxMessageBox(_T("��ѡ��һ����¼��"));
// 		return;
// 	}	
// 
// 	CComboBox* pComBo = (CComboBox*)GetDlgItem(IDC_COMBO_TDRS_MAF);
// 	CString strTDRS;
// 	pComBo->GetWindowText(strTDRS);
// 
// 	int nSelectedItem = -1;
// 	POSITION pos = m_listUPVisibletable.GetFirstSelectedItemPosition();
// 	if (pos == NULL)
// 	{
// 		AfxMessageBox(_T("No items were selected!\n"));
// 		return;
// 	}
// 	else
// 	{
// 		while (pos)
// 		{
// 			nSelectedItem = m_listUPVisibletable.GetNextSelectedItem(pos);
// 		}
// 	}
// 
// 	CString strStartTime = m_listUPVisibletable.GetItemText(nSelectedItem, 2);
// 	CString strEndTime   = m_listUPVisibletable.GetItemText(nSelectedItem, 3);
// 
// 	CDlgCreateRequest dlg(this);
// 	dlg.SetControlValue(strTDRS, m_strSic, strStartTime, strEndTime);
// 	dlg.SetServiceType(enmSrvType_MAF);
// 	dlg.DoModal();
// }

//ֱ�ӵ���tdrs-us �ɼ�ʱ��Ρ�
//2013��4��8��21:22:14
void CDlgCreateMAFService::OnBnClickedBtnUsTdrsVisibleTime()
{
	//UpdateData(TRUE);
	//CBussinessData* pData = CBussinessData::Instance();

	//int nTDRS = _tcstoul(m_strTdrs.GetBuffer(), NULL, 10);
	//int nSIC  = _tcstoul(m_strSic.GetBuffer(), NULL, 10);

	//CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	//CString strEnd   = MyTools::GetFormatedDate(m_ctimeEndDate);
	//CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	//CString strTimeEnd = MyTools::GetFormatedTime(m_ctimeEndTime);
	//CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	//CString strNewEndDateTime = strEnd + _T(" ")+strTimeEnd;

	//CTime timeNewStartDateTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	//CTime timeNewEndDateTime = MyTools::GetCTimeFromStringEx(strNewEndDateTime);

	////1.����csv�ɼ�ʱ���
	//pData->ImportUSAndTDRSVisibleTimeTable();


	////2.ʱ�����뵽�ռ�ListCtrl��
	//BOOL bNoUpTable = TRUE;
	//m_listUPVisibletable.DeleteAllItems();
	//int nIndx=0;
	//for (ListTimeTable::iterator itor = pData->m_listVisbleTime.begin();
	//	itor != pData->m_listVisbleTime.end(); ++itor)
	//{
	//	CTime timeStart = MyTools::GetCTimeFromStringEx(itor->strStartTime);
	//	CTime timeEnd = MyTools::GetCTimeFromStringEx(itor->strEndTime);
	//	int nTD = _tcstoul(itor->strTDRS.GetBuffer(), NULL, 10);
	//	int nSI = _tcstoul(itor->strSIC.GetBuffer(), NULL, 10);

	//	if (nTD == nTDRS
	//		&& nSI == nSIC
	//		&& timeStart >= timeNewStartDateTime
	//		&& timeEnd <= timeNewEndDateTime)
	//	{
	//		bNoUpTable = FALSE;
	//		// Insert usable table control.
	//		m_listUPVisibletable.InsertItem(nIndx, itor->strTDRS);
	//		m_listUPVisibletable.SetItemText(nIndx,1, itor->strSIC);
	//		m_listUPVisibletable.SetItemText(nIndx,2, itor->strStartTime);
	//		m_listUPVisibletable.SetItemText(nIndx,3, itor->strEndTime);
	//		m_listUPVisibletable.SetItemText(nIndx,4, itor->strLastTime);

	//		nIndx++;
	//	}	
	//}

	//if (bNoUpTable)
	//{
	//	AfxMessageBox(_T("��ǰû�������ѯ������US-TRDS�ɼ�ʱ��Σ�"));
	//}
}


////�����������ʱ��ν��н������㣬�ó�UP����ʱ��Ρ�������һ��dialog��չ�ָ��û�ȥ����
////2013��4��8��21:23:16
//void CDlgCreateMAFService::OnBnClickedBtnMafCreateUpTime()
//{
//	//1.���ɿɼ�ʱ�䴰��ȡ�������㡣
//	CBussinessData::Instance()->CalculateUsableTimeTable();
//	
//	//2.��ʾ���ؼ����Ŀɼ�ʱ�䴰��
//	CDlgUsableTimeTable dlg;
//	dlg.SetServiceType(enmSrvType_MAF);
//	dlg.DoModal();
//}

//����maf-�ڶ��׶ε�ģ���
void CDlgCreateMAFService::OnBnClickedMafPhase2Show()
{
	CDlgMAFPhase2 dlg;
	dlg.DoModal();
}
