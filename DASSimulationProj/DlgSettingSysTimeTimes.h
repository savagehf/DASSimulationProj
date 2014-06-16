#pragma once


// CDlgSettingSysTimeTimes dialog

class CDlgSettingSysTimeTimes : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingSysTimeTimes)

public:
	CDlgSettingSysTimeTimes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingSysTimeTimes();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_TIME_TIMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	CComboBox m_comboTimeVel;

	DECLARE_MESSAGE_MAP()

};
