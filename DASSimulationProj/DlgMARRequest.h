#pragma once

#include "afxcmn.h"

class CDlgCreateTaskWithUsabletime;
class CDlgMARRequest : public CDialog
{
	DECLARE_DYNAMIC(CDlgMARRequest)

public:
	CDlgMARRequest(CWnd* pParent = NULL);  
	virtual ~CDlgMARRequest();

	void ShowMARServiceDlg();
	void UpdateListCtrl();

	enum { IDD = IDD_DLG_MAR_REQUEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	BOOL GetNewStartEndDataTime(CTime & newStart, CTime & newEnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnMarCreateUpTime();
	afx_msg void OnBnClickedBtnMarServicePlanReq();
	

private:
	void InsertStkDataToListctrl();
	void InsertTaskToListctrl();
	CString ChangeSpanTimeFormat(CString strSpanTime);

public:
	//CListCtrl m_listAllMARTasks;
	CListCtrl m_listUserVisableTable;
	CListCtrl m_listMARTask;

	//CButtonST	m_btnShowMarPhase2;
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
	
	int m_nProfRadio;

	CString m_strMARSSC;

	CDlgCreateTaskWithUsabletime* m_pUsableTimeDlg;
};
