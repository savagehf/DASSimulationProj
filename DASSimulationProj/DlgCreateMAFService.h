#if !defined(AFX_DLGDASVISREQ_H__0B1B76D0_462B_4107_B0E9_9D0648ED3DD8__INCLUDED_)
#define AFX_DLGDASVISREQ_H__0B1B76D0_462B_4107_B0E9_9D0648ED3DD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "afxcmn.h"
#include "BtnST.h"

class CDlgCreateMAFService : public CDialog
{
public:
	//void UpdateList();
	CDlgCreateMAFService(CWnd* pParent = NULL);  

	enum { IDD = IDD_DLG_DAS_VISIBILITY_REQUEST };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();

	afx_msg void OnProduceUPUsableTime();
	//afx_msg void OnCreateTaskWithUpusableTime();
	//afx_msg void OnBnClickedBtnCreateMafWithtut();

	afx_msg void OnBnClickedBtnUsTdrsVisibleTime();
	
	afx_msg void OnBnClickedMafPhase2Show();

	DECLARE_MESSAGE_MAP()

public:
	CDateTimeCtrl	m_ctlEndTime;
	CDateTimeCtrl	m_ctlStartTime;
	CDateTimeCtrl	m_ctlEndDate;
	CDateTimeCtrl	m_ctlStartDate;
	
	
	CTime	m_ctimeStartTime;
	CTime	m_ctimeEndTime;
	CTime	m_ctimeEndDate;
	CTime	m_ctimeStartDate;
	CString	m_strTdrs;
	

	CButtonST m_BtnShowPhase2;
	

};


#endif 
