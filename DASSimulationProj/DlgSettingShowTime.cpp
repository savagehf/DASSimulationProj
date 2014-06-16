// DlgSettingShowTime.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgSettingShowTime.h"
#include "DASRender.h"
#include "MyTools.h"


IMPLEMENT_DYNAMIC(CDlgSettingShowTime, CDialog)

CDlgSettingShowTime::CDlgSettingShowTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingShowTime::IDD, pParent)
{
}

CDlgSettingShowTime::~CDlgSettingShowTime()
{
}

void CDlgSettingShowTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_DATEPICKER_START_SHOWTIME, m_ctlStartDate);
	DDX_Control(pDX, IDC_TIMEPICKER_START_SHOWTIME, m_ctlStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_START_SHOWTIME, m_ctimeStartDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_START_SHOWTIME, m_ctimeStartTime);
	DDX_Control(pDX, IDC_COMBO_SETTING_TIME_VEL, m_comboTimeVel);
}


BEGIN_MESSAGE_MAP(CDlgSettingShowTime, CDialog)

	ON_BN_CLICKED(IDC_BTN_SET_SHOW_TIME, &CDlgSettingShowTime::OnBnClickedBtnSetShowTime)
	ON_BN_CLICKED(IDC_BTN_SETTING_SIMULATE_TIME_DAS, &CDlgSettingShowTime::OnBnClickedBtnSettingSimulateTimeDas)
	ON_BN_CLICKED(IDC_BTN_SETTING_TIME_FREQUENCY_DAS, &CDlgSettingShowTime::OnBnClickedBtnSettingTimeFrequencyDas)
END_MESSAGE_MAP()


BOOL CDlgSettingShowTime::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_comboTimeVel.InsertString(0, _T("1"));
	m_comboTimeVel.InsertString(1, _T("2"));
	m_comboTimeVel.InsertString(2, _T("3"));
	m_comboTimeVel.InsertString(3, _T("4"));
	m_comboTimeVel.InsertString(4, _T("5"));
	m_comboTimeVel.InsertString(5, _T("6"));
	m_comboTimeVel.InsertString(6, _T("10"));
	m_comboTimeVel.InsertString(7, _T("20"));
	m_comboTimeVel.InsertString(8, _T("30"));
	m_comboTimeVel.InsertString(9, _T("60"));
	m_comboTimeVel.SetCurSel(0);

	m_ctimeStartDate = CDASRender::Instance()->GetShowTime();//CTime::GetCurrentTime();
	m_ctimeStartTime = CDASRender::Instance()->GetShowTime();//CTime::GetCurrentTime();

	UpdateData(FALSE);

	return TRUE;

}
// CDlgSettingShowTime message handlers

void CDlgSettingShowTime::OnBnClickedBtnSetShowTime()
{
	UpdateData(TRUE);
	
	CString strTimeVel;
	m_comboTimeVel.GetWindowText(strTimeVel);
	UINT nTimeVel = _tcstoul(strTimeVel.GetBuffer(), NULL, 10);
	CDASRender::Instance()->SetSysTimeFrequency(nTimeVel);


	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	CTime timeStart = MyTools::GetCTimeFromStringEx(strNewStartDateTime);

	CDASRender::Instance()->SetNewTimeInView(CTime::GetCurrentTime(), timeStart);

	CDialog::OnOK();
}

void CDlgSettingShowTime::UpdateControl()
{
	m_ctimeStartDate = CDASRender::Instance()->GetShowTime();
	m_ctimeStartTime = CDASRender::Instance()->GetShowTime();

	UpdateData(FALSE);
}

void CDlgSettingShowTime::OnBnClickedBtnSettingSimulateTimeDas()
{
	UpdateData(TRUE);

	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	CTime timeStart = MyTools::GetCTimeFromStringEx(strNewStartDateTime);

	CDASRender::Instance()->SetNewTimeInView(CTime::GetCurrentTime(), timeStart);
	GetDlgItem(IDC_BTN_SETTING_SIMULATE_TIME_DAS)->EnableWindow(FALSE);

	//CDialog::OnOK();
}

void CDlgSettingShowTime::OnBnClickedBtnSettingTimeFrequencyDas()
{
	UpdateData(TRUE);

	CString strTimeVel;
	m_comboTimeVel.GetWindowText(strTimeVel);
	UINT nTimeVel = _tcstoul(strTimeVel.GetBuffer(), NULL, 10);
	CDASRender::Instance()->SetSysTimeFrequency(nTimeVel);

	GetDlgItem(IDC_BTN_SETTING_TIME_FREQUENCY_DAS)->EnableWindow(FALSE);
}
