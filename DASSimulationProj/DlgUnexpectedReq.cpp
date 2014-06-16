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
	dwStyle |= LVS_EX_FULLROWSELECT;        // ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;            // �����ߣ�ֻ������report����listctrl��
	m_listctrlUnexpentedTask.SetExtendedStyle(dwStyle);

	// TODO: Add extra initialization here
	/*m_listctrlUnexpentedTask.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlUnexpentedTask.InsertColumn(2, _T("SIC"), LVCFMT_LEFT, 50);
	m_listctrlUnexpentedTask.InsertColumn(3, _T("��ʼʱ��"), LVCFMT_LEFT, 150);
	m_listctrlUnexpentedTask.InsertColumn(4, _T("����ʱ��"), LVCFMT_LEFT, 150);
	m_listctrlUnexpentedTask.InsertColumn(5, _T("����ʱ��"), LVCFMT_LEFT, 150);*/
	
	m_listctrlUnexpentedTask.InsertColumn(0, _T("��������"), LVCFMT_RIGHT, 67);
	m_listctrlUnexpentedTask.InsertColumn(1, _T("TDRS"), LVCFMT_RIGHT, 50);
	m_listctrlUnexpentedTask.InsertColumn(2, _T("�û�SIC"), LVCFMT_LEFT, 65);
	m_listctrlUnexpentedTask.InsertColumn(3, _T("����SSC"), LVCFMT_LEFT, 65);
	m_listctrlUnexpentedTask.InsertColumn(4, _T("����"), LVCFMT_LEFT, 50);
	m_listctrlUnexpentedTask.InsertColumn(5, _T("��ʼʱ��"), LVCFMT_LEFT, 120);
	m_listctrlUnexpentedTask.InsertColumn(6, _T("����ʱ��"), LVCFMT_LEFT, 120);
	m_listctrlUnexpentedTask.InsertColumn(7, _T("����ʱ��"), LVCFMT_LEFT, 67);
	m_listctrlUnexpentedTask.InsertColumn(8, _T("����״̬"), LVCFMT_LEFT, 67);
	m_listctrlUnexpentedTask.InsertColumn(9, _T("����ͨ��"), LVCFMT_LEFT, 67);



	InitstkDatawithToday();
	return TRUE;
}

void CDlgUnexpectedReq::InitstkDatawithToday()
{
	UpdateData(TRUE);


	//stk�����Ѿ��ڵ����ͻ������ģʽ����ʱ����Ѿ�������ˡ�
	//����ط�ֻ�漴ȡ��һ����Ϊͻ�������ʱ��Σ���Ҫ�һ�
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

			m_listctrlUnexpentedTask.InsertItem(0, _T("ͻ������ģʽ"));
			m_listctrlUnexpentedTask.SetItemText(0,1, rit->strTDRS);
			m_listctrlUnexpentedTask.SetItemText(0,2, rit->strSIC);
			m_listctrlUnexpentedTask.SetItemText(0,3, _T("12345"));
			m_listctrlUnexpentedTask.SetItemText(0,4, _T("1"));
			m_listctrlUnexpentedTask.SetItemText(0,5, rit->strStartTime);
			m_listctrlUnexpentedTask.SetItemText(0,6, rit->strEndTime);
			m_listctrlUnexpentedTask.SetItemText(0,7, rit->strLastTime);
			m_listctrlUnexpentedTask.SetItemText(0,8, _T("δִ��"));
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

	//ת��������
	if (dStart>dEnd || dStart == 0 || dEnd == 0 || dStart>31)
	{
		return 0;
	}

	return (dEnd-dStart)*24*3600;
}


//�������ܾ����ͽ�������ͻ����������̡�
void CDlgUnexpectedReq::OnBnClickedDeny()
{
	CDialog::OnOK();
}

//������ܣ��������ˣ��Ϳ�ʼ�ظ�3D�źŸ�UP���ٽ������ͽ��г����DAR���ݴ��䡣
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

	//�����Լ�����Ϊ�ڽ�������3D�ظ�Answer�׶Σ���Ӧ�ó��ָöԻ���
	ShowWindow(SW_HIDE);
	
}
