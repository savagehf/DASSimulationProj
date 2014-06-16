#pragma once

#include "Resource.h"
// CLeftDockWnd dialog

class CDlgMAFRequest;
// class CRegisterDlg;
//class CLoginDlg;
class CDlgMARRequest;
class CDlgDasIntroduction;
class CDlgSettimeToStarttask;
class CDlgSettingShowTime;
class CSysModuleIntroduction;
class CDlgMAFMARQuery;

class CLeftDockWnd : public CDialog
{
	DECLARE_DYNAMIC(CLeftDockWnd)

public:
	CLeftDockWnd(CWnd* pParent = NULL);  
	virtual ~CLeftDockWnd();

	enum { IDD = IDD_DLG_LEFT_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	void  GetMinMaxInfo(MINMAXINFO* lpMMI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	//afx_msg void OnBnClickedRegister();
	//afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedBtnDasTaskQuery();
	afx_msg void OnBnClickedBtnMafForword();
	afx_msg void OnBnClickedBtnMarDeforword();
	afx_msg void OnBnClickedBtnDasIntroduce();
	//afx_msg void OnBnClickedSystemSetting();
	//afx_msg void OnBnClickedBtnDasLogout();

public:
	//CRegisterDlg*			m_pRegisterDlg;
	//CLoginDlg*			m_pLoginDlg;
	CDlgMAFRequest*			m_pQueryTaskDlg;
	CDlgMARRequest*			m_pCreateMARDlg;
	CDlgMAFMARQuery*		m_pMAFMARDlg;

	CDlgDasIntroduction*	m_pDASIntroDlg;
	CDlgSettimeToStarttask*		m_pSettingSysTimeDlg;
	CDlgSettingShowTime*    m_pSettingSystime;
	CSysModuleIntroduction* m_pDASIntroduce;
	

public:
	afx_msg void OnBnClickedBtnSetSysTime();
	afx_msg void OnBnClickedBtnSettingTimes();
	afx_msg void OnBnClickedBtnDasSystem();
	afx_msg void OnBnClickedDafdarMode();
	afx_msg void OnBnClickedUnexpectMode();
};
