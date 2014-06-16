// DlgUnexpectedReq.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgUnexpectedReq.h"
#include "BussinessData.h"
#include "MyTools.h"
#include "DlgUnMar2DShow.h"


// CDlgUnexpectedReq dialog

IMPLEMENT_DYNAMIC(CDlgUnexpectedReq, CDialog)

CDlgUnexpectedReq::CDlgUnexpectedReq(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUnexpectedReq::IDD, pParent)
{
	m_pUnMarShow = NULL;
}

CDlgUnexpectedReq::~CDlgUnexpectedReq()
{
}

void CDlgUnexpectedReq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNEXPECTED_MAR_TASK, m_listctrlUnexpentedTask);
	DDX_Control(pDX, IDC_BTN_DENY_UNEXPECT_MAR, m_btnDeny);
	DDX_Control(pDX, IDC_BTN_ACCEPT_UNEXPECTED_MAR, m_btnAccept);
}


BEGIN_MESSAGE_MAP(CDlgUnexpectedReq, CDialog)
	ON_BN_CLICKED(IDC_BTN_DENY_UNEXPECT_MAR, &CDlgUnexpectedReq::OnBnClickedDeny)
	ON_BN_CLICKED(IDC_BTN_ACCEPT_UNEXPECTED_MAR, &CDlgUnexpectedReq::OnBnClickedAccept)
END_MESSAGE_MAP()


BOOL CDlgUnexpectedReq::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);

	DWORD dwStyle = m_listctrlUnexpentedTask.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        // 选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;            // 网格线（只适用与report风格的listctrl）
	m_listctrlUnexpentedTask.SetExtendedStyle(dwStyle);

	// TODO: Add extra initialization here
	/*m_listctrlUnexpentedTask.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlUnexpentedTask.InsertColumn(2, _T("SIC"), LVCFMT_LEFT, 50);
	m_listctrlUnexpentedTask.InsertColumn(3, _T("开始时间"), LVCFMT_LEFT, 150);
	m_listctrlUnexpentedTask.InsertColumn(4, _T("结束时间"), LVCFMT_LEFT, 150);
	m_listctrlUnexpentedTask.InsertColumn(5, _T("持续时间"), LVCFMT_LEFT, 150);*/
	
	m_listctrlUnexpentedTask.InsertColumn(0, _T("任务类型"), LVCFMT_RIGHT, 67);
	m_listctrlUnexpentedTask.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlUnexpentedTask.InsertColumn(2, _T("用户SIC"), LVCFMT_LEFT, 65);
	m_listctrlUnexpentedTask.InsertColumn(3, _T("服务SSC"), LVCFMT_LEFT, 65);
	m_listctrlUnexpentedTask.InsertColumn(4, _T("级别"), LVCFMT_LEFT, 50);
	m_listctrlUnexpentedTask.InsertColumn(5, _T("开始时间"), LVCFMT_LEFT, 120);
	m_listctrlUnexpentedTask.InsertColumn(6, _T("结束时间"), LVCFMT_LEFT, 120);
	m_listctrlUnexpentedTask.InsertColumn(7, _T("持续时间"), LVCFMT_LEFT, 67);
	m_listctrlUnexpentedTask.InsertColumn(8, _T("任务状态"), LVCFMT_LEFT, 67);
	m_listctrlUnexpentedTask.InsertColumn(9, _T("服务通道"), LVCFMT_LEFT, 67);



	InitstkDatawithToday();
	return TRUE;
}

void CDlgUnexpectedReq::InitstkDatawithToday()
{
	UpdateData(TRUE);


	//stk数据已经在点击“突发反向模式”的时候就已经计算好了。
	//这个地方只随即取第一条作为突发任务的时间段，且要灰化
	CBussinessData* pData = CBussinessData::Instance();
	m_listctrlUnexpentedTask.DeleteAllItems();

	::EnterCriticalSection(&(pData->m_csVisableTime));
	for(ListTimeTable::reverse_iterator rit = pData->m_listVisbleTime.rbegin();
		rit != pData->m_listVisbleTime.rend(); ++rit)
	{
		if (rit->bUnexp && rit->bPlayed && !rit->bOuccupied)
		{
			CTime timeStart = MyTools::GetCTimeFromStringEx(rit->strStartTime);
			CTime timeEnd = MyTools::GetCTimeFromStringEx(rit->strEndTime);
			int nTD = _tcstoul(rit->strTDRS.GetBuffer(), NULL, 10);
			int nSI = _tcstoul(rit->strSIC.GetBuffer(), NULL, 10);

			CString strNewSpanTime = rit->strLastTime;

			m_listctrlUnexpentedTask.InsertItem(0, _T("突发返向模式"));
			m_listctrlUnexpentedTask.SetItemText(0,1, rit->strTDRS);
			m_listctrlUnexpentedTask.SetItemText(0,2, rit->strSIC);
			m_listctrlUnexpentedTask.SetItemText(0,3, _T("12345"));
			m_listctrlUnexpentedTask.SetItemText(0,4, _T("1"));
			m_listctrlUnexpentedTask.SetItemText(0,5, rit->strStartTime);
			m_listctrlUnexpentedTask.SetItemText(0,6, rit->strEndTime);
			m_listctrlUnexpentedTask.SetItemText(0,7, rit->strLastTime);
			m_listctrlUnexpentedTask.SetItemText(0,8, _T("未执行"));
			m_listctrlUnexpentedTask.SetItemText(0,9, _T("6"));

			break;
		}
	}
	
	::LeaveCriticalSection(&(pData->m_csVisableTime));
}

void CDlgUnexpectedReq::CreateUnexpectedTask()
{
	UpdateData(TRUE);

	CBussinessData* pData = CBussinessData::Instance();
	::EnterCriticalSection(&(pData->m_csVisableTime));

	int nSize = pData->m_listVisbleTime.size();
	for (ListTimeTable::reverse_iterator rit = pData->m_listVisbleTime.rbegin();
		rit != pData->m_listVisbleTime.rend(); ++rit)
	{
		if (rit->bUnexp && rit->bPlayed && !rit->bOuccupied)
		{
			rit->bOuccupied = TRUE;

			//MARTask marItem;
			TaskItem marItem;
			marItem.strSIC = rit->strSIC;
			marItem.strTDRS = rit->strTDRS;
			marItem.strSSC  = _T("12345");
			marItem.strStartTime = rit->strStartTime;
			marItem.strEndTime = rit->strEndTime;
			marItem.strLastTime = rit->strLastTime;

			marItem.uLevel = (ETaskLevel)1;
			marItem.uStatus = enmTaskStatus_UnProcess;

			int nChannel = 6;
			marItem.nChannelID = nChannel;
			marItem.eTaskType = enmTaskType_MAR;

			CBussinessData::Instance()->AddDASTaskItem(marItem);
			break;
		}
		
	}
	
	nSize = pData->m_listVisbleTime.size();

	::LeaveCriticalSection(&(pData->m_csVisableTime));

}
UINT CDlgUnexpectedReq::GetSpanDayTimeInSenconds(CString strStartDay, CString strEndDay)
{
	int indexStart = strStartDay.ReverseFind('-');
	int indexEnd   = strEndDay.ReverseFind('-');
	CString str1 = strStartDay.Right(strStartDay.GetLength()-indexStart-1);
	CString str2 = strEndDay.Right(strEndDay.GetLength()-indexEnd-1);
	UINT dStart = _tcstoul(str1.GetBuffer(),NULL,10);
	UINT dEnd = _tcstoul(str2.GetBuffer(), NULL, 10);

	//转换出错了
	if (dStart>dEnd || dStart == 0 || dEnd == 0 || dStart>31)
	{
		return 0;
	}

	return (dEnd-dStart)*24*3600;
}


//如果点击拒绝，就结束整个突发返向传输过程。
void CDlgUnexpectedReq::OnBnClickedDeny()
{
	CDialog::OnOK();
}

//如果接受，当接受了，就开始回复3D信号给UP，再接下来就进行常规的DAR数据传输。
void CDlgUnexpectedReq::OnBnClickedAccept()
{
	// TODO: Add your control notification handler code here
	
	if (m_pUnMarShow != NULL)
	{
		m_pUnMarShow->DestroyWindow();
		delete m_pUnMarShow;
		m_pUnMarShow = NULL;
	}

	m_pUnMarShow = new CDlgUnMar2DShow;
	m_pUnMarShow->Create(CDlgUnMar2DShow::IDD, this);
	m_pUnMarShow->ShowWindow(SW_SHOW);

	//隐藏自己，因为在接下来的3D回复Answer阶段，不应该出现该对话框。
	ShowWindow(SW_HIDE);
	
}
