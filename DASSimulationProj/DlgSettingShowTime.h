#pragma once


class CDlgSettingShowTime : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingShowTime)

public:
	CDlgSettingShowTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingShowTime();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_VIEW_TIME };

	void UpdateControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:

	CComboBox m_comboTimeVel;
	CDateTimeCtrl	m_ctlStartTime;
	CDateTimeCtrl	m_ctlStartDate;
	CTime	m_ctimeStartDate;
	CTime	m_ctimeStartTime;

public:
	afx_msg void OnBnClickedBtnSetShowTime();
	afx_msg void OnBnClickedBtnSettingSimulateTimeDas();
	afx_msg void OnBnClickedBtnSettingTimeFrequencyDas();
};
