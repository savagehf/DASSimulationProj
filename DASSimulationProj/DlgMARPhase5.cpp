// DlgMARPhase5.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMARPhase5.h"
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
// CDlgMARPhase5 dialog

IMPLEMENT_DYNAMIC(CDlgMARPhase5, CDialog)

CDlgMARPhase5::CDlgMARPhase5(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMARPhase5::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;
	m_bStep5 = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMARPhase5::~CDlgMARPhase5()
{
}

void CDlgMARPhase5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMARPhase5, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgMARPhase5 message handlers
BOOL CDlgMARPhase5::OnInitDialog()
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


void CDlgMARPhase5::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("DAR用户数据传输阶段(五)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


BOOL CDlgMARPhase5::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMARPhase5::OnClose()
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
			pViewer->OnMarPhase6();
		}
	}
	__super::OnClose();
}


void CDlgMARPhase5::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAR_PHASE_5_BITMAP))
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

void CDlgMARPhase5::DrawClient(CDC* pDC)
{

	CRect rect;
	GetClientRect(rect);

	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);
	
 	CFont* pOldFont = pDC->SelectObject(&m_font);
 	if (m_bStep1)
 	{
 
 		////绘制提示信息
 		CRect rcTip1;
 		rcTip1.left = rect.left+3;
 		rcTip1.right = m_rcSGLT.right;
 		rcTip1.bottom = rect.bottom-4;
 		rcTip1.top = m_rcSGLT.bottom+20;

		pDC->Rectangle(&rcTip1);

 		CString strText("1.SGLT原有设备负责接收TDRSS转发来的用户数据,对其进行处理,形成一路中频信号进入DBF系统。");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		//CPoint ptStart(rcTip1.left, rcTip1.bottom-15);
 		//CPoint ptEnd(rcTermina.right, rcTip1.bottom-15);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(0,0,0));
 
 		//
 		//CPoint ptDashStart(rcTip1.left, rcTip1.top+15);
 		//CPoint ptDashEnd(rcTermina.right, rcTip1.top+15);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//SGLT-->DBF
		CPoint ptStart(m_rcSGLT.right, m_rcDBF.top+20);
		CPoint ptEnd(m_rcDBF.left, m_rcDBF.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 
 	}
 
 	if (m_bStep2)
 	{
 		//绘制提示信息
 		CRect rcTip2;
 		rcTip2.top = m_rcDBF.top;
 		rcTip2.bottom = m_rcDARCM.bottom;
 		rcTip2.left = m_rcDARCM.left-10;
 		rcTip2.right = m_rcDARCM.right+10;

		pDC->Rectangle(&rcTip2);
 
 		CString strText("2.DBF系统接收中频信号,形成指向用户星波束,最终复合成一路中频信号(BF_Signal),送入到DMG系统。");
 		DrawMultiLine(pDC, rcTip2, strText);
 
 		//CPoint ptStart(rcDBFSub1.left+rcDBFSub1.Width()/2, rcTip2.top+15);
 		//CPoint ptEnd(rcDBFSub1.left+rcDBFSub1.Width()/2, rcDBFSub1.bottom);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(0,0,0));
 
 		//ptStart.SetPoint(rcTermina.right, rcDBF.top+rcDBF.Height()/2);
 		//ptEnd.SetPoint(rcDBFSub1.left, rcDBF.top+rcDBF.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(0,204,255));
 		//
 		//ptStart.SetPoint(rcDBF.right, rcDBF.top+rcDBF.Height()/2);
 		//ptEnd.SetPoint(rcDMGSub1.left, rcDBF.top+rcDBF.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(), &ptStart, 
 		//	&ptEnd, 8, 10, RGB(0,204,255));
 
 		////竖
 		//CPoint ptDashStart(rcDBF.left+10, rcTip2.top+15);
 		//CPoint ptDashEnd(rcDBF.left+10, rcDBF.bottom);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//DBF--->DMG
		CPoint ptStart(m_rcDMG.left+40, m_rcDBF.bottom);
		CPoint ptEnd(m_rcDMG.left+40, m_rcDMG.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	if (m_bStep3)
 	{
 		//绘制提示信息
 		CRect rcTip3;
 		rcTip3.left = m_rcDMG.left;
 		rcTip3.right = m_rcDMG.right+40;
 		rcTip3.top = m_rcDMG.bottom+4;
 		rcTip3.bottom = m_rcNCC.top+40;
		
		pDC->Rectangle(&rcTip3);
 
 		CString strText("3.DMG系统接收中频信号,对其进行伪噪声解扩,载波捕获,解调等处理,将处理完成后的信号送入DSER系统。");
 		DrawMultiLine(pDC, rcTip3, strText);
 
 		//CPoint ptStart(rcDMGSub1.left+rcDMGSub1.Width()/2, rcTip3.top+15);
 		//CPoint ptEnd(rcDMGSub1.left+rcDMGSub1.Width()/2, rcDMGSub1.bottom);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(0,0,0));
 
 		//ptStart.SetPoint(rcDMGSub1.right, rcDMGSub1.top+rcDMG.Height()/2);
 		//ptEnd.SetPoint(rcConDataInter.left, rcDMGSub1.top+rcDMG.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(100,204,155));
 		//ptStart.SetPoint(rcConDataInter.right, rcDSER.top+rcDSER.Height()/2);
 		//ptEnd.SetPoint(rcDSER.left, rcDSER.top+rcDSER.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(100,204,155));
 
 		////竖
 		//CPoint ptDashStart(rcDMG.left+rcDMG.Width()/2, rcDMG.bottom);
 		//CPoint ptDashEnd(rcDMG.left+rcDMG.Width()/2, rcTip3.top+15);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		
		int abc;
		//DMG-->control
		CPoint ptStart(m_rcDMG.right, m_rcDMG.top+60);
		CPoint ptEnd(m_rcControl.left, m_rcDMG.top+60);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		//Control-->DSER
		ptStart.SetPoint(m_rcControl.right, m_rcDSER.top+40);
		ptEnd.SetPoint(m_rcDSER.left, m_rcDSER.top+40);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

 	}
 
 	if (m_bStep4)
 	{
 		CRect rcTip4;
 		rcTip4.left = m_rcControl.right+10;
 		rcTip4.top = m_rcDSER.bottom+5;
 		rcTip4.right = m_rcDSER.right-60;
 		rcTip4.bottom = m_rcControl.bottom;
		
		pDC->Rectangle(&rcTip4);

 		CString strText("4.DSER系统接收DMG系统传来的信号,对其进行路由,格式化归档等。最终以TCP/IP包的格式通过IONet向用户发送数据。");
 		DrawMultiLine(pDC, rcTip4, strText);
 
 		//CPoint ptStart(rcDSER.right, rcDSER.top+rcDSER.Height()/2);
 		//CPoint ptEnd(rcIONET.left, rcDSER.top+rcDSER.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(204,0,255));
 		//ptStart.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2);
 		//ptEnd.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(204,0,255));
 
 		////竖
 		//CPoint ptDashStart(rcDSER.left+rcDSER.Width()/2, rcDSER.top);
 		//CPoint ptDashEnd(rcDSER.left+rcDSER.Width()/2, rcTip4.top+30);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 		//ptDashStart.SetPoint(rcDSER.left+rcDSER.Width()/2, rcTip4.top+30);
 		//ptDashEnd.SetPoint(rcTip4.left, rcTip4.top+30);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//DSER-->IONET
		CPoint ptStart(m_rcDSER.right-10, m_rcDSER.bottom);
		CPoint ptEnd(m_rcDSER.right-10, m_rcIONET.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//IONET-->POCC
		ptStart.SetPoint(m_rcPOCC.left+40, m_rcIONET.bottom-20);
		ptEnd.SetPoint(m_rcPOCC.left+40, m_rcPOCC.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

 
 	}
 
 	if (m_bStep5)
 	{
 		CRect rcTip5;
 		rcTip5.left = m_rcPOCC.right+3;
		rcTip5.right = rcTip5.left+100;
 		rcTip5.top = m_rcPOCC.top;
 		rcTip5.bottom = m_rcPOCC.bottom-3;

		pDC->Rectangle(&rcTip5);

 		CString strText("5.用户POCC接收数据(Return Data)。");
 		DrawMultiLine(pDC, rcTip5, strText);
 
 		/*CPoint ptStart(rcPOCC.left+rcPOCC.Width()/2, rcSWSI.bottom);
 		CPoint ptEnd(rcPOCC.left+rcPOCC.Width()/2, rcPOCC.top);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(255,204,0));
 
 		CPoint ptDashStart(rcPOCC.left, rcPOCC.top+rcPOCC.Height()/2);
 		CPoint ptDashEnd(rcTip5.right, rcPOCC.top+rcPOCC.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
 
 	}
 
 	pDC->SelectObject(pOldFont);

}

void CDlgMARPhase5::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMARPhase5::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMARPhase5::OnTimer(UINT_PTR nIDEvent)
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
		SetTimer(enmTimer_Step4, 10000, NULL);
	}
	else if (enmTimer_Step4 == nIDEvent)
	{
		m_bStep4 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step4);
		SetTimer(enmTimer_Step5, 10000, NULL);
	}
	else if (enmTimer_Step5 == nIDEvent)
	{
		m_bStep5 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step5);
		SetTimer(enmTimer_Exit, 4000, NULL);
	}
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMARPhase5::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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
