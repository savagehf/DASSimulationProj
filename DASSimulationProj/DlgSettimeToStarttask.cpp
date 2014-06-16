// DlgSettingSysTime.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgSettimeToStarttask.h"
#include "MainFrm.h"
#include "MyTools.h"
#include "DASRender.h"
#include "DASSimulationView.h"
#include "BussinessData.h"



// CDlgSettimeToStarttask dialog

IMPLEMENT_DYNAMIC(CDlgSettimeToStarttask, CDialog)

CDlgSettimeToStarttask::CDlgSettimeToStarttask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettimeToStarttask::IDD, pParent)
{

}

CDlgSettimeToStarttask::~CDlgSettimeToStarttask()
{
}

void CDlgSettimeToStarttask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_DATEPICKER_START, m_ctlStartDate);
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_ctlStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_START, m_ctimeStartDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_START, m_ctimeStartTime);

	DDX_Control(pDX, IDC_LIST_ALL_TASK, m_listctrlAllTasks);
	DDX_Control(pDX, IDC_EDIT_TASK_STATUS, m_editTaskStatus);
}


BEGIN_MESSAGE_MAP(CDlgSettimeToStarttask, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALL_TASK, OnClickListctrl)
END_MESSAGE_MAP()

BOOL CDlgSettimeToStarttask::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_listctrlAllTasks.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;          
	m_listctrlAllTasks.SetExtendedStyle(dwStyle);


	m_ctimeStartDate = CTime::GetCurrentTime();
	m_ctimeStartTime = CTime::GetCurrentTime();

	m_listctrlAllTasks.InsertColumn(0, _T("��������"), LVCFMT_RIGHT, 67);
	m_listctrlAllTasks.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlAllTasks.InsertColumn(2, _T("�û�SIC"), LVCFMT_LEFT, 65);
	m_listctrlAllTasks.InsertColumn(3, _T("����SSC"), LVCFMT_LEFT, 65);
	m_listctrlAllTasks.InsertColumn(4, _T("����"), LVCFMT_LEFT, 50);
	m_listctrlAllTasks.InsertColumn(5, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listctrlAllTasks.InsertColumn(6, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listctrlAllTasks.InsertColumn(7, _T("����ʱ��"), LVCFMT_LEFT, 67);
	m_listctrlAllTasks.InsertColumn(8, _T("����״̬"), LVCFMT_LEFT, 67);
	m_listctrlAllTasks.InsertColumn(9, _T("����ͨ��"), LVCFMT_LEFT, 67);

	UpdateData(FALSE);

	InitTaskListctrl();
	
	return TRUE;
}

void CDlgSettimeToStarttask::InitTaskListctrl()
{
	UpdateData(TRUE);

	CBussinessData* pData = CBussinessData::Instance();
	m_listctrlAllTasks.DeleteAllItems();

	int nIndex = 0;
	::EnterCriticalSection(&(pData->m_csAllTasks));
	for (ListTaskTable::iterator it = pData->m_listAllTasks.begin();
		it != pData->m_listAllTasks.end(); ++it)
	{
		
		CString strLevel;
		strLevel.Format(_T("%d"), it->uLevel);
		CString strChannel;
		if (it->nChannelID == 0)
		{
			strChannel = _T("");
		}
		else
		{
			strChannel.Format(_T("%d"), it->nChannelID);
		}


		CString strStats;

		switch(it->uStatus)
		{
		case enmTaskStatus_UnProcess:
			{
				strStats = _T("��ִ��");
			}
			break;
		case enmTaskStatus_Prepare:
			{
				strStats = _T("׼����");
			}
		case enmTaskStatus_Prcessing:
			{
				strStats = _T("ִ����");
			}
			break;
		case enmTaskStatus_Processed:
			{
				strStats = _T("ִ����");
			}
			break;
		default:
			break;
		}

		CString strTaskType;
		if (it->eTaskType == enmTaskType_MAF)
		{
			strTaskType = _T("ǰ��");
		}
		else if(it->eTaskType == enmTaskType_MAR)
		{
			strTaskType = _T("����");
		}
		else if (it->eTaskType == enmTaskType_MAFMAR)
		{
			strTaskType = _T("˫��");
		}


		m_listctrlAllTasks.InsertItem(nIndex, strTaskType);
		m_listctrlAllTasks.SetItemText(nIndex,1,it->strTDRS);
		m_listctrlAllTasks.SetItemText(nIndex,2,it->strSIC);
		m_listctrlAllTasks.SetItemText(nIndex,3,it->strSSC);
		m_listctrlAllTasks.SetItemText(nIndex,4,strLevel);
		m_listctrlAllTasks.SetItemText(nIndex,5,it->strStartTime);
		m_listctrlAllTasks.SetItemText(nIndex,6,it->strEndTime);
		m_listctrlAllTasks.SetItemText(nIndex,7,it->strLastTime);
		m_listctrlAllTasks.SetItemText(nIndex,8,strStats);
		m_listctrlAllTasks.SetItemText(nIndex,9,strChannel);

		nIndex++;
	}
	::LeaveCriticalSection(&(pData->m_csAllTasks));

}

void CDlgSettimeToStarttask::HighLightTaskItem()
{
	int nLastItem = m_listctrlAllTasks.GetItemCount() - 1 ;

	m_listctrlAllTasks.SetFocus();
	m_listctrlAllTasks.EnsureVisible(nLastItem, FALSE);
	m_listctrlAllTasks.SetItemState(nLastItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CDlgSettimeToStarttask::OnClickListctrl(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	LPNMITEMACTIVATE pStruct = (LPNMITEMACTIVATE)pNotifyStruct;

	int nSelectedItem = pStruct->iItem;
	if(nSelectedItem != -1)
	{
		// Get subitem text from select item
		CString strStartTime = m_listctrlAllTasks.GetItemText(nSelectedItem, 5);
		CString strTaskStatus = m_listctrlAllTasks.GetItemText(nSelectedItem, 8);

		CTime timeStart = MyTools::GetCTimeFromStringEx(strStartTime);
		timeStart  = timeStart.GetTime()-TIME_BEFORE_TASK_BEGIN;

		CString str = MyTools::GetFormatedDateTime(timeStart);
		//m_ctlStartDate.SetTime(&timeStart);
		//m_ctlStartTime.SetTime(&timeStart);
		m_ctimeStartDate = timeStart;
		m_ctimeStartTime = timeStart;

		m_editTaskStatus.SetWindowText(strTaskStatus);

		UpdateData(FALSE);
	}


	*pResult = 0;

}

void CDlgSettimeToStarttask::OnOK()
{
	UpdateData(TRUE);

	CString strStatus;
	m_editTaskStatus.GetWindowText(strStatus);
	if (strStatus == _T("ִ����") || strStatus  == __T("ִ����"))
	{
		CString str;
		str.Format(_T("��ǰ����״̬Ϊ [%s] ,�����Լ���ִ�С�"), strStatus);
		AfxMessageBox(str);
		return;
	}
	

	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	CTime timeStart = MyTools::GetCTimeFromStringEx(strNewStartDateTime);


	CDASRender::Instance()->SetNewTimeInView(CTime::GetCurrentTime(), timeStart);
// 	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
// 	if (NULL != pMain)
// 	{
// 		CDASSimulationView* pView = (CDASSimulationView*)(pMain->GetActiveView());
// 		pView->CheckIfTaskAvailable();
// 	}


	CDialog::OnOK();
}

// CDlgSettimeToStarttask message handlers
