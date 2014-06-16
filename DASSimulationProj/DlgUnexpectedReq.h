#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgUnexpectedReq dialog
class CDlgUnMar2DShow;
class CDlgUnexpectedReq : public CDialog
{
	DECLARE_DYNAMIC(CDlgUnexpectedReq)

public:
	CDlgUnexpectedReq(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUnexpectedReq();

// Dialog Data
	enum { IDD = IDD_DLG_UNEXPECTED_MAR_REQ };

	void CreateUnexpectedTask();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedDeny();
	afx_msg void OnBnClickedAccept();

	void InitstkDatawithToday();
	UINT GetSpanDayTimeInSenconds(CString strStartDay, CString strEndDay);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrlUnexpentedTask;
	CButton m_btnDeny;
	CButton m_btnAccept;
	CDlgUnMar2DShow* m_pUnMarShow;
	
};
