// DlgMAFPhase5.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFPhase5.h"

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
// CDlgMAFPhase5 dialog

IMPLEMENT_DYNAMIC(CDlgMAFPhase5, CDialog)

CDlgMAFPhase5::CDlgMAFPhase5(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFPhase5::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMAFPhase5::~CDlgMAFPhase5()
{
}

void CDlgMAFPhase5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFPhase5, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgMAFPhase5 message handlers
BOOL CDlgMAFPhase5::OnInitDialog()
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



void CDlgMAFPhase5::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//��Щ���⣬��ʾ����Ϣ��
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcClient.top+20;
	pDC->DrawText(_T("DAF�û����ݴ���׶�(��)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


BOOL CDlgMAFPhase5::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFPhase5::OnClose()
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
			pViewer->OnMafPhase6();
		}
	}

	__super::OnClose();
}

void CDlgMAFPhase5::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAF_PHASE_5_BITMAP))
		{
			BITMAP bmpInfo;
			bmp.GetBitmap(&bmpInfo);

			CDC dcMemory;
			dcMemory.CreateCompatibleDC(&dc);

			//��λͼѡ�뵽�ڴ�DC��
			CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

			DrawClient(&dcMemory);

			//
			dc.StretchBlt(0,0,rect.Width(),rect.Height(),
				&dcMemory,
				0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);

			dcMemory.SelectObject(pOldBitmap);

		}

		CDialog::OnPaint();
	}
}

void CDlgMAFPhase5::DrawClient(CDC* pDC)
{

	CRect rect;
	GetClientRect(rect);

	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);

 	CFont* pOldFont = pDC->SelectObject(&m_font);
 	if (m_bStep1)
 	{
 
 		//������ʾ��Ϣ
 		CRect rcTip1;
 		rcTip1.right = m_rcIONET.right+5;
 		rcTip1.left = m_rcPOCC.right+2;
 		rcTip1.top = m_rcPOCC.top;
 		rcTip1.bottom = m_rcPOCC.bottom;
		pDC->Rectangle(&rcTip1);
 
 		CString strText("1.POCCͨ��IONet�����ݷ��͵�PTP��");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		/*CPoint ptStart(rcPOCC.left+rcPOCC.Width()/2, rcPOCC.top);
 		CPoint ptEnd(rcPOCC.left+rcPOCC.Width()/2, rcSWSI.bottom);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(0,204,255));
 		ptStart.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2);
 		ptEnd.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(0,204,255));
 		ptStart.SetPoint(rcIONET.left, rcDSER.top+rcDSER.Height()/2);
 		ptEnd.SetPoint(rcDSER.right, rcDSER.top+rcDSER.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(0,204,255));
 
 		CPoint ptDashStart(rcPOCC.left, rcPOCC.top+rcPOCC.Height()/2);
 		CPoint ptDashEnd(rcTip1.right, rcPOCC.top+rcPOCC.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
		int abc;
		///pocc-->ionet
		CPoint ptStart(m_rcDASCON.left+17, m_rcPOCC.top);
		CPoint ptEnd(m_rcDASCON.left+17, m_rcIONET.bottom-20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//ionet-->dser
		ptStart.SetPoint(m_rcDSER.right-20, m_rcIONET.top-15);
		ptEnd.SetPoint(m_rcDSER.right-20, m_rcDSER.bottom);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		//������ʾ��Ϣ
		CRect rcTip2;
		rcTip2.left = m_rcControl.right+2;
		rcTip2.right = m_rcDSER.right-30;
		rcTip2.top = m_rcDSER.bottom+4;
		rcTip2.bottom = m_rcDASCON.bottom+10;
		pDC->Rectangle(&rcTip2);

		strText = "2.PTP��������ݽ��и�ʽ��·��,�����䷢�͵�DAFǰ��������豸��";
		DrawMultiLine(pDC, rcTip2, strText);
 
 	}
 
 	if (m_bStep2)
 	{
 		////������ʾ��Ϣ
 		//CRect rcTip2;
 		//rcTip2.left = rcConDataInter.right+15;
 		//rcTip2.right = rcIONET.left-15;
 		//rcTip2.top = rect.top+30;
 		//rcTip2.bottom = rcDSER.top-6;
 
 		//CString strText("2.PTP��������ݽ��и�ʽ��·��,�����䷢�͵�MAFǰ��������豸��");
 		//DrawMultiLine(pDC, rcTip2, strText);
 
 		/*CPoint ptStart(rcDSER.left, rcDSER.top+rcDSER.Height()/2);
 		CPoint ptEnd(rcConDataInter.right, rcDSER.top+rcDSER.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(204,0,255));
 
 		ptStart.SetPoint(rcConDataInter.left, rcConDataInter.bottom-10);
 		ptEnd.SetPoint(rcTermina.right, rcConDataInter.bottom-10);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(204,0,255));*/
		int abc;
		//dser-->control
		CPoint ptStart(m_rcDSER.left, m_rcDSER.top+20);
		CPoint ptEnd(m_rcControl.right, m_rcDSER.top+20);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		//control-->SGLT
		ptStart.SetPoint(m_rcControl.left, m_rcControl.bottom-10);
		ptEnd.SetPoint(m_rcSGLT.right, m_rcControl.bottom-10);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
		
		CRect rcTip3;
		rcTip3.left = 5;
		rcTip3.right = m_rcSGLT.right;
		rcTip3.bottom = m_rcFDF.bottom;
		rcTip3.top = m_rcSGLT.bottom+5;
		pDC->Rectangle(&rcTip3);
		CString strText("3.ԭ��DAF�豸�����û�����ͨ��RF��·���͵�TDRS��");
		DrawMultiLine(pDC, rcTip3, strText);


 	}
 
 	if (m_bStep3)
 	{
 		//������ʾ��Ϣ
 		//CRect rcTip3;
 		//rcTip3.left = rcTermina.right+15;
 		//rcTip3.right = rcTip3.left+160;
 		//rcTip3.bottom = rcTermina.bottom-40;
 		//rcTip3.top = rcTip3.bottom-60;
 
 		//CString strText("3.ԭ��MAF�豸�����û�����ͨ��RF��·���͵�TDRS��");
 		//DrawMultiLine(pDC, rcTip3, strText);
 
 		////��
 		//CPoint ptDashStart(rcTip3.left, rcTip3.top+rcTip3.Height()/2);
 		//CPoint ptDashEnd(rcTermina.right, rcTip3.top+rcTip3.Height()/2);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);
		
 
 	}
 
 	pDC->SelectObject(pOldFont);

}


void CDlgMAFPhase5::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFPhase5::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
{
	//͸��������ɫ������ʾ����ʾ��ֻ�ǰ�ɫˢ�ӵ���ɫ��
	//pDC->SetBkMode(TRANSPARENT); 

	CRect boundary;//�����ı�Ҫ��ʾ��������
	boundary = rect;

	//ʹ��DT_CALCRECT��DT_CENTER��DT_EDITCONTROL��DT_WORDBREAK����������Ҫ���ı��߶�
	int height = pDC->DrawText(strText, rect, DT_CALCRECT|DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK);
	rect = boundary;//������ɺ�ԭ�������򸳻�rect
	if(boundary.Height() > height)//�жϸ߶��Ƿ񳬳���Χ��������ָ������
		rect.top += (boundary.Height() - height)/2;//����հ׸߶ȵ�һ�룬����Ҫ��+=
	pDC->DrawText(strText, rect,DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK); //�����ֱ����+���Ҿ���+�Զ����е��ı����
}
void CDlgMAFPhase5::OnTimer(UINT_PTR nIDEvent)
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
	/*else if (enmTimer_Step4 == nIDEvent)
	{
		m_bStep4 = TRUE;
		Invalidate(FALSE);
		KillTimer(enmTimer_Step4);
	}*/
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMAFPhase5::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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
