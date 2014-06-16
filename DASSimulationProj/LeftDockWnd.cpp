
#include "stdafx.h"
#include "MainFrm.h"
#include "DASSimulationView.h"
#include "DASSimulation.h"
#include "LeftDockWnd.h"
#include "DlgMAFRequest.h"
#include "DlgDasIntroduction.h"
#include "DlgMARRequest.h"
#include "DlgSettimeToStarttask.h"
#include "DlgSettingShowTime.h"
#include "SysModuleIntroduction.h"
#include "DlgMAFMARQuery.h"


// CLeftDockWnd dialog

IMPLEMENT_DYNAMIC(CLeftDockWnd, CDialog)

CLeftDockWnd::CLeftDockWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLeftDockWnd::IDD, pParent)
{
	
	//m_pRegisterDlg	 = NULL;
	//m_pLoginDlg		 = NULL;
	m_pQueryTaskDlg	 = NULL;
	m_pCreateMARDlg  = NULL;
	m_pDASIntroDlg   = NULL;
	m_pSettingSysTimeDlg = NULL;
	m_pSettingSystime = NULL;
	m_pDASIntroduce = NULL;
	m_pMAFMARDlg = NULL;
}

CLeftDockWnd::~CLeftDockWnd()
{
// 	if(NULL != m_pRegisterDlg)
// 	{
// 		if (NULL != m_pRegisterDlg->GetSafeHwnd())
// 		{
// 			m_pRegisterDlg->DestroyWindow();
// 		}
// 		delete m_pRegisterDlg;
// 		m_pRegisterDlg = NULL;
// 	}

// 	if(NULL != m_pLoginDlg)
// 	{
// 		if (NULL != m_pLoginDlg->GetSafeHwnd())
// 		{
// 			m_pLoginDlg->DestroyWindow();
// 		}
// 		delete m_pLoginDlg;
// 		m_pLoginDlg = NULL;
// 	}

	if (NULL != m_pQueryTaskDlg)
	{
		if (NULL != m_pQueryTaskDlg->GetSafeHwnd())
		{
			m_pQueryTaskDlg->DestroyWindow();
		}
		
		delete m_pQueryTaskDlg;
		m_pQueryTaskDlg = NULL;
	}

	if(NULL != m_pCreateMARDlg)
	{
		if (NULL != m_pCreateMARDlg->GetSafeHwnd())
		{
			m_pCreateMARDlg->DestroyWindow();
		}
		delete m_pCreateMARDlg;
		m_pCreateMARDlg = NULL;
	}
	if(NULL != m_pDASIntroDlg)
	{
		if (NULL != m_pDASIntroDlg->GetSafeHwnd())
		{
			m_pDASIntroDlg->DestroyWindow();
		}
		delete m_pDASIntroDlg;
		m_pDASIntroDlg = NULL;
	}
	if (NULL != m_pSettingSysTimeDlg)
	{
		m_pSettingSysTimeDlg->DestroyWindow();
		delete m_pSettingSysTimeDlg;
		m_pSettingSysTimeDlg = NULL;
	}

	if (NULL != m_pSettingSystime)
	{
		m_pSettingSystime->DestroyWindow();
		delete m_pSettingSystime;
		m_pSettingSystime = NULL;
	}

	if (NULL != m_pDASIntroduce)
	{
		m_pDASIntroduce->DestroyWindow();
		delete m_pDASIntroduce;
		m_pDASIntroduce = NULL;
	}

}

void CLeftDockWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLeftDockWnd, CDialog)
	ON_WM_GETMINMAXINFO()
	//ON_BN_CLICKED(IDC_BTN_LOGIN, OnBnClickedLogin)
	//ON_BN_CLICKED(IDC_BTN_REGISTER, OnBnClickedRegister)
	ON_BN_CLICKED(IDC_BTN_DAS_TASK_QUERY, &CLeftDockWnd::OnBnClickedBtnDasTaskQuery)
	ON_BN_CLICKED(IDC_BTN_MAF_FORWORD, &CLeftDockWnd::OnBnClickedBtnMafForword)
	ON_BN_CLICKED(IDC_BTN_MAR_DEFORWORD, &CLeftDockWnd::OnBnClickedBtnMarDeforword)
	ON_BN_CLICKED(IDC_BTN_DAS_INTRODUCE, &CLeftDockWnd::OnBnClickedBtnDasIntroduce)
	//ON_BN_CLICKED(IDC_BTN_DAS_SETTING, OnBnClickedSystemSetting)
	
	//ON_BN_CLICKED(IDC_BTN_DAS_LOGOUT, &CLeftDockWnd::OnBnClickedBtnDasLogout)
	ON_BN_CLICKED(IDC_BTN_SET_SYS_TIME, &CLeftDockWnd::OnBnClickedBtnSetSysTime)
	ON_BN_CLICKED(IDC_BTN_SETTING_TIMES, &CLeftDockWnd::OnBnClickedBtnSettingTimes)
	ON_BN_CLICKED(IDC_BTN_DAS_SYSTEM, &CLeftDockWnd::OnBnClickedBtnDasSystem)
	ON_BN_CLICKED(IDC_DAFDAR_MODE, &CLeftDockWnd::OnBnClickedDafdarMode)
	ON_BN_CLICKED(IDC_UNEXPECT_MODE, &CLeftDockWnd::OnBnClickedUnexpectMode)
END_MESSAGE_MAP()


// CLeftDockWnd message handlers
// void CLeftDockWnd::OnBnClickedRegister()
// {
// // 	CRegisterDlg dlg;
// // 	dlg.DoModal();
// 
// 	if (NULL == m_pRegisterDlg)
// 	{
// 		m_pRegisterDlg = new CRegisterDlg();
// 		m_pRegisterDlg->Create(CRegisterDlg::IDD, NULL);
// 		m_pRegisterDlg->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		m_pRegisterDlg->ShowWindow(SW_RESTORE);
// 	}
// }

// void CLeftDockWnd::OnBnClickedLogin()
// {
// 	/*CLoginDlg dlg;
// 	dlg.DoModal();*/
// 
// // 	if (NULL == m_pLoginDlg)
// // 	{
// // 		m_pLoginDlg = new CLoginDlg();
// // 		m_pLoginDlg->Create(CLoginDlg::IDD, NULL);
// // 		m_pLoginDlg->ShowWindow(SW_SHOW);
// // 	}
// // 	else
// // 	{
// // 		m_pLoginDlg->ShowWindow(SW_RESTORE);
// // 	}
// }

void CLeftDockWnd::OnBnClickedBtnDasTaskQuery()
{
// 	CDlgMAFRequest dlg;
// 	dlg.DoModal();
	if (NULL == m_pQueryTaskDlg)
	{
		m_pQueryTaskDlg = new CDlgMAFRequest();
		m_pQueryTaskDlg->Create(CDlgMAFRequest::IDD, NULL);
		m_pQueryTaskDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pQueryTaskDlg->ShowWindow(SW_RESTORE);
	}
}

void CLeftDockWnd::OnBnClickedBtnMafForword()
{
	if (NULL == m_pQueryTaskDlg)
	{
		m_pQueryTaskDlg = new CDlgMAFRequest();
		m_pQueryTaskDlg->Create(CDlgMAFRequest::IDD, NULL);
		m_pQueryTaskDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pQueryTaskDlg->ShowWindow(SW_RESTORE);
	}
}

void CLeftDockWnd::OnBnClickedBtnMarDeforword()
{
	if (NULL == m_pCreateMARDlg)
	{
		m_pCreateMARDlg = new CDlgMARRequest();
		m_pCreateMARDlg->Create(CDlgMARRequest::IDD, NULL);
		m_pCreateMARDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pCreateMARDlg->ShowWindow(SW_RESTORE);
	}
}

void CLeftDockWnd::OnBnClickedBtnDasIntroduce()
{
	/*CDlgDasIntroduction dlg;
	dlg.DoModal();*/

	if (NULL == m_pDASIntroDlg)
	{
		m_pDASIntroDlg = new CDlgDasIntroduction();
		m_pDASIntroDlg->Create(CDlgDasIntroduction::IDD, NULL);
		m_pDASIntroDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDASIntroDlg->ShowWindow(SW_RESTORE);
	}
}


// void CLeftDockWnd::OnBnClickedSystemSetting()
// {
// 	CUserDataCache* pData = CUserDataCache::Instance();
// 	CString strUserName = pData->GetCurrentLoginUserName();
// 	if (strUserName.IsEmpty())
// 	{
// 		AfxMessageBox(_T("管理员没有登陆，不能操作！"));
// 		return;
// 	}
// 	else
// 	{
// 		if (strUserName != _T("admin"))
// 		{
// 			AfxMessageBox(_T("您不是管理员，没有权限操作系统设置！"));
// 			return;
// 		}
// 		else
// 		{
// 			CDlgDASSetting dlg;
// 			dlg.DoModal();
// 		}
// 	}
// }


// void CLeftDockWnd::OnBnClickedBtnDasLogout()
// {
// 	CUserDataCache* pData = CUserDataCache::Instance();
// 	CString strUserName = pData->GetCurrentLoginUserName();
// 	if (strUserName.IsEmpty())
// 	{
// 		AfxMessageBox(_T("当前没有用户登陆！"));
// 		return;
// 	}
// 
// 	pData->SetUserStatus(strUserName, enmStatus_Logout);
// 
// 	AfxGetMainWnd()->SetWindowText(_T("DAS:当前没有用户登陆！"));
// 	
// 	CString strMsg;
// 	strMsg.Format(_T("%s已经退出系统！"), strUserName);
// 	AfxMessageBox(strMsg);
// }



void CLeftDockWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	GetMinMaxInfo(lpMMI);

	__super::OnGetMinMaxInfo(lpMMI);
}

void  CLeftDockWnd::GetMinMaxInfo(MINMAXINFO* lpMMI)
{
	HWND hWnd = GetSafeHwnd();

	if(NULL != hWnd && ::IsWindow(hWnd))
	{
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		HMONITOR hMonitor = MonitorFromWindow(GetSafeHwnd(), MONITOR_DEFAULTTONULL);

		if(NULL != hMonitor)
		{
			MONITORINFO stMonitorInfo = { 0 };
			stMonitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &stMonitorInfo);

			CRect rcWork(stMonitorInfo.rcWork);
			CRect rcMonitor(stMonitorInfo.rcMonitor);

			rcWork.NormalizeRect();
			rcMonitor.NormalizeRect();

			APPBARDATA data = { 0 };
			data.cbSize = sizeof(APPBARDATA);
			::SHAppBarMessage(ABM_GETTASKBARPOS, &data);

			CRect rcTaskBar = data.rc;

			UINT uState = (UINT)::SHAppBarMessage(ABM_GETSTATE, &data);
			int nOffset = 0;

			lpMMI->ptMaxPosition.x = 0;
			lpMMI->ptMaxPosition.y = 0;

			lpMMI->ptMaxSize.x = rcWork.Width();
			lpMMI->ptMaxSize.y = rcWork.Height() - nOffset;

			CRect rcIntersection;

			BOOL bIntersect = rcIntersection.IntersectRect(&rcTaskBar, &rcMonitor);

			if(bIntersect)
			{
				if( 0 != (ABS_AUTOHIDE & uState) )
				{
					if(rcTaskBar.Width() == rcMonitor.Width() &&
						rcTaskBar.bottom == rcMonitor.bottom)
					{
						lpMMI->ptMaxSize.y -= 2;
					}
				}
				else
				{
					if(rcTaskBar.Height() == rcMonitor.Height())
					{
						if(rcTaskBar.left == 0)
						{
							lpMMI->ptMaxPosition.x += rcTaskBar.Width();
							lpMMI->ptMaxSize.x -= rcTaskBar.Height();
						}
					}
					else if(rcTaskBar.Width() == rcMonitor.Width())
					{
						if(rcTaskBar.top == 0)
						{
							lpMMI->ptMaxPosition.y += rcTaskBar.Height();
							lpMMI->ptMaxSize.y -= rcTaskBar.Height();
						}
					}
				}
			}
		}
	}
}

void CLeftDockWnd::OnBnClickedBtnSetSysTime()
{
	if (NULL == m_pSettingSysTimeDlg)
	{
		m_pSettingSysTimeDlg = new CDlgSettimeToStarttask;
		m_pSettingSysTimeDlg->Create(CDlgSettimeToStarttask::IDD, NULL);
		m_pSettingSysTimeDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSettingSysTimeDlg->InitTaskListctrl();
		m_pSettingSysTimeDlg->ShowWindow(SW_RESTORE);
	}
}

void CLeftDockWnd::OnBnClickedBtnSettingTimes()
{

	if (NULL != m_pSettingSystime)
	{
		m_pSettingSystime->DestroyWindow();
		delete m_pSettingSystime;
		m_pSettingSystime = NULL;
	}

// 	if (NULL == m_pSettingSystime)
// 	{
		m_pSettingSystime = new CDlgSettingShowTime;
		m_pSettingSystime->Create(CDlgSettingShowTime::IDD, NULL);
		m_pSettingSystime->ShowWindow(SW_SHOW);
//	}
// 	else
// 	{
// 		m_pSettingSystime->UpdateControl();
// 		m_pSettingSystime->ShowWindow(SW_SHOW);
// 	}
}

void CLeftDockWnd::OnBnClickedBtnDasSystem()
{
	if (NULL != m_pDASIntroduce)
	{
		m_pDASIntroduce->DestroyWindow();
		delete m_pDASIntroduce;
		m_pDASIntroduce = NULL;
	}

	m_pDASIntroduce = new CSysModuleIntroduction;
	m_pDASIntroduce->Create(CSysModuleIntroduction::IDD, this);
	m_pDASIntroduce->ShowWindow(SW_SHOW);	
}

void CLeftDockWnd::OnBnClickedDafdarMode()
{
	if (NULL != m_pMAFMARDlg)
	{
		m_pMAFMARDlg->DestroyWindow();
		delete m_pMAFMARDlg;
		m_pMAFMARDlg = NULL;
	}

	m_pMAFMARDlg = new CDlgMAFMARQuery;
	m_pMAFMARDlg->Create(CDlgMAFMARQuery::IDD, this);
	m_pMAFMARDlg->ShowWindow(SW_SHOW);
}

void CLeftDockWnd::OnBnClickedUnexpectMode()
{
	//3.3D下绘制信号线:pocc--->地面站
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (NULL != pMain)
	{
		CDASSimulationView* pView = (CDASSimulationView*)(pMain->GetActiveView());
		//1.先根据当前的时间往后推24小时，计算一天的stk数据，写入到stk cache。
		//pView->PreSimulatestkData();
		//数据已经生成过了，直接从visible table里面去取。


		//2.时间跳转到最后一条可见时间段
		pView->GotonewviewTime();

		//3.然后进行3d交互。
		pView->StartDrawUnexpectedRequestSingal(FALSE);
	}

}
