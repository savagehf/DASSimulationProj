// DlgMAFPhase4.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFPhase4.h"
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

	enmTimer_Exit = 100,
};
// CDlgMAFPhase4 dialog

IMPLEMENT_DYNAMIC(CDlgMAFPhase4, CDialog)

CDlgMAFPhase4::CDlgMAFPhase4(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFPhase4::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMAFPhase4::~CDlgMAFPhase4()
{
}

void CDlgMAFPhase4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFPhase4, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgMAFPhase4 message handlers
BOOL CDlgMAFPhase4::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(enmTimer_Step1, 2000, NULL);

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

BOOL CDlgMAFPhase4::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFPhase4::OnClose()
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
			pViewer->OnMafPhase5();
		}
	}

	__super::OnClose();
}


void CDlgMAFPhase4::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("DAF服务实施阶段(四)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


void CDlgMAFPhase4::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAF_PHASE_4_BITMAP))
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

void CDlgMAFPhase4::DrawClient(CDC* pDC)
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
 		rcTip1.top = rect.top+20;
 		rcTip1.bottom = m_rcDARCM.bottom;
 		rcTip1.left = m_rcDARCM.left-6;
 		rcTip1.right = m_rcControl.right;

		pDC->Rectangle(&rcTip1);
 
 		CString strText("1.PTP以1次/秒的频率向DASCON报告系统状态(DSER_Status).");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		/*CPoint ptStart(rcDSER.left+rcDSER.Width()/2, rcDSER.bottom);
 		CPoint ptEnd(rcDSER.left+rcDSER.Width()/2, rcDASCON.top);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(0,204,255));*/
		int abc;
		//dser-->dascon
		CPoint ptStart(m_rcDSER.left+20, m_rcDSER.bottom);
		CPoint ptEnd(m_rcDSER.left+20, m_rcDASCON.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

 	}
 
 	if (m_bStep2)
 	{
 		//绘制提示信息
 		CRect rcTip2;
 		rcTip2.right = m_rcControl.right;
 		rcTip2.left = m_rcDARCM.left-6;
 		rcTip2.top = m_rcDARCM.bottom+4;
 		rcTip2.bottom = m_rcControl.bottom;
 		pDC->Rectangle(&rcTip2);
 		CString strText("2.DASCON采集PTP状态信息,并且以1次/秒的频率通过SWSI终端向用户POCC报告服务状态,必要时,发送警告信息。");
 		DrawMultiLine(pDC, rcTip2, strText);
 
 		/*CPoint ptStart(rcDASCON.right, rcDASCON.top+rcDASCON.Height()/2);
 		CPoint ptEnd(rcIONET.left, rcDASCON.top+rcDASCON.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(204,0,255));
 
 		ptStart.SetPoint(rcIONET.right, rcDASCON.top+rcDASCON.Height()/2);
 		ptEnd.SetPoint(rcSWSI.left,rcDASCON.top+rcDASCON.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(204,0,255));
 
 		ptStart.SetPoint(rcSWSI.left+rcSWSI.Width()/2, rcSWSI.bottom);
 		ptEnd.SetPoint(rcSWSI.left+rcSWSI.Width()/2, rcPOCC.top);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(204,0,255));
 
 		CPoint ptDashStart(rcDASCON.left, rcDASCON.top);
 		CPoint ptDashEnd(rcTip2.right, rcTip2.top+rcTip2.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

		int abc;
		//dascon-->ionet
		CPoint ptStart(m_rcDASCON.left+70, m_rcDASCON.bottom);
		CPoint ptEnd(m_rcDASCON.left+70, m_rcIONET.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//ionet-->pocc
		ptStart.SetPoint(m_rcDASCON.left+70, m_rcIONET.bottom-25);
		ptEnd.SetPoint(m_rcDASCON.left+70, m_rcPOCC.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		
		CRect rcTip3;
		rcTip3.left = m_rcNCC.right+2;
		rcTip3.right = m_rcPOCC.left-2;
		rcTip3.top = m_rcPOCC.top-40;
		rcTip3.bottom = m_rcPOCC.bottom;

		pDC->Rectangle(&rcTip3);
		strText = "3.用户POCC接收服务状态信息,并且在服务实施阶段可以随时发送控制命令(Commands)到DASCON以对服务做出控制和调整。";
		DrawMultiLine(pDC, rcTip3, strText);

 	}
 
 	if (m_bStep3)
 	{
 		//绘制提示信息
 		/*CRect rcTip3;
 		rcTip3.left = rcDASCON.left-10;
 		rcTip3.right = rcPOCC.left-20;
 		rcTip3.top = rcIONET.bottom+5;
 		rcTip3.bottom = rcPOCC.bottom+10;
 
 		CString strText("3.用户POCC接收服务状态信息,并且在服务实施阶段可以随时发送控制命令(Commands)到DASCON以对服务做出控制和调整。");
 		DrawMultiLine(pDC, rcTip3, strText);
 
 		CPoint ptEnd(rcSWSI.left+rcSWSI.Width()/2+15, rcSWSI.bottom);
 		CPoint ptStart(rcSWSI.left+rcSWSI.Width()/2+15, rcPOCC.top);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(255,204,0));
 
 		ptEnd.SetPoint(rcIONET.right, rcDASCON.top+rcDASCON.Height()/2-15);
 		ptStart.SetPoint(rcSWSI.left, rcDASCON.top+rcDASCON.Height()/2-15);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(255,204,0));
 
 		ptEnd.SetPoint(rcDASCON.right, rcDASCON.top+rcDASCON.Height()/2-15);
 		ptStart.SetPoint(rcIONET.left, rcDASCON.top+rcDASCON.Height()/2-15);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(255,204,0));
 
 		CPoint ptDashStart(rcPOCC.left, rcPOCC.top+rcPOCC.Height()/2);
 		CPoint ptDashEnd(rcTip3.right, rcPOCC.top+rcPOCC.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int abc;
		///pocc-->ionet
		CPoint ptStart(m_rcDASCON.left+17, m_rcPOCC.top);
		CPoint ptEnd(m_rcDASCON.left+17, m_rcIONET.bottom-20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		//ionet-->dascon
		ptStart.SetPoint(m_rcDASCON.left+17, m_rcIONET.top+10);
		ptEnd.SetPoint(m_rcDASCON.left+17, m_rcDASCON.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	pDC->SelectObject(pOldFont);

}

void CDlgMAFPhase4::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFPhase4::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFPhase4::OnTimer(UINT_PTR nIDEvent)
{
	if (enmTimer_Step1 == nIDEvent)
	{
		m_bStep1 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step1);
		SetTimer(enmTimer_Step2, 10000, NULL);
	}
	else if (enmTimer_Step2 == nIDEvent)
	{
		m_bStep2 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step2);
		SetTimer(enmTimer_Step3, 10000, NULL);
	}
	else if (enmTimer_Step3 == nIDEvent)
	{
		m_bStep3 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step3);
		SetTimer(enmTimer_Exit, 10000, NULL);
	}
	
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMAFPhase4::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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
