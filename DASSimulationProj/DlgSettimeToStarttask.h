#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CDlgSettimeToStarttask : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettimeToStarttask)

public:
	CDlgSettimeToStarttask(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettimeToStarttask();

// Dialog Data
	enum { IDD = IDD_DLG_SETTIME_TO_START_TASK };

	
	void InitTaskListctrl();
	void HighLightTaskItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnClickListctrl(NMHDR* pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:

	CDateTimeCtrl	m_ctlStartTime;
	CDateTimeCtrl	m_ctlStartDate;
	CTime	m_ctimeStartDate;
	CTime	m_ctimeStartTime;


	CListCtrl m_listctrlAllTasks;
	CEdit m_editTaskStatus;
};
