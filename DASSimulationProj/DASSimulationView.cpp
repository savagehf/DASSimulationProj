
#include "stdafx.h"
#include "DASSimulation.h"
#include "DASSimulationDoc.h"
#include "DASSimulationView.h"
#include "BussinessData.h"
#include "MyTools.h"

#include "DlgMAFPhase1.h"
#include "DlgMAFPhase2.h"
#include "DlgMAFPhase3.h"
#include "DlgMAFPhase4.h"
#include "DlgMAFPhase5.h"
#include "DlgMAFPhase6.h"

#include "DlgMARPhase1.h"
#include "DlgMARPhase2.h"
#include "DlgMARPhase3.h"
#include "DlgMARPhase4.h"
#include "DlgMARPhase5.h"
#include "DlgMARPhase6.h"
#include "DlgMAFTermFlash.h"
#include "DlgMARTermFlash.h"
#include "SysModuleIntroduction.h"

#include "DlgSettingShowTime.h"

#include "MainFrm.h"
#include "DASRender.h"

#include "Logging/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CDASSimulationView, CView)

BEGIN_MESSAGE_MAP(CDASSimulationView, CView)

	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDASSimulationView::OnFilePrintPreview)
	//ON_COMMAND(ID_MAF_PHASE_1, &CDASSimulationView::OnMafPhase_1)
	//ON_COMMAND(ID_MAF_PHASE_2, &CDASSimulationView::OnMafPhase2)
	//ON_COMMAND(ID_MAF_PHASE_3, &CDASSimulationView::OnMafPhase3)
	//ON_COMMAND(ID_MAF_PHASE_4, &CDASSimulationView::OnMafPhase4)
// 	ON_COMMAND(ID_MAF_PHASE_5, &CDASSimulationView::OnMafPhase5)
// 	ON_COMMAND(ID_MAF_PHASE_6, &CDASSimulationView::OnMafPhase6)
// 	ON_COMMAND(ID_MAR_PHASE_1, &CDASSimulationView::OnMarPhase1)
// 	ON_COMMAND(ID_MAR_PHASE_2, &CDASSimulationView::OnMarPhase2)
// 	ON_COMMAND(ID_MAR_PHASE_3, &CDASSimulationView::OnMarPhase3)
// 	ON_COMMAND(ID_MAR_PHASE_4, &CDASSimulationView::OnMarPhase4)
// 	ON_COMMAND(ID_MAR_PHASE_5, &CDASSimulationView::OnMarPhase5)
// 	ON_COMMAND(ID_MAR_PHASE_6, &CDASSimulationView::OnMarPhase6)
	ON_COMMAND(ID_MAF_SHOWALL, &CDASSimulationView::OnMafShowall)
	ON_COMMAND(ID_MAR_SHOWALL, &CDASSimulationView::OnMarShowall)
	ON_COMMAND(ID_SYS_STRUCTRUE, &CDASSimulationView::OnSysStructrue)
	ON_COMMAND(ID_SHOW_LEFT_DOCKPANE, &CDASSimulationView::OnShowLeftDockpane)
	ON_COMMAND(ID_HIDE_LEFT_DOCKPANE, &CDASSimulationView::OnHideLeftDockpane)
	ON_COMMAND(ID_MAF_CLOSE_LOOP_PLAY, &CDASSimulationView::OnMafCloseLoopPlay)
	ON_COMMAND(ID_MAR_CLOSE_LOOP_PLAY, &CDASSimulationView::OnMarCloseLoopPlay)
END_MESSAGE_MAP()

CDASSimulationView::CDASSimulationView()
{
	m_bMAF = TRUE;
	m_bMafMar = FALSE;
	m_pSysStructure = NULL;

	m_pMAFPhase1 = NULL;
	m_pMAFPhase2 = NULL;
	m_pMAFPhase3 = NULL;
	m_pMAFPhase4 = NULL;
	m_pMAFPhase5 = NULL;
	m_pMAFPhase6 = NULL;

	m_pMARPhase1 = NULL;
	m_pMARPhase2 = NULL;
	m_pMARPhase3 = NULL;
	m_pMARPhase4 = NULL;
	m_pMARPhase5 = NULL;
	m_pMARPhase6 = NULL;
}

CDASSimulationView::~CDASSimulationView()
{
	if (NULL != m_pSysStructure)
	{
		m_pSysStructure->DestroyWindow();
		delete m_pSysStructure;
	}

	if(m_pMAFPhase1 != NULL)
	{
		m_pMAFPhase1->DestroyWindow();
		delete m_pMAFPhase1;
		m_pMAFPhase1 = NULL;
	}
	if(m_pMAFPhase2)
	{
		m_pMAFPhase2->DestroyWindow();
		delete m_pMAFPhase2;
		m_pMAFPhase2 = NULL;
	}
	if(m_pMAFPhase3)
	{
		m_pMAFPhase3->DestroyWindow();
		delete m_pMAFPhase3;
		m_pMAFPhase3 = NULL;
	}

	if(m_pMAFPhase4)
	{
		m_pMAFPhase4->DestroyWindow();
		delete m_pMAFPhase4;
		m_pMAFPhase4 = NULL;
	}
	if(m_pMAFPhase5)
	{
		m_pMAFPhase5->DestroyWindow();
		delete m_pMAFPhase5;
		m_pMAFPhase5 = NULL;
	}
	if (m_pMAFPhase6)
	{
		m_pMAFPhase6->DestroyWindow();
		delete m_pMAFPhase6;
		m_pMAFPhase6 = NULL;
	}
	

	if(m_pMARPhase1)
	{
		m_pMARPhase1->DestroyWindow();
		delete m_pMARPhase1;
		m_pMARPhase1 = NULL;
	}
	if(m_pMARPhase2)
	{
		m_pMARPhase2->DestroyWindow();
		delete m_pMARPhase2;
		m_pMARPhase2 = NULL;
	}

	if(m_pMARPhase3)
	{
		m_pMARPhase3->DestroyWindow();
		delete m_pMARPhase3;
		m_pMARPhase3 = NULL;
	}
	if(m_pMARPhase4)
	{
		m_pMARPhase4->DestroyWindow();
		delete m_pMARPhase4;
		m_pMARPhase4 = NULL;
	}
	if(m_pMARPhase5)
	{
		m_pMARPhase5->DestroyWindow();
		delete m_pMARPhase5;
		m_pMARPhase5 = NULL;
	}
	if(m_pMARPhase6)
	{
		m_pMARPhase6->DestroyWindow();
		delete m_pMARPhase6;
		m_pMARPhase6 = NULL;
	}
}

BOOL CDASSimulationView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

int CDASSimulationView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = new CClientDC(this);
	
	// Create log file.
	CString sLoggingPathname = MyTools::GetCurrentPath();
	sLoggingPathname += _T("\\Database\\Logfile.html");
	Logging::CreateInstance();
#ifdef NDEBUG
	theLog()->InitLogging(LEVEL_INFO, new CHtmlAppender(sLoggingPathname));
#else
	theLog()->InitLogging(LEVEL_TRACE, new CHtmlAppender(sLoggingPathname));
#endif

	CDASRender::Instance()->InitOpenGL(pDC);
	CDASRender::Instance()->InitLight();
	CDASRender::Instance()->Load3dsModels();
	CDASRender::Instance()->LoadTextures();

	//Init tut��10��
	CTime	ctimeStartDate = CTime::GetCurrentTime();
	CString strStartDate = MyTools::GetFormatedDate(ctimeStartDate);
	CString strNewStartDateTime = strStartDate + _T(" 0:0:0");

	CTime newStart = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	CTime newEnd = newStart + CTimeSpan(0,23,59,59);
	CBussinessData::Instance()->SimulateTUT(newStart, newEnd);
	
	//Init �ɼ�ʱ�䴰��10��
	//���ɿɼ�ʱ��-д��csv�ļ�-->cache���ڴ�m_listvisbletime
	map<CString, CString> mapVisTimeTable;
	CString strTestStart = MyTools::GetFormatedDateTime(newStart);
	CString strTestend = MyTools::GetFormatedDateTime(newEnd);
	CBussinessData::Instance()->SimulateVisableTime(newStart, newEnd, mapVisTimeTable);
	CBussinessData::Instance()->ImportUSAndTDRSVisibleTimeTable();
	
	
	SetTimer(eTimer_DrawSence,300,NULL);

	m_pSysStructure = new CSysModuleIntroduction;
	m_pSysStructure->Create(CSysModuleIntroduction::IDD, this);
	m_pSysStructure->ShowWindow(SW_HIDE);
	return 0;
}

void CDASSimulationView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (!CDASRender::Instance()->IsOpenGL()) {
		return;
	}

	if(cy == 0)
	{
		cy = 1;
	}

	CDASRender::Instance()->ResizeViewPort(cx, cy);
}

void CDASSimulationView::OnDraw(CDC* /*pDC*/)
{
	CDASSimulationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//OpenGL����ģʽ
	
	CDASRender::Instance()->DrawDASScence();
}

//����һ���stk���ݣ���ѡȡ���һ����ʱ����Ϊͻ�������ʱ�䡣
void CDASSimulationView::PreSimulatestkData()
{
	int nTDRS  = 1;//= _tcstoul(strTDRS.GetBuffer(), NULL, 10);
	int nSIC   = 1;//_tcstoul(strSIC.GetBuffer(), NULL, 10);

	CTime m_ctimeStartDate = CTime::GetCurrentTime();
	CTime m_ctimeEndDate = CTime::GetCurrentTime()+24*3600;
	CTime m_ctimeStartTime = CTime::GetCurrentTime();
	CTime m_ctimeEndTime = CTime::GetCurrentTime();


	CString strStart = MyTools::GetFormatedDate(m_ctimeStartDate);
	CString strEnd   = MyTools::GetFormatedDate(m_ctimeEndDate);
	CString strTimeStart = MyTools::GetFormatedTime(m_ctimeStartTime);
	CString strTimeEnd = MyTools::GetFormatedTime(m_ctimeEndTime);
	CString strNewStartDateTime = strStart+_T(" ")+strTimeStart;
	CString strNewEndDateTime = strEnd + _T(" ")+strTimeEnd;

	CTime timeNewStartDateTime = MyTools::GetCTimeFromStringEx(strNewStartDateTime);
	CTime timeNewEndDateTime = MyTools::GetCTimeFromStringEx(strNewEndDateTime);

	//////////////////////////////////////////////////////////////////////////
	//ģ��UP ��

	CBussinessData* pData = CBussinessData::Instance();

	//���ɿɼ�ʱ��-canceled-2013��12��15��23:38:45
	//map<CString, CString> mapVisTimeTable;
	//pData->SimulateVisableTime(timeNewStartDateTime, timeNewEndDateTime, mapVisTimeTable);

	//���ؿɼ�ʱ��
	//pData->ImportUSAndTDRSVisibleTimeTable();
}

void CDASSimulationView::GotonewviewTime()
{
	CBussinessData* pData = CBussinessData::Instance();
	CTime timeStart;
	::EnterCriticalSection(&(pData->m_csVisableTime));
	for (ListTimeTable::reverse_iterator rit = pData->m_listVisbleTime.rbegin();
		rit != pData->m_listVisbleTime.rend(); ++rit)
	{
		if (!rit->bPlayed && !rit->bOuccupied)
		{
			rit->bUnexp = TRUE;
			timeStart = MyTools::GetCTimeFromStringEx(rit->strStartTime);
			break;
		}
	}	
	
	::LeaveCriticalSection(&(pData->m_csVisableTime));

	CDASRender::Instance()->SetNewTimeInView(CTime::GetCurrentTime(), timeStart);
}

//����ͻ��ģʽ�������ź���
void CDASSimulationView::StartDrawUnexpectedRequestSingal(BOOL bMAF/* = FALSE*/)
{
	m_bMAF = bMAF;

	//
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_UnexpectedMAR_Requset);
	CDASRender::Instance()->SetMAFFlag(bMAF);
	CDASRender::Instance()->SetMAFMARFlag(FALSE);

	//��ʼ3D�׶ε�������ʾ
	SetTimer(enmTimer_Phase1_UnexpectedMar_Request, 15000, NULL);
}

void CDASSimulationView::StartDrawUnexpectedAnswerSignal(BOOL bMAF)
{
	m_bMAF = bMAF;

	//���� ����վ--->pocc��һ���ظ��ź��ߡ�����3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_UnexpectedMAR_Answer);

	SetTimer(enmTimer_Phase1_UnexpectedMar_Answer, 15000, NULL);
}

void CDASSimulationView::StartDrawPhase1RequestSignal(BOOL bMAF)
{
	m_bMAF = bMAF;

	//phase1�Ի�������1���ʼ����һ���ź��ߣ�pocc-->����վ������3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_Request);
	CDASRender::Instance()->SetMAFFlag(bMAF);
	CDASRender::Instance()->SetMAFMARFlag(FALSE);

	//��ʼ3D�׶ε�������ʾ
	SetTimer(enmTimer_Phase1_Request, 15000, NULL);
}

void CDASSimulationView::NotifyBeOccupied()
{
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_Occupy);
	SetTimer(enmTimer_Phase1_Occupy_Notify, 15000, NULL);
}

void CDASSimulationView::StartDrawMAFMARPhase1ReuestSignal(BOOL bMAF)
{
	m_bMAF = bMAF;
	//m_bMafMar = TRUE;

	//phase1�Ի�������1���ʼ����һ���ź��ߣ�pocc-->����վ������3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_Request);
	CDASRender::Instance()->SetMAFFlag(bMAF);
	//CDASRender::Instance()->SetMAFMARFlag(TRUE);

	SetTimer(enmTimer_Phase1_MAFMAR_Request, 15000, NULL);
}

void CDASSimulationView::StartDrawPhase1AnswerSignal(BOOL bMAF)
{
	m_bMAF = bMAF;

	//���� ����վ--->pocc��һ���ظ��ź��ߡ�����3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_Answer);
	
	SetTimer(enmTimer_Phase1_Answer, 8000, NULL);
}

void CDASSimulationView::StartDrawMAFMARPhase1AnswerSignal(BOOL bMAF)
{
	//m_bMAF = bMAF;
	m_bMafMar = TRUE;

	//���� ����վ--->pocc��һ���ظ��ź��ߡ�����3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase1_Answer);

	SetTimer(enmTimer_Phase1_MAFMAR_Answer, 8000, NULL);
}

void CDASSimulationView::StartDrawPhase2AnswerSignal(BOOL bMAF)
{
	//m_bMAF = bMAF;
	m_bMafMar = TRUE;

	//���� ����վ--->pocc��һ���ظ��ź��ߡ�����3��
	CDASRender::Instance()->SetPhase12Flag(enmPhase2_Answer);

	SetTimer(enmTimer_Phase2_MAFMAR_Answer, 8000, NULL);
}


void CDASSimulationView::StartSimmulate(ETaskType eTaskType)
{
	if (eTaskType == ETaskType::enmTaskType_MAFMAR)
	{
		m_bMafMar = TRUE;
		//ģ�⿪ʼ�ˣ����Ի����źš�
		CDASRender::Instance()->ReSetStatus();
		CDASRender::Instance()->SetSimulateFlag(TRUE);
		CDASRender::Instance()->SetMAFMARFlag(TRUE);

		//2S��ʼ���� ����վ-->�м��� ���ź�
		SetTimer(eTimer_MAF_DrawTer2TDRS, 2000, NULL);
		// 2s��ʼ���� �м���-->����վ ���ź�
		SetTimer(eTimer_MAR_DrawTDRS2Ter, 2000, NULL);
	}
	else
	{
		BOOL bMAF = FALSE;
		if (eTaskType == enmTaskType_MAF)
		{
			bMAF = TRUE;
		}

		//ģ�⿪ʼ�ˣ����Ի����źš�
		CDASRender::Instance()->ReSetStatus();
		CDASRender::Instance()->SetSimulateFlag(TRUE);
		CDASRender::Instance()->SetMAFFlag(bMAF);
		m_bMAF = bMAF;

		if (m_bMAF)
		{
			//2S��ʼ���� ����վ-->�м��� ���ź�
			SetTimer(eTimer_MAF_DrawTer2TDRS, 2000, NULL);
		}
		else
		{
			// 2s��ʼ���� �м���-->����վ ���ź�
			SetTimer(eTimer_MAR_DrawTDRS2Ter, 2000, NULL);
		}
	}
	

}

// void CDASSimulationView::CheckIfTaskAvailable()
// {
// 	SetTimer(enmTimer_CheckTaskPrepare, 500, NULL);
// }

void CDASSimulationView::OnTimer(UINT nIDEvent) 
{
	//������ÿ��200ms���ƵĶ���������
	if (nIDEvent == eTimer_DrawSence) 
	{	
		CDASRender::Instance()->ReCalculateAngle();
		Invalidate();	
		CDASRender::Instance()->ReFreshTime();
	}
	else if (nIDEvent == eTimer_MAR_DrawTDRS2Ter || nIDEvent == eTimer_MAF_DrawTer2TDRS)
	{
		if (m_bMafMar)
		{
			//��ʼ���� ����վ-->�м��� ���ź�
			CDASRender::Instance()->SetDrawTer2TDRS(TRUE);
			KillTimer(eTimer_MAF_DrawTer2TDRS);

			//2s��ʼ���� �м���-->���� ���ź�
			SetTimer(eTimer_MAF_DrawTDRS2US, 2000, NULL);


			// ��ʼ���� �м���-->����վ ���ź�
			CDASRender::Instance()->SetDrawTDRS2Ter(TRUE);
			KillTimer(eTimer_MAR_DrawTDRS2Ter);

			//2s��ʼ���� ����վ-->�û����� ���ź�
			SetTimer(eTimer_MAR_DrawTer2UserCen, 2000, NULL);
		}
		else
		{
			if (m_bMAF)
			{
				//��ʼ���� ����վ-->�м��� ���ź�
				CDASRender::Instance()->SetDrawTer2TDRS(TRUE);
				KillTimer(eTimer_MAF_DrawTer2TDRS);

				//2s��ʼ���� �м���-->���� ���ź�
				SetTimer(eTimer_MAF_DrawTDRS2US, 2000, NULL);
			}
			else
			{
				// ��ʼ���� �м���-->����վ ���ź�
				CDASRender::Instance()->SetDrawTDRS2Ter(TRUE);
				KillTimer(eTimer_MAR_DrawTDRS2Ter);

				//2s��ʼ���� ����վ-->�û����� ���ź�
				SetTimer(eTimer_MAR_DrawTer2UserCen, 2000, NULL);
			}
		}
		
	
	}
	else if (nIDEvent == eTimer_MAR_DrawTer2UserCen || nIDEvent == eTimer_MAF_DrawTDRS2US)
	{
		if (m_bMafMar)
		{
			CDASRender::Instance()->SetDrawTDRS2US(TRUE);
			KillTimer(eTimer_MAF_DrawTDRS2US);

			// ��ʼ���� ����վ-->�û����� ���ź�
			CDASRender::Instance()->SetDrawTer2UsrCen(TRUE);
			KillTimer(eTimer_MAR_DrawTer2UserCen);
		}
		else
		{
			if (m_bMAF)
			{
				CDASRender::Instance()->SetDrawTDRS2US(TRUE);
				KillTimer(eTimer_MAF_DrawTDRS2US);
			}
			else
			{
				// ��ʼ���� ����վ-->�û����� ���ź�
				CDASRender::Instance()->SetDrawTer2UsrCen(TRUE);
				KillTimer(eTimer_MAR_DrawTer2UserCen);
			}
		}
		
		
	}
	//����Ƿ����������׼���׶�
// 	else if (nIDEvent == enmTimer_CheckTaskPrepare)
// 	{
// 		//�������ڽ�������2���Ӽ���ִ��
// 		if(AnyTaskPrepare())
// 		{
// 			KillTimer(enmTimer_CheckTaskPrepare);
// 			
// 			CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
// 			pMain->PostMessage(WM_SHOW_PHASE3_DIALOG);
// 		}
// 	}
	//��һ�׶λ���һ���ź��ߣ�pocc-->����վ�Ѿ�3s�ˣ�����ֹͣ������
	else if (nIDEvent == enmTimer_Phase1_Request)
	{
		KillTimer(enmTimer_Phase1_Request);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if(m_bMAF)
		{
			pMain->PostMessage(WM_SHOW_PHASE1_MAF);
		}
		else
		{
			pMain->PostMessage(WM_SHOW_PHASE1_MAR);
		}
	}
	else if (nIDEvent == enmTimer_Phase1_Answer)
	{
		KillTimer(enmTimer_Phase1_Answer);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		//������ʾQuery�Ի���
		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if (m_bMAF)
		{
			pMain->PostMessage(WM_RETORE_HIDEN_QUERY_DLG);
		}
		else
		{
			pMain->PostMessage(WM_RESTORE_HIDE_MAR_DLG);
		}
		
	}
	//ͻ��marģʽ����׶�3D��ʾ������
	else if (nIDEvent == enmTimer_Phase1_UnexpectedMar_Request)
	{
		KillTimer(enmTimer_Phase1_UnexpectedMar_Request);
		//����״̬Ϊ��ʼ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		pMain->PostMessage(WM_SHOW_UNEXPECTED_MAR_REQUEST_DIALOG);
	}
	else if (nIDEvent == enmTimer_Phase1_UnexpectedMar_Answer)
	{
		KillTimer(enmTimer_Phase1_UnexpectedMar_Answer);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		//ֱ�Ӵ���ͻ���������񣬽������ݴ���׶�
		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		pMain->PostMessage(WM_CREATE_UNEXPECTED_MAR_TASK);
		
	}
	else if (enmTimer_Phase1_MAFMAR_Request == nIDEvent)
	{
		KillTimer(enmTimer_Phase1_MAFMAR_Request);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if(m_bMAF)
		{
			pMain->PostMessage(WM_SHOW_PHASE1_MAFMAR);
		}
	}
	else if (nIDEvent == enmTimer_Phase1_MAFMAR_Answer)
	{
		KillTimer(enmTimer_Phase1_MAFMAR_Answer);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		//������ʾQuery�Ի���
		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if (m_bMAF)
		{
			pMain->PostMessage(WM_RESTORE_HIDE_MAFMAR_DLG);
		}
	}
	else if (nIDEvent == enmTimer_Phase2_MAFMAR_Answer)
	{
		KillTimer(enmTimer_Phase2_MAFMAR_Answer);
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		//checkifsuccess.
		//m_pparent->CheckIfSuccess();

	}
	else if (nIDEvent == enmTimer_Phase1_Occupy_Notify)
	{
		KillTimer(enmTimer_Phase1_Occupy_Notify);
		//����״̬Ϊ��ʾ״̬
		CDASRender::Instance()->SetPhase12Flag(enmPhase_None);

		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if(m_bMAF)
		{
			pMain->PostMessage(WM_SHOW_OCCUPIED);
		}

	}
	
	CView::OnTimer(nIDEvent);
}


//��ѵ��Щ�����ڽ�������2���ӿ��Կ�ʼ
// BOOL CDASSimulationView::AnyTaskPrepare()
// {
// 	CBussinessData* pData = CBussinessData::Instance();
// 	CTime timeShown = CDASRender::Instance()->GetShowTime();
// 
// 	::EnterCriticalSection(&(pData->m_csAllTasks));
// 	for (ListTaskTable::iterator it = pData->m_listAllTasks.begin();
// 		it != pData->m_listAllTasks.end(); ++it)
// 	{
// 		CString strStartTime = it->strStartTime;
// 		CTime taskTimeStart = MyTools::GetCTimeFromStringEx(strStartTime);
// 		UINT nTimeGap = taskTimeStart.GetTime()-timeShown.GetTime();
// 		
// 		if (taskTimeStart > timeShown && nTimeGap <= TIME_BEFORE_TASK_BEGIN)
// 		{
// 			m_taskItemTobeShown = *it;
// 
// 			::LeaveCriticalSection(&(pData->m_csAllTasks));
// 			return TRUE;
// 		}
// 	}
// 
// 	::LeaveCriticalSection(&(pData->m_csAllTasks));
// 	return FALSE;
// }


void CDASSimulationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDASRender::Instance()->ScaleView(nChar);
	
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDASSimulationView::OnEraseBkgnd(CDC* pDC) 
{
	if (CDASRender::Instance()->IsOpenGL()) 
	{
		return true;
	}
	else
		return CView::OnEraseBkgnd(pDC);
}

void CDASSimulationView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDASSimulationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CDASSimulationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}

void CDASSimulationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CDASSimulationView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDASSimulationView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

#ifdef _DEBUG
void CDASSimulationView::AssertValid() const
{
	CView::AssertValid();
}

void CDASSimulationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDASSimulationDoc* CDASSimulationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDASSimulationDoc)));
	return (CDASSimulationDoc*)m_pDocument;
}
#endif //_DEBUG


void CDASSimulationView::OnDestroy() 
{
	CView::OnDestroy();

	CBussinessData::DestroyInstance();
	CDASRender::DestroyInstance();
}

//void CDASSimulationView::Test(CArray<CTask,CTask&> taskList)
//{
//
//}
//
//void CDASSimulationView::LoadTdrs(CString filename)
//{
//	if( m_listTdrs.getNumObjects() > 0 ) {
//		m_listTdrs.removeAllObjects();
//	}
//
//	C3dsReader Loader;
//	m_bIsTdrsLoaded = Loader.Reader( (LPSTR)(LPCTSTR)filename, &m_listTdrs);
//	m_listTdrs.doAfterMath();
//}
//
//
//void CDASSimulationView::DrawString2f(float x,float y,CString content){
//	glRasterPos2f(x, y);			 
//	for(int j = 0; j < content.GetLength(); j++){
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, content[j]);
//	}
//}

void CDASSimulationView::OnMafShowall()
{
	OnMafPhase_1();
}

void CDASSimulationView::OnMafPhase_1()
{
	if (NULL != m_pMAFPhase1)
	{
		m_pMAFPhase1->DestroyWindow();
		delete m_pMAFPhase1;
		m_pMAFPhase1 = NULL;
	}

	m_pMAFPhase1 = new CDlgMAFPhase1;
	m_pMAFPhase1->SetDialogMode(TRUE);
	m_pMAFPhase1->Create(CDlgMAFPhase1::IDD);
	m_pMAFPhase1->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMafPhase2()
{
	if (NULL != m_pMAFPhase2)
	{
		m_pMAFPhase2->DestroyWindow();
		delete m_pMAFPhase2;
		m_pMAFPhase2 = NULL;
	}

	m_pMAFPhase2 = new CDlgMAFPhase2;
	m_pMAFPhase2->SetDialogMode(TRUE);
	m_pMAFPhase2->Create(CDlgMAFPhase2::IDD);
	m_pMAFPhase2->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMafPhase3()
{
	if (NULL != m_pMAFPhase3)
	{
		m_pMAFPhase3->DestroyWindow();
		delete m_pMAFPhase3;
		m_pMAFPhase3 = NULL;
	}
	m_pMAFPhase3 = new CDlgMAFPhase3;
	m_pMAFPhase3->SetDialogMode(TRUE);
	m_pMAFPhase3->Create(CDlgMAFPhase3::IDD);
	m_pMAFPhase3->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMafPhase4()
{
	if (NULL != m_pMAFPhase4)
	{
		m_pMAFPhase4->DestroyWindow();
		delete m_pMAFPhase4;
		m_pMAFPhase4 = NULL;
	}

	m_pMAFPhase4 = new CDlgMAFPhase4;
	m_pMAFPhase4->SetDialogMode(TRUE);
	m_pMAFPhase4->Create(CDlgMAFPhase4::IDD);
	m_pMAFPhase4->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMafPhase5()
{
	if (NULL != m_pMAFPhase5)
	{
		m_pMAFPhase5->DestroyWindow();
		delete m_pMAFPhase5;
		m_pMAFPhase5 = NULL;
	}

	m_pMAFPhase5 = new CDlgMAFPhase5;
	m_pMAFPhase5->SetDialogMode(TRUE);
	m_pMAFPhase5->Create(CDlgMAFPhase5::IDD);
	m_pMAFPhase5->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMafPhase6()
{
	if (NULL != m_pMAFPhase6)
	{
		m_pMAFPhase6->DestroyWindow();
		delete m_pMAFPhase6;
		m_pMAFPhase6 = NULL;
	}

	m_pMAFPhase6 = new CDlgMAFPhase6;
	m_pMAFPhase6->SetDialogMode(TRUE);
	m_pMAFPhase6->Create(CDlgMAFPhase6::IDD);
	m_pMAFPhase6->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase1()
{
	if (NULL != m_pMARPhase1)
	{
		m_pMARPhase1->DestroyWindow();
		delete m_pMARPhase1;
		m_pMARPhase1 = NULL;
	}

	m_pMARPhase1 = new CDlgMARPhase1;
	m_pMARPhase1->SetDialogMode(TRUE);
	m_pMARPhase1->Create(CDlgMARPhase1::IDD);
	m_pMARPhase1->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase2()
{
	if (NULL != m_pMARPhase2)
	{
		m_pMARPhase2->DestroyWindow();
		delete m_pMARPhase2;
		m_pMARPhase2 = NULL;
	}

	m_pMARPhase2 = new CDlgMARPhase2;
	m_pMARPhase2->SetDialogMode(TRUE);
	m_pMARPhase2->Create(CDlgMARPhase2::IDD);
	m_pMARPhase2->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase3()
{
	if (NULL != m_pMARPhase3)
	{
		m_pMARPhase3->DestroyWindow();
		delete m_pMARPhase3;
		m_pMARPhase3 = NULL;
	}

	m_pMARPhase3 = new CDlgMARPhase3;
	m_pMARPhase3->SetDialogMode(TRUE);
	m_pMARPhase3->Create(CDlgMARPhase3::IDD);
	m_pMARPhase3->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase4()
{
	if (NULL != m_pMARPhase4)
	{
		m_pMARPhase4->DestroyWindow();
		delete m_pMARPhase4;
		m_pMARPhase4 = NULL;
	}

	m_pMARPhase4 = new CDlgMARPhase4;
	m_pMARPhase4->SetDialogMode(TRUE);
	m_pMARPhase4->Create(CDlgMARPhase4::IDD);
	m_pMARPhase4->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase5()
{
	if (NULL != m_pMARPhase5)
	{
		m_pMARPhase5->DestroyWindow();
		delete m_pMARPhase5;
		m_pMARPhase5 = NULL;
	}

	m_pMARPhase5 = new CDlgMARPhase5;
	m_pMARPhase5->SetDialogMode(TRUE);
	m_pMARPhase5->Create(CDlgMARPhase5::IDD);
	m_pMARPhase5->ShowWindow(SW_SHOW);
}

void CDASSimulationView::OnMarPhase6()
{
	if (NULL != m_pMARPhase6)
	{
		m_pMARPhase6->DestroyWindow();
		delete m_pMARPhase6;
		m_pMARPhase6 = NULL;
	}

	m_pMARPhase6 = new CDlgMARPhase6;
	m_pMARPhase6->SetDialogMode(TRUE);
	m_pMARPhase6->Create(CDlgMARPhase6::IDD);
	m_pMARPhase6->ShowWindow(SW_SHOW);
}


void CDASSimulationView::OnMarShowall()
{
	OnMarPhase1();
}

void CDASSimulationView::OnSysStructrue()
{
	if (NULL != m_pSysStructure && NULL != m_pSysStructure->GetSafeHwnd())
	{
		m_pSysStructure->ShowWindow(SW_SHOW);
	}
		
}

void CDASSimulationView::OnShowLeftDockpane()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pFrame)
	{
		pFrame->SetLeftViewerShow(TRUE);
		pFrame->PostMessage(WM_SHOW_HIDE_LEFTVIEW);
	}
}

void CDASSimulationView::OnHideLeftDockpane()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pFrame)
	{
		pFrame->SetLeftViewerShow(FALSE);
		pFrame->PostMessage(WM_SHOW_HIDE_LEFTVIEW);
	}
}

void CDASSimulationView::OnMafCloseLoopPlay()
{
	if (NULL != m_pMAFPhase1)
	{
		m_pMAFPhase1->SetDialogMode(FALSE);
		m_pMAFPhase1->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMAFPhase2)
	{
		m_pMAFPhase2->SetDialogMode(FALSE);
		m_pMAFPhase2->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMAFPhase3)
	{
		m_pMAFPhase3->SetDialogMode(FALSE);
		m_pMAFPhase3->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMAFPhase4)
	{
		m_pMAFPhase4->SetDialogMode(FALSE);
		m_pMAFPhase4->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMAFPhase5)
	{
		m_pMAFPhase5->SetDialogMode(FALSE);
		m_pMAFPhase5->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMAFPhase6)
	{
		m_pMAFPhase6->SetDialogMode(FALSE);
		m_pMAFPhase6->PostMessage(WM_CLOSE);
	}
}

void CDASSimulationView::OnMarCloseLoopPlay()
{
	if (NULL != m_pMARPhase1)
	{
		m_pMARPhase1->SetDialogMode(FALSE);
		m_pMARPhase1->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMARPhase2)
	{
		m_pMARPhase2->SetDialogMode(FALSE);
		m_pMARPhase2->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMARPhase3)
	{
		m_pMARPhase3->SetDialogMode(FALSE);
		m_pMARPhase3->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMARPhase4)
	{
		m_pMARPhase4->SetDialogMode(FALSE);
		m_pMARPhase4->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMARPhase5)
	{
		m_pMARPhase5->SetDialogMode(FALSE);
		m_pMARPhase5->PostMessage(WM_CLOSE);
	}
	if (NULL != m_pMARPhase6)
	{
		m_pMARPhase6->SetDialogMode(FALSE);
		m_pMARPhase6->PostMessage(WM_CLOSE);
	}
}
