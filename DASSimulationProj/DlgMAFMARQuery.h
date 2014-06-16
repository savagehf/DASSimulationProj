#pragma once

class CDlgCreateTaskWithUsabletime;

class CDlgMAFMARQuery : public CDialog
{
	DECLARE_DYNAMIC(CDlgMAFMARQuery)

public:
	CDlgMAFMARQuery(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMAFMARQuery();

// Dialog Data
	enum { IDD = IDD_DLG_MAFMAR_QUERY };

	void ShowMAFMARDlg();
	void UpdateListctrl();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedBtnMafMarServicePlanReq();
	afx_msg void OnBnClickedBtnMafMarCreateUpTime();
	
	DECLARE_MESSAGE_MAP()

protected:
	void InsertStkDataToListctrl();
	void InsertTaskToListctrl();
	BOOL GetNewStartEndDataTime(CTime & newStart, CTime & newEnd);

protected:
	CListCtrl m_listUserVisableTable;
	CListCtrl m_listMARTask;

	CComboBox	m_comboSIC;
	CComboBox	m_ctlTdrs;
	CDateTimeCtrl	m_ctlStartTime;
	CDateTimeCtrl	m_ctlEndTime;
	CDateTimeCtrl	m_ctlStartDate;
	CDateTimeCtrl	m_ctlEndDate;

	CTime	m_ctimeStartDate;
	CTime	m_ctimeEndDate;
	CTime	m_ctimeStartTime;
	CTime	m_ctimeEndTime;

	CString m_strMARSSC;

	CDlgCreateTaskWithUsabletime* m_pUsableTimeDlg;

};
