// DlgMARPhase6.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMARPhase6.h"
#include "MainFrm.h"
#include "DASSimulationView.h"
#include "GlobalConstants.h"

#include <complex>

using namespace std;


enum ETimer
{
	enmTimer_Step1 = 1,
	enmTimer_Step2 = 2,
	enmTimer_Step3 = 3,
	enmTimer_Step4 = 4,
	enmTimer_Step5 = 5,
	enmTimer_Pause = 6,
	enmTimer_Flash = 10,


	enmTimer_Exit = 100,
};
// CDlgMARPhase6 dialog

IMPLEMENT_DYNAMIC(CDlgMARPhase6, CDialog)

CDlgMARPhase6::CDlgMARPhase6(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMARPhase6::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;

	m_bStep1Flash = FALSE;
	m_bStep2Flash = FALSE;
	m_bStep3Flash = FALSE;

	m_nCurLineLen = 10;
	m_nCurLineLen1 = 10;
	m_nTotalLen = 0;
	m_nTotalLen1 = 0;

	m_bPause = FALSE;
	m_bContinuePlay = FALSE;
	m_bUnExpMar = FALSE;
}

CDlgMARPhase6::~CDlgMARPhase6()
{
}

void CDlgMARPhase6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMARPhase6, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MAR6_PAUSE_FLASH, &CDlgMARPhase6::OnBnClickedBtnMar6PauseFlash)
END_MESSAGE_MAP()


// CDlgMARPhase6 message handlers
BOOL CDlgMARPhase6::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(enmTimer_Step1, 2000, NULL);

	if (!m_strTitle.IsEmpty())
	{
		SetWindowText(m_strTitle);
	}
	

	m_font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));                 // lpszFacename



	return TRUE; 
}

BOOL CDlgMARPhase6::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMARPhase6::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Step3);
	KillTimer(enmTimer_Exit);

	if (m_bContinuePlay)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrm)
		{
			CDASSimulationView* pViewer = (CDASSimulationView*)pMainFrm->GetActiveView();
			pViewer->OnMarPhase1();
		}
	}

	__super::OnClose();
}



void CDlgMARPhase6::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	if (m_bUnExpMar)
	{
		pDC->DrawText(_T("突发返向服务终止阶段(六)"), &rcTitle, DT_CENTER);
	}
	else
	{
		pDC->DrawText(_T("DAR服务终止阶段(六)"), &rcTitle, DT_CENTER);
	}
	

	pDC->SelectObject(pOldPen);
}


void CDlgMARPhase6::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CPaintDC dc(this);
		CRect rect;
		GetClientRect(rect);
		CBitmap bmp;
		if (bmp.LoadBitmap(IDB_MAR_PHASE_6_BITMAP))
		{
			BITMAP bmpInfo;
			bmp.GetBitmap(&bmpInfo);

			CDC dcMemory;
			dcMemory.CreateCompatibleDC(&dc);

			//将位图选入到内存DC中
			CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

			DrawClient(&dcMemory);

			dc.StretchBlt(0,0,rect.Width(),rect.Height(),
				&dcMemory,
				0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);

			dcMemory.SelectObject(pOldBitmap);

		}

		CDialog::OnPaint();
	}
}

void CDlgMARPhase6::DrawClient(CDC* pDC)
{

	CRect rect;
	GetClientRect(rect);

	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);

	
 	CFont* pOldFont = pDC->SelectObject(&m_font);
 	if (m_bStep1)
 	{
 
 		//绘制提示信息
 		CRect rcTip1;
 		rcTip1.left = m_rcControl.right+5;
 		rcTip1.right = m_rcDASCON.right+40;
 		rcTip1.top = m_rcDASCON.bottom+10;
 		rcTip1.bottom = rcTip1.top+80;

		pDC->Rectangle(&rcTip1);
 
 		CString strText("1.DASCON发送服务终止命令到DARCM和DSER。");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		//m_nTotalLen = rcDASCON.left - rcConDataInter.right;
 		//m_nTotalLen1 = rcConDataInter.left - rcDARCM.right;
 
 		//if (m_bStep1Flash)
 		//{
 		//	CPoint ptStart(rcDASCON.left, rcDASCON.top+rcDASCON.Height()/2);
 		//	CPoint ptEnd(rcDASCON.left-m_nCurLineLen, rcDASCON.top+rcDASCON.Height()/2);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 
 		//	ptStart.SetPoint(rcConDataInter.left, rcDARCM.top+rcDARCM.Height()/2);
 		//	ptEnd.SetPoint(rcConDataInter.left-m_nCurLineLen1, rcDARCM.top+rcDARCM.Height()/2);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 		//}
 		//else
 		//{
 		//	CPoint ptStart(rcDASCON.left, rcDASCON.top+rcDASCON.Height()/2);
 		//	CPoint ptEnd(rcConDataInter.right, rcDASCON.top+rcDASCON.Height()/2);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 
 		//	ptStart.SetPoint(rcConDataInter.left, rcDARCM.top+rcDARCM.Height()/2);
 		//	ptEnd.SetPoint(rcDARCM.right, rcDARCM.top+rcDARCM.Height()/2);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 		//}
 
 
 		////竖
 		//CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.bottom);
 		//CPoint ptDashEnd(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top+rcTip1.Height()/2);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 		////横
 		//ptDashStart.SetPoint(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top+rcTip1.Height()/2);
 		//ptDashEnd.SetPoint(rcTip1.left, rcTip1.top+rcTip1.Height()/2);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//DASCON-->Control
		CPoint ptStart(m_rcDASCON.left, m_rcDASCON.top+40);
		CPoint ptEnd(m_rcControl.right, m_rcDASCON.top+40);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//Control--->DARCM
		ptStart.SetPoint(m_rcControl.left, m_rcDARCM.top+20);
		ptEnd.SetPoint(m_rcDARCM.right, m_rcDARCM.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//DASCON-->DSER
		ptStart.SetPoint(m_rcDASCON.left+20, m_rcDASCON.top);
		ptEnd.SetPoint(m_rcDASCON.left+20, m_rcDSER.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		
		
		rcTip1.left = m_rcDBF.right+10;
		rcTip1.right = m_rcControl.left-5;
		rcTip1.top = rect.top+20;
		rcTip1.bottom = m_rcDARCM.top-5;
		
		pDC->Rectangle(&rcTip1);
		strText = "2.PTP终止数据格式化和路由,并且中断到用户POCC的通信链路。";
		DrawMultiLine(pDC, rcTip1, strText);

 	}
 
 	if (m_bStep2)
 	{
 		//绘制提示信息
 		CRect rcTip2;
 		rcTip2.left = m_rcDMG.right+10;
 		rcTip2.right = m_rcControl.left-10;
 		rcTip2.bottom = m_rcFDF.top;
 		rcTip2.top = m_rcDARCM.bottom+5;

		pDC->Rectangle(&rcTip2);
 
 		CString strText("3.DARCM系统接收服务终止命令,停止配置DBF和DMG。");
 		DrawMultiLine(pDC, rcTip2, strText);
 
 
 		//m_nTotalLen = rcDASCON.top - rcDSER.bottom;
 		//if (m_bStep2Flash)
 		//{
 		//	CPoint ptStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 		//	CPoint ptEnd(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top-m_nCurLineLen);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 
 		//}
 		//else
 		//{
 		//	CPoint ptStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 		//	CPoint ptEnd(rcDASCON.left+rcDASCON.Width()/2, rcDSER.bottom);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 
 		//}
 
 		////竖
 		//CPoint ptDashStart(rcDSER.left+rcDSER.Width()/2, rcDSER.top);
 		//CPoint ptDashEnd(rcDSER.left+rcDSER.Width()/2, rcDSER.top-20);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 		////横
 		//ptDashStart.SetPoint(rcDSER.left+rcDSER.Width()/2, rcDSER.top-20);
 		//ptDashEnd.SetPoint(rcTip2.left, rcDSER.top-20);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//DARCM-->DBF
		CPoint ptStart(m_rcDARCM.left, m_rcDARCM.top+20);
		CPoint ptEnd(m_rcDBF.right, m_rcDARCM.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//DARCM-->DMG
		ptStart.SetPoint(m_rcDARCM.left, m_rcDARCM.bottom-15);
		ptEnd.SetPoint(m_rcDMG.right, m_rcDARCM.bottom-15);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	if (m_bStep3)
 	{
 		//绘制提示信息
 		//CRect rcTip3;
 		//rcTip3.left = rcTermina.right-10;
 		//rcTip3.right = rcDARCMICON.right;
 		//rcTip3.top = rcDARCM.bottom+10;
 		//rcTip3.bottom = rcTermina.bottom+10;
 
 		//CString strText("3.DARCM系统接收服务终止命令,停止配置DBF和DMG。");
 		//DrawMultiLine(pDC, rcTip3, strText);
 
 		//m_nTotalLen = rcDARCMICON.top - rcDBFSub1.bottom;
 		//if (m_bStep3Flash)
 		//{
 		//	CPoint ptStart(rcDARCMICON.left+rcDARCMICON.Width()/2, rcDARCMICON.top);
 		//	CPoint ptEnd(rcDARCMICON.left+rcDARCMICON.Width()/2, rcDARCMICON.top-m_nCurLineLen);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(204,0,255));
 
 		//	ptStart.SetPoint(rcDARCMDCON.left+rcDARCMDCON.Width()/2, rcDARCMDCON.top);
 		//	ptEnd.SetPoint(rcDARCMDCON.left+rcDARCMDCON.Width()/2, rcDARCMICON.top-m_nCurLineLen);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(204,0,255));
 
 		//}
 		//else
 		//{
 		//	CPoint ptStart(rcDARCMICON.left+rcDARCMICON.Width()/2, rcDARCMICON.top);
 		//	CPoint ptEnd(rcDARCMICON.left+rcDARCMICON.Width()/2, rcDBFSub1.bottom);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(204,0,255));
 
 		//	ptStart.SetPoint(rcDARCMDCON.left+rcDARCMDCON.Width()/2, rcDARCMDCON.top);
 		//	ptEnd.SetPoint(rcDARCMDCON.left+rcDARCMDCON.Width()/2, rcDMGSub1.bottom);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(204,0,255));
 
 		//}
 		//
 
 		////竖
 		//CPoint ptDashStart(rcDARCMDCON.left, rcDARCM.bottom);
 		//CPoint ptDashEnd(rcDARCMDCON.left, rcTip3.top+30);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 		////横
 		//ptDashStart.SetPoint(rcDARCMDCON.left, rcTip3.top+30);
 		//ptDashEnd.SetPoint(rcTip3.right, rcTip3.top+30);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 	}
 
 	pDC->SelectObject(pOldFont);

}

void CDlgMARPhase6::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMARPhase6::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
{
	//透明：背景色不会显示，显示的只是白色刷子的颜色。
	//pDC->SetBkMode(TRANSPARENT); 

	CRect boundary;//设置文本要显示矩形区域
	boundary = rect;

	//使用DT_CALCRECT，DT_CENTER，DT_EDITCONTROL，DT_WORDBREAK参数计算需要的文本高度
	int height = pDC->DrawText(strText, rect, DT_CALCRECT|DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK);
	rect = boundary;//计算完成后将原来的区域赋回rect
	if(boundary.Height() > height)//判断高度是否超出范围，以免出现负数情况
		rect.top += (boundary.Height() - height)/2;//计算空白高度的一半，这里要用+=
	pDC->DrawText(strText, rect,DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK); //输出垂直居中+左右居中+自动换行的文本结果
}


static int s_step = 20;
void CDlgMARPhase6::OnTimer(UINT_PTR nIDEvent)
{
	if (enmTimer_Step1 == nIDEvent)
	{
		m_bStep1 = TRUE;
		Invalidate(FALSE);

		m_bStep1Flash = TRUE;

		KillTimer(enmTimer_Step1);
		SetTimer(enmTimer_Flash, 400, NULL);
		SetTimer(enmTimer_Step2, 10000, NULL);
	}
	else if (enmTimer_Step2 == nIDEvent)
	{
		m_bStep1Flash = FALSE;

		m_bStep2Flash = TRUE;
		m_bStep2 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step2);
		SetTimer(enmTimer_Step3, 10000, NULL);
	}
	else if (enmTimer_Step3 == nIDEvent)
	{
		m_bStep2Flash = FALSE;

		m_bStep3 = TRUE;
		m_bStep3Flash = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step3);
		SetTimer(enmTimer_Pause, 4000, NULL);
	}
	else if (enmTimer_Flash == nIDEvent)
	{
		if (m_bStep1Flash)
		{
			if (m_nCurLineLen >= m_nTotalLen)
			{
				m_nCurLineLen = 10;
			}
			else
			{
				m_nCurLineLen += s_step;
				if (m_nCurLineLen > m_nTotalLen)
				{
					m_nCurLineLen = m_nTotalLen;
				}
			}	

			if (m_nCurLineLen1 >= m_nTotalLen1)
			{
				m_nCurLineLen1 = 10;
			}
			else
			{
				m_nCurLineLen1 += s_step;
				if (m_nCurLineLen1 > m_nTotalLen1)
				{
					m_nCurLineLen1 = m_nTotalLen1;
				}
			}
		}
		else
		{
			if (m_nCurLineLen >= m_nTotalLen)
			{
				m_nCurLineLen = 10;
			}
			else
			{
				m_nCurLineLen += s_step;
				if (m_nCurLineLen > m_nTotalLen)
				{
					m_nCurLineLen = m_nTotalLen;
				}
			}	
		}


		//刷新下
		Invalidate(FALSE);
	}
	else if (enmTimer_Pause == nIDEvent)
	{
		KillTimer(enmTimer_Flash);
		m_bStep3Flash = FALSE;
		Invalidate(FALSE);

		KillTimer(enmTimer_Pause);	
		SetTimer(enmTimer_Exit, 4000, NULL);
	}
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMARPhase6::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
								 double nArrowBorderLen, double xAngleInRadians, COLORREF cr)
{
	const double PI = 3.1415926;

	double xAngleLine = 0;
	if (lpPointStart->x == lpPointEnd->x)
		xAngleLine = PI / 2;
	else
	{
		xAngleLine = atan((double(lpPointEnd->y) - double(lpPointStart->y)) / (double(lpPointEnd->x) - double(lpPointStart->x)));
	}

	CPoint ptLineLeft;
	if (lpPointStart->x > lpPointEnd->x)
	{
		ptLineLeft.x = long(lpPointEnd->x /*+*/- cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		ptLineLeft.y = long(lpPointEnd->y /*+*/- sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
	}
	else if (lpPointStart->x == lpPointEnd->x)
	{
		if (lpPointStart->y > lpPointEnd->y)
		{
			ptLineLeft.x = lpPointStart->x-5;
			ptLineLeft.y = lpPointEnd->y + 5;
		}
		else
		{
			ptLineLeft.x = lpPointEnd->x-5;
			ptLineLeft.y = lpPointEnd->y-5;
		}

	}
	else
	{
		ptLineLeft.x = long(lpPointEnd->x /*-*/+ cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		ptLineLeft.y = long(lpPointEnd->y /*-*/+ sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
	}

	CPoint ptLineRight;
	if (lpPointStart->x > lpPointEnd->x)
	{
		ptLineRight.x = long(lpPointEnd->x /*+*/- cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		ptLineRight.y = long(lpPointEnd->y /*+*/- sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
	}
	else if (lpPointStart->x == lpPointEnd->x)
	{
		if (lpPointStart->y > lpPointEnd->y)
		{
			ptLineRight.x = lpPointEnd->x + 5;
			ptLineRight.y = lpPointEnd->y + 5;
		}
		else
		{
			ptLineRight.x = lpPointEnd->x + 5;
			ptLineRight.y = lpPointEnd->y - 5;
		}

	}
	else
	{
		ptLineRight.x = long(lpPointEnd->x /*-*/+ cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		ptLineRight.y = long(lpPointEnd->y /*-*/+ sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
	}

	HPEN hPen = ::CreatePen(PS_SOLID, 4, cr);
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = cr;
	HBRUSH hBrush = ::CreateBrushIndirect(&lb);

	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	// Draw line
	::MoveToEx(hDC, lpPointStart->x, lpPointStart->y, NULL);
	::LineTo(hDC, lpPointEnd->x, lpPointEnd->y);

	// Draw arrow
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	POINT ptPolygon[3] = {lpPointEnd->x, lpPointEnd->y, ptLineLeft.x, ptLineLeft.y, ptLineRight.x, ptLineRight.y};
	::Polygon(hDC, ptPolygon, 3);

	::SelectObject(hDC, hOldBrush);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hBrush);
	::DeleteObject(hPen);

	return 0;
}

void CDlgMARPhase6::OnBnClickedBtnMar6PauseFlash()
{
	if (m_bPause)
	{
		m_bPause = FALSE;
		GetDlgItem(IDC_BTN_MAR6_PAUSE_FLASH)->SetWindowText(_T("暂定"));
		SetTimer(enmTimer_Step1, 500, NULL);
	}
	else
	{
		m_bPause = TRUE;
		GetDlgItem(IDC_BTN_MAR6_PAUSE_FLASH)->SetWindowText(_T("开始"));
		m_bStep1 = FALSE;
		m_bStep2 = FALSE;
		m_bStep3 = FALSE;
		m_bStep4 = FALSE;
		m_bStep1Flash = FALSE;
		m_bStep2Flash = FALSE;
		m_bStep3Flash = FALSE;

		KillTimer(enmTimer_Step1);
		KillTimer(enmTimer_Step2);
		KillTimer(enmTimer_Step3);
		KillTimer(enmTimer_Step4);
		KillTimer(enmTimer_Flash);
		KillTimer(enmTimer_Pause);
		KillTimer(enmTimer_Exit);
	}
}
