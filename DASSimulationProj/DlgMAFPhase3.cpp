// DlgMAFPhase3.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFPhase3.h"
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
// CDlgMAFPhase3 dialog

IMPLEMENT_DYNAMIC(CDlgMAFPhase3, CDialog)

CDlgMAFPhase3::CDlgMAFPhase3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFPhase3::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;

	m_bStep1Flash = FALSE;
	m_bStep2Flash = FALSE;
	m_bStep4Flash = FALSE;

	m_nCurLineLen = 10;
	m_nTotalLen = 0;

	m_nTaskStartGap = 0;
	m_bPause = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMAFPhase3::~CDlgMAFPhase3()
{
}

void CDlgMAFPhase3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFPhase3, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MAF3_PAUSE_FLASH, &CDlgMAFPhase3::OnBnClickedBtnMaf3PauseFlash)
END_MESSAGE_MAP()


// CDlgMAFPhase3 message handlers
BOOL CDlgMAFPhase3::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(enmTimer_Step1, 2000, NULL);

	if (!m_strTitle.IsEmpty())
	{
		SetWindowText(m_strTitle);
	}
	
	//必须大于0，否则，定时器导致的日历弹不出来的。
	if (m_nTaskStartGap>0)
	{
		SetTimer(enmTimer_Exit, m_nTaskStartGap*1000, NULL);
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

BOOL CDlgMAFPhase3::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFPhase3::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Step3);
	KillTimer(enmTimer_Step4);
	KillTimer(enmTimer_Flash);
	KillTimer(enmTimer_Pause);
	KillTimer(enmTimer_Exit);

	if (m_bContinuePlay)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrm)
		{
			CDASSimulationView* pViewer = (CDASSimulationView*)pMainFrm->GetActiveView();
			pViewer->OnMafPhase4();
		}
	}

	
	__super::OnClose();
}


void CDlgMAFPhase3::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("DAF服务准备阶段(三)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


void CDlgMAFPhase3::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAF_PHASE_3_BITMAP))
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

void CDlgMAFPhase3::DrawClient(CDC* pDC)
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
 		rcTip1.left = m_rcPOCC.left;
 		rcTip1.right = m_rcIONET.right+5;
 		rcTip1.top = m_rcIONET.bottom;
 		rcTip1.bottom = m_rcPOCC.bottom;
		pDC->Rectangle(&rcTip1);
 		CString strText("1.DASCON转发用户DAF服务规格(DA_SHO)到NCC，并且配置DSER系统。");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 
 		/*m_nTotalLen = rcIONET.left -rcDASCON.right;
 		if (m_bStep1Flash)
 		{
 			CPoint ptStart(rcDASCON.right, rcDASCON.top+rcDASCON.Height()/2);
 			CPoint ptEnd(rcDASCON.right+m_nCurLineLen, rcDASCON.top+rcDASCON.Height()/2);
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
 		CPoint ptDashEnd(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top+rcTip1.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 		ptDashStart.SetPoint(rcDASCON.left+rcDASCON.Width()/2, rcTip1.top+rcTip1.Height()/2);
 		ptDashEnd.SetPoint(rcTip1.left, rcTip1.top+rcTip1.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

		int abc;
		//dascon-->ionet
		CPoint ptStart(m_rcDASCON.left+70, m_rcDASCON.bottom);
		CPoint ptEnd(m_rcDASCON.left+70, m_rcIONET.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 
 	}
 
 	if (m_bStep2)
 	{
 		//绘制提示信息
 		CRect rcTip2;
 		rcTip2.top = m_rcFDF.bottom;
 		rcTip2.bottom = m_rcNCC.bottom;
 		rcTip2.left = rect.left+4;
 		rcTip2.right = m_rcNCC.left-3;
		
		pDC->Rectangle(&rcTip2);
 
 		CString strText("2.NCC接收DASCON转发的DAF服务请求，配置DAF服务链路(NCC_SHO).(其控制流线条在图中未标注)");
 		DrawMultiLine(pDC, rcTip2, strText);
 
 		/*m_nTotalLen = rcNCC.left - rcIONET.right;
 		if (m_bStep2Flash)
 		{
 			CPoint ptStart(rcIONET.right, rcNCC.top+rcNCC.Height()/2);
 			CPoint ptEnd(rcIONET.right+m_nCurLineLen, rcNCC.top+rcNCC.Height()/2);
 			DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 				&ptEnd, 8, 10, RGB(204,0,255));
 		}
 		else
 		{
 			CPoint ptStart(rcIONET.right, rcNCC.top+rcNCC.Height()/2);
 			CPoint ptEnd(rcNCC.left, rcNCC.top+rcNCC.Height()/2);
 			DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 				&ptEnd, 8, 10, RGB(204,0,255));
 		}
 		
 
 		CPoint ptDashStart(rcNCC.left+rcNCC.Width()/2, rcTip2.bottom);
 		CPoint ptDashEnd(rcNCC.left+rcNCC.Width()/2, rcNCC.top);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int abc;
		//dascon-->ionet
		//先绘制一条直线，然后再绘制一条的带箭头的线
		CPoint ptStart(m_rcNCC.right+200,m_rcIONET.bottom-30);
		CPoint ptEnd(m_rcNCC.right+200, m_rcNCC.top+40);
		DrawOneLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, RGB(255,0,255));
		//ionet-->ncc
		ptStart.SetPoint(m_rcNCC.right+200, m_rcNCC.top+40);
		ptEnd.SetPoint(m_rcNCC.right, m_rcNCC.top+40);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	if (m_bStep3)
 	{
 		//绘制提示信息
 		CRect rcTip3;
 		rcTip3.left = rect.left+3;
 		rcTip3.right = m_rcFDF.left;
 		rcTip3.top = m_rcSGLT.bottom+4;
 		rcTip3.bottom = m_rcFDF.bottom;
		pDC->Rectangle(&rcTip3);
 		CString strText("3.DAF服务链路设备配置。");
 		DrawMultiLine(pDC, rcTip3, strText);
 
 		/*CPoint ptDashStart(rcTermina.right, rcTip3.top+rcTip3.Height()/2);
 		CPoint ptDashEnd(rcTip3.left, rcTip3.top+rcTip3.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int ac;
		//

 	}
 
 	if (m_bStep4)
 	{
 		CRect rcTip4;
 		rcTip4.top = rect.top+20;
 		rcTip4.bottom = m_rcDARCM.bottom;
 		rcTip4.left = m_rcDARCM.left;
 		rcTip4.right = m_rcControl.right;

		pDC->Rectangle(&rcTip4);

 		CString strText("4.DSER系统根据配置命令端口地址配置PTP到用户POCC的通信链路。");
 		DrawMultiLine(pDC, rcTip4, strText);
 
 		/*m_nTotalLen = rcDASCON.top - rcDSER.bottom;
 		if (m_bStep4Flash)
 		{
 			CPoint ptStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 			CPoint ptEnd(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top-m_nCurLineLen);
 			DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 				&ptEnd, 8, 10, RGB(255, 0,204));
 
 		}
 		else
 		{
 			CPoint ptStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 			CPoint ptEnd(rcDASCON.left+rcDASCON.Width()/2, rcDSER.bottom);
 			DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 				&ptEnd, 8, 10, RGB(255, 0,204));
 
 		}*/
		int abc;
		//dascon-->dser
		CPoint ptStart(m_rcDASCON.left+20, m_rcDASCON.top);
		CPoint ptEnd(m_rcDASCON.left+20, m_rcDSER.bottom);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	pDC->SelectObject(pOldFont);

}


void CDlgMAFPhase3::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFPhase3::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFPhase3::OnTimer(UINT_PTR nIDEvent)
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

		m_bStep2 = TRUE;
		m_bStep2Flash = TRUE;

		Invalidate(FALSE);
		KillTimer(enmTimer_Step2);
		SetTimer(enmTimer_Step3, 10000, NULL);
	}
	else if (enmTimer_Step3 == nIDEvent)
	{
		m_bStep3 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step3);
		SetTimer(enmTimer_Step4, 4000, NULL);
	}
	else if (enmTimer_Step4 == nIDEvent)
	{
		m_bStep2Flash = FALSE;

		m_bStep4 = TRUE;
		m_bStep4Flash = TRUE;

		Invalidate(FALSE);
		KillTimer(enmTimer_Step4);
		SetTimer(enmTimer_Pause,10000, NULL);
	}
	else if (enmTimer_Flash == nIDEvent)
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

		//刷新下
		Invalidate(FALSE);
	}
	else if (enmTimer_Pause == nIDEvent)
	{
		KillTimer(enmTimer_Flash);
		m_bStep4Flash = FALSE;
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

void CDlgMAFPhase3::DrawOneLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, COLORREF cr)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 4, cr);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	// Draw line
	::MoveToEx(hDC, lpPointStart->x, lpPointStart->y, NULL);
	::LineTo(hDC, lpPointEnd->x, lpPointEnd->y);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);

}

int CDlgMAFPhase3::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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

void CDlgMAFPhase3::OnBnClickedBtnMaf3PauseFlash()
{
	if (m_bPause)
	{
		m_bPause = FALSE;
		GetDlgItem(IDC_BTN_MAF3_PAUSE_FLASH)->SetWindowText(_T("暂定"));
		SetTimer(enmTimer_Step1, 500, NULL);
	}
	else
	{
		m_bPause = TRUE;
		GetDlgItem(IDC_BTN_MAF3_PAUSE_FLASH)->SetWindowText(_T("开始"));
		m_bStep1 = FALSE;
		m_bStep2 = FALSE;
		m_bStep3 = FALSE;
		m_bStep4 = FALSE;
		m_bStep1Flash = FALSE;
		m_bStep2Flash = FALSE;
		m_bStep4Flash = FALSE;

		KillTimer(enmTimer_Step1);
		KillTimer(enmTimer_Step2);
		KillTimer(enmTimer_Step3);
		KillTimer(enmTimer_Step4);
		KillTimer(enmTimer_Flash);
		KillTimer(enmTimer_Pause);
		KillTimer(enmTimer_Exit);
	}
}
