
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

		CDC MemDC; //首先定义一个显示设备对象
		CBitmap MemBitmap;//定义一个位图对象

		CRect rect;
		GetClientRect(rect);


		//随后建立与屏幕显示兼容的内存显示设备
		MemDC.CreateCompatibleDC(NULL);
		//这时还不能绘图，因为没有地方画
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
		MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

		//将位图选入到内存显示设备中
		//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		//先用背景色将位图清除干净，这里我用的是白色作为背景
		//你也可以用自己应该用的颜色
		MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

		//绘图
		DrawClient(&MemDC);

		//将内存中的图拷贝到屏幕上进行显示
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);

		//绘图完成后的清理
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();

		CDialog::OnPaint();
	}
}


void CDlgInstroduceSystem::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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

void CDlgInstroduceSystem::DrawClient(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	// create and select a solid blue brush
	//白色刷子刷整个矩形框为白色。
	CBrush whiteBlue(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBlue);

	// create and select a thick, black pen
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	//这个背景色在DrawText时使用。跟刷子的颜色一样。
	//COLORREF clrOld = pDC->SetBkColor(RGB(153,204,255));


	//背景色清除为白色。
	pDC->FillRect(&rect, &whiteBlue);
	pDC->SelectObject(pOldBrush);


	//选蓝色刷子
	//CBrush brushBlue(RGB(153,204,255));
	//CBrush* pOlderBrush = pDC->SelectObject(&brushBlue);


	//选择小字体
	pDC->SelectObject(pOldPen);
	CFont* pOldFont = pDC->SelectObject(&m_font);

	//pDC->DrawText(m_strContent, &rect, DT_CENTER);
	DrawMultiLine(pDC, &rect, m_strContent);

	// put back the old objects
	//选回系统默认的刷子，取消蓝刷子
	//pDC->SelectObject(pOlderBrush);
	pDC->SelectObject(pOldFont);
}