
#include "stdafx.h"

#include "DlgMAFRequest.h"
//#include "Task.h"
#include "MyTools.h"
#include "DASRender.h"
#include "BussinessData.h"
#include "MainFrm.h"
#include "DASSimulationView.h"

#include "DlgMARPhase1.h"
#include "DlgCreateTaskWithUsabletime.h"
#include "DlgMAFPhase1.h"
#include "DlgMAFTermFlash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// extern CArray<CTask,CTask&> g_taskArray;

CDlgMAFRequest::CDlgMAFRequest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFRequest::IDD, pParent)
	, m_strSic(_T(""))
	, m_strSSC(_T(""))
{
	m_ctimeStartDate = 0;
	m_ctimeEndDate = 0;
	m_ctimeStartTime = 0;
	m_ctimeEndTime = 0;
	m_strSic = _T("001");

	m_pUsableTimeDlg = NULL;

}

CDlgMAFRequest::~CDlgMAFRequest()
{
	if (NULL != m_pUsableTimeDlg)
	{
		if (NULL != m_pUsableTimeDlg->GetSafeHwnd())
		{
			m_pUsableTimeDlg->DestroyWindow();
		}
		delete m_pUsableTimeDlg;
		m_pUsableTimeDlg = NULL;
	}
}

void CDlgMAFRequest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TASK_STATUS, m_ctlTaskStatus);
	DDX_Control(pDX, IDC_COMBO_TDRS_QUERY, m_ctlTdrs);
	DDX_Control(pDX, IDC_COM_TASK_TYPE, m_comboTaskType);

	DDX_Control(pDX, IDC_TIMEPICKER_START, m_ctlStartTime);
	DDX_Control(pDX, IDC_TIMEPICKER_END, m_ctlEndTime);
	DDX_Control(pDX, IDC_DATEPICKER_START, m_ctlStartDate);
	DDX_Control(pDX, IDC_DATEPICKER_END, m_ctlEndDate);
	DDX_Control(pDX, IDC_LIST_TASK, m_ctlTaskList);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_START, m_ctimeStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_END, m_ctimeEndDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_START, m_ctimeStartTime);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_END, m_ctimeEndTime);
	DDX_Control(pDX, IDC_LIST_TDRSS_TUT, m_listTUT);
	DDX_Control(pDX, IDC_LIST_EMPTY_WIN, m_listUPVisibletable);

	DDX_Control(pDX, IDC_EDIT_MAF_TASK_SSC, m_editSSC);
	DDX_Control(pDX, IDC_COMBO_QUERY_SIC, m_comboSIC);
	DDX_CBString(pDX, IDC_COMBO_QUERY_SIC, m_strSic);
	DDX_Text(pDX, IDC_EDIT_MAF_TASK_SSC, m_strSSC);
}


BEGIN_MESSAGE_MAP(CDlgMAFRequest, CDialog)
	ON_BN_CLICKED(ID_QUERY_TASK, OnClickServicePlaningRequest)
	//ON_BN_CLICKED(ID_TASK_SIMULATION, OnSimulate)
	ON_BN_CLICKED(IDC_BTN_MAF_CREATE_UP_TIME, OnBnClickedBtnMafCreateUpTime)
END_MESSAGE_MAP()

BOOL CDlgMAFRequest::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	m_ctlTaskStatus.SetCurSel(0);

	DWORD dwStyle = m_listUPVisibletable.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listUPVisibletable.SetExtendedStyle(dwStyle);

	DWORD dwSty = m_listTUT.GetExtendedStyle();
	dwSty |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwSty |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listTUT.SetExtendedStyle(dwSty);

	
	DWORD dwStyle2 = m_ctlTaskList.GetExtendedStyle();
	dwStyle2 |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle2 |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_ctlTaskList.SetExtendedStyle(dwStyle2);
	
	m_ctlStartDate.SetFormat(_T("yyyy-MM-dd"));
	m_ctlEndDate.SetFormat(_T("yyyy-MM-dd"));
	
	m_ctimeStartDate = CTime::GetCurrentTime();
	m_ctimeEndDate = CTime::GetCurrentTime();
	m_ctimeStartTime = CTime::GetCurrentTime();
	m_ctimeEndTime = CTime::GetCurrentTime();
	
	UpdateData(0);

	m_comboTaskType.InsertString(0, _T("ǰ��"));
	//m_comboTaskType.InsertString(1, _T("����"));
	m_comboTaskType.SetCurSel(0);

	m_ctlTdrs.InsertString(0, _T("1"));
	m_ctlTdrs.SetCurSel(0);

	m_comboSIC.InsertString(0, _T("001"));
	//m_comboSIC.InsertString(1, _T("002"));
	//m_comboSIC.InsertString(2, _T("003"));
	m_comboSIC.SetCurSel(0);
	
	int index = 0;
	m_ctlTaskList.InsertColumn(index++, _T("��������"), LVCFMT_RIGHT, 67);
	m_ctlTaskList.InsertColumn(index++, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_ctlTaskList.InsertColumn(index++, _T("�û�SIC"), LVCFMT_LEFT, 65);
	m_ctlTaskList.InsertColumn(index++, _T("����SSC"), LVCFMT_LEFT, 65);
	m_ctlTaskList.InsertColumn(index++, _T("����"), LVCFMT_LEFT, 50);
	m_ctlTaskList.InsertColumn(index++, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_ctlTaskList.InsertColumn(index++, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_ctlTaskList.InsertColumn(index++, _T("����ʱ��"), LVCFMT_LEFT, 67);
	m_ctlTaskList.InsertColumn(index++, _T("����״̬"), LVCFMT_LEFT, 67);
	m_ctlTaskList.InsertColumn(index++, _T("����ͨ��"), LVCFMT_LEFT, 67);
	
	m_listTUT.InsertColumn(0, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listTUT.InsertColumn(1, _T("SIC"), LVCFMT_RIGHT, 40);
	m_listTUT.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listTUT.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listTUT.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 70);

	m_listUPVisibletable.InsertColumn(0, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listUPVisibletable.InsertColumn(1, _T("SIC"), LVCFMT_RIGHT, 40);
	m_listUPVisibletable.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listUPVisibletable.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listUPVisibletable.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 70);

	//�������б�ӽ�����
	InitTaskListctrl();


	return TRUE; 
}

// Button��������滮����
void CDlgMAFRequest::OnClickServicePlaningRequest() 
{
	//1.����ʾMAF phase 1�Ի���
	UpdateData(TRUE);

	if ( m_strSSC.IsEmpty() )
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
		pView->StartDrawPhase1RequestSignal(TRUE);
	}	
}


void CDlgMAFRequest::ShowQueryDlg()
{
	//
	ShowWindow(SW_RESTORE);

	//2.չʾ��Ϻ󣬲ſ�ʼ�������ݵ�listctrl��
	BOOL bSuccess  = InitTaskListctrl() && 
					 InitTutListCtrl() &&
					 InitVisibleTimeListCtrl();

	if (bSuccess)
	{

	}
	else
	{
		AfxMessageBox(_T("��ʼ������ʧ�ܣ�"));
		return;
	}
}

BOOL CDlgMAFRequest::GetNewStartEndDataTime(CTime & newStart, CTime & newEnd)
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

//����listctrl TUT+Visable��+task��
void CDlgMAFRequest::UdpateListctrl()
{
	InitTaskListctrl();
	InitTutListCtrl();
	InitVisibleTimeListCtrl();
}

void CDlgMAFRequest::UpdateTaskAndVisabletime()
{
	InitTaskListctrl();
	InitVisibleTimeListCtrl();
}

BOOL CDlgMAFRequest::InitTaskListctrl()
{
	
	UpdateData(TRUE);
	//if(m_ctimeStartDate >= m_ctimeEndDate)
	CTime newStartTime;
	CTime newEndTime;
	GetNewStartEndDataTime(newStartTime, newEndTime);
	
	/*if (newStartTime >= newEndTime)
	{
		AfxMessageBox(_T("����'����ʱ��'���ڻ����'��ʼʱ��'!"));
		return FALSE;
	}*/

	CBussinessData* pData = CBussinessData::Instance();

	ETaskType nTaskType = (ETaskType)(m_comboTaskType.GetCurSel()+1);

	int nStatus = m_ctlTaskStatus.GetCurSel();
	CString strTDRS;
	m_ctlTdrs.GetWindowText(strTDRS);
	strTDRS.MakeLower();

	m_ctlTaskList.DeleteAllItems();

	BOOL bNoTask = TRUE;
	int nIndex = 0;
	::EnterCriticalSection(&(pData->m_csAllTasks));
	for (ListTaskTable::iterator it = pData->m_listAllTasks.begin();
		it != pData->m_listAllTasks.end(); ++it)
	{
		CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
		CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);

		if (nStatus == 0)
		{
			CString strTD = it->strTDRS;
			strTD.MakeLower();
			if ( nTaskType == it->eTaskType
				&& strTDRS == strTD
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
				else if (it->eTaskType == enmTaskType_MAR)
				{
					strTaskType = _T("����");
				}
				else if (it->eTaskType == enmTaskType_MAFMAR)
				{
					strTaskType = _T("˫��");
				}


				m_ctlTaskList.InsertItem(nIndex, strTaskType);
				m_ctlTaskList.SetItemText(nIndex,1,it->strTDRS);
				m_ctlTaskList.SetItemText(nIndex,2,it->strSIC);
				m_ctlTaskList.SetItemText(nIndex,3,it->strSSC);
				m_ctlTaskList.SetItemText(nIndex,4,strLevel);
				m_ctlTaskList.SetItemText(nIndex,5,it->strStartTime);
				m_ctlTaskList.SetItemText(nIndex,6,it->strEndTime);
				m_ctlTaskList.SetItemText(nIndex,7,it->strLastTime);
				m_ctlTaskList.SetItemText(nIndex,8,strStats);
				m_ctlTaskList.SetItemText(nIndex,9,strChannel);

				nIndex++;
			}

		}
		else
		{
			CString strTD = it->strTDRS;
			strTD.MakeLower();
			if ( nTaskType == it->eTaskType
				&& (int)it->uStatus == nStatus 
				&& it->strTDRS == strTD
				&& timeStart>= newStartTime
				&& timeEnd  <= newEndTime)
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
				else if (it->eTaskType == enmTaskType_MAR)
				{
					strTaskType = _T("����");
				}
				else if (it->eTaskType == enmTaskType_MAFMAR)
				{
					strTaskType = _T("˫��");
				}


				m_ctlTaskList.InsertItem(nIndex, strTaskType);
				m_ctlTaskList.SetItemText(nIndex,1,it->strTDRS);
				m_ctlTaskList.SetItemText(nIndex,2,it->strSIC);
				m_ctlTaskList.SetItemText(nIndex,3,it->strSSC);
				m_ctlTaskList.SetItemText(nIndex,4,strLevel);
				m_ctlTaskList.SetItemText(nIndex,5,it->strStartTime);
				m_ctlTaskList.SetItemText(nIndex,6,it->strEndTime);
				m_ctlTaskList.SetItemText(nIndex,7,it->strLastTime);
				m_ctlTaskList.SetItemText(nIndex,8,strStats);
				m_ctlTaskList.SetItemText(nIndex,9,strChannel);

				nIndex++;
			}

		}

	}
	::LeaveCriticalSection(&(pData->m_csAllTasks));

	if (bNoTask)
	{
		//AfxMessageBox(_T("��ǰû����������������"));
	}

	return TRUE;

}

BOOL CDlgMAFRequest::InitTutListCtrl()
{
	//////////////////////////////////////////////////////////////////////////
	//2013��4��11��22:07:18
	UpdateData(TRUE);
	CTime newStartDateTime;
	CTime newEndDataTime;
	GetNewStartEndDataTime(newStartDateTime, newEndDataTime);

	//1.����tutʱ��Ρ�
	CBussinessData* pData = CBussinessData::Instance();
	
	//2.�����ݲ��뵽ListCtrl
	CString strTDRS;
	m_ctlTdrs.GetWindowText(strTDRS);
	int nTDRS = _tcstoul(strTDRS.GetBuffer(), NULL, 10);
	int nSIC  = _tcstoul(m_strSic.GetBuffer(), NULL, 10);

	// tut ��
	BOOL bNotTut = TRUE;
	m_listTUT.DeleteAllItems();
	int nIndex = 0;
	for (ListTimeTable::iterator it = pData->m_listTUT.begin();
		it != pData->m_listTUT.end(); ++it)
	{
		CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
		CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);
		int nTD = _tcstoul(it->strTDRS.GetBuffer(), NULL, 10);
		int nSI = _tcstoul(it->strSIC.GetBuffer(), NULL, 10);

		if (nTD == nTDRS
			&& nSI == nSIC
			&& timeStart>= newStartDateTime
			&& timeEnd <= newEndDataTime)
		{
			bNotTut = FALSE;
			// Insert TuT
			m_listTUT.InsertItem(nIndex, it->strTDRS);
			m_listTUT.SetItemText(nIndex, 1, it->strSIC);
			m_listTUT.SetItemText(nIndex, 2, it->strStartTime);
			m_listTUT.SetItemText(nIndex, 3, it->strEndTime);
			m_listTUT.SetItemText(nIndex, 4, it->strLastTime);

			nIndex++;
		}

	}

	if (bNotTut)
	{
		AfxMessageBox(_T("��ǰû�������ѯ������TUT��"));
	}

	return TRUE;
}

BOOL CDlgMAFRequest::InitVisibleTimeListCtrl()
{
	//////////////////////////////////////////////////////////////////////////
	//2013��4��11��22:08:53 ����stk����
	UpdateData(TRUE);
	CBussinessData* pData = CBussinessData::Instance();

	CString strTDRS;
	m_ctlTdrs.GetWindowText(strTDRS);
	int nTDRS = _tcstoul(strTDRS.GetBuffer(), NULL, 10);
	int nSIC  = _tcstoul(m_strSic.GetBuffer(), NULL, 10);

	CTime newStartDateTime;
	CTime newEndDateTime;
	GetNewStartEndDataTime(newStartDateTime, newEndDateTime);

	//canceled-2013��12��15��23:38:22
	//map<CString, CString> mapVisTimeTable;
	//CBussinessData::Instance()->SimulateVisableTime(newStartDateTime, newEndDateTime, mapVisTimeTable);


	//1.����csv�ɼ�ʱ���
	//pData->ImportUSAndTDRSVisibleTimeTable();


	//2.ʱ�����뵽�ռ�ListCtrl��
	BOOL bNoUpTable = TRUE;
	m_listUPVisibletable.DeleteAllItems();
	int nIndx=0;
	::EnterCriticalSection(&(pData->m_csVisableTime));
	for (ListTimeTable::iterator itor = pData->m_listVisbleTime.begin();
		itor != pData->m_listVisbleTime.end(); ++itor)
	{
		CTime timeStart = MyTools::GetCTimeFromStringEx(itor->strStartTime);
		CTime timeEnd = MyTools::GetCTimeFromStringEx(itor->strEndTime);
		int nTD = _tcstoul(itor->strTDRS.GetBuffer(), NULL, 10);
		int nSI = _tcstoul(itor->strSIC.GetBuffer(), NULL, 10);

		if (nTD == nTDRS
			&& nSI == nSIC
			&& timeStart >= newStartDateTime
			&& timeEnd <= newEndDateTime)
		{
			bNoUpTable = FALSE;
			// Insert usable table control.
			m_listUPVisibletable.InsertItem(nIndx, itor->strTDRS);
			m_listUPVisibletable.SetItemText(nIndx,1, itor->strSIC);
			m_listUPVisibletable.SetItemText(nIndx,2, itor->strStartTime);
			m_listUPVisibletable.SetItemText(nIndx,3, itor->strEndTime);
			m_listUPVisibletable.SetItemText(nIndx,4, itor->strLastTime);

			nIndx++;
		}	
	}
	::LeaveCriticalSection(&(pData->m_csVisableTime));

	if (bNoUpTable)
	{
		AfxMessageBox(_T("��ǰû�������ѯ������US-TRDS�ɼ�ʱ��Σ�"));
	}

	return TRUE;
}

CString CDlgMAFRequest::ChangeSpanTimeFormat(CString strSpanTime)
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

//Button:������UP����ʱ�䡱
//�����������ʱ��ν��н������㣬�ó�UP����ʱ��Ρ�������һ��dialog��չ�ָ��û�ȥ����
//2013��4��8��21:23:16
void CDlgMAFRequest::OnBnClickedBtnMafCreateUpTime()
{
	//1.���ɿɼ�ʱ�䴰��ȡ�������㡣
	CBussinessData::Instance()->CalculateUsableTimeTable();

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
	m_pUsableTimeDlg->m_strSsc = m_strSSC;
	m_pUsableTimeDlg->SetServiceType(enmSrvType_MAF);
	m_pUsableTimeDlg->SetFilterTime(timeBegin, timeEnd);
	m_pUsableTimeDlg->Create(CDlgCreateTaskWithUsabletime::IDD, this);
	m_pUsableTimeDlg->ShowWindow(SW_SHOW);
}




