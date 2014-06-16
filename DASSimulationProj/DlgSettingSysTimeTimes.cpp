// DlgSettingSysTimeTimes.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgSettingSysTimeTimes.h"
#include "DASRender.h"


// CDlgSettingSysTimeTimes dialog

IMPLEMENT_DYNAMIC(CDlgSettingSysTimeTimes, CDialog)

CDlgSettingSysTimeTimes::CDlgSettingSysTimeTimes(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingSysTimeTimes::IDD, pParent)
{

}

CDlgSettingSysTimeTimes::~CDlgSettingSysTimeTimes()
{
}

void CDlgSettingSysTimeTimes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SETTING_TIME_VEL, m_comboTimeVel);
}

BOOL CDlgSettingSysTimeTimes::OnInitDialog() 
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


	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgSettingSysTimeTimes, CDialog)
END_MESSAGE_MAP()

void CDlgSettingSysTimeTimes::OnOK()
{
	CString strTimeVel;
	m_comboTimeVel.GetWindowText(strTimeVel);
	UINT nTimeVel = _tcstoul(strTimeVel.GetBuffer(), NULL, 10);

	CDASRender::Instance()->SetSysTimeFrequency(nTimeVel);

	CDialog::OnOK();
}
