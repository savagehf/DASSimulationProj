// DlgMAFMARQuery.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFMARQuery.h"
#include "MainFrm.h"
#include "DASSimulationView.h"
#include "MyTools.h"
#include "BussinessData.h"
#include "DlgCreateTaskWithUsabletime.h"


// CDlgMAFMARQuery dialog

IMPLEMENT_DYNAMIC(CDlgMAFMARQuery, CDialog)

CDlgMAFMARQuery::CDlgMAFMARQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFMARQuery::IDD, pParent)
	, m_strMARSSC(_T(""))
{
	m_pUsableTimeDlg = NULL;
}

CDlgMAFMARQuery::~CDlgMAFMARQuery()
{
	if (m_pUsableTimeDlg != NULL)
	{
		if (m_pUsableTimeDlg->GetSafeHwnd() != NULL)
		{
			m_pUsableTimeDlg->DestroyWindow();
		}
		delete m_pUsableTimeDlg;
		m_pUsableTimeDlg = NULL;
	}
}

void CDlgMAFMARQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAFMAR_COMBO_SIC, m_comboSIC);
	DDX_Control(pDX, IDC_MAFMAR_COMBO_TDRS, m_ctlTdrs);

	DDX_Control(pDX, IDC_MAFMAR_TIMEPICKER_START, m_ctlStartTime);
	DDX_Control(pDX, IDC_MAFMAR_TIMEPICKER_END, m_ctlEndTime);
	DDX_Control(pDX, IDC_MAFMAR_DATEPICKER_START, m_ctlStartDate);
	DDX_Control(pDX, IDC_MAFMAR_DATEPICKER_END, m_ctlEndDate);

	DDX_DateTimeCtrl(pDX, IDC_MAFMAR_DATEPICKER_START, m_ctimeStartDate);
	DDX_DateTimeCtrl(pDX, IDC_MAFMAR_DATEPICKER_END, m_ctimeEndDate);
	DDX_DateTimeCtrl(pDX, IDC_MAFMAR_TIMEPICKER_START, m_ctimeStartTime);
	DDX_DateTimeCtrl(pDX, IDC_MAFMAR_TIMEPICKER_END, m_ctimeEndTime);

	DDX_Control(pDX, IDC_LIST_MAFMAR_US_VIS_TABLE, m_listUserVisableTable);
	DDX_Control(pDX, IDC_LIST_MAFMAR_TASK_CTRL, m_listMARTask);
	DDX_Text(pDX, IDC_MAFMAR_EDIT_MAR_SSC, m_strMARSSC);
}


BEGIN_MESSAGE_MAP(CDlgMAFMARQuery, CDialog)
	ON_BN_CLICKED(IDC_BTN_MAFMAR_CREATE_UP_TIME, &CDlgMAFMARQuery::OnBnClickedBtnMafMarCreateUpTime)
	ON_BN_CLICKED(IDC_MAFMAR_PLAN_REQ, &CDlgMAFMARQuery::OnBnClickedBtnMafMarServicePlanReq)
END_MESSAGE_MAP()


BOOL CDlgMAFMARQuery::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctimeStartDate = CTime::GetCurrentTime();
	m_ctimeEndDate = CTime::GetCurrentTime();
	m_ctimeStartTime = CTime::GetCurrentTime();
	m_ctimeEndTime = CTime::GetCurrentTime();

	UpdateData(FALSE);
	m_ctlTdrs.InsertString(0, _T("1"));
	m_ctlTdrs.SetCurSel(0);
	m_comboSIC.InsertString(0, _T("001"));
	//m_comboSIC.InsertString(1, _T("002"));
	//m_comboSIC.InsertString(2, _T("003"));
	m_comboSIC.SetCurSel(0);

	DWORD dwStyle = m_listUserVisableTable.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listUserVisableTable.SetExtendedStyle(dwStyle);

	DWORD dwStyle2 = m_listMARTask.GetExtendedStyle();
	dwStyle2 |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle2 |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listMARTask.SetExtendedStyle(dwStyle2);

	// TODO: Add extra initialization here
	m_listUserVisableTable.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listUserVisableTable.InsertColumn(2, _T("SIC"), LVCFMT_LEFT, 50);
	m_listUserVisableTable.InsertColumn(3, _T("��ʼʱ��"), LVCFMT_LEFT, 150);
	m_listUserVisableTable.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 150);
	m_listUserVisableTable.InsertColumn(5, _T("����ʱ��"), LVCFMT_LEFT, 150);

	int index = 0;
	m_listMARTask.InsertColumn(index++, _T("��������"), LVCFMT_RIGHT, 67);
	m_listMARTask.InsertColumn(index++, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listMARTask.InsertColumn(index++, _T("�û�SIC"), LVCFMT_LEFT, 65);
	m_listMARTask.InsertColumn(index++, _T("����SSC"), LVCFMT_LEFT, 65);
	m_listMARTask.InsertColumn(index++, _T("����"), LVCFMT_LEFT, 50);
	m_listMARTask.InsertColumn(index++, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listMARTask.InsertColumn(index++, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listMARTask.InsertColumn(index++, _T("����ʱ��"), LVCFMT_LEFT, 67);
	m_listMARTask.InsertColumn(index++, _T("����״̬"), LVCFMT_LEFT, 67);
	m_listMARTask.InsertColumn(index++, _T("����ͨ��"), LVCFMT_LEFT, 67);

	return TRUE;
}

// CDlgMAFMARQuery message handlers
void CDlgMAFMARQuery::OnBnClickedBtnMafMarServicePlanReq()
{
	//1.����ʾMAF phase 1�Ի���
	UpdateData(TRUE);

	if ( m_strMARSSC.IsEmpty() )
	{
		AfxMessageBox(_T("SSC ������Ϊ�գ�"));
		return;
	}

	//���ж���ʱ����д�Ƿ���ȷ
	CTime newStartTime;
	CTime newEndTime;
	GetNewStartEndDataTime(newStartTime, newEndTime);

	if (newStartTime >= newEndTime)
	{
		AfxMessageBox(_T("����'����ʱ��'���ڻ����'��ʼʱ��'!"));
		return ;
	}

	//2.�����Լ���
	this->ShowWindow(SW_HIDE);

	//3.3D�»����ź���:pocc--->����վ
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (NULL != pMain)
	{
		CDASSimulationView* pView = (CDASSimulationView*)(pMain->GetActiveView());
		pView->StartDrawMAFMARPhase1ReuestSignal(TRUE);
	}	

}

void CDlgMAFMARQuery::ShowMAFMARDlg()
{
	ShowWindow(SW_RESTORE);

	//2.show task an stk time table in listctrl
	InsertStkDataToListctrl();
	InsertTaskToListctrl();
}

void CDlgMAFMARQuery::UpdateListctrl()
{
	InsertStkDataToListctrl();
	InsertTaskToListctrl();
}


void CDlgMAFMARQuery::InsertStkDataToListctrl()
{
	UpdateData(TRUE);

	CString strTDRS;
	m_ctlTdrs.GetWindowText(strTDRS);
	strTDRS.MakeLower();
	CString strSIC;
	m_comboSIC.GetWindowText(strSIC);
	strSIC.MakeLower();
	int nTDRS = _tcstoul(strTDRS.GetBuffer(), NULL, 10);
	int nSIC  = _tcstoul(strSIC.GetBuffer(), NULL, 10);

	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strEnd   = MyTools::GetFormatedDate(m_ctimeEndDate);
	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strTimeEnd = MyTools::GetFormatedTime(m_ctimeEndTime);
	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	CString strNewEndDateTime = strEnd + _T(" ")+strTimeEnd;

	CTime timeNewStartDateTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	CTime timeNewEndDateTime = MyTools::GetCTimeFromStringEx(strNewEndDateTime);

	//////////////////////////////////////////////////////////////////////////
	//ģ��UP ��

	CBussinessData* pData = CBussinessData::Instance();

	//���ɿɼ�ʱ��-canceled-2013��12��15��23:37:33
	//map<CString, CString> mapVisTimeTable;
	//pData->SimulateVisableTime(timeNewStartDateTime, timeNewEndDateTime, mapVisTimeTable);

	//���ؿɼ�ʱ��
	//pData->ImportUSAndTDRSVisibleTimeTable();

	m_listUserVisableTable.DeleteAllItems();
	int indx = 0;
	::EnterCriticalSection(&(pData->m_csVisableTime));
	for (ListTimeTable::iterator it = pData->m_listVisbleTime.begin();
		it != pData->m_listVisbleTime.end(); ++it)
	{
		CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
		CTime timeEnd = MyTools::GetCTimeFromStringEx(it->strEndTime);
		int nTD = _tcstoul(it->strTDRS.GetBuffer(), NULL, 10);
		int nSI = _tcstoul(it->strSIC.GetBuffer(), NULL, 10);

		CString strNewSpanTime = it->strLastTime;

		if (nTD == nTDRS
			&& nSI == nSIC
			&& timeStart >= timeNewStartDateTime
			&& timeEnd <= timeNewEndDateTime)
		{

			m_listUserVisableTable.InsertItem(indx, it->strTDRS);
			m_listUserVisableTable.SetItemText(indx,1, it->strSIC);
			m_listUserVisableTable.SetItemText(indx,2, it->strStartTime);
			m_listUserVisableTable.SetItemText(indx,3, it->strEndTime);
			m_listUserVisableTable.SetItemText(indx,4, strNewSpanTime);

			indx++;
		}
	}

	::LeaveCriticalSection(&(pData->m_csVisableTime));

	if (indx == 0)
	{
		AfxMessageBox(_T("��ǰû�������ѯ������US-TDRS�ɼ�ʱ��Σ�"));
	}

}
void CDlgMAFMARQuery::InsertTaskToListctrl()
{
	UpdateData(TRUE);
	//if(m_ctimeStartDate >= m_ctimeEndDate)
	CTime newStartTime;
	CTime newEndTime;
	GetNewStartEndDataTime(newStartTime, newEndTime);

	if (newStartTime >= newEndTime)
	{
		AfxMessageBox(_T("����'����ʱ��'���ڻ����'��ʼʱ��'!"));
		return;
	}

	CBussinessData* pData = CBussinessData::Instance();

	CString strTDRS;
	m_ctlTdrs.GetWindowText(strTDRS);
	strTDRS.MakeLower();

	m_listMARTask.DeleteAllItems();

	BOOL bNoTask = TRUE;
	int nIndex = 0;
	::EnterCriticalSection(&(pData->m_csAllTasks));
	for (ListTaskTable::iterator it = pData->m_listAllTasks.begin();
		it != pData->m_listAllTasks.end(); ++it)
	{
		CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
		CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);

		CString strTD = it->strTDRS;
		strTD.MakeLower();
		if (   strTDRS == strTD
			&& timeStart >= newStartTime
			&& timeEnd   <= newEndTime)
		{
			bNoTask = FALSE;

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


			m_listMARTask.InsertItem(nIndex, strTaskType);
			m_listMARTask.SetItemText(nIndex,1,it->strTDRS);
			m_listMARTask.SetItemText(nIndex,2,it->strSIC);
			m_listMARTask.SetItemText(nIndex,3,it->strSSC);
			m_listMARTask.SetItemText(nIndex,4,strLevel);
			m_listMARTask.SetItemText(nIndex,5,it->strStartTime);
			m_listMARTask.SetItemText(nIndex,6,it->strEndTime);
			m_listMARTask.SetItemText(nIndex,7,it->strLastTime);
			m_listMARTask.SetItemText(nIndex,8,strStats);
			m_listMARTask.SetItemText(nIndex,9,strChannel);

			nIndex++;
		}
	}
	::LeaveCriticalSection(&(pData->m_csAllTasks));

	if (bNoTask)
	{
		//AfxMessageBox(_T("��ǰû����������������"));
	}
}


void CDlgMAFMARQuery::OnBnClickedBtnMafMarCreateUpTime()
{
	UpdateData(TRUE);

	//1.���ɿɼ�ʱ�䴰��ȡ�������㡣
	//lee
	//CBussinessData::Instance()->CalculateUsableTimeTable();

	//2.MAF: ��ʾ���ؼ����Ŀɼ�ʱ�䴰��
	if (m_pUsableTimeDlg != NULL)
	{
		if (m_pUsableTimeDlg->GetSafeHwnd() != NULL)
		{
			m_pUsableTimeDlg->DestroyWindow();
		}
		delete m_pUsableTimeDlg;
		m_pUsableTimeDlg = NULL;
	}

	CTime timeBegin,timeEnd;
	GetNewStartEndDataTime(timeBegin, timeEnd);

	m_pUsableTimeDlg = new CDlgCreateTaskWithUsabletime;
	m_pUsableTimeDlg->m_strSsc = m_strMARSSC;
	m_pUsableTimeDlg->SetServiceType(enmSrvType_MAFMAR);
	m_pUsableTimeDlg->SetFilterTime(timeBegin, timeEnd);
	m_pUsableTimeDlg->Create(CDlgCreateTaskWithUsabletime::IDD, this);
	m_pUsableTimeDlg->ShowWindow(SW_SHOW);

}

BOOL CDlgMAFMARQuery::GetNewStartEndDataTime(CTime & newStart, CTime & newEnd)
{
	CString strStartDate = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strStartTime = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strEndDate = MyTools::GetFormatedDate(m_ctimeEndDate);
	CString strEndTime = MyTools::GetFormatedTime(m_ctimeEndTime);
	CString strNewStartDateTime = strStartDate + _T(" ") + strStartTime;
	CString strNewEndDateTime = strEndDate + _T(" ") + strEndTime;

	newStart = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	newEnd = MyTools::GetCTimeFromStringEx(strNewEndDateTime);

	return TRUE;
}