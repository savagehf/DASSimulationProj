#if !defined(AFX_DLGDASTASKQUERY_H__3F77ACC1_D22C_4F0A_85F0_D62825A17529__INCLUDED_)
#define AFX_DLGDASTASKQUERY_H__3F77ACC1_D22C_4F0A_85F0_D62825A17529__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "afxwin.h"

class CDlgCreateTaskWithUsabletime;
class CDlgMAFRequest : public CDialog
{

public:
	
	CDlgMAFRequest(CWnd* pParent = NULL);  
	~CDlgMAFRequest();

	enum { IDD = IDD_DLG_MAF_REQUEST };
	

public:
	void ShowQueryDlg();
	void UdpateListctrl();
	void UpdateTaskAndVisabletime();

public:
	CString m_strSic;
	CComboBox m_comboSIC;
	CComboBox	m_ctlTaskStatus;
	CComboBox	m_ctlTdrs;
	CComboBox   m_comboTaskType;
	CEdit		m_editSSC;

	CDateTimeCtrl	m_ctlStartTime;
	CDateTimeCtrl	m_ctlEndTime;
	CDateTimeCtrl	m_ctlStartDate;
	CDateTimeCtrl	m_ctlEndDate;

	CListCtrl	m_ctlTaskList;
	CListCtrl m_listTUT;
	CListCtrl	m_listUPVisibletable;
	CTime	m_ctimeStartDate;
	CTime	m_ctimeEndDate;
	CTime	m_ctimeStartTime;
	CTime	m_ctimeEndTime;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnClickServicePlaningRequest();
	afx_msg void OnBnClickedBtnMafCreateUpTime();

private:
	BOOL InitTaskListctrl();
	BOOL InitTutListCtrl();
	BOOL InitVisibleTimeListCtrl();
	BOOL GetNewStartEndDataTime(CTime & newStart, CTime & newEnd);
	CString ChangeSpanTimeFormat(CString strSpanTime);

	DECLARE_MESSAGE_MAP()
public:
	
	CDlgCreateTaskWithUsabletime* m_pUsableTimeDlg;
	
	
	CString m_strSSC;
};


#endif // !defined(AFX_DLGDASTASKQUERY_H__3F77ACC1_D22C_4F0A_85F0_D62825A17529__INCLUDED_)
