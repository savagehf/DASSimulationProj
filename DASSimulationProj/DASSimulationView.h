/************************************************************************/
/*	DAS System
	Created : 2013.3.1
	Modified: 2014.5.1
	Modified: lee
/************************************************************************/


#pragma once

#include "BusinessDataDefine.h"

class CDASSimulationDoc;
class CDlgMAFPhase1;
class CDlgMAFPhase2;
class CDlgMAFPhase3;
class CDlgMAFPhase4;
class CDlgMAFPhase5;
class CDlgMAFPhase6;
class CDlgMARPhase1;
class CDlgMARPhase2;
class CDlgMARPhase3;
class CDlgMARPhase4;
class CDlgMARPhase5;
class CDlgMARPhase6;

class CSysModuleIntroduction;

class CDASSimulationView : public CView
{
protected: 
	CDASSimulationView();
	DECLARE_DYNCREATE(CDASSimulationView)

public:
	enum
	{
		eTimer_DrawSence = 1,
		//eTimer_CountDown = 2,
		//eTimer_CountDownExpired = 3,

		eTimer_MAF_DrawTer2TDRS = 4,
		eTimer_MAF_DrawTDRS2US = 5,

		eTimer_MAR_DrawTDRS2Ter = 6,
		eTimer_MAR_DrawTer2UserCen = 7,

		//设置用户发送用户请求时的一个信号线
		enmTimer_Phase1_Request = 20,
		enmTimer_Phase1_Answer  = 21,

		//双向任务时的用户请求
		enmTimer_Phase1_MAFMAR_Request = 23,
		enmTimer_Phase1_MAFMAR_Answer = 24,

		enmTimer_Phase1_UnexpectedMar_Request = 25,
		enmTimer_Phase1_UnexpectedMar_Answer = 26,

		enmTimer_Phase2_MAFMAR_Answer = 27,

		enmTimer_Phase1_Occupy_Notify = 28,
	};
public:
	void PreSimulatestkData();
	void GotonewviewTime();
	
	void StartDrawUnexpectedRequestSingal(BOOL bMAF = FALSE);
	void StartDrawUnexpectedAnswerSignal(BOOL bMAF);
	
	void StartDrawPhase1RequestSignal(BOOL bMAF);
	void StartDrawPhase1AnswerSignal(BOOL bMAF);
	
	void StartDrawMAFMARPhase1ReuestSignal(BOOL bMAF);
	void StartDrawMAFMARPhase1AnswerSignal(BOOL bMAF);
	void StartDrawPhase2AnswerSignal(BOOL bMAF);
	
	void StartSimmulate(ETaskType eTaskType);
	void NotifyBeOccupied();
	//void CheckIfTaskAvailable();
	//BOOL AnyTaskPrepare();


	CDASSimulationDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	
	//void LoadTdrs(CString filename);
	//void DrawString2f(float x, float y, CString content);	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CDASSimulationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bMAF;
	BOOL m_bMafMar;
	//TaskItem m_taskItemTobeShown;
	//CDlgMARPhase3* m_pmarPhase3;
	//CDlgMAFPhase3* m_pMAFPhase3;
	
	
	CSysModuleIntroduction* m_pSysStructure;

	CDlgMAFPhase1* m_pMAFPhase1;
	CDlgMAFPhase2* m_pMAFPhase2;
	CDlgMAFPhase3* m_pMAFPhase3;
	CDlgMAFPhase4* m_pMAFPhase4;
	CDlgMAFPhase5* m_pMAFPhase5;
	CDlgMAFPhase6* m_pMAFPhase6;

	CDlgMARPhase1* m_pMARPhase1;
	CDlgMARPhase2* m_pMARPhase2;
	CDlgMARPhase3* m_pMARPhase3;
	CDlgMARPhase4* m_pMARPhase4;
	CDlgMARPhase5* m_pMARPhase5;
	CDlgMARPhase6* m_pMARPhase6;

	
public:
	afx_msg void OnMafPhase_1();
	afx_msg void OnMafPhase2();
	afx_msg void OnMafPhase3();
	afx_msg void OnMafPhase4();
	afx_msg void OnMafPhase5();
	afx_msg void OnMafPhase6();
	afx_msg void OnMarPhase1();
	afx_msg void OnMarPhase2();
	afx_msg void OnMarPhase3();
	afx_msg void OnMarPhase4();
	afx_msg void OnMarPhase5();
	afx_msg void OnMarPhase6();
	afx_msg void OnMafShowall();
	afx_msg void OnMarShowall();
	afx_msg void OnSysStructrue();
	afx_msg void OnShowLeftDockpane();
	afx_msg void OnHideLeftDockpane();
	afx_msg void OnMafCloseLoopPlay();
	afx_msg void OnMarCloseLoopPlay();
};

#ifndef _DEBUG  // debug version in DASSimulationView.cpp
inline CDASSimulationDoc* CDASSimulationView::GetDocument() const
   { return reinterpret_cast<CDASSimulationDoc*>(m_pDocument); }
#endif

