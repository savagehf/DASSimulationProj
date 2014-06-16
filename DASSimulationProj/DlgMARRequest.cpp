
#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMARRequest.h"
#include "BussinessData.h"
// #include "DlgCreateRequest.h"
#include "MyTools.h"
#include "DlgMARPhase2.h"
#include "DlgCreateTaskWithUsabletime.h"
#include "DlgMARPhase1.h"
#include "MainFrm.h"
#include "DASSimulationView.h"




IMPLEMENT_DYNAMIC(CDlgMARRequest, CDialog)

CDlgMARRequest::CDlgMARRequest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMARRequest::IDD, pParent)
	, m_nProfRadio(0)
	, m_strMARSSC(_T(""))
{
	m_pUsableTimeDlg = NULL;
}

CDlgMARRequest::~CDlgMARRequest()
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

void CDlgMARRequest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAR_COMBO_SIC, m_comboSIC);
	DDX_Control(pDX, IDC_MAR_COMBO_TDRS, m_ctlTdrs);

	DDX_Control(pDX, IDC_MAR_TIMEPICKER_START, m_ctlStartTime);
	DDX_Control(pDX, IDC_MAR_TIMEPICKER_END, m_ctlEndTime);
	DDX_Control(pDX, IDC_MAR_DATEPICKER_START, m_ctlStartDate);
	DDX_Control(pDX, IDC_MAR_DATEPICKER_END, m_ctlEndDate);

	DDX_DateTimeCtrl(pDX, IDC_MAR_DATEPICKER_START, m_ctimeStartDate);
	DDX_DateTimeCtrl(pDX, IDC_MAR_DATEPICKER_END, m_ctimeEndDate);
	DDX_DateTimeCtrl(pDX, IDC_MAR_TIMEPICKER_START, m_ctimeStartTime);
	DDX_DateTimeCtrl(pDX, IDC_MAR_TIMEPICKER_END, m_ctimeEndTime);

	//DDX_Control(pDX, IDC_LIST_UNUSED_CHANNEL_RES, m_listAllMARTasks);
	DDX_Control(pDX, IDC_LIST_MAR_US_VIS_TABLE, m_listUserVisableTable);
	DDX_Radio(pDX, IDC_RADIO3, m_nProfRadio);
	DDX_Control(pDX, IDC_LIST_MAR_TASK_CTRL, m_listMARTask);
	DDX_Text(pDX, IDC_EDIT_MAR_SSC, m_strMARSSC);
}

BOOL CDlgMARRequest::OnInitDialog()
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
	dwStyle |= LVS_EX_FULLROWSELECT;        // 选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;            // 网格线（只适用与report风格的listctrl）
	m_listUserVisableTable.SetExtendedStyle(dwStyle);

	DWORD dwStyle2 = m_listMARTask.GetExtendedStyle();
	dwStyle2 |= LVS_EX_FULLROWSELECT;        // 选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle2 |= LVS_EX_GRIDLINES;            // 网格线（只适用与report风格的listctrl）
	m_listMARTask.SetExtendedStyle(dwStyle2);

	// TODO: Add extra initialization here
	m_listUserVisableTable.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listUserVisableTable.InsertColumn(2, _T("SIC"), LVCFMT_LEFT, 50);
	m_listUserVisableTable.InsertColumn(3, _T("开始时间"), LVCFMT_LEFT, 150);
	m_listUserVisableTable.InsertColumn(4, _T("结束时间"), LVCFMT_LEFT, 150);
	m_listUserVisableTable.InsertColumn(5, _T("持续时间"), LVCFMT_LEFT, 150);

	int index = 0;
	m_listMARTask.InsertColumn(index++, _T("任务类型"), LVCFMT_RIGHT, 67);
	m_listMARTask.InsertColumn(index++, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listMARTask.InsertColumn(index++, _T("用户SIC"), LVCFMT_LEFT, 65);
	m_listMARTask.InsertColumn(index++, _T("服务SSC"), LVCFMT_LEFT, 65);
	m_listMARTask.InsertColumn(index++, _T("级别"), LVCFMT_LEFT, 50);
	m_listMARTask.InsertColumn(index++, _T("开始时间"), LVCFMT_LEFT, 120);
	m_listMARTask.InsertColumn(index++, _T("结束时间"), LVCFMT_LEFT, 120);
	m_listMARTask.InsertColumn(index++, _T("持续时间"), LVCFMT_LEFT, 67);
	m_listMARTask.InsertColumn(index++, _T("任务状态"), LVCFMT_LEFT, 67);
	m_listMARTask.InsertColumn(index++, _T("服务通道"), LVCFMT_LEFT, 67);

	/*m_btnShowMarPhase2.SetTooltipText(_T("显示该阶段模拟过程"));
	m_btnShowMarPhase2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,0,0));
	m_btnShowMarPhase2.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
	m_btnShowMarPhase2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255,0,0));
	m_btnShowMarPhase2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));
	m_btnShowMarPhase2.DrawBorder();*/

	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgMARRequest, CDialog)
	ON_BN_CLICKED(IDC_BTN_MAR_CREATE_UP_TIME, &CDlgMARRequest::OnBnClickedBtnMarCreateUpTime)
	ON_BN_CLICKED(IDC_BTN_MAR_SERVICE_PLAN_REQ, &CDlgMARRequest::OnBnClickedBtnMarServicePlanReq)
END_MESSAGE_MAP()

void CDlgMARRequest::UpdateListCtrl()
{
	InsertStkDataToListctrl();
	InsertTaskToListctrl();
}

void CDlgMARRequest::InsertStkDataToListctrl()
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
	//模拟UP 表

	CBussinessData* pData = CBussinessData::Instance();

	//生成可见时间-canceled-2013年12月15日23:37:07
//	map<CString, CString> mapVisTimeTable;
//	pData->SimulateVisableTime(timeNewStartDateTime, timeNewEndDateTime, mapVisTimeTable);
	//加载可见时间
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
	
		CString strNewSpanTime = it->strLastTime;//ChangeSpanTimeFormat(it->strLastTime);

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
		AfxMessageBox(_T("当前没有满足查询条件的US-TDRS可见时间段！"));
	}

}

//生成UP可用时间段，2013年4月8日21:54:50
void CDlgMARRequest::OnBnClickedBtnMarCreateUpTime()
{
	UpdateData(TRUE);

	if (NULL != m_pUsableTimeDlg)
	{
		if (NULL != m_pUsableTimeDlg->GetSafeHwnd())
		{
			m_pUsableTimeDlg->DestroyWindow();
		}
		delete m_pUsableTimeDlg;
		m_pUsableTimeDlg = NULL;
	}

	CTime timeBegin,timeEnd;
	GetNewStartEndDataTime(timeBegin, timeEnd);

	//专用用户
	if (m_nProfRadio == 0)
	{
		m_pUsableTimeDlg = new CDlgCreateTaskWithUsabletime;
		m_pUsableTimeDlg->m_strSsc = m_strMARSSC;
		m_pUsableTimeDlg->SetServiceType(enmSrvType_MAR);
		m_pUsableTimeDlg->SetFilterTime(timeBegin, timeEnd);
		m_pUsableTimeDlg->SetMARUserFlag(TRUE);
		m_pUsableTimeDlg->Create(CDlgCreateTaskWithUsabletime::IDD, this);
		m_pUsableTimeDlg->ShowWindow(SW_SHOW);
	}
	//非专用用户
	else if (m_nProfRadio == 1)
	{
		m_pUsableTimeDlg = new CDlgCreateTaskWithUsabletime;
		m_pUsableTimeDlg->m_strSsc = m_strMARSSC;
		m_pUsableTimeDlg->SetServiceType(enmSrvType_MAR);
		m_pUsableTimeDlg->SetFilterTime(timeBegin, timeEnd);
		m_pUsableTimeDlg->SetMARUserFlag(FALSE);
		m_pUsableTimeDlg->Create(CDlgCreateTaskWithUsabletime::IDD, this);
		m_pUsableTimeDlg->ShowWindow(SW_SHOW);
	}
}


void CDlgMARRequest::OnBnClickedBtnMarServicePlanReq()
{
	UpdateData(TRUE);
	if ( m_strMARSSC.IsEmpty() )
	{
		AfxMessageBox(_T("SSC 不可以为空！"));
		return;
	}

	//2.隐藏自己。
	this->ShowWindow(SW_HIDE);

	//3.3D下绘制信号线:pocc--->地面站
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (NULL != pMain)
	{
		CDASSimulationView* pView = (CDASSimulationView*)(pMain->GetActiveView());
		pView->StartDrawPhase1RequestSignal(FALSE);
	}
	
}

void CDlgMARRequest::ShowMARServiceDlg()
{
	ShowWindow(SW_RESTORE);


	//2.show task an stk time table in listctrl
	InsertStkDataToListctrl();
	InsertTaskToListctrl();
}

BOOL CDlgMARRequest::GetNewStartEndDataTime(CTime & newStart, CTime & newEnd)
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

void CDlgMARRequest::InsertTaskToListctrl()
{
	UpdateData(TRUE);
	//if(m_ctimeStartDate >= m_ctimeEndDate)
	CTime newStartTime;
	CTime newEndTime;
	GetNewStartEndDataTime(newStartTime, newEndTime);

	if (newStartTime >= newEndTime)
	{
		AfxMessageBox(_T("错误：'结束时间'早于或等于'开始时间'!"));
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
					strStats = _T("待执行");
				}
				break;
			case enmTaskStatus_Prcessing:
				{
					strStats = _T("执行中");
				}
				break;
			case enmTaskStatus_Processed:
				{
					strStats = _T("执行完");
				}
				break;
			default:
				break;
			}

			CString strTaskType;
			if (it->eTaskType == enmTaskType_MAF)
			{
				strTaskType = _T("前向");
			}
			else if (it->eTaskType == enmTaskType_MAR)
			{
				strTaskType = _T("返向");
			}
			else if (it->eTaskType == enmTaskType_MAFMAR)
			{
				strTaskType = _T("双向");
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
		//AfxMessageBox(_T("当前没有满足条件的任务！"));
	}
}

CString CDlgMARRequest::ChangeSpanTimeFormat(CString strSpanTime)
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