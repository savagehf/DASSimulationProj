// DlgMAFMARPhase6.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFMARPhase6.h"
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

// CDlgMAFMARPhase6 dialog

IMPLEMENT_DYNAMIC(CDlgMAFMARPhase6, CDialog)

CDlgMAFMARPhase6::CDlgMAFMARPhase6(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFMARPhase6::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;

	m_bStep1Flash = FALSE;
	m_bStep2Flash = FALSE;
	m_bStep3Flash = FALSE;

	m_nCurrLineLen  = 10;
	m_nTotalLineLen = 0;

	m_bPause = FALSE;

	m_bContinuePlay = FALSE;
}

CDlgMAFMARPhase6::~CDlgMAFMARPhase6()
{
}

void CDlgMAFMARPhase6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFMARPhase6, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MAFMAR6_PAUSE_FLASH, OnBnClickedBtnMafMar6PauseFlash)
END_MESSAGE_MAP()

BOOL CDlgMAFMARPhase6::OnInitDialog()
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

BOOL CDlgMAFMARPhase6::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFMARPhase6::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Step3);
	KillTimer(enmTimer_Flash);
	KillTimer(enmTimer_Pause);
	KillTimer(enmTimer_Exit);

	if (m_bContinuePlay)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrm)
		{
			CDASSimulationView* pViewer = (CDASSimulationView*)pMainFrm->GetActiveView();
			//pViewer->OnMafPhase_1();
		}
	}

	__super::OnClose();
}


void CDlgMAFMARPhase6::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("双向服务终止阶段(六)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


void CDlgMAFMARPhase6::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAFMAR_PHASE_6_BITMAP))
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

void CDlgMAFMARPhase6::DrawClient(CDC* pDC)
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
 		rcTip1.left = m_rcPOCC.left-5;
 		rcTip1.right = m_rcPOCC.right+20;
 		rcTip1.top = m_rcPOCC.top;
 		rcTip1.bottom = m_rcPOCC.bottom;
		pDC->Rectangle(&rcTip1);
 
 		CString strText("1.DASCON发送终止命令到DARCM和DSER，同时通报给NCC");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		
 		/*m_nTotalLineLen = rcIONET.left - rcDASCON.right;
 
 		if (m_bStep1Flash)
 		{
 			CPoint ptStart(rcDASCON.right, rcDASCON.top+rcDASCON.Height()/2);
 			CPoint ptEnd(rcDASCON.right+m_nCurrLineLen, rcDASCON.top+rcDASCON.Height()/2);
 			DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 				&ptEnd, 8, 10,RGB(0,204,255));
 		}
 		else
 		{
 			CPoint ptStart(rcDASCON.right, rcDASCON.top+rcDASCON.Height()/2);
 			CPoint ptEnd(rcIONET.left, rcDASCON.top+rcDASCON.Height()/2);
 			DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 				&ptEnd, 8, 10,RGB(0,204,255));
 		}
 		
 		CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.bottom);
 		CPoint ptDashEnd(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top+5);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int abc;
		//dascon-->ionet
		CPoint ptStart(m_rcDASCON.left+20, m_rcDASCON.bottom);
		CPoint ptEnd(m_rcDASCON.left+20, m_rcIONET.top+5);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		//先绘制一条直线，然后再绘制一条的带箭头的线
		ptStart.SetPoint(m_rcNCC.right+200,m_rcIONET.bottom-30);
		ptEnd.SetPoint(m_rcNCC.right+200, m_rcNCC.top+40);
		DrawOneLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, RGB(255,0,255));
		//ionet-->ncc
		ptStart.SetPoint(m_rcNCC.right+200, m_rcNCC.top+40);
		ptEnd.SetPoint(m_rcNCC.right, m_rcNCC.top+40);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//dascon-->dser
		ptStart.SetPoint(m_rcDSER.left+20, m_rcDASCON.top);
		ptEnd.SetPoint(m_rcDSER.left+20, m_rcDSER.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//dascon-->controlnet
		ptStart.SetPoint(m_rcDASCON.left, m_rcDASCON.top+30);
		ptEnd.SetPoint(m_rcControl.right, m_rcDASCON.top+30);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//controlnet-->darcm
		ptStart.SetPoint(m_rcControl.left, m_rcDARCM.top+30);
		ptEnd.SetPoint(m_rcDARCM.right, m_rcDARCM.top+30);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		/*CRect rcTip3;
		rcTip3.left = rect.left+2;
		rcTip3.right = m_rcNCC.left-2;
		rcTip3.top = m_rcNCC.top-40;
		rcTip3.bottom = m_rcNCC.bottom;
		pDC->Rectangle(&rcTip3);

		strText = "2.NCC接收到服务终止命令，停止配置DAF资源。";
		DrawMultiLine(pDC, rcTip3, strText);*/
 
 	}
 
 	if (m_bStep2)
 	{
		CRect rcTip3;
		rcTip3.left = rect.left+2;
		rcTip3.right = m_rcNCC.left-2;
		rcTip3.top = m_rcNCC.top-40;
		rcTip3.bottom = m_rcNCC.bottom;
		pDC->Rectangle(&rcTip3);

		CString strText("2.NCC接收到服务终止命令，停止配置DAF资源。");
		DrawMultiLine(pDC, rcTip3, strText);

		//ncc-->SGLT
		CPoint ptStart(m_rcNCC.left, m_rcNCC.top);
		CPoint ptEnd(m_rcNCC.left, m_rcSGLT.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

 	}
 
 	if (m_bStep3)
 	{
		//绘制提示信息
		CRect rcTip2;
		rcTip2.top = m_rcDSER.bottom;
		rcTip2.bottom = m_rcDARCM.bottom;
		rcTip2.left = m_rcDARCM.left;
		rcTip2.right = m_rcDSER.left-5;
		pDC->Rectangle(&rcTip2);

		CString strText("3.DAF设备接收到服务终止命令，停止工作。");
		DrawMultiLine(pDC, rcTip2, strText);


		CRect rcTip4;
		rcTip4.left = rect.left+3;
		rcTip4.right = m_rcSGLT.right+5;
		rcTip4.top = m_rcSGLT.bottom+5;
		rcTip4.bottom = m_rcFDF.bottom;
		pDC->Rectangle(&rcTip4);

		CString strText2("4.PTP终止数据格式化和路由，并且中断到用户POCC的通信链路。");
		DrawMultiLine(pDC, rcTip4, strText2);

 
 	}
 
 	if (m_bStep4)
 	{
		CRect rcTip5;
		rcTip5 = m_rcSGLT;
		pDC->Rectangle(&rcTip5);
		CString str(_T(" 5.DARCM系统接收服务终止命令，停止配置DBF和DMG。"));
		DrawMultiLine(pDC, rcTip5, str);

		//DARCM-DBF，DARCM-DMG
		CPoint ptStart(m_rcDARCM.left, m_rcDARCM.top+5);
		CPoint ptEnd(m_rcDBF.right, m_rcDARCM.top+5);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		ptStart.SetPoint(m_rcDARCM.left, m_rcDARCM.bottom);
		ptEnd.SetPoint(m_rcDMG.right, m_rcDARCM.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

 	}
 
 	pDC->SelectObject(pOldFont);

}


void CDlgMAFMARPhase6::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFMARPhase6::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFMARPhase6::OnTimer(UINT_PTR nIDEvent)
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
		SetTimer(enmTimer_Step4, 10000, NULL);
	}
	else if (enmTimer_Step4 == nIDEvent)
	{
		KillTimer(enmTimer_Flash);
		m_bStep3Flash = FALSE;

		m_bStep4 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step4);
		SetTimer(enmTimer_Pause, 4000, NULL);
	}
	else if (enmTimer_Flash == nIDEvent)
	{		
		if (m_nCurrLineLen >= m_nTotalLineLen)
		{
			m_nCurrLineLen = 10;
		}
		else
		{
			m_nCurrLineLen += s_step;
			if (m_nCurrLineLen > m_nTotalLineLen)
			{
				m_nCurrLineLen = m_nTotalLineLen;
			}
		}	

		//刷新下
		Invalidate(FALSE);
	}
	else if (enmTimer_Pause == nIDEvent)
	{
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

void CDlgMAFMARPhase6::DrawOneLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, COLORREF cr)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 4, cr);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	// Draw line
	::MoveToEx(hDC, lpPointStart->x, lpPointStart->y, NULL);
	::LineTo(hDC, lpPointEnd->x, lpPointEnd->y);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);

}

int CDlgMAFMARPhase6::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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

void CDlgMAFMARPhase6::OnBnClickedBtnMafMar6PauseFlash()
{
	if (m_bPause)
	{
		m_bPause = FALSE;
		GetDlgItem(IDC_BTN_MAFMAR6_PAUSE_FLASH)->SetWindowText(_T("暂定"));
		SetTimer(enmTimer_Step1, 500, NULL);
	}
	else
	{
		m_bPause = TRUE;
		GetDlgItem(IDC_BTN_MAFMAR6_PAUSE_FLASH)->SetWindowText(_T("开始"));
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
