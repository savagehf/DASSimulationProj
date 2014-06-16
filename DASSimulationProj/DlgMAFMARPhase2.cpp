// DlgMAFMARPhase2.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFMARPhase2.h"
#include "DlgCreateTaskWithUsabletime.h"
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



// CDlgMAFMARPhase2 dialog

IMPLEMENT_DYNAMIC(CDlgMAFMARPhase2, CDialog)

CDlgMAFMARPhase2::CDlgMAFMARPhase2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFMARPhase2::IDD, pParent)
{
	m_pparent = (CDlgCreateTaskWithUsabletime*)pParent;

	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;

	m_bStep1Flash = FALSE;
	m_bStep2Flash = FALSE;
	m_bStep3Flash = FALSE;

	m_nCurLineLen1 = 10;
	m_nCurLineLen2 = 10;
	m_nCurLineLen3 = 10;

	m_nTotalLen1 = 0;
	m_nTotalLen2 = 0;
	m_nTotalLen3 = 0;

	m_bPause = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMAFMARPhase2::~CDlgMAFMARPhase2()
{
}

void CDlgMAFMARPhase2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFMARPhase2, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MAFMAR2_PAUSE_FLASH, OnBnClickedBtnMar2PauseFlash)
END_MESSAGE_MAP()


BOOL CDlgMAFMARPhase2::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(enmTimer_Step1, 2000, NULL);
	//SetTimer(enmTimer_Step2, 3000, NULL);
	//SetTimer(enmTimer_Step3, 5000, NULL);
	//SetTimer(enmTimer_Exit, 8000, NULL);

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
// CDlgMARPhase2 message handlers
BOOL CDlgMAFMARPhase2::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFMARPhase2::OnClose()
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
			//pViewer->OnMarPhase3();
		}
	}
	else
	{
		//2014年1月19日12:44:40修改：需要绘制一条信号线给Pocc1.然后才能进行CheckIfsuccess的调用。
		/*CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMain)
		{
			CDASSimulationView* pView = (CDASSimulationView*)pMain->GetActiveView();
			pView->StartDrawPhase2AnswerSignal(TRUE);
		}*/

		if (NULL != m_pparent)
		{
			m_pparent->CheckIfSuccess();
		}
	}


	__super::OnClose();
}

void CDlgMAFMARPhase2::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("双向服务请求阶段(二)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFMARPhase2::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAR_PHASE_2_BITMAP))
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

void CDlgMAFMARPhase2::DrawClient(CDC* pDC)
{

	CRect rect;
	GetClientRect(rect);

	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);


	CFont* pOldFont = pDC->SelectObject(&m_font);
	if (m_bStep1)
	{

		//绘制提示信息
		//绘制提示信息
		CRect rcTip1;
		rcTip1.top = m_rcPOCC.top;
		rcTip1.bottom = rect.bottom-5;
		rcTip1.left = m_rcNCC.right+5;
		rcTip1.right = m_rcPOCC.left-5;

		pDC->Rectangle(&rcTip1);

		CString strText("1.用户POCC通过DAS规划工具向DASCON提交服务请求Service Request.");
		DrawMultiLine(pDC, rcTip1, strText);

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

	if (m_bStep2)
	{
		//绘制提示信息
		CRect rcTip2;
		rcTip2.top = m_rcDARCM.bottom+5;
		rcTip2.bottom = m_rcControl.bottom;
		rcTip2.right = m_rcControl.left-4;
		rcTip2.left = m_rcDMG.right+4;

		pDC->Rectangle(&rcTip2);

		CString strText("2.DASCON对服务请求进行规划决策，并回传给用户决策信息。");
		DrawMultiLine(pDC, rcTip2, strText);

	}

	if (m_bStep3)
	{
		//绘制提示信息
		CRect rcTip3;
		rcTip3.top = m_rcPOCC.top-30;
		rcTip3.bottom = rect.bottom-5;
		rcTip3.left = m_rcPOCC.right+2;
		rcTip3.right = rect.right-3;

		pDC->Rectangle(&rcTip3);

		CString strText("3.用户POCC通过DAS工具接收决策信息");
		DrawMultiLine(pDC, rcTip3, strText);

		int abc;
		//返回线条
		//dascon-->ionet
		CPoint ptStart(m_rcDASCON.left+70, m_rcDASCON.bottom);
		CPoint ptEnd(m_rcDASCON.left+70, m_rcIONET.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//ionet-->pocc
		ptStart.SetPoint(m_rcDASCON.left+70, m_rcIONET.bottom-25);
		ptEnd.SetPoint(m_rcDASCON.left+70, m_rcPOCC.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

	}

	pDC->SelectObject(pOldFont);

}


void CDlgMAFMARPhase2::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFMARPhase2::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFMARPhase2::OnTimer(UINT_PTR nIDEvent)
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
		m_bStep2Flash = FALSE;

		m_bStep3 = TRUE;
		m_bStep3Flash = TRUE;

		Invalidate(FALSE);
		KillTimer(enmTimer_Step3);
		SetTimer(enmTimer_Pause, 10000, NULL);
	}
	else if (enmTimer_Flash == nIDEvent)
	{
		if (m_bStep1Flash)
		{
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

		if (m_bStep2Flash)
		{
			if (m_nCurLineLen2 >= m_nTotalLen2)
			{
				m_nCurLineLen2 = 10;
			}
			else
			{
				m_nCurLineLen2 += s_step;
				if (m_nCurLineLen2 > m_nTotalLen2)
				{
					m_nCurLineLen2 = m_nTotalLen2;
				}
			}	

			if (m_nCurLineLen3 >= m_nTotalLen3)
			{
				m_nCurLineLen3 = 10;
			}
			else
			{
				m_nCurLineLen3 += s_step;
				if (m_nCurLineLen3 > m_nTotalLen3)
				{
					m_nCurLineLen3 = m_nTotalLen3;
				}
			}	
		}

		if (m_bStep3Flash)
		{
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

			if (m_nCurLineLen2 >= m_nTotalLen2)
			{
				m_nCurLineLen2 = 10;
			}
			else
			{
				m_nCurLineLen2 += s_step;
				if (m_nCurLineLen2 > m_nTotalLen2)
				{
					m_nCurLineLen2 = m_nTotalLen2;
				}
			}	

			if (m_nCurLineLen3 >= m_nTotalLen3)
			{
				m_nCurLineLen3 = 10;
			}
			else
			{
				m_nCurLineLen3 += s_step;
				if (m_nCurLineLen3 > m_nTotalLen3)
				{
					m_nCurLineLen3 = m_nTotalLen3;
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

int CDlgMAFMARPhase2::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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

	HPEN hPen = ::CreatePen(PS_SOLID,4, cr);
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

void CDlgMAFMARPhase2::OnBnClickedBtnMar2PauseFlash()
{
	if (m_bPause)
	{
		m_bPause = FALSE;
		GetDlgItem(IDC_BTN_MAFMAR2_PAUSE_FLASH)->SetWindowText(_T("暂定"));
		SetTimer(enmTimer_Step1, 500, NULL);
	}
	else
	{
		m_bPause = TRUE;
		GetDlgItem(IDC_BTN_MAFMAR2_PAUSE_FLASH)->SetWindowText(_T("开始"));
		m_bStep1 = FALSE;
		m_bStep2 = FALSE;
		m_bStep3 = FALSE;
		m_bStep1Flash = FALSE;
		m_bStep2Flash = FALSE;
		m_bStep3Flash = FALSE;

		KillTimer(enmTimer_Step1);
		KillTimer(enmTimer_Step2);
		KillTimer(enmTimer_Step3);
		KillTimer(enmTimer_Flash);
		KillTimer(enmTimer_Pause);
		KillTimer(enmTimer_Exit);
	}
}
