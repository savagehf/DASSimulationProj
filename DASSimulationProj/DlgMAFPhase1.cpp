// DlgMAFPhase1.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgMAFPhase1.h"
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
// CDlgMAFPhase1 dialog

IMPLEMENT_DYNAMIC(CDlgMAFPhase1, CDialog)

CDlgMAFPhase1::CDlgMAFPhase1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMAFPhase1::IDD, pParent)
{
	m_bStep1 = FALSE;
	m_bStep2 = FALSE;
	m_bStep3 = FALSE;
	m_bStep4 = FALSE;
	m_bContinuePlay = FALSE;
}

CDlgMAFPhase1::~CDlgMAFPhase1()
{
}

void CDlgMAFPhase1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMAFPhase1, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PAUSE_MAF1, &CDlgMAFPhase1::OnBnClickedBtnPauseMaf1)
END_MESSAGE_MAP()


// CDlgMAFPhase1 message handlers
BOOL CDlgMAFPhase1::OnInitDialog()
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


void CDlgMAFPhase1::DrawTitleTips(CDC* pDC, CRect rcClient)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//��Щ���⣬��ʾ����Ϣ��
	CRect rcTitle;
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top  = rcClient.top+2;
	rcTitle.bottom = rcTitle.top+20;
	pDC->DrawText(_T("DAF����滮�׶�(һ)"), &rcTitle, DT_CENTER);

	pDC->SelectObject(pOldPen);
}


BOOL CDlgMAFPhase1::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgMAFPhase1::OnClose()
{
	KillTimer(enmTimer_Step1);
	KillTimer(enmTimer_Step2);
	KillTimer(enmTimer_Step3);

	if (m_bContinuePlay)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrm)
		{
			CDASSimulationView* pViewer = (CDASSimulationView*)pMainFrm->GetActiveView();
			pViewer->OnMafPhase2();
		}
	}

	__super::OnClose();
}

void CDlgMAFPhase1::OnPaint()
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
		if (bmp.LoadBitmap(IDB_MAF_PHASE_1_BITMAP))
		{
			BITMAP bmpInfo;
			bmp.GetBitmap(&bmpInfo);

			CDC dcMemory;
			dcMemory.CreateCompatibleDC(&dc);

			//��λͼѡ�뵽�ڴ�DC��
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


void CDlgMAFPhase1::DrawClient(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	//����͸��Ч��
	pDC->SetBkMode(TRANSPARENT);

	DrawTitleTips(pDC, rect);

 	CFont* pOldFont = pDC->SelectObject(&m_font);
 	if (m_bStep1)
 	{
 
 		//������ʾ��Ϣ
 		CRect rcTip1;
 		rcTip1.top = m_rcIONET.bottom-20;
 		rcTip1.bottom = m_rcPOCC.bottom;
 		rcTip1.left = m_rcPOCC.right+3;
 		rcTip1.right = m_rcPOCC.right+160;

		pDC->Rectangle(&rcTip1);
 
 		CString strText("1.�û�POCCͨ��DAS�滮������DASCON�������滮����Planning info Request.");
 		DrawMultiLine(pDC, rcTip1, strText);
 
 		//CPoint ptStart(rcPOCC.left+15, rcPOCC.top);
 		//CPoint ptEnd(rcSWSI.left+15, rcSWSI.bottom);
 		//DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 		//	&ptEnd, 8, 10,RGB(0,204,255));
 		////pDC->SetTextColor(crOld);
 
 		//CPoint ptDashStart(rcPOCC.left+15, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
 		//CPoint ptDashEnd(rcPOCC.left-60, rcPOCC.top);
 		//DrawDASHLine(pDC, ptDashStart, ptDashEnd);

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
 		/*CPoint ptStart(rcFDF.left, rcFDF.top+rcFDF.Height()/2);
 		CPoint ptEnd(rcIONET.right, rcFDF.top+rcFDF.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(200, 0, 255));
 		ptStart.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2);
 		ptEnd.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(200, 0, 255));
 
 		CPoint ptDashStart(rcFDF.left, rcFDF.top+8);
 		CPoint ptDashEnd(rcIONET.left-5, rcFDF.top+8);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

		int abc;
		//fdf-->ionet
		CPoint ptStart(m_rcFDF.right, m_rcFDF.top+40);
		CPoint ptEnd(m_rcIONET.left, m_rcFDF.top+40);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		//������ʾ��Ϣ
		CRect rcTip2;
		rcTip2.top = m_rcSGLT.bottom+10;
		rcTip2.bottom = m_rcFDF.bottom+5;
		rcTip2.left = rect.left+2;
		rcTip2.right = m_rcFDF.left-2;

		pDC->Rectangle(&rcTip2);

		CString strText("2.FDF��DASCON�ṩTDRS���û�������״̬����FDF_SV");
		DrawMultiLine(pDC, rcTip2, strText);

		//ncc-->ionet
		//�Ȼ���һ��ֱ�ߣ�Ȼ���ٻ���һ�����ϵĴ���ͷ����
		ptStart.SetPoint(m_rcNCC.right, m_rcNCC.top+40);
		ptEnd.SetPoint(m_rcNCC.right+200, m_rcNCC.top+40);
		DrawOneLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, RGB(255,0,255));
		ptStart.SetPoint(m_rcNCC.right+200, m_rcNCC.top+40);
		ptEnd.SetPoint(m_rcNCC.right+200,m_rcIONET.bottom-30);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		rcTip2.left = rect.left+2;
		rcTip2.top = m_rcNCC.top-5;
		rcTip2.right = m_rcNCC.left-2;
		rcTip2.bottom = m_rcNCC.bottom+5;
		
		pDC->Rectangle(&rcTip2);
		strText = "2.NCC��ʱ������DASCON�ṩTUT��(NCC_TUT)";
		DrawMultiLine(pDC, rcTip2, strText);
 	}
 
 	if (m_bStep3)
 	{
 		//������ʾ��Ϣ
 		CRect rcTip3;
 		rcTip3.left = m_rcDARCM.left;
		rcTip3.right = m_rcDASCON.left-5 ;
		rcTip3.top = m_rcDARCM.bottom+5;
		rcTip3.bottom = m_rcControl.bottom;
		pDC->Rectangle(&rcTip3);
 
 		CString strText("3.DASCON���չ滮����,ͨ��FDF�ṩ��״̬����,�����û���������TDRS�ɼ���ʱ���,���DAS�豸������,�ش����û��滮��ϢPlanning Infomation.");
 		DrawMultiLine(pDC, rcTip3, strText);
 
 		/*CPoint ptStart(rcIONET.left, rcIONET.bottom-30);
 		CPoint ptEnd(rcDASCON.right, rcIONET.bottom-30);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(255,200,0));
 
 		CPoint ptDashStart(rcDASCON.left+rcDASCON.Width()/2, rcDASCON.top);
 		CPoint ptDashEnd(rcTip3.right, rcTip3.top+rcTip3.Height()/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/

		int abc;
		//��������
		//dascon-->ionet
		CPoint ptStart(m_rcDASCON.left+70, m_rcDASCON.bottom);
		CPoint ptEnd(m_rcDASCON.left+70, m_rcIONET.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));

		CRect rcTip4;
		rcTip4.top = m_rcDSER.bottom-40;
		rcTip4.bottom = m_rcDASCON.top-3;
		rcTip4.left = m_rcDASCON.left-5;
		rcTip4.right = m_rcDSER.right;

		pDC->Rectangle(&rcTip4);

		strText = "4.�û�POCCͨ��DAS���߽���DASCON�ش��Ĺ滮��Ϣ";
		DrawMultiLine(pDC, rcTip4, strText);


		//ionet-->pocc
		ptStart.SetPoint(m_rcDASCON.left+70, m_rcIONET.bottom-25);
		ptEnd.SetPoint(m_rcDASCON.left+70, m_rcPOCC.top);
		DrawArrowLine(pDC->GetSafeHdc(), &ptStart, &ptEnd, 8, 10, RGB(255,0,255));
 	}
 
 	if (m_bStep4)
 	{
 		//������ʾ��Ϣ
 		/*CRect rcTip4;
 		rcTip4.top = rcFDF.bottom+20;
 		rcTip4.bottom = rcSWSI.top-15;
 		rcTip4.left = rcFDF.left-60;
 		rcTip4.right = rcFDF.right+20;
 
 		CString strText("4.�û�POCCͨ��DAS���߽���DASCON�ش��Ĺ滮��Ϣ");
 		DrawMultiLine(pDC, rcTip4, strText);
 */
 		/*CPoint ptStart(rcDASCON.right, rcIONET.bottom-10);
 		CPoint ptEnd(rcIONET.left, rcIONET.bottom-10);
 		DrawArrowLine(pDC->GetSafeHdc(),&ptStart,
 			&ptEnd, 8, 10,RGB(100,250,80));
 
 		ptStart.SetPoint(rcIONET.right, rcSWSI.top+rcSWSI.Height()/2+15);
 		ptEnd.SetPoint(rcSWSI.left, rcSWSI.top+rcSWSI.Height()/2+15);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(100,250,80));
 
 		ptStart.SetPoint(rcSWSI.right-20, rcSWSI.bottom);
 		ptEnd.SetPoint(rcSWSI.right-20, rcPOCC.top);
 		DrawArrowLine(pDC->GetSafeHdc(), &ptStart,
 			&ptEnd, 8, 10, RGB(100,250,80));
 
 		CPoint ptDashStart(rcSWSI.right+10, rcTip4.bottom-25);
 		CPoint ptDashEnd(rcSWSI.right+10, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);
 
 		ptDashStart.SetPoint(rcTip4.right-10, rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
 		ptDashEnd.SetPoint(rcSWSI.right-15,rcSWSI.bottom+(rcPOCC.top-rcSWSI.bottom)/2);
 		DrawDASHLine(pDC, ptDashStart, ptDashEnd);*/
 
 	}
 
 	pDC->SelectObject(pOldFont);

}

void CDlgMAFPhase1::DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd)
{
	CPen pen(PS_DASH,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CDlgMAFPhase1::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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
void CDlgMAFPhase1::OnTimer(UINT_PTR nIDEvent)
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
		SetTimer(enmTimer_Exit, 4000, NULL);
	}
	else if (enmTimer_Exit == nIDEvent)
	{
		KillTimer(enmTimer_Exit);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}

int CDlgMAFPhase1::DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
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

void CDlgMAFPhase1::DrawOneLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, COLORREF cr)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 4, cr);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	// Draw line
	::MoveToEx(hDC, lpPointStart->x, lpPointStart->y, NULL);
	::LineTo(hDC, lpPointEnd->x, lpPointEnd->y);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);

}

void CDlgMAFPhase1::OnBnClickedBtnPauseMaf1()
{
	
}
