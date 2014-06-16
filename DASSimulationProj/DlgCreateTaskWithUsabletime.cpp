// DlgUsableTimeTable.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgCreateTaskWithUsabletime.h"
// #include "DlgCreateRequest.h"
#include "BussinessData.h"
#include "MyTools.h"
#include "DlgMAFPhase2.h"
#include "DlgMARPhase2.h"
#include "DlgMAFMARPhase2.h"
#include "DlgSettimeToStarttask.h"
#include "MainFrm.h"
#include "DASSimulationView.h"


IMPLEMENT_DYNAMIC(CDlgCreateTaskWithUsabletime, CDialog)

CDlgCreateTaskWithUsabletime::CDlgCreateTaskWithUsabletime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateTaskWithUsabletime::IDD, pParent)
{
	m_editStartTime = _T("");
	m_editEndTime = _T("");
	m_strTdrs = _T("");
	m_strSic = _T("");
	m_strSsc = _T("");

	m_bProfUser = TRUE;

	m_pMAFPhase2Dlg = NULL;
	m_pMARPhase2Dlg = NULL;
	m_pMAFMARPhase2Dlg = NULL;

	m_pSettingSysTimeDlg = NULL;
	m_nSelectedItem = -1;
}

CDlgCreateTaskWithUsabletime::~CDlgCreateTaskWithUsabletime()
{
	if(m_pMAFPhase2Dlg != NULL)
	{
		if (m_pMAFPhase2Dlg->GetSafeHwnd() != NULL)
		{
			m_pMAFPhase2Dlg->DestroyWindow();
		}
		delete m_pMAFPhase2Dlg;
		m_pMAFPhase2Dlg = NULL;
	}
	if (m_pMARPhase2Dlg != NULL)
	{
		if (m_pMARPhase2Dlg->GetSafeHwnd() != NULL)
		{
			m_pMARPhase2Dlg->DestroyWindow();
		}
		delete m_pMARPhase2Dlg;
		m_pMARPhase2Dlg = NULL;
	}

	if (m_pMAFMARPhase2Dlg != NULL)
	{
		if (m_pMAFMARPhase2Dlg->GetSafeHwnd() != NULL)
		{
			m_pMAFMARPhase2Dlg->DestroyWindow();
		}
		delete m_pMAFMARPhase2Dlg;
		m_pMAFMARPhase2Dlg = NULL;
	}


	if (NULL != m_pSettingSysTimeDlg)
	{
		m_pSettingSysTimeDlg->DestroyWindow();
		delete m_pSettingSysTimeDlg;
		m_pSettingSysTimeDlg = NULL;
	}
}

void CDlgCreateTaskWithUsabletime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UP_TDRS_USABLE_TIME, m_listctrlUsableTimeTable);

	DDX_Text(pDX, IDC_EDIT_WINDOW_START_DATETIME, m_editStartTime);
	DDX_Text(pDX, IDC_EDIT_WINDOW_END_DATETIME, m_editEndTime);
	DDX_Text(pDX, IDC_EDIT_TDRS, m_strTdrs);
	DDX_Text(pDX, IDC_EDIT_SIC, m_strSic);
	DDX_Text(pDX, IDC_EDIT_SSC, m_strSsc);

	DDX_Control(pDX, IDC_DATEPICKER_SERIVCE_START_TIME, m_serviceStartTime);
	DDX_Control(pDX, IDC_DATEPICKER_SERIVCE_END_TIME, m_serviceEndTime);
	DDX_Control(pDX, IDC_DATEPICKER_SERIVCE_START_DATE, m_serviceStartDate);
	DDX_Control(pDX, IDC_DATEPICKER_SERIVCE_END_DATE, m_serviceEndDate);
	DDX_Control(pDX, IDC_COMBO_SERVICE_LEVEL, m_comboServiceLevel);
	DDX_Control(pDX, IDC_COMBO_MAR_CHANNEL, m_comboChannelID);
}


BEGIN_MESSAGE_MAP(CDlgCreateTaskWithUsabletime, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UP_TDRS_USABLE_TIME, OnClickListctrl)
	ON_BN_CLICKED(IDC_BTN_APPLY_REQUEST, &CDlgCreateTaskWithUsabletime::OnCreateTask)
END_MESSAGE_MAP()

int global_task_count = 0;
//�����ĸ���ʱ������
void CDlgCreateTaskWithUsabletime::MockLowLevelTask()
{
	if (global_task_count>=4)
	{
		return;
	}
	int nCount = m_listctrlUsableTimeTable.GetItemCount();
	if (m_eSeviceType == enmSrvType_MAF && nCount > 4)
	{
		for (int i=1; i<=4; i++)
		{
			global_task_count++;
			CString strNewStartDateTime = m_listctrlUsableTimeTable.GetItemText(nCount-i, 2);
			CString strNewEndDataTime   = m_listctrlUsableTimeTable.GetItemText(nCount-i, 3);
			CString strSpanTime			= m_listctrlUsableTimeTable.GetItemText(nCount-i, 4);

			//2.normal add task.
			TaskItem oneItem;
			oneItem.strTDRS = _T("1");
			oneItem.strSIC = i%2==0? _T("002") : _T("003");
			CString strRandSSC;
			strRandSSC.Format(_T("%s_%d"),m_strSsc, i);
			oneItem.strSSC = strRandSSC/*m_strSsc*/;
			oneItem.strStartTime = strNewStartDateTime;
			oneItem.strEndTime = strNewEndDataTime;
			oneItem.strLastTime = strSpanTime;
			oneItem.uLevel = enmTaskLevel_Low;
			oneItem.uStatus = enmTaskStatus_UnProcess;
			oneItem.nChannelID = 0;//MAF Ĭ�ϵ�channelͨ������0
			oneItem.eTaskType = enmTaskType_MAF;


			CBussinessData::Instance()->AddDASTaskItem(oneItem);

			TimeItem item;
			item.bOuccupied = TRUE;
			item.strStartTime = strNewStartDateTime;
			item.strEndTime = strNewEndDataTime;
			item.strLastTime = strSpanTime;
			item.strSIC = i%2==0? _T("002") : _T("003");;
			item.strTDRS = _T("1");
			item.nLevel = 1;
			item.eType = enmSrvType_MAF;
			CBussinessData::Instance()->AddOcupiedTime(item);
			CBussinessData::Instance()->UpdateUsableTime(strNewStartDateTime, strNewEndDataTime, strNewStartDateTime, strNewEndDataTime);
			CBussinessData::Instance()->UpdateVisbleTime(strNewStartDateTime, strNewEndDataTime, strNewStartDateTime, strNewEndDataTime);
			CBussinessData::Instance()->UpdateTUT(strNewStartDateTime, strNewEndDataTime, strNewStartDateTime, strNewEndDataTime);
		}
	}

	//����MAF tut+visabletime
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pMainFrm)
	{
		pMainFrm->PostMessage(WM_UPDATE_MAF_LISTCTRL);
	}

	//Update usable time table
	LoadUsableTimeTable();
}

BOOL CDlgCreateTaskWithUsabletime::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_listctrlUsableTimeTable.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listctrlUsableTimeTable.SetExtendedStyle(dwStyle);

	UpdateData(0);

	m_listctrlUsableTimeTable.InsertColumn(0, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlUsableTimeTable.InsertColumn(1, _T("SIC"), LVCFMT_RIGHT, 50);
	m_listctrlUsableTimeTable.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listctrlUsableTimeTable.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listctrlUsableTimeTable.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listctrlUsableTimeTable.InsertColumn(5, _T("�Ƿ�ռ��"), LVCFMT_LEFT, 80);
	m_listctrlUsableTimeTable.InsertColumn(6, _T("���ȼ�"), LVCFMT_LEFT, 60);
	m_listctrlUsableTimeTable.InsertColumn(7, _T("��������"), LVCFMT_LEFT, 80);

	UpdateData(FALSE);

	if (m_eSeviceType == enmSrvType_MAR)
	{
		CEdit* pEditSIC = (CEdit*)(GetDlgItem(IDC_EDIT_SIC));
		pEditSIC->EnableWindow(TRUE);
	}

	m_comboServiceLevel.InsertString(0, _T("1"));
	m_comboServiceLevel.InsertString(1, _T("2"));
	m_comboServiceLevel.InsertString(2, _T("3"));
	m_comboServiceLevel.SetCurSel(0);

	m_comboChannelID.InsertString(0, _T("1"));
	m_comboChannelID.InsertString(1, _T("2"));
	m_comboChannelID.InsertString(2, _T("3"));
	m_comboChannelID.InsertString(3, _T("4"));
	m_comboChannelID.InsertString(4, _T("5"));
	m_comboChannelID.SetCurSel(0);
	if (m_eSeviceType == enmSrvType_MAF || m_eSeviceType == enmSrvType_MAFMAR)
	{
		m_comboChannelID.EnableWindow(FALSE);
	}

	if (enmSrvType_MAF == m_eSeviceType)
	{
		SetWindowText(_T("����ǰ������"));
	}
	else if(enmSrvType_MAR == m_eSeviceType)
	{
		SetWindowText(_T("������������"));
	}
	else if (enmSrvType_MAFMAR == m_eSeviceType)
	{
		SetWindowText(_T("����˫������"));
	}

	UpdateData(FALSE);

	LoadUsableTimeTable();

	MockLowLevelTask();


	return TRUE; 
}

void CDlgCreateTaskWithUsabletime::SetControlValue(CString strTDRS, CString strSIC, CString strStartTime, CString strEndTime)
{
	m_strTdrs = strTDRS;
	m_strSic = strSIC;
	m_editStartTime = strStartTime;
	m_editEndTime   = strEndTime;
}

void CDlgCreateTaskWithUsabletime::LoadUsableTimeTable()
{
	CBussinessData* pData = CBussinessData::Instance();
	m_listctrlUsableTimeTable.DeleteAllItems();
	int indx = 0;

	if (enmSrvType_MAF == m_eSeviceType)
	{
		//����ȡ������Ŀ���ʱ���
		for (ListTimeTable::iterator it = pData->m_listUsableTime.begin();
			it != pData->m_listUsableTime.end(); ++it)
		{
			CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
			CTime timeEnd = MyTools::GetCTimeFromStringEx(it->strEndTime);

			//�󲿷������|--------|		|------|
			///              |---|			|------|
			if (timeStart >= m_timeFilterBegin && timeEnd <= m_timeFilterEnd)
			{
				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, it->strStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, it->strEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, it->strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("ǰ��"));
				indx++;
			}

			//filter time:		 |--------|			|--------|
			//real time:	|------|			|------------|
			if (timeStart < m_timeFilterBegin && timeEnd > m_timeFilterBegin && timeEnd >= m_timeFilterEnd)
			{
				CString newStartTime = it->strStartTime;
				CString newEndTime   = MyTools::GetFormatedDateTime(timeEnd);
				CTimeSpan span = timeEnd-m_timeFilterBegin;
				CString strLastTime  = MyTools::GetFormatSpanTime(span);

				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("ǰ��"));
				indx++;
			}

			//|-------|					|-------|
			//	   |-------|			|---------------|
			if (timeStart >= m_timeFilterBegin && timeStart <m_timeFilterEnd
				&& timeEnd> m_timeFilterEnd)
			{
				CString newStartTime = MyTools::GetFormatedDateTime(timeStart);
				CString newEndTime   = it->strEndTime;
				CTimeSpan span = m_timeFilterEnd-timeStart;
				CString strLastTime  = MyTools::GetFormatSpanTime(span);

				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("ǰ��"));
				indx++;
			}
		}

		//���ر�ռ�õĵ��ǻ�û��ִ�е�ʱ��Ρ�
		::EnterCriticalSection(&pData->m_csOcupiedTime);
		for (ListTimeTable::iterator itor = pData->m_listOcupiedTime.begin();
			itor != pData->m_listOcupiedTime.end(); ++itor)
		{
			if (itor->eType == enmSrvType_MAF && !itor->bPlayed)
			{
				m_listctrlUsableTimeTable.InsertItem(indx, itor->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, itor->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, itor->strStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, itor->strEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, itor->strLastTime);
				if (itor->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), itor->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("ǰ��"));

				indx++;
			}
		}
		::LeaveCriticalSection(&pData->m_csOcupiedTime);
	}
	//����stk �����ʱ���
	else if(enmSrvType_MAR == m_eSeviceType)
	{
		//ר���û�������ȫ��������
		if (m_bProfUser)
		{
			::EnterCriticalSection(&(pData->m_csVisableTime));
			for (ListTimeTable::iterator it = pData->m_listVisbleTime.begin();
				it != pData->m_listVisbleTime.end(); ++it)
			{
				CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
				CTime timeEnd = MyTools::GetCTimeFromStringEx(it->strEndTime);

				//�󲿷������|--------|		|------|
				///              |---|			|------|
				if (timeStart >= m_timeFilterBegin && timeEnd <= m_timeFilterEnd)
				{
					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, it->strStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, it->strEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, it->strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}

				//filter time:		 |--------|			|--------|
				//real time:	|------|			|------------|
				if (timeStart < m_timeFilterBegin && timeEnd > m_timeFilterBegin && timeEnd >= m_timeFilterEnd)
				{
					CString newStartTime = it->strStartTime;
					CString newEndTime   = MyTools::GetFormatedDateTime(timeEnd);
					CTimeSpan span = timeEnd-m_timeFilterBegin;
					CString strLastTime  = MyTools::GetFormatSpanTime(span);

					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}

				//|-------|					|-------|
				//	   |-------|			|---------------|
				if (timeStart >= m_timeFilterBegin && timeStart <m_timeFilterEnd
					&& timeEnd> m_timeFilterEnd)
				{
					CString newStartTime = MyTools::GetFormatedDateTime(timeStart);
					CString newEndTime   = it->strEndTime;
					CTimeSpan span = m_timeFilterEnd-timeStart;
					CString strLastTime  = MyTools::GetFormatSpanTime(span);

					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}
			}
			::LeaveCriticalSection(&(pData->m_csVisableTime));

			//���ر�ռ�õĵ��ǻ�û��ִ�е�ʱ��Ρ�
			::EnterCriticalSection(&pData->m_csOcupiedTime);
			for (ListTimeTable::iterator itor = pData->m_listOcupiedTime.begin();
				itor != pData->m_listOcupiedTime.end(); ++itor)
			{
				if (itor->eType == enmSrvType_MAR)
				{
					m_listctrlUsableTimeTable.InsertItem(indx, itor->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, itor->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, itor->strStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, itor->strEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, itor->strLastTime);
					if (itor->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), itor->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));

					indx++;
				}
			}
			::LeaveCriticalSection(&pData->m_csOcupiedTime);
		}
		//��ר���û������ز�������
		else
		{

			::EnterCriticalSection(&(pData->m_csVisableTime));
			for (ListTimeTable::iterator it = pData->m_listVisbleTime.begin();
				it != pData->m_listVisbleTime.end();++it)
			{
				CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
				CTime timeEnd = MyTools::GetCTimeFromStringEx(it->strEndTime);

				//�󲿷������|--------|		|------|
				///              |---|			|------|
				if (timeStart >= m_timeFilterBegin && timeEnd <= m_timeFilterEnd)
				{
					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, it->strStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, it->strEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, it->strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}

				//filter time:		 |--------|			|--------|
				//real time:	|------|			|------------|
				if (timeStart < m_timeFilterBegin && timeEnd > m_timeFilterBegin && timeEnd >= m_timeFilterEnd)
				{
					CString newStartTime = it->strStartTime;
					CString newEndTime   = MyTools::GetFormatedDateTime(timeEnd);
					CTimeSpan span = timeEnd-m_timeFilterBegin;
					CString strLastTime  = MyTools::GetFormatSpanTime(span);

					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}

				//|-------|					|-------|
				//	   |-------|			|---------------|
				if (timeStart >= m_timeFilterBegin && timeStart <m_timeFilterEnd
					&& timeEnd> m_timeFilterEnd)
				{
					CString newStartTime = MyTools::GetFormatedDateTime(timeStart);
					CString newEndTime   = it->strEndTime;
					CTimeSpan span = m_timeFilterEnd-timeStart;
					CString strLastTime  = MyTools::GetFormatSpanTime(span);

					m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
					if (it->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), it->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
					indx++;
				}

				if (it == pData->m_listVisbleTime.end())
				{
					break;
				}	
				else
				{
					++it;
					if (it == pData->m_listVisbleTime.end())
					{
						break;
					}
				}
			}
			::LeaveCriticalSection(&(pData->m_csVisableTime));


			//���ر�ռ�õĵ��ǻ�û��ִ�е�ʱ��Ρ�
			::EnterCriticalSection(&pData->m_csOcupiedTime);
			for (ListTimeTable::iterator itor = pData->m_listOcupiedTime.begin();
				itor != pData->m_listOcupiedTime.end(); ++itor)
			{
				if (itor->eType == enmSrvType_MAR)
				{
					m_listctrlUsableTimeTable.InsertItem(indx, itor->strTDRS);
					m_listctrlUsableTimeTable.SetItemText(indx,1, itor->strSIC);
					m_listctrlUsableTimeTable.SetItemText(indx,2, itor->strStartTime);
					m_listctrlUsableTimeTable.SetItemText(indx,3, itor->strEndTime);
					m_listctrlUsableTimeTable.SetItemText(indx,4, itor->strLastTime);
					if (itor->bOuccupied)
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					else
					{
						m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
					}
					CString strLevel;
					strLevel.Format(_T("%d"), itor->nLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
					m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));

					indx++;
				}
			}
			::LeaveCriticalSection(&pData->m_csOcupiedTime);
		}
		
	}
	//����˫��Ŀ���ʱ��Ρ�
	//lee
	else if (m_eSeviceType == enmSrvType_MAFMAR)
	{
		::EnterCriticalSection(&(pData->m_csVisableTime));
		for (ListTimeTable::iterator it = pData->m_listVisbleTime.begin();
			it != pData->m_listVisbleTime.end(); ++it)
		{
			CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
			CTime timeEnd = MyTools::GetCTimeFromStringEx(it->strEndTime);

			//�󲿷������|--------|		|------|
			///              |---|			|------|
			if (timeStart >= m_timeFilterBegin && timeEnd <= m_timeFilterEnd)
			{
				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, it->strStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, it->strEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, it->strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
				indx++;
			}

			//filter time:		 |--------|			|--------|
			//real time:	|------|			|------------|
			if (timeStart < m_timeFilterBegin && timeEnd > m_timeFilterBegin && timeEnd >= m_timeFilterEnd)
			{
				CString newStartTime = it->strStartTime;
				CString newEndTime   = MyTools::GetFormatedDateTime(timeEnd);
				CTimeSpan span = timeEnd-m_timeFilterBegin;
				CString strLastTime  = MyTools::GetFormatSpanTime(span);

				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
				indx++;
			}

			//|-------|					|-------|
			//	   |-------|			|---------------|
			if (timeStart >= m_timeFilterBegin && timeStart <m_timeFilterEnd
				&& timeEnd> m_timeFilterEnd)
			{
				CString newStartTime = MyTools::GetFormatedDateTime(timeStart);
				CString newEndTime   = it->strEndTime;
				CTimeSpan span = m_timeFilterEnd-timeStart;
				CString strLastTime  = MyTools::GetFormatSpanTime(span);

				m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, newStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, newEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, strLastTime);
				if (it->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), it->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("����"));
				indx++;
			}
		}
		::LeaveCriticalSection(&(pData->m_csVisableTime));

		//���ر�ռ�õĵ��ǻ�û��ִ�е�ʱ��Ρ�
		::EnterCriticalSection(&pData->m_csOcupiedTime);
		for (ListTimeTable::iterator itor = pData->m_listOcupiedTime.begin();
			itor != pData->m_listOcupiedTime.end(); ++itor)
		{
			if (itor->eType == enmSrvType_MAF || itor->eType == enmSrvType_MAFMAR)
			{
				m_listctrlUsableTimeTable.InsertItem(indx, itor->strTDRS);
				m_listctrlUsableTimeTable.SetItemText(indx,1, itor->strSIC);
				m_listctrlUsableTimeTable.SetItemText(indx,2, itor->strStartTime);
				m_listctrlUsableTimeTable.SetItemText(indx,3, itor->strEndTime);
				m_listctrlUsableTimeTable.SetItemText(indx,4, itor->strLastTime);
				if (itor->bOuccupied)
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				else
				{
					m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
				}
				CString strLevel;
				strLevel.Format(_T("%d"), itor->nLevel);
				m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
				
				if (itor->eType == enmSrvType_MAF)
				{
					m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("ǰ��"));
				}
				else if (itor->eType == enmSrvType_MAFMAR)
				{
						m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("˫��"));
				}	

				indx++;
			}
		}
		::LeaveCriticalSection(&pData->m_csOcupiedTime);

		//::EnterCriticalSection(&(pData->m_csVisableTime));
		//for (ListTimeTable::iterator it = pData->m_listVisbleTime.begin();
		//	it != pData->m_listVisbleTime.end(); ++it)
		//{
		//	m_listctrlUsableTimeTable.InsertItem(indx, it->strTDRS);
		//	m_listctrlUsableTimeTable.SetItemText(indx,1, it->strSIC);
		//	m_listctrlUsableTimeTable.SetItemText(indx,2, it->strStartTime);
		//	m_listctrlUsableTimeTable.SetItemText(indx,3, it->strEndTime);
		//	m_listctrlUsableTimeTable.SetItemText(indx,4, it->strLastTime/*ChangeSpanTimeFormat(it->strLastTime)*/);
		//	if (it->bOuccupied)
		//	{
		//		m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
		//	}
		//	else
		//	{
		//		m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
		//	}
		//	CString strLevel;
		//	strLevel.Format(_T("%d"), it->nLevel);
		//	m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
		//	//m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("˫��"));


		//	indx++;
		//}
		//::LeaveCriticalSection(&(pData->m_csVisableTime));

		////���ر�ռ�õĵ��ǻ�û��ִ�е�ʱ��Ρ�
		//::EnterCriticalSection(&pData->m_csOcupiedTime);
		//for (ListTimeTable::iterator itor = pData->m_listOcupiedTime.begin();
		//	itor != pData->m_listOcupiedTime.end(); ++itor)
		//{
		//	if (itor->eType == enmSrvType_MAFMAR)
		//	{
		//		m_listctrlUsableTimeTable.InsertItem(indx, itor->strTDRS);
		//		m_listctrlUsableTimeTable.SetItemText(indx,1, itor->strSIC);
		//		m_listctrlUsableTimeTable.SetItemText(indx,2, itor->strStartTime);
		//		m_listctrlUsableTimeTable.SetItemText(indx,3, itor->strEndTime);
		//		m_listctrlUsableTimeTable.SetItemText(indx,4, itor->strLastTime);
		//		if (itor->bOuccupied)
		//		{
		//			m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
		//		}
		//		else
		//		{
		//			m_listctrlUsableTimeTable.SetItemText(indx,5, _T("��"));
		//		}
		//		CString strLevel;
		//		strLevel.Format(_T("%d"), itor->nLevel);
		//		m_listctrlUsableTimeTable.SetItemText(indx, 6, strLevel);
		//		m_listctrlUsableTimeTable.SetItemText(indx, 7, _T("˫��"));

		//		indx++;
		//	}
		//}
		//::LeaveCriticalSection(&pData->m_csOcupiedTime);
	}

	if (indx == 0)
	{
		AfxMessageBox(_T("��ǰû�����������Ŀ���ʱ��Σ�"));
	}

}

void CDlgCreateTaskWithUsabletime::SetMARUserFlag(BOOL bProf)
{
	m_bProfUser = bProf;
}
void CDlgCreateTaskWithUsabletime::SetServiceType(EServiceType eType)
{
	m_eSeviceType = eType;
}
void CDlgCreateTaskWithUsabletime::SetFilterTime(CTime timeBegin, CTime timeEnd)
{
	m_timeFilterBegin = timeBegin;
	m_timeFilterEnd  = timeEnd;
}

void CDlgCreateTaskWithUsabletime::OnClickListctrl(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	LPNMITEMACTIVATE pStruct = (LPNMITEMACTIVATE)pNotifyStruct;

	int nSelectedItem = pStruct->iItem;
	m_nSelectedItem = nSelectedItem;
	if(nSelectedItem != -1)
	{
		// Get subitem text from select item
		CString strTDRS = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 0);
		CString strSIC  = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 1);
		CString strStartTime = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 2);
		CString strEndTime = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 3);
		CString strSpanTime = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 4);
		CString strBeOcuped = m_listctrlUsableTimeTable.GetItemText(nSelectedItem, 5);

		// Set subitem text to control
		m_strSic = _T("001")/*strSIC*/;//2014��3��19��Ϊ��ģ����ռ�����ص�ȡ���û�002,003��
		m_strTdrs = strTDRS;
		m_editStartTime = strStartTime;
		m_editEndTime = strEndTime;

		CTime timeStart = MyTools::GetCTimeFromStringEx(strStartTime);
		CTime timeEnd	= MyTools::GetCTimeFromStringEx(strEndTime);

		m_serviceStartDate.SetTime(&timeStart);
		m_serviceStartTime.SetTime(&timeStart);
		m_serviceEndDate.SetTime(&timeEnd);
		m_serviceEndTime.SetTime(&timeEnd);

		UpdateData(FALSE);
	}


	*pResult = 0;

}
//����MAF & MAR ��������
void CDlgCreateTaskWithUsabletime::OnCreateTask()
{
	ApplyTaskRequest();
}

//�������ȼ����жϣ�����ǰ����������ʱ���Ƿ��
//��ǰϵͳ�е�ĳЩ�����ȼ��������ʱ���ͻ����ͻ���Ƿ�����ռ��
void CDlgCreateTaskWithUsabletime::ApplyTaskRequest()
{
	UpdateData(TRUE);

	
	
	int nLevel = m_comboServiceLevel.GetCurSel()+1;

	if (m_strSic.IsEmpty() || m_strSsc.IsEmpty())
	{
		AfxMessageBox(_T("SSC��SIC����Ϊ��"));
		return;
	}

	// show maf phase 2
	if (m_eSeviceType == enmSrvType_MAF)
	{
		if(m_pMAFPhase2Dlg != NULL)
		{
			if (m_pMAFPhase2Dlg->GetSafeHwnd() != NULL)
			{
				m_pMAFPhase2Dlg->DestroyWindow();
			}
			delete m_pMAFPhase2Dlg;
			m_pMAFPhase2Dlg = NULL;
		}

		m_pMAFPhase2Dlg = new CDlgMAFPhase2(this);
		m_pMAFPhase2Dlg->Create(CDlgMAFPhase2::IDD, this);
		m_pMAFPhase2Dlg->ShowWindow(SW_SHOW);
	}
	else if (m_eSeviceType == enmSrvType_MAR)
	{
		if (m_pMARPhase2Dlg != NULL)
		{
			if (m_pMARPhase2Dlg->GetSafeHwnd() != NULL)
			{
				m_pMARPhase2Dlg->DestroyWindow();
			}
			delete m_pMARPhase2Dlg;
			m_pMARPhase2Dlg = NULL;
		}

		m_pMARPhase2Dlg = new CDlgMARPhase2(this);
		m_pMARPhase2Dlg->Create(CDlgMARPhase2::IDD, this);
		m_pMARPhase2Dlg->ShowWindow(SW_SHOW);

	}
	else if (enmSrvType_MAFMAR == m_eSeviceType)
	{
		if (m_pMAFMARPhase2Dlg != NULL)
		{
			if (m_pMAFMARPhase2Dlg->GetSafeHwnd() != NULL)
			{
				m_pMAFMARPhase2Dlg->DestroyWindow();
			}
			delete m_pMAFMARPhase2Dlg;
			m_pMAFMARPhase2Dlg = NULL;
		}

		m_pMAFMARPhase2Dlg = new CDlgMAFMARPhase2(this);
		m_pMAFMARPhase2Dlg->Create(CDlgMAFMARPhase2::IDD, this);
		m_pMAFMARPhase2Dlg->ShowWindow(SW_SHOW);

	}
	
}

//������ռ��ǰ��ֻ��ռǰ�����񡣷�����ռ��������˫��ֻ��ǰ������
void CDlgCreateTaskWithUsabletime::CheckIfSuccess()
{
	int nLevel = m_comboServiceLevel.GetCurSel()+1;
	CString strBeOcuped;
	CString strLevel;
	CString strType;
	if (m_nSelectedItem >= 0)
	{
		strBeOcuped = m_listctrlUsableTimeTable.GetItemText(m_nSelectedItem, 5);
		strLevel = m_listctrlUsableTimeTable.GetItemText(m_nSelectedItem, 6);
		strType   = m_listctrlUsableTimeTable.GetItemText(m_nSelectedItem, 7);
	}
	
	//�õ����õ�ʱ���
	if (m_editStartTime.IsEmpty() || m_editEndTime.IsEmpty())
	{
		AfxMessageBox(_T("���ڿ�ʼʱ��򴰿ڽ���ʱ�䲻����Ϊ�գ���û��ѡ��һ������ʱ�������������"));
		return;
	}
	CTime timeStartUsable = MyTools::GetCTimeFromStringEx( m_editStartTime);
	CTime timeEndUsable   = MyTools::GetCTimeFromStringEx( m_editEndTime);

	//�õ��û���д��ʱ���
	CTime startTime,endTime,startDate,endDate;

	m_serviceStartDate.GetTime(startDate);
	CString strStartDate = MyTools::GetFormatedDate(startDate);
	m_serviceStartTime.GetTime(startTime);
	CString strStarttime = MyTools::GetFormatedTime(startTime);
	CString strNewStartDateTime = strStartDate + _T(" ") + strStarttime;
	CTime newStartDataTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);


	m_serviceEndDate.GetTime(endDate);
	CString strEnddata = MyTools::GetFormatedDate(endDate);
	m_serviceEndTime.GetTime(endTime);
	CString strEndtime = MyTools::GetFormatedTime(endTime);
	CString strNewEndDataTime = strEnddata + _T(" ") + strEndtime;
	CTime newEndDataTime = MyTools::GetCTimeFromStringEx(strNewEndDataTime);

	if (enmSrvType_MAF == m_eSeviceType)
	{

		//�ж��û���ʱ���Ƿ���Ч
		if (newStartDataTime >= timeStartUsable 
			&& newEndDataTime <= timeEndUsable)
		{
			//1.��鵱ǰ����ʱ���Ƿ���ϵͳ��δִ�е������ʱ���ͻ��
			//CheckIfTaskTimeOuccupied();

			//��鵱ǰ�Ƿ��Ǹ����ȼ���ռ��
			BOOL bOccupied = FALSE;
			if (strBeOcuped == _T("��"))
			{
				bOccupied = TRUE;
			}

			//�������ռģʽ��
			if (bOccupied)
			{
				int nOccupiedLevel = _tcstoul(strLevel.GetBuffer(), NULL,10);
				if (nLevel <= nOccupiedLevel)
				{
					CString strError;
					strError.Format(_T("��ʱ����ѱ�ռ�ã�SSC: %s���񴴽�ʧ��!"), m_strSsc);
					AfxMessageBox(strError);
					return;
				}
				else
				{
					if (strType == _T("ǰ��"))
					{
						if (IDYES == AfxMessageBox(_T("��ʱ��α������ȼ�����ռ�ã��Ƿ���ռ��Դ��"), MB_YESNO))
						{
							this->ShowWindow(SW_HIDE);

							//3.3D�»����ź���:pocc--->����վ
							CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
							if (NULL != pMain)
							{
								CDASSimulationView* pView = (CDASSimulationView*)(pMain->GetActiveView());
								pView->NotifyBeOccupied();
							}

							//��ռ�ɹ���
							TaskItem oneItem;
							oneItem.strTDRS = m_strTdrs;
							oneItem.strSIC = m_strSic;
							oneItem.strSSC = m_strSsc;
							oneItem.strStartTime = strNewStartDateTime;
							oneItem.strEndTime = strNewEndDataTime;
							UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
							CTimeSpan spanTime = endTime-startTime;
							CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
							CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
							oneItem.strLastTime = strSpanTime;
							//���ȼ���һ��
							oneItem.uLevel = (ETaskLevel)nLevel;
							oneItem.uStatus = enmTaskStatus_UnProcess;
							oneItem.nChannelID = 0;//MAF Ĭ�ϵ�channelͨ������0
							oneItem.eTaskType = enmTaskType_MAF;

							//ɾ���ɵ���������������
							CBussinessData::Instance()->DeleteTaskItem(m_editStartTime, m_editEndTime);
							//���������
							CBussinessData::Instance()->AddDASTaskItem(oneItem);


							//ɾ���ɵı�ռ��ʱ���
							CBussinessData::Instance()->DeleteOcupiedTimeTable(enmSrvType_MAF, m_editStartTime, m_editEndTime);
							//�����µı�ռ�õ�ʱ��Ρ�
							TimeItem item;
							item.bOuccupied = TRUE;
							item.strStartTime = strNewStartDateTime;
							item.strEndTime = strNewEndDataTime;
							item.strLastTime = strSpanTime;
							item.strSIC = m_strSic;
							item.strTDRS = m_strTdrs;
							item.nLevel = nLevel;
							item.eType = enmSrvType_MAF;
							CBussinessData::Instance()->AddOcupiedTime(item);

							//����ʱ���tut+visable+usable time
							CBussinessData::Instance()->UpdateTimeAfterOcupiedTask(enmSrvType_MAF, m_editStartTime, m_editEndTime,item);


							//����MAF tut+visabletime
							CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
							if (NULL != pMainFrm)
							{
								pMainFrm->PostMessage(WM_UPDATE_MAF_LISTCTRL);
							}

							//Update usable time table
							LoadUsableTimeTable();


							AfxMessageBox(_T("��ռ�����ȼ�����ɹ���"));
							ShowTaskDlg();

						}
						else
						{
							CString strInfo;
							strInfo.Format(_T("SSC: %s ���񴴽�ʧ�ܣ�"), m_strSsc);
							AfxMessageBox(strInfo);
							return;
						}
					}
					else
					{
						CString strError;
						strError.Format(_T("��ʱ�����������Ϊ:%s����ռʧ�ܣ�"), strType);
						AfxMessageBox(strError);
						return;
					}
				}

			}
			//һ��ģʽ��
			else
			{
				//2.normal add task.
				TaskItem oneItem;
				oneItem.strTDRS = m_strTdrs;
				oneItem.strSIC = m_strSic;
				oneItem.strSSC = m_strSsc;
				oneItem.strStartTime = strNewStartDateTime;
				oneItem.strEndTime = strNewEndDataTime;

				UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
				CTimeSpan spanTime = endTime-startTime;
				CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
				CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
				oneItem.strLastTime = strSpanTime;

				oneItem.uLevel = (ETaskLevel)nLevel;
				oneItem.uStatus = enmTaskStatus_UnProcess;
				oneItem.nChannelID = 0;//MAF Ĭ�ϵ�channelͨ������0
				oneItem.eTaskType = enmTaskType_MAF;


				CBussinessData::Instance()->AddDASTaskItem(oneItem);

				TimeItem item;
				item.bOuccupied = TRUE;
				item.strStartTime = strNewStartDateTime;
				item.strEndTime = strNewEndDataTime;
				item.strLastTime = strSpanTime;
				item.strSIC = m_strSic;
				item.strTDRS = m_strTdrs;
				item.nLevel = nLevel;
				item.eType = enmSrvType_MAF;
				CBussinessData::Instance()->AddOcupiedTime(item);
				CBussinessData::Instance()->UpdateUsableTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
				CBussinessData::Instance()->UpdateVisbleTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
				CBussinessData::Instance()->UpdateTUT(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
			
				//����MAF tut+visabletime
				CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
				if (NULL != pMainFrm)
				{
					pMainFrm->PostMessage(WM_UPDATE_MAF_LISTCTRL);
				}

				//Update usable time table
				LoadUsableTimeTable();


				AfxMessageBox(_T("�������ɹ���"));
				ShowTaskDlg();
			}
		}
		else
		{
			AfxMessageBox(_T("��������ʧ�ܣ�����д��ʱ�䲻�ڿ���ʱ����ڣ�"));
			return;
		}

	}
	//����Ҫ����TUT
	else if (enmSrvType_MAR == m_eSeviceType)
	{
		if (newStartDataTime >= timeStartUsable 
			&& newEndDataTime <= timeEndUsable)
		{
			//��鵱ǰ�Ƿ��Ǹ����ȼ���ռ��
			BOOL bOccupied = FALSE;
			if (strBeOcuped == _T("��"))
			{
				bOccupied = TRUE;
			}

			//�������ռģʽ��
			if (bOccupied)
			{
				int nOccupiedLevel = _tcstoul(strLevel.GetBuffer(), NULL,10);
				if (nLevel <= nOccupiedLevel)
				{
					CString strError;
					strError.Format(_T("��ʱ����ѱ�ռ�ã�SSC: %s���񴴽�ʧ��!"), m_strSsc);
					AfxMessageBox(strError);
					return;
				}
				else
				{
					if (strType == _T("����"))
					{
						if (IDYES == AfxMessageBox(_T("��ʱ��α������ȼ�����ռ�ã��Ƿ���ռ��Դ��"), MB_YESNO))
						{
							//��ռ�ɹ���
							TaskItem oneItem;
							oneItem.strTDRS = m_strTdrs;
							oneItem.strSIC = m_strSic;
							oneItem.strSSC = m_strSsc;
							oneItem.strStartTime = strNewStartDateTime;
							oneItem.strEndTime = strNewEndDataTime;
							UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
							CTimeSpan spanTime = endTime-startTime;
							CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
							CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
							oneItem.strLastTime = strSpanTime;
							//���ȼ���һ��
							oneItem.uLevel = (ETaskLevel)nLevel;
							oneItem.uStatus = enmTaskStatus_UnProcess;
							oneItem.nChannelID = m_comboChannelID.GetCurSel()+1;
							oneItem.eTaskType = enmTaskType_MAR;

							//ɾ���ɵ���������������
							CBussinessData::Instance()->DeleteTaskItem(m_editStartTime, m_editEndTime);
							//���������
							CBussinessData::Instance()->AddDASTaskItem(oneItem);


							//ɾ���ɵı�ռ��ʱ���
							CBussinessData::Instance()->DeleteOcupiedTimeTable(enmSrvType_MAR, m_editStartTime, m_editEndTime);
							//�����µı�ռ�õ�ʱ��Ρ�
							TimeItem item;
							item.bOuccupied = TRUE;
							item.strStartTime = strNewStartDateTime;
							item.strEndTime = strNewEndDataTime;
							item.strLastTime = strSpanTime;
							item.strSIC = m_strSic;
							item.strTDRS = m_strTdrs;
							item.nLevel = nLevel;
							item.eType = enmSrvType_MAR;
							CBussinessData::Instance()->AddOcupiedTime(item);

							CBussinessData::Instance()->UpdateTimeAfterOcupiedTask(enmSrvType_MAR, m_editStartTime, m_editEndTime,item);

							CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
							if (NULL != pMainFrm)
							{
								pMainFrm->PostMessage(WM_UPDATE_MAR_LISTCTRL);
							}

							//Update usable time table
							LoadUsableTimeTable();

							AfxMessageBox(_T("��ռ�����ȼ�����ɹ���"));
							ShowTaskDlg();

						}
						else
						{
							CString strInfo;
							strInfo.Format(_T("SSC: %s ���񴴽�ʧ�ܣ�"), m_strSsc);
							AfxMessageBox(strInfo);
							return;
						}
					}
					else
					{
						CString strInfo;
						strInfo.Format(_T("��ʱ�����������Ϊ:%s����ռʧ�ܣ�"), strType);
						AfxMessageBox(strInfo);
						return;
					}
				}

			}
			else
			{
				//2.normal add task.
				TaskItem oneItem;
				oneItem.strTDRS = m_strTdrs;
				oneItem.strSIC = m_strSic;
				oneItem.strSSC = m_strSsc;
				oneItem.strStartTime = strNewStartDateTime;
				oneItem.strEndTime = strNewEndDataTime;

				UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
				CTimeSpan spanTime = endTime-startTime;
				CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
				CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
				oneItem.strLastTime = strSpanTime;

				oneItem.uLevel = (ETaskLevel)nLevel;
				oneItem.uStatus = enmTaskStatus_UnProcess;
				oneItem.nChannelID = m_comboChannelID.GetCurSel()+1;
				oneItem.eTaskType = enmTaskType_MAR;


				CBussinessData::Instance()->AddDASTaskItem(oneItem);

				TimeItem item;
				item.bOuccupied = TRUE;
				item.strStartTime = strNewStartDateTime;
				item.strEndTime = strNewEndDataTime;
				item.strLastTime = strSpanTime;
				item.strSIC = m_strSic;
				item.strTDRS = m_strTdrs;
				item.nLevel = nLevel;
				item.eType = enmSrvType_MAR;
				CBussinessData::Instance()->AddOcupiedTime(item);
				CBussinessData::Instance()->UpdateUsableTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
				CBussinessData::Instance()->UpdateVisbleTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
			
				//����visabletime
				CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
				if (NULL != pMainFrm)
				{
					pMainFrm->PostMessage(WM_UPDATE_MAR_LISTCTRL);
				}

				//Update usable time table
				LoadUsableTimeTable();

				AfxMessageBox(_T("�������ɹ���"));
				ShowTaskDlg();
			}
		}
		else
		{
			AfxMessageBox(_T("��������ʧ�ܣ�����д��ʱ�䲻�ڿ���ʱ����ڣ�"));
			return;
		}
	}
	else if (enmSrvType_MAFMAR == m_eSeviceType)
	{
		//�ж��û���ʱ���Ƿ���Ч
		if (newStartDataTime >= timeStartUsable 
			&& newEndDataTime <= timeEndUsable)
		{
			//1.��鵱ǰ����ʱ���Ƿ���ϵͳ��δִ�е������ʱ���ͻ��
			//CheckIfTaskTimeOuccupied();

			//��鵱ǰ�Ƿ��Ǹ����ȼ���ռ��
			BOOL bOccupied = FALSE;
			if (strBeOcuped == _T("��"))
			{
				bOccupied = TRUE;
			}

			//�������ռģʽ��
			if (bOccupied)
			{
				int nOccupiedLevel = _tcstoul(strLevel.GetBuffer(), NULL,10);
				if (nLevel <= nOccupiedLevel)
				{
					CString strError;
					strError.Format(_T("��ʱ����ѱ�ռ�ã�SSC: %s���񴴽�ʧ��!"), m_strSsc);
					AfxMessageBox(strError);
					return;
				}
				else
				{
					//˫������ֻ������ռǰ������+˫��
					if (strType == _T("ǰ��") || strType == _T("˫��"))
					{
						if (IDYES == AfxMessageBox(_T("��ʱ��α������ȼ�����ռ�ã��Ƿ���ռ��Դ��"), MB_YESNO))
						{
							//��ռ�ɹ���
							TaskItem oneItem;
							oneItem.strTDRS = m_strTdrs;
							oneItem.strSIC = m_strSic;
							oneItem.strSSC = m_strSsc;
							oneItem.strStartTime = strNewStartDateTime;
							oneItem.strEndTime = strNewEndDataTime;
							UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
							CTimeSpan spanTime = endTime-startTime;
							CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
							CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
							oneItem.strLastTime = strSpanTime;
							//���ȼ���һ��
							oneItem.uLevel = (ETaskLevel)nLevel;
							oneItem.uStatus = enmTaskStatus_UnProcess;
							oneItem.nChannelID = 0;//MAF Ĭ�ϵ�channelͨ������0
							oneItem.eTaskType = enmTaskType_MAFMAR;

							//ɾ���ɵ���������������
							CBussinessData::Instance()->DeleteTaskItem(m_editStartTime, m_editEndTime);
							//���������
							CBussinessData::Instance()->AddDASTaskItem(oneItem);


							//ɾ���ɵı�ռ��ʱ���
							if (strType == _T("ǰ��"))
							{
								CBussinessData::Instance()->DeleteOcupiedTimeTable(enmSrvType_MAF, m_editStartTime, m_editEndTime);
							}
							else if (strType == _T("˫��"))
							{
								CBussinessData::Instance()->DeleteOcupiedTimeTable(enmSrvType_MAFMAR, m_editStartTime, m_editEndTime);
							}
							
							//�����µı�ռ�õ�ʱ��Ρ�
							TimeItem item;
							item.bOuccupied = TRUE;
							item.strStartTime = strNewStartDateTime;
							item.strEndTime = strNewEndDataTime;
							item.strLastTime = strSpanTime;
							item.strSIC = m_strSic;
							item.strTDRS = m_strTdrs;
							item.nLevel = nLevel;
							item.eType = enmSrvType_MAFMAR;
							CBussinessData::Instance()->AddOcupiedTime(item);

							//����ʱ���tut+visable+usable time
							if (strType == _T("ǰ��"))
							{
								CBussinessData::Instance()->UpdateTimeAfterOcupiedTask(enmSrvType_MAF, m_editStartTime, m_editEndTime,item);
							}
							else if (strType == _T("˫��"))
							{
								CBussinessData::Instance()->UpdateTimeAfterOcupiedTask(enmSrvType_MAFMAR, m_editStartTime, m_editEndTime,item);
							}
							


							//����MAF tut+visabletime
							CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
							if (NULL != pMainFrm)
							{
								pMainFrm->PostMessage(WM_UPDATE_MAFMAR_LISTCTRL);
							}

							//Update usable time table
							LoadUsableTimeTable();


							AfxMessageBox(_T("��ռ�����ȼ�����ɹ���"));
							ShowTaskDlg();

						}
						else
						{
							CString strInfo;
							strInfo.Format(_T("SSC: %s ���񴴽�ʧ�ܣ�"), m_strSsc);
							AfxMessageBox(strInfo);
							return;
						}
					}
					else
					{
						CString strError;
						strError.Format(_T("��ʱ�����������Ϊ:%s����ռʧ�ܣ�"), strType);
						AfxMessageBox(strError);
						return;
					}
				}

			}
			//һ��ģʽ��
			else
			{
				//2.normal add task.
				TaskItem oneItem;
				oneItem.strTDRS = m_strTdrs;
				oneItem.strSIC = m_strSic;
				oneItem.strSSC = m_strSsc;
				oneItem.strStartTime = strNewStartDateTime;
				oneItem.strEndTime = strNewEndDataTime;

				UINT nSpanDayTime = GetSpanDayTimeInSenconds(strStartDate, strEnddata);
				CTimeSpan spanTime = endTime-startTime;
				CTimeSpan newSpanTime = spanTime.GetTimeSpan() + nSpanDayTime;
				CString strSpanTime = MyTools::GetFormatSpanTime(newSpanTime);
				oneItem.strLastTime = strSpanTime;

				oneItem.uLevel = (ETaskLevel)nLevel;
				oneItem.uStatus = enmTaskStatus_UnProcess;
				oneItem.nChannelID = 0;//MAF Ĭ�ϵ�channelͨ������0
				oneItem.eTaskType = enmTaskType_MAFMAR;


				CBussinessData::Instance()->AddDASTaskItem(oneItem);

				TimeItem item;
				item.bOuccupied = TRUE;
				item.strStartTime = strNewStartDateTime;
				item.strEndTime = strNewEndDataTime;
				item.strLastTime = strSpanTime;
				item.strSIC = m_strSic;
				item.strTDRS = m_strTdrs;
				item.nLevel = nLevel;
				item.eType = enmSrvType_MAFMAR;
				CBussinessData::Instance()->AddOcupiedTime(item);
				CBussinessData::Instance()->UpdateUsableTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
				CBussinessData::Instance()->UpdateVisbleTime(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);
				CBussinessData::Instance()->UpdateTUT(m_editStartTime, m_editEndTime, strNewStartDateTime, strNewEndDataTime);

				//����MAF tut+visabletime
				CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
				if (NULL != pMainFrm)
				{
					pMainFrm->PostMessage(WM_UPDATE_MAFMAR_LISTCTRL);
				}

				//Update usable time table
				LoadUsableTimeTable();


				AfxMessageBox(_T("�������ɹ���"));
				ShowTaskDlg();
			}
		}
		else
		{
			AfxMessageBox(_T("��������ʧ�ܣ�����д��ʱ�䲻�ڿ���ʱ����ڣ�"));
			return;
		}
	}
	
}
UINT CDlgCreateTaskWithUsabletime::GetSpanDayTimeInSenconds(CString strStartDay, CString strEndDay)
{
	int indexStart = strStartDay.ReverseFind('-');
	int indexEnd   = strEndDay.ReverseFind('-');
	CString str1 = strStartDay.Right(strStartDay.GetLength()-indexStart-1);
	CString str2 = strEndDay.Right(strEndDay.GetLength()-indexEnd-1);
	UINT dStart = _tcstoul(str1.GetBuffer(),NULL,10);
	UINT dEnd = _tcstoul(str2.GetBuffer(), NULL, 10);

	//ת��������
	if (dStart>dEnd || dStart == 0 || dEnd == 0 || dStart>31)
	{
		return 0;
	}

	return (dEnd-dStart)*24*3600;
}

CString CDlgCreateTaskWithUsabletime::ChangeSpanTimeFormat(CString strSpanTime)
{
	int nIndex = strSpanTime.Find('.');
	CString strSecondes = strSpanTime.Left(nIndex);
	int nSenconds = _tcstoul(strSecondes.GetBuffer(), NULL, 10);
	int nHour = 0;
	int nMini = 0;
	int nSec  = 0;
	nHour = nSenconds/3600;
	if (nHour>0)
	{
		nSenconds = nSenconds-3600*nHour;
	}
	nMini = nSenconds/60;
	if (nMini>0)
	{
		nSenconds = nSenconds - nMini*60;
	}
	nSec = nSenconds;

	CString strNewSpanTime;
	strNewSpanTime.Format(_T("%d:%d:%d"), nHour, nMini, nSec);

	return strNewSpanTime;
}

void CDlgCreateTaskWithUsabletime::ShowTaskDlg()
{
	if (NULL != m_pSettingSysTimeDlg)
	{
		m_pSettingSysTimeDlg->DestroyWindow();
		delete m_pSettingSysTimeDlg;
		m_pSettingSysTimeDlg = NULL;
	}

	m_pSettingSysTimeDlg = new CDlgSettimeToStarttask;
	m_pSettingSysTimeDlg->Create(CDlgSettimeToStarttask::IDD, NULL);
	m_pSettingSysTimeDlg->HighLightTaskItem();
	m_pSettingSysTimeDlg->ShowWindow(SW_SHOW);

	//�Լ��˳���̫�ԵĶԻ����ˡ�
	PostMessage(WM_CLOSE);
}
