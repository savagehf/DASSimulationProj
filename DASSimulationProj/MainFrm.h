
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "DASLeftView.h"

#define WM_SHOW_OCCUPIED		WM_USER+1050

#define WM_SHOW_PHASE1_MAF		WM_USER+100
#define WM_SHOW_PHASE1_MAR		WM_USER+101
#define WM_SHOW_PHASE1_MAFMAR	WM_USER+102

#define WM_RETORE_HIDEN_QUERY_DLG	WM_USER+103
#define WM_RESTORE_HIDE_MAR_DLG		WM_USER+104
#define WM_RESTORE_HIDE_MAFMAR_DLG	WM_USER+105

//第一阶段的3d演示结束后，需要展示一个对话框给用户,
//询问是否进行突发返向模式的请求，"接收"or"拒绝"
#define WM_SHOW_UNEXPECTED_MAR_REQUEST_DIALOG WM_USER+108
#define WM_CREATE_UNEXPECTED_MAR_TASK		  WM_USER+109

#define WM_SHOW_PHASE3_DIALOG	WM_USER+1003
#define WM_SHOW_PHASE6_DIALOG	WM_USER+1006


#define WM_VISIBLE_TIME_BEGIN   WM_USER+1018
#define WM_TASK_PREPARE			WM_USER+1019
#define WM_TASK_BEGIN			WM_USER+1020
#define WM_TASK_END				WM_USER+1021

#define WM_SHOW_HIDE_LEFTVIEW	WM_USER+1030

#define WM_UPDATE_MAF_LISTCTRL	WM_USER+1040
#define WM_UPDATE_MAR_LISTCTRL	WM_USER+1041
#define WM_UPDATE_MAFMAR_LISTCTRL WM_USER+1042


class CDlgUnexpectedReq;
class CDlgMAFTermFlash;
class CDlgMARTermFlash;
class CDlgMAFMARTermFlash;
class CDlgMAFPhase3;
class CDlgMARPhase3;
class CDlgMAFMARPhase3;
class CDlgMAFPhase6;
class CDlgMARPhase6;
class CDlgMAFMARPhase6;
class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void SetLeftViewerShow(BOOL bShow)
	{
		m_bShowLeftViewer = !bShow;
	}
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CDASLeftView      m_wndFileView;
	//CClassView        m_wndClassView;

	CDlgUnexpectedReq* m_pUnexpectedReqDlg;
	CDlgMAFTermFlash* m_pMAF1FlashDlg;
	CDlgMARTermFlash* m_pMAR1FlashDlg;
	CDlgMAFMARTermFlash* m_pMAFMAR1FlashDlg;

	CDlgMAFPhase3* m_pMAFPhase3Dlg;
	CDlgMARPhase3* m_pMARPhase3Dlg;
	CDlgMAFMARPhase3* m_pMAFMARPhase3Dlg;

	CDlgMARPhase6* m_pmarPhase6;
	CDlgMAFPhase6* m_pMAFPhase6;
	CDlgMAFMARPhase6* m_pMafMarPhase6;

	BOOL m_bShowLeftViewer;

// Generated message map functions

public:
	//show出突发返向模式询问对话框
	afx_msg LRESULT OnShowUnexpectedMarReqDlg(WPARAM w, LPARAM l);
	afx_msg LRESULT OnCreateUnexpectMARTask(WPARAM w, LPARAM l);
	afx_msg LRESULT OnShowPhase1MAF(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnShowPhase1MAR(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnShowPhase1MAFMAR(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUpdateMAFListCtrl(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdateMARListCtrl(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdateMAFMARListCtrl(WPARAM W, LPARAM L);
	afx_msg LRESULT OnShowOccupied(WPARAM w, LPARAM l);
	
	afx_msg LRESULT OnShowQueryDlg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnShowMarFlashDlg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnShowMafMarPhase1(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnShowPhase3Dlg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT ShowPhase6Dlg(WPARAM wPara, LPARAM lPara);

	afx_msg LRESULT BeginVisibleTime(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT PrepareOneTask(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT BeginOneTask(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT EndOneTask(WPARAM wPara, LPARAM lPara);

	afx_msg LRESULT ShowHideLeftViewer(WPARAM wPara, LPARAM lPara);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);



	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


