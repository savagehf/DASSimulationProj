// DlgMAFTermFlash.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFTermFlash.h"
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
// CDlgMAFTermFlash dialog

static int s_step = 40;

IMPLEMENT_DYNAMIC(CDlgMAFTermFlash, CDialog)

CDlgMAFTermFlash::CDlgMAFTermFlash(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFTermFlash::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep1Flash = FALSE;
	m_bStep2Flash = FALSE;
	m_nCurLineLen = 10;
	m_nTotalLineLen = 0;

	m_bPause = FALSE;
}

CDlgMAFTermFlash::~CDlgMAFTermFlash()
{
}

void CDlgMAFTermFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFTermFlash, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_PAUSE_MAF_FLASH, &CDlgMAFTermFlash::OnBnClickedBtnPauseMafFlash)
END_MESSAGE_MAP()


BOOL CDlgMAFTermFlash::OnInitDialog()
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

BOOL CDlgMAFTermFlash::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFTermFlash::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Pause);
	KillTimer(enmTimer_Exit);
	KillTimer(enmTimer_Flash);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pMain)
	{
		CDASSimulationView* pView = (CDASSimulationView*)pMain->GetActiveView();
		pView->StartDrawPhase1AnswerSignal(TRUE);
	}
	
	__super::OnClose();
}
void CDlgMAFTermFlash::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAF_MAR_TERMINATION))
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

void CDlgMAFTermFlash::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("DAF服务规划阶段(一)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFTermFlash::DrawClient(CDC* pDC)
{

	CRect rect;
	GetClientRect(rect);

	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);

 	CFont* pOldFont = pDC->SelectObject(&m_font);
 
 	if (m_bStep1)
 	{
 		CRect rcTip1;
 		rcTip1.left = m_rcDARCM.left+55;
 		rcTip1.right = m_rcControl.right+55;
 		rcTip1.top = m_rcDARCM.bottom+10+60;
 		rcTip1.bottom = m_rcControl.bottom+10;
		
		pDC->Rectangle(&rcTip1);
 
 		CString str(_T("1.用户POCC通过DAS规划工具向DASCON发起服务规划请求:Planning info Request."));
 		DrawMultiLine(pDC, rcTip1, str);
 
		CPoint ptStart(m_rcDASCON.left+20+70, m_rcDASCON.bottom+60+40);
		CPoint ptEnd(m_rcDASCON.left+20+70, m_rcDASCON.bottom+50);
		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,&ptEnd, 8, 10,RGB(255,0,255));
 
 		//CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.bottom);
 		//CPoint ptDashEnd(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 
 		//m_nTotalLineLen = (rect.right-20) - (rcDASCON.right);
 
 		//// 1.闪烁阶段
 		//if (m_bStep1Flash)
 		//{
 		//	CPoint ptStart(rect.right-20, rcDASCON.top+40);
 		//	CPoint ptEnd(rect.right-20 - m_nCurLineLen , rcDASCON.top+40);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 		//}
 		//// 2.闪烁结束了
 		//else 
 		//{
 		//	CPoint ptStart(rect.right-20, rcDASCON.top+40);
 		//	CPoint ptEnd(rcDASCON.right, rcDASCON.top+40);
 		//	DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//		&ptEnd, 8, 10,RGB(0,204,255));
 		//}
 	}
 
 	if (m_bStep2)
 	{
 
 		//绘制提示信息
 		CRect rcTip1;
 		rcTip1.left = m_rcControl.right+50+15;
 		rcTip1.right = m_rcDASCON.right+60+15;
 		rcTip1.top = m_rcDSER.top+50;
		rcTip1.bottom = m_rcDASCON.top;
		pDC->Rectangle(&rcTip1);
 
 		CString strText("2.DASCON检验用户所请求的DAF服务是否在TUT时间段内，返回服务请求响应。");
 		DrawMultiLine(pDC, rcTip1, strText);
 
		CPoint ptStart(m_rcDASCON.right+50, m_rcDASCON.bottom+50);
		CPoint ptEnd(m_rcDASCON.right+50, m_rcDASCON.bottom+40+60);
		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,&ptEnd, 8, 10,RGB(255,0,255));
 		/*CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 		CPoint ptDashEnd(rcDASCON.left+rcDASCON.Width()/2, rcTip1.bottom);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 
 		m_nTotalLineLen = (rect.right-20) - (rcDASCON.right);
 
 		if (m_bStep2Flash)
 		{
 			CPoint ptStart(rcDASCON.right, rcDASCON.top+10);
 			CPoint ptEnd(rcDASCON.right + m_nCurLineLen, rcDASCON.top+10);
 			DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 				&ptEnd, 8, 10,RGB(204,0,255));
 		}
 		else
 		{
 			CPoint ptStart(rcDASCON.right, rcDASCON.top+10);
 			CPoint ptEnd(rect.right-20, rcDASCON.top+10);
 			DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 				&ptEnd, 8, 10,RGB(204,0,255));
 		}	*/
 
 	}
 

	// put back the old objects
	//选回系统默认的刷子，取消蓝刷
	pDC->SelectObject(pOldFont);

}


void CDlgMAFTermFlash::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFTermFlash::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFTermFlash::OnTimer(UINT_PTR nIDEvent)
{
	if (enmTimer_Step1 == nIDEvent)
	{
		m_bStep1 = TRUE;
		Invalidate(FALSE);
		m_bStep1Flash = TRUE;
		KillTimer(enmTimer_Step1);
		SetTimer(enmTimer_Flash, 400, NULL);
		//m_timeBegin = ::GetCurrentTime();

		SetTimer(enmTimer_Step2, 10000, NULL);
	}
	else if (enmTimer_Step2 == nIDEvent)
	{
		//第一阶段的flash结束
		m_bStep1Flash = FALSE;
		m_nCurLineLen = 0;
		m_nTotalLineLen = 0;


		//第二阶段的flash开始
		m_bStep2 = TRUE;
		m_bStep2Flash = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step2);
		SetTimer(enmTimer_Pause, 10000, NULL);
	}
	else if (enmTimer_Pause == nIDEvent)
	{
		KillTimer(enmTimer_Flash);
		m_bStep2Flash = FALSE;
		//m_nCurLineLen = m_nTotalLineLen;
		Invalidate(FALSE);

		KillTimer(enmTimer_Pause);
		SetTimer(enmTimer_Exit, 4000, NULL);
	}
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	else if (enmTimer_Flash == nIDEvent)
	{
		//统计共flash了多长时间。
		//m_nFlashTotalTime += 400;

		if (m_nCurLineLen >= m_nTotalLineLen)
		{
			m_nCurLineLen = 10;
		}
		else
		{
			m_nCurLineLen += s_step;
			if (m_nCurLineLen > m_nTotalLineLen)
			{
				m_nCurLineLen = m_nTotalLineLen;
			}
		}

		//刷新下
		Invalidate(FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMAFTermFlash::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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

void CDlgMAFTermFlash::OnBnClickedBtnPauseMafFlash()
{

	if (m_bPause)
	{
		m_bPause = FALSE;
		GetDlgItem(IDC_BTN_PAUSE_MAF_FLASH)->SetWindowText(_T("暂定"));
		SetTimer(enmTimer_Step1, 500, NULL);
	}
	else
	{
		m_bPause = TRUE;
		GetDlgItem(IDC_BTN_PAUSE_MAF_FLASH)->SetWindowText(_T("开始"));
		m_bStep1 = FALSE;
		m_bStep2 = FALSE;
		m_bStep1Flash = FALSE;
		m_bStep2Flash = FALSE;

		KillTimer(enmTimer_Step1);
		KillTimer(enmTimer_Step2);
		KillTimer(enmTimer_Flash);
		KillTimer(enmTimer_Pause);
		KillTimer(enmTimer_Exit);
	}

}
