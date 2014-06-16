#pragma once
#include "afxcmn.h"
// #include "DlgCreateRequest.h"
#include "BusinessDataDefine.h"


class CDlgMAFPhase2;
class CDlgMARPhase2;
class CDlgMAFMARPhase2;
class CDlgSettimeToStarttask;

class CDlgCreateTaskWithUsabletime : public CDialog
{
	DECLARE_DYNAMIC(CDlgCreateTaskWithUsabletime)

public:
	CDlgCreateTaskWithUsabletime(CWnd* pParent = NULL); 
	virtual ~CDlgCreateTaskWithUsabletime();

	void SetServiceType(EServiceType eType);
	void SetMARUserFlag(BOOL bProf);
	void SetFilterTime(CTime timeBegin, CTime timeEnd);
	void LoadUsableTimeTable();
	void CheckIfSuccess();
	void ShowTaskDlg();
	//void UpdateUsableTimeTable();

	enum { IDD = IDD_DLG_CREATE_TASK_WITH_USABLE_TIME  };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();

	afx_msg void OnCreateTask();
	afx_msg void OnClickListctrl(NMHDR* pNotifyStruct, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()

private:
	void MockLowLevelTask();
	CString ChangeSpanTimeFormat(CString strSpanTime);
	UINT GetSpanDayTimeInSenconds(CString strStartDay, CString strEndDay);
	void ApplyTaskRequest();
	void SetControlValue(CString strTDRS, CString strSIC,
		CString strStartTime, CString strEndTime);
public:
	CString	m_editStartTime;
	CString	m_editEndTime;
	CString	m_strTdrs;
	CString	m_strSic;
	CString	m_strSsc;

	CDateTimeCtrl m_serviceStartTime;
	CDateTimeCtrl m_serviceEndTime;
	CDateTimeCtrl m_serviceStartDate;
	CDateTimeCtrl m_serviceEndDate;
	CComboBox m_comboServiceLevel;
	CComboBox m_comboChannelID;

	//EServiceType m_eServiceType;

	CListCtrl m_listctrlUsableTimeTable;
	EServiceType m_eSeviceType;
	BOOL	m_bProfUser;

	int m_nSelectedItem;

	CTime m_timeFilterBegin;
	CTime m_timeFilterEnd;


	CDlgMARPhase2* m_pMARPhase2Dlg;
	CDlgMAFPhase2* m_pMAFPhase2Dlg;
	CDlgMAFMARPhase2* m_pMAFMARPhase2Dlg;

	CDlgSettimeToStarttask* m_pSettingSysTimeDlg;
};
