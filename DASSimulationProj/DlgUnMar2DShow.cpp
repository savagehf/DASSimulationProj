// DlgUnMar2DShow.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgUnMar2DShow.h"
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


// CDlgUnMar2DShow dialog

IMPLEMENT_DYNAMIC(CDlgUnMar2DShow, CDialog)

CDlgUnMar2DShow::CDlgUnMar2DShow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUnMar2DShow::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;
}

CDlgUnMar2DShow::~CDlgUnMar2DShow()
{
}

void CDlgUnMar2DShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUnMar2DShow, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgUnMar2DShow message handlers
BOOL CDlgUnMar2DShow::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(enmTimer_Step1, 2000, NULL);

	m_font.CreateFont(
		14,                        // nHeight
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
		_T("新宋体"));                 // lpszFacename



	return TRUE; 
}

BOOL CDlgUnMar2DShow::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgUnMar2DShow::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Step3);
	KillTimer(enmTimer_Exit);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pMain)
	{
		CDASSimulationView* pView = (CDASSimulationView*)pMain->GetActiveView();
		pView->StartDrawUnexpectedAnswerSignal(FALSE);
	}

	/*if (m_bContinuePlay)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrm)
		{
			CDASSimulationView* pViewer = (CDASSimulationView*)pMainFrm->GetActiveView();
			pViewer->OnMarPhase2();
		}
	}*/

	__super::OnClose();
}

void CDlgUnMar2DShow::OnPaint()
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
		if (bmp.LoadBitmap(IDB_DAS_SYSTEM/*IDB_MAR_PHASE_1_BITMAP*/))
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

void CDlgUnMar2DShow::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//先些标题，提示语信息：
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("突发返向DAR服务"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}

void CDlgUnMar2DShow::DrawClient(CDC* pDC)
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
		rcTip1.top = m_rcSGLT.bottom - 20;
		rcTip1.bottom = m_rcSGLT.bottom;
		rcTip1.left = m_rcSGLT.right+4;
		rcTip1.right = m_rcDMG.right+70;

		pDC->Rectangle(&rcTip1);

		CString strText("1.SGLT设备将服务请求发送到DASCON。");
		DrawMultiLine(pDC, rcTip1, strText);

		//pDC->SelectObject(hOldFont);

		////CPen penRed;
		////penRed.CreatePen(PS_DASH, 1, RGB(255,0,0));
		////CPen* pOldPen = pDC->SelectObject(&penRed);
		//
		////COLORREF crOld = pDC->SetTextColor(RGB(0,204,255));
		//CPoint ptStart(rcPOCC.left+15, rcPOCC.top);
		//CPoint ptEnd(rcSWSI.left+15, rcSWSI.bottom);
		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
		//	&ptEnd, 8, 10,RGB(0,204,255));
		////pDC->SetTextColor(crOld);

		//CPoint ptDashStart(rcPOCC.left+15, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
		//CPoint ptDashEnd(rcPOCC.left-60, rcPOCC.top);
		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		int abc;
		//sglt--dascon
		CPoint ptStart(m_rcSGLT.right, m_rcSGLT.bottom-70);
		CPoint ptEnd(m_rcDASCON.left, m_rcSGLT.bottom-70);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
	}

	if (m_bStep2)
	{
		//绘制提示信息
		CRect rcTip2;
		rcTip2.top = m_rcDARCM.bottom;
		rcTip2.bottom = m_rcDASCON.bottom-10;
		rcTip2.left = m_rcDARCM.left+10;
		rcTip2.right = m_rcDARCM.right+10;
		pDC->Rectangle(&rcTip2);

		CString strText("2.DASCON将该服务所需的配置参数发送给DARCM");
		DrawMultiLine(pDC, rcTip2, strText);

		/*CPoint ptStart(rcFDF.left, rcFDF.top+rcFDF.Height()/2);
		CPoint ptEnd(rcIONET.right, rcFDF.top+rcFDF.Height()/2);
		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
		&ptEnd, 8, 10,RGB(204,0,255));
		ptStart.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2);
		ptEnd.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
		&ptEnd, 8, 10, RGB(204,0,255));

		CPoint ptDashStart(rcFDF.left, rcFDF.top+8);
		CPoint ptDashEnd(rcIONET.left-5, rcFDF.top+8);
		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int abc;
		//dascon--ionet
		CPoint ptStart(m_rcDASCON.left, m_rcDASCON.top+m_rcDASCON.Height()/2);
		CPoint ptEnd(m_rcDASCON.left-20, m_rcDASCON.top+m_rcDASCON.Height()/2);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		
		//ionet--darcm
		/*ptStart.SetPoint(m_rcIONET.left, m_rcDARCM.top+m_rcDARCM.Height()/2);
		ptEnd.SetPoint(m_rcDARCM.right, m_rcDARCM.top+m_rcDARCM.Height()/2);
		DrawArrowLine(pDC->GetSafeHdc(),  &ptStart, &ptEnd, 8, 10, RGB(255,0,255));*/
	}

	if (m_bStep3)
	{
		//绘制提示信息
		CRect rcTip3;
		rcTip3.top = m_rcDBF.top;
		rcTip3.bottom = m_rcDARCM.top-5;
		rcTip3.right = m_rcDARCM.right;
		rcTip3.left = m_rcDBF.right+5;
		pDC->Rectangle(&rcTip3);

		CString strText("3.DARCM发送配置信息到DBF和DMG.");
		DrawMultiLine(pDC, rcTip3, strText);

		/*CPoint ptStart(rcIONET.left, rcIONET.bottom-30);
		CPoint ptEnd(rcDASCON.right, rcIONET.bottom-30);
		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
		&ptEnd, 8, 10,RGB(255,204,0));

		CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
		CPoint ptDashEnd(rcTip3.right, rcTip3.top+rcTip3.Height()/2);
		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

		//darcm--dbf
		CPoint ptStart(m_rcDARCM.left, m_rcDARCM.top+20);
		CPoint ptEnd(m_rcDBF.right, m_rcDARCM.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//darcm--dmg
		ptStart.SetPoint(m_rcDARCM.left, m_rcDMG.top+20);
		ptEnd.SetPoint(m_rcDMG.right, m_rcDMG.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
	}

	if (m_bStep4)
	{
		//绘制提示信息
		CRect rcTip4;
		rcTip4.top = m_rcDMG.top+30;
		rcTip4.bottom = m_rcDMG.bottom-20;
		rcTip4.left = m_rcSGLT.left+20;
		rcTip4.right = m_rcSGLT.right+10;
		pDC->Rectangle(&rcTip4);

		CString strText("4.DASCON已确认用户身份，接受服务请求，并返回服务状态信息");
		DrawMultiLine(pDC, rcTip4, strText);

		CPoint ptStart(m_rcDARCM.left, m_rcDARCM.top+m_rcDARCM.Height()/2);
		CPoint ptEnd(m_rcSGLT.right, m_rcDARCM.top+m_rcDARCM.Height()/2);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		/*CPoint ptStart(rcDASCON.right, rcIONET.bottom-10);
		CPoint ptEnd(rcIONET.left, rcIONET.bottom-10);
		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
		&ptEnd, 8, 10,RGB(10, 100, 200));

		ptStart.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2+16);
		ptEnd.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2+16);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
		&ptEnd, 8, 10, RGB(10, 100, 200));

		ptStart.SetPoint(rcSWSI.right-20, rcSWSI.bottom);
		ptEnd.SetPoint(rcSWSI.right-20, rcPOCC.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
		&ptEnd, 8, 10, RGB(10, 100, 200));

		CPoint ptDashStart(rcSWSI.right+10, rcTip4.bottom-25);
		CPoint ptDashEnd(rcSWSI.right+10, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
		DrawDASHLine(pDC, ptDashStart, ptDashEnd);

		ptDashStart.SetPoint(rcTip4.right-10, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
		ptDashEnd.SetPoint(rcSWSI.right-15,rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

	}

	pDC->SelectObject(pOldFont);
	// 	
}


void CDlgUnMar2DShow::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgUnMar2DShow::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgUnMar2DShow::OnTimer(UINT_PTR nIDEvent)
{
	if (enmTimer_Step1 == nIDEvent)
	{
		m_bStep1 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step1);
		SetTimer(enmTimer_Step2, 8000, NULL);
	}
	else if (enmTimer_Step2 == nIDEvent)
	{
		m_bStep2 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step2);
		SetTimer(enmTimer_Step3, 8000, NULL);
	}
	else if (enmTimer_Step3 == nIDEvent)
	{
		m_bStep3 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step3);
		SetTimer(enmTimer_Step4, 8000, NULL);
	}
	else if (enmTimer_Step4 == nIDEvent)
	{
		m_bStep4 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step4);
		SetTimer(enmTimer_Exit, 4000, NULL);
	}
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgUnMar2DShow::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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
