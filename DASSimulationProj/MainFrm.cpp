
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "MainFrm.h"
#include "DASSimulationView.h"
#include "DASRender.h"
#include "MyTools.h"
#include "BussinessData.h"

#include "DlgUnexpectedReq.h"
#include "DlgMAFTermFlash.h"
#include "DlgMARTermFlash.h"
#include "DlgMAFMARTermFlash.h"
#include "DlgMAFPhase3.h"
#include "DlgMARPhase3.h"
#include "DlgMAFMARPhase3.h"
#include "DlgMAFPhase6.h"
#include "DlgMARPhase6.h"
#include "DlgMAFMARPhase6.h"
#include "LeftDockWnd.h"
#include "DlgMAFRequest.h"
#include "DlgMARRequest.h"
#include "DlgMAFMARQuery.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_MESSAGE(WM_SHOW_UNEXPECTED_MAR_REQUEST_DIALOG, OnShowUnexpectedMarReqDlg)
	ON_MESSAGE(WM_CREATE_UNEXPECTED_MAR_TASK, OnCreateUnexpectMARTask)
	ON_MESSAGE(WM_SHOW_PHASE1_MAF, OnShowPhase1MAF)
	ON_MESSAGE(WM_SHOW_PHASE1_MAFMAR, OnShowPhase1MAFMAR)
	ON_MESSAGE(WM_SHOW_PHASE1_MAR, OnShowPhase1MAR)
	ON_MESSAGE(WM_RETORE_HIDEN_QUERY_DLG, OnShowQueryDlg)
	ON_MESSAGE(WM_RESTORE_HIDE_MAFMAR_DLG, OnShowMafMarPhase1)
	ON_MESSAGE(WM_RESTORE_HIDE_MAR_DLG, OnShowMarFlashDlg)
	ON_MESSAGE(WM_SHOW_PHASE3_DIALOG, OnShowPhase3Dlg)
	ON_MESSAGE(WM_SHOW_PHASE6_DIALOG, ShowPhase6Dlg)
	ON_MESSAGE(WM_VISIBLE_TIME_BEGIN, BeginVisibleTime)
	ON_MESSAGE(WM_TASK_PREPARE, PrepareOneTask)
	ON_MESSAGE(WM_TASK_BEGIN, BeginOneTask)
	ON_MESSAGE(WM_TASK_END, EndOneTask)
	ON_MESSAGE(WM_SHOW_HIDE_LEFTVIEW,ShowHideLeftViewer)
	ON_MESSAGE(WM_UPDATE_MAF_LISTCTRL, OnUpdateMAFListCtrl)
	ON_MESSAGE(WM_UPDATE_MAR_LISTCTRL, OnUpdateMARListCtrl)
	ON_MESSAGE(WM_UPDATE_MAFMAR_LISTCTRL, OnUpdateMAFMARListCtrl)
	ON_MESSAGE(WM_SHOW_OCCUPIED, OnShowOccupied)

	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);

	m_pUnexpectedReqDlg = NULL;
	m_pMAF1FlashDlg = NULL;
	m_pMAFMAR1FlashDlg = NULL;
	m_pMAR1FlashDlg = NULL;
	m_pMAFPhase3Dlg = NULL;
	m_pMARPhase3Dlg = NULL;
	m_pMAFMARPhase3Dlg = NULL;
	m_pmarPhase6 = NULL;
	m_pMAFPhase6 = NULL;
	m_pMafMarPhase6 = NULL;
	m_bShowLeftViewer = TRUE;

}

CMainFrame::~CMainFrame()
{
	if (NULL != m_pUnexpectedReqDlg && NULL != m_pUnexpectedReqDlg->GetSafeHwnd())
	{
		m_pUnexpectedReqDlg->DestroyWindow();
		delete m_pUnexpectedReqDlg;
		m_pUnexpectedReqDlg = NULL;
	}

	if (NULL != m_pMAF1FlashDlg && NULL != m_pMAF1FlashDlg->GetSafeHwnd())
	{
		m_pMAF1FlashDlg->DestroyWindow();
		delete m_pMAF1FlashDlg;
		m_pMAF1FlashDlg = NULL;
	}
	if (NULL != m_pMAFMAR1FlashDlg && NULL != m_pMAFMAR1FlashDlg->GetSafeHwnd())
	{
		m_pMAFMAR1FlashDlg->DestroyWindow();
		delete m_pMAFMAR1FlashDlg;
		m_pMAFMAR1FlashDlg = NULL;
	}

	if (NULL != m_pMAR1FlashDlg && NULL != m_pMAR1FlashDlg->GetSafeHwnd())
	{
		m_pMAR1FlashDlg->DestroyWindow();
		delete m_pMAR1FlashDlg;
		m_pMAR1FlashDlg = NULL;
	}

	if (NULL != m_pMAFPhase3Dlg && NULL != m_pMAFPhase3Dlg->GetSafeHwnd())
	{
		m_pMAFPhase3Dlg->DestroyWindow();
		delete m_pMAFPhase3Dlg;
		m_pMAFPhase3Dlg = NULL;
	}

	if (NULL != m_pMARPhase3Dlg && NULL != m_pMARPhase3Dlg->GetSafeHwnd())
	{
		m_pMARPhase3Dlg->DestroyWindow();
		delete m_pMARPhase3Dlg;
		m_pMARPhase3Dlg = NULL;
	}

	if (NULL != m_pMAFMARPhase3Dlg && NULL != m_pMAFMARPhase3Dlg->GetSafeHwnd())
	{
		m_pMAFMARPhase3Dlg->DestroyWindow();
		delete m_pMAFMARPhase3Dlg;
		m_pMAFMARPhase3Dlg = NULL;
	}

	if (NULL != m_pmarPhase6)
	{
		m_pmarPhase6->DestroyWindow();
		delete m_pmarPhase6;
		m_pmarPhase6 = NULL;
	}

	if (NULL != m_pMAFPhase6)
	{
		m_pMAFPhase6->DestroyWindow();
		delete m_pMAFPhase6;
		m_pMAFPhase6 = NULL;
	}

	if (NULL != m_pMafMarPhase6)
	{
		m_pMafMarPhase6->DestroyWindow();
		delete m_pMafMarPhase6;
		m_pMafMarPhase6 = NULL;
	}


}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);


	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	CBussinessData::Instance()->SetMainFrm(this);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Class View window\n");
	//	return FALSE; // failed to create
	//}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	/*HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);*/

}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


LRESULT CMainFrame::OnShowQueryDlg(WPARAM wPara, LPARAM lPara)
{
	if (m_wndFileView.m_wndLeft.m_pQueryTaskDlg->GetSafeHwnd() != NULL)
	{
		m_wndFileView.m_wndLeft.m_pQueryTaskDlg->ShowQueryDlg();
	}

	return 0;
}

LRESULT CMainFrame::OnShowMafMarPhase1(WPARAM wPara, LPARAM lPara)
{
	if (m_wndFileView.m_wndLeft.m_pMAFMARDlg->GetSafeHwnd() != NULL)
	{
		m_wndFileView.m_wndLeft.m_pMAFMARDlg->ShowMAFMARDlg();
	}
	return 0;
}

LRESULT CMainFrame::OnShowMarFlashDlg(WPARAM wPara, LPARAM lPara)
{
	if (m_wndFileView.m_wndLeft.m_pCreateMARDlg->GetSafeHwnd() != NULL)
	{
		m_wndFileView.m_wndLeft.m_pCreateMARDlg->ShowMARServiceDlg();
	}
	

	return 0;
}

LRESULT CMainFrame::OnShowPhase1MAFMAR(WPARAM wp, LPARAM lp)
{
	if (m_pMAFMAR1FlashDlg != NULL )
	{
		if (m_pMAFMAR1FlashDlg->GetSafeHwnd() != NULL)
		{
			m_pMAFMAR1FlashDlg->DestroyWindow();
		}

		delete m_pMAFMAR1FlashDlg;
		m_pMAFMAR1FlashDlg = NULL;
	}

	m_pMAFMAR1FlashDlg = new CDlgMAFMARTermFlash;
	m_pMAFMAR1FlashDlg->Create(CDlgMAFMARTermFlash::IDD, this);
	m_pMAFMAR1FlashDlg->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CMainFrame::OnShowUnexpectedMarReqDlg(WPARAM w, LPARAM l)
{
	if (m_pUnexpectedReqDlg != NULL )
	{
		if (m_pUnexpectedReqDlg->GetSafeHwnd() != NULL)
		{
			m_pUnexpectedReqDlg->DestroyWindow();
		}

		delete m_pUnexpectedReqDlg;
		m_pUnexpectedReqDlg = NULL;
	}

	m_pUnexpectedReqDlg = new CDlgUnexpectedReq;
	m_pUnexpectedReqDlg->Create(CDlgUnexpectedReq::IDD, this);
	m_pUnexpectedReqDlg->ShowWindow(SW_SHOW);

	return 0;
}

//直接创建突发反向任务，进入数据传输阶段
LRESULT CMainFrame::OnCreateUnexpectMARTask(WPARAM w, LPARAM l)
{
	if (m_pUnexpectedReqDlg != NULL )
	{
		m_pUnexpectedReqDlg->CreateUnexpectedTask();
	}

	return 0;
}

LRESULT CMainFrame::OnUpdateMAFListCtrl(WPARAM w, LPARAM l)
{
	//更新当前的maf的task+tut+visbaletime。
	if (NULL != m_wndFileView.m_wndLeft.m_pQueryTaskDlg)
	{
		m_wndFileView.m_wndLeft.m_pQueryTaskDlg->UdpateListctrl();
	}
	
	//如果mar的窗口也是开着的，就更新下mar的task+visabletime
	if (NULL != m_wndFileView.m_wndLeft.m_pCreateMARDlg)
	{
		m_wndFileView.m_wndLeft.m_pCreateMARDlg->UpdateListCtrl();
	}
	return 0;
}

LRESULT CMainFrame::OnUpdateMARListCtrl(WPARAM w, LPARAM l)
{
	//更新当前的mar的task+visabletime.
	if (NULL != m_wndFileView.m_wndLeft.m_pCreateMARDlg)
	{
		m_wndFileView.m_wndLeft.m_pCreateMARDlg->UpdateListCtrl();
	}
	
	//如果前向查询窗口也是开着的，就要更新maf里面的task+visabletime.
	if (NULL != m_wndFileView.m_wndLeft.m_pQueryTaskDlg)
	{
		m_wndFileView.m_wndLeft.m_pQueryTaskDlg->UpdateTaskAndVisabletime();
	}

	return 0;
}

LRESULT CMainFrame::OnUpdateMAFMARListCtrl(WPARAM W, LPARAM L)
{
	if (NULL != m_wndFileView.m_wndLeft.m_pMAFMARDlg)
	{
		m_wndFileView.m_wndLeft.m_pMAFMARDlg->UpdateListctrl();
	}
	//如果前向查询窗口也是开着的，就要更新maf里面的task+visabletime.
	if (NULL != m_wndFileView.m_wndLeft.m_pQueryTaskDlg)
	{
		m_wndFileView.m_wndLeft.m_pQueryTaskDlg->UpdateTaskAndVisabletime();
	}

	return 0;
}

LRESULT CMainFrame::OnShowOccupied(WPARAM w, LPARAM l)
{
	return 0;
}

LRESULT CMainFrame::OnShowPhase1MAF(WPARAM wPara, LPARAM lPara)
{

	if (m_pMAF1FlashDlg != NULL )
	{
		if (m_pMAF1FlashDlg->GetSafeHwnd() != NULL)
		{
			m_pMAF1FlashDlg->DestroyWindow();
		}
		
		delete m_pMAF1FlashDlg;
		m_pMAF1FlashDlg = NULL;
	}

	m_pMAF1FlashDlg = new CDlgMAFTermFlash;
	m_pMAF1FlashDlg->Create(CDlgMAFTermFlash::IDD, this);
	m_pMAF1FlashDlg->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CMainFrame::OnShowPhase1MAR(WPARAM wPara, LPARAM lPara)
{

	if (m_pMAR1FlashDlg != NULL )
	{
		if (m_pMAR1FlashDlg->GetSafeHwnd() != NULL)
		{
			m_pMAR1FlashDlg->DestroyWindow();
		}

		delete m_pMAR1FlashDlg;
		m_pMAR1FlashDlg = NULL;
	}

	m_pMAR1FlashDlg = new CDlgMARTermFlash;
	m_pMAR1FlashDlg->Create(CDlgMARTermFlash::IDD, this);
	m_pMAR1FlashDlg->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CMainFrame::OnShowPhase3Dlg(WPARAM wPara, LPARAM lPara)
{
	TaskItem* pTaskItem = (TaskItem*)wPara;

	CString strTitle;
	strTitle.Format(_T("任务SSC=%s进入准备阶段！"),pTaskItem->strSSC);

	CTime timeShown = CDASRender::Instance()->GetShowTime();
	CTime taskTimeStart = MyTools::GetCTimeFromStringEx(pTaskItem->strStartTime);
	UINT nTimeGap = taskTimeStart.GetTime()-timeShown.GetTime();

	if (pTaskItem->eTaskType == enmTaskType_MAF)
	{
		if (NULL != m_pMAFPhase3Dlg)
		{
			if (NULL != m_pMAFPhase3Dlg->GetSafeHwnd())
			{
				m_pMAFPhase3Dlg->DestroyWindow();
			}
			delete m_pMAFPhase3Dlg;
			m_pMAFPhase3Dlg = NULL;
		}

		m_pMAFPhase3Dlg = new CDlgMAFPhase3;
		m_pMAFPhase3Dlg->SetTitle(strTitle);
		m_pMAFPhase3Dlg->SetStartGapTime(nTimeGap);
		m_pMAFPhase3Dlg->Create(CDlgMAFPhase3::IDD, this);
		m_pMAFPhase3Dlg->ShowWindow(SW_SHOW);
	}
	else if (pTaskItem->eTaskType == enmTaskType_MAR)
	{
		if (NULL != m_pMARPhase3Dlg)
		{
			if (NULL != m_pMARPhase3Dlg->GetSafeHwnd())
			{
				m_pMARPhase3Dlg->DestroyWindow();
			}
			delete m_pMARPhase3Dlg;
			m_pMARPhase3Dlg = NULL;
		}

		m_pMARPhase3Dlg = new CDlgMARPhase3;
		m_pMARPhase3Dlg->SetTitle(strTitle);
		m_pMARPhase3Dlg->SetStartGapTime(nTimeGap);
		m_pMARPhase3Dlg->Create(CDlgMARPhase3::IDD, this);
		m_pMARPhase3Dlg->ShowWindow(SW_SHOW);
	}
	else if (pTaskItem->eTaskType == enmTaskType_MAFMAR)
	{
		if (NULL != m_pMAFMARPhase3Dlg)
		{
			if (NULL != m_pMAFMARPhase3Dlg->GetSafeHwnd())
			{
				m_pMAFMARPhase3Dlg->DestroyWindow();
			}
			delete m_pMAFMARPhase3Dlg;
			m_pMAFMARPhase3Dlg = NULL;
		}

		m_pMAFMARPhase3Dlg = new CDlgMAFMARPhase3;
		m_pMAFMARPhase3Dlg->SetTitle(strTitle);
		m_pMAFMARPhase3Dlg->SetStartGapTime(nTimeGap);
		m_pMAFMARPhase3Dlg->Create(CDlgMAFMARPhase3::IDD, this);
		m_pMAFMARPhase3Dlg->ShowWindow(SW_SHOW);
	}

	delete pTaskItem;

	return 0;
}

LRESULT CMainFrame::ShowPhase6Dlg(WPARAM wPara, LPARAM lPara)
{
	TaskItem* pTaskItem = (TaskItem*)wPara;

	CDASSimulationView* pView = (CDASSimulationView*)GetActiveView();

	//修改任务状态为“完成时”
	if (pTaskItem->eTaskType == enmTaskType_MAF)
	{
		if (NULL != m_pMAFPhase6 && NULL != m_pMAFPhase6->GetSafeHwnd())
		{
			m_pMAFPhase6->DestroyWindow();
			delete m_pMAFPhase6;
			m_pMAFPhase6 = NULL;
		}

		m_pMAFPhase6 = new CDlgMAFPhase6;
		CString strTitle;
		strTitle.Format(_T("任务SSC=%s进入结束阶段！"), pTaskItem->strSSC);
		m_pMAFPhase6->Create(CDlgMAFPhase6::IDD, this);
		m_pMAFPhase6->SetWindowText(strTitle);//SetTitle(strTitle);
		m_pMAFPhase6->ShowWindow(SW_SHOW);
	}
	else if (pTaskItem->eTaskType == enmTaskType_MAR)
	{
		if (NULL != m_pmarPhase6 && NULL != m_pmarPhase6->GetSafeHwnd())
		{
			m_pmarPhase6->DestroyWindow();
			delete m_pmarPhase6;
			m_pmarPhase6 = NULL;
		}
		m_pmarPhase6 = new CDlgMARPhase6;
		//if is 突发反向任务：ugly but easy...
		if (pTaskItem->nChannelID == 6 && pTaskItem->strSSC == _T("12345"))
		{
			m_pmarPhase6->SetUnexpectMarflag(TRUE);
		}
		CString strTitle;
		strTitle.Format(_T("任务SSC=%s进入结束阶段！"), pTaskItem->strSSC);
		m_pmarPhase6->Create(CDlgMARPhase6::IDD, this);
		m_pmarPhase6->SetWindowText(strTitle);
		m_pmarPhase6->ShowWindow(SW_SHOW);
	}
	else if (pTaskItem->eTaskType == enmTaskType_MAFMAR)
	{
		if (NULL != m_pMafMarPhase6 && NULL != m_pMafMarPhase6->GetSafeHwnd())
		{
			m_pMafMarPhase6->DestroyWindow();
			delete m_pMafMarPhase6;
			m_pMafMarPhase6 = NULL;
		}
		m_pMafMarPhase6 = new CDlgMAFMARPhase6;
		CString strTitle;
		strTitle.Format(_T("任务SSC=%s进入结束阶段！"), pTaskItem->strSSC);
		m_pMafMarPhase6->Create(CDlgMAFMARPhase6::IDD, this);
		m_pMafMarPhase6->SetWindowText(strTitle);
		m_pMafMarPhase6->ShowWindow(SW_SHOW);
	}


	//设置任务状态为完成时。
	CBussinessData* pData = CBussinessData::Instance();
	::EnterCriticalSection(&(pData->m_csAllTasks));
	for (ListTaskTable::iterator it = pData->m_listAllTasks.begin();
		it != pData->m_listAllTasks.end(); ++it)
	{
		CString strStartTime = it->strStartTime;
		CString strEndTime   = it->strEndTime;
		if (strStartTime == pTaskItem->strStartTime
			&&  strEndTime == pTaskItem->strEndTime)
		{
			it->uStatus = enmTaskStatus_Processed;
			break;
		}
	}
	::LeaveCriticalSection(&(pData->m_csAllTasks));

	delete pTaskItem ;

	return 0;
}

LRESULT CMainFrame::BeginVisibleTime(WPARAM wPara, LPARAM lPara)
{
	TimeItem* pTime = (TimeItem*)wPara;
	DWORD dwEllapseTime = (DWORD)lPara;

	CTime timeBegin = MyTools::GetCTimeFromStringEx(pTime->strStartTime);
	CTime timeEnd = MyTools::GetCTimeFromStringEx(pTime->strEndTime);
	CDASRender::Instance()->CalculateUsAngleStep(timeBegin, timeEnd, dwEllapseTime);

	delete pTime;

	return 0;
}

LRESULT CMainFrame::PrepareOneTask(WPARAM wPara, LPARAM lPara)
{
	PostMessage(WM_SHOW_PHASE3_DIALOG, wPara);

	return 0;
}

LRESULT CMainFrame::BeginOneTask(WPARAM wPara, LPARAM lPara)
{
	TaskItem* pTaskItem = (TaskItem*)wPara;

	CDASSimulationView* pView = (CDASSimulationView*)GetActiveView();
	pView->StartSimmulate(pTaskItem->eTaskType);

	delete pTaskItem;

	return 0;

}

LRESULT CMainFrame::EndOneTask(WPARAM wPara, LPARAM lPara)
{
	//任务到时间了，停止画信号线
	CDASRender::Instance()->SetSimulateFlag(FALSE);
	CDASRender::Instance()->SetSysTimeFrequency(1);//将时间倍率设置回默认值！
	
	PostMessage(WM_SHOW_PHASE6_DIALOG, wPara);

	return 0;
}
LRESULT CMainFrame::ShowHideLeftViewer(WPARAM wPara, LPARAM lPara)
{
	if (m_bShowLeftViewer)
	{
		m_bShowLeftViewer = FALSE;
	}
	else
	{
		m_bShowLeftViewer = TRUE;
	}

	if (m_wndFileView.GetSafeHwnd() != NULL)
	{
		m_wndFileView.ShowPane(m_bShowLeftViewer, TRUE, FALSE);
		PostMessage(WM_SIZE);
	}

	return 0;
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{

		if( pMsg->wParam == 'L' && GetAsyncKeyState(VK_CONTROL) )
		{
			PostMessage(WM_SHOW_HIDE_LEFTVIEW);
		}
	}

	return __super::PreTranslateMessage(pMsg);
}
