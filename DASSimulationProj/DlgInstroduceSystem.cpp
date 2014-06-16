
#include "stdafx.h"
#include "DASSimulation.h"
#include "DlgInstroduceSystem.h"

IMPLEMENT_DYNAMIC(CDlgInstroduceSystem, CDialog)

CDlgInstroduceSystem::CDlgInstroduceSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInstroduceSystem::IDD, pParent)
{

}

CDlgInstroduceSystem::~CDlgInstroduceSystem()
{
}

void CDlgInstroduceSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInstroduceSystem, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CDlgInstroduceSystem::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(m_strTitle);

	m_font.CreateFont(
		30,                        // nHeight
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

BOOL CDlgInstroduceSystem::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CDlgInstroduceSystem::OnPaint()
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

		CDC MemDC; //���ȶ���һ����ʾ�豸����
		CBitmap MemBitmap;//����һ��λͼ����

		CRect rect;
		GetClientRect(rect);


		//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
		MemDC.CreateCompatibleDC(NULL);
		//��ʱ�����ܻ�ͼ����Ϊû�еط���
		//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
		MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

		//��λͼѡ�뵽�ڴ���ʾ�豸��
		//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
		//��Ҳ�������Լ�Ӧ���õ���ɫ
		MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

		//��ͼ
		DrawClient(&MemDC);

		//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);

		//��ͼ��ɺ������
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();

		CDialog::OnPaint();
	}
}


void CDlgInstroduceSystem::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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

void CDlgInstroduceSystem::DrawClient(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	// create and select a solid blue brush
	//��ɫˢ��ˢ�������ο�Ϊ��ɫ��
	CBrush whiteBlue(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBlue);

	// create and select a thick, black pen
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	//�������ɫ��DrawTextʱʹ�á���ˢ�ӵ���ɫһ����
	//COLORREF clrOld = pDC->SetBkColor(RGB(153,204,255));


	//����ɫ���Ϊ��ɫ��
	pDC->FillRect(&rect, &whiteBlue);
	pDC->SelectObject(pOldBrush);


	//ѡ��ɫˢ��
	//CBrush brushBlue(RGB(153,204,255));
	//CBrush* pOlderBrush = pDC->SelectObject(&brushBlue);


	//ѡ��С����
	pDC->SelectObject(pOldPen);
	CFont* pOldFont = pDC->SelectObject(&m_font);

	//pDC->DrawText(m_strContent, &rect, DT_CENTER);
	DrawMultiLine(pDC, &rect, m_strContent);

	// put back the old objects
	//ѡ��ϵͳĬ�ϵ�ˢ�ӣ�ȡ����ˢ��
	//pDC->SelectObject(pOlderBrush);
	pDC->SelectObject(pOldFont);
}