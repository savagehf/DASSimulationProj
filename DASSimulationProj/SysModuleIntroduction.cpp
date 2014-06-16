// SysModuleIntroduction.cpp : implementation file
//

#include "stdafx.h"
#include "DASSimulation.h"
#include "SysModuleIntroduction.h"
#include "DlgInstroduceSystem.h"
#include "GlobalConstants.h"

// CSysModuleIntroduction dialog

IMPLEMENT_DYNAMIC(CSysModuleIntroduction, CDialog)

CSysModuleIntroduction::CSysModuleIntroduction(CWnd* pParent /*=NULL*/)
	: CDialog(CSysModuleIntroduction::IDD, pParent)
{
	m_pIntroModuleDlg = NULL;
}

CSysModuleIntroduction::~CSysModuleIntroduction()
{
	if (NULL != m_pIntroModuleDlg)
	{
		m_pIntroModuleDlg->DestroyWindow();
		delete m_pIntroModuleDlg;
		m_pIntroModuleDlg = NULL;
	}
}

void CSysModuleIntroduction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSysModuleIntroduction, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgMARPhase5 message handlers
BOOL CSysModuleIntroduction::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

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


	m_pIntroModuleDlg = new CDlgInstroduceSystem;
	m_pIntroModuleDlg->Create(CDlgInstroduceSystem::IDD);
	m_pIntroModuleDlg->ShowWindow(SW_HIDE);

	return TRUE; 
}

BOOL CSysModuleIntroduction::OnEraseBkgnd(CDC* pDC )
{
	return TRUE;
}

void CSysModuleIntroduction::OnPaint()
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
		//CPaintDC dc(this);

		//CDC MemDC; //首先定义一个显示设备对象
		//CBitmap MemBitmap;//定义一个位图对象

		//CRect rect;
		//GetClientRect(rect);


		////随后建立与屏幕显示兼容的内存显示设备
		//MemDC.CreateCompatibleDC(NULL);
		////这时还不能绘图，因为没有地方画
		////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
		//MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

		////将位图选入到内存显示设备中
		////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		////先用背景色将位图清除干净，这里我用的是白色作为背景
		////你也可以用自己应该用的颜色
		//MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

		////绘图
		//DrawClient(&MemDC);

		////将内存中的图拷贝到屏幕上进行显示
		//dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);

		////绘图完成后的清理
		//MemBitmap.DeleteObject();
		//MemDC.DeleteDC();

		//CDialog::OnPaint();

		CPaintDC dc(this);
		CRect rect;
		GetClientRect(rect);
		CBitmap bmp;
		if (bmp.LoadBitmap(IDB_DAS_SYSTEM))
		{
			BITMAP bmpInfo;
			bmp.GetBitmap(&bmpInfo);

			CDC dcMemory;
			dcMemory.CreateCompatibleDC(&dc);

			//将位图选入到内存DC中
			CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

			//DrawClient(&dcMemory);

			dc.StretchBlt(0,0,rect.Width(),rect.Height(),
				&dcMemory,
				0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);

			dcMemory.SelectObject(pOldBitmap);

		}
		CDialog::OnPaint();
	}
}

void CSysModuleIntroduction::DrawClient(CDC* pDC)
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
	COLORREF clrOld = pDC->SetBkColor(RGB(153,204,255));


	//背景色清除为白色。
	pDC->FillRect(&rect, &whiteBlue);
	pDC->SelectObject(pOldBrush);

	//选蓝色刷子
	CBrush brushBlue(RGB(153,204,255));
	CBrush* pOlderBrush = pDC->SelectObject(&brushBlue);

	////FDF矩形框
	//CRect rcFDF;
	//rcFDF.right = rect.right-20;
	//rcFDF.left  = rcFDF.right-80;
	//rcFDF.top = rect.top+30;
	//rcFDF.bottom = rcFDF.top+50;
	//m_rcFDF = rcFDF;
	//pDC->Rectangle(&rcFDF);//绘制封闭的shape时，使用默认选择的刷子刷封闭空间内的背景色。

	//CRect rcFDFText;
	//rcFDFText = rcFDF;
	//rcFDFText.top += 20;
	//pDC->DrawText(_T("FDF"), &rcFDFText, DT_CENTER);

	//CRect rcNCC;
	//rcNCC.top = rcFDF.bottom+20;
	//rcNCC.bottom = rcNCC.top+50;
	//rcNCC.left = rcFDF.left;
	//rcNCC.right = rcFDF.right;
	//m_rcNCC = rcNCC;
	//pDC->Rectangle(&rcNCC);
	//CRect rcNCCText;
	//rcNCCText = rcNCC;
	//rcNCCText.top += 20;
	//pDC->DrawText(_T("NCC"), &rcNCCText, DT_CENTER);

	////SWSI矩形框
	//CRect rcSWSI;
	//rcSWSI.right = rect.right-20;
	//rcSWSI.left = rcSWSI.right-80;
	//rcSWSI.top = rcFDF.bottom+100;
	//rcSWSI.bottom = rcSWSI.top+100;
	//m_rcSWSI = rcSWSI;
	//pDC->Rectangle(&rcSWSI);
	//CRect rcSub1;
	//rcSub1.top = rcSWSI.top+10;
	//rcSub1.bottom = rcSub1.top+30;
	//rcSub1.left = rcSWSI.left+5;
	//rcSub1.right=rcSWSI.right-5;
	//pDC->Rectangle(&rcSub1);
	//CRect rcSub1Text;
	//rcSub1Text = rcSub1;
	//rcSub1Text.top +=9;
	//pDC->DrawText(_T("SWSI"), &rcSub1Text, DT_CENTER);

	//CRect rcSub2;
	//rcSub2.bottom = rcSWSI.bottom-10;
	//rcSub2.top = rcSub2.bottom-30;
	//rcSub2.left = rcSub1.left;
	//rcSub2.right = rcSub1.right;
	//pDC->Rectangle(&rcSub2);
	//CRect rcSub2Text;
	//rcSub2Text = rcSub2;
	//rcSub2Text.top += 9;
	//pDC->DrawText(_T("规划工具"), &rcSub2Text,DT_CENTER);


	////POCC矩形框
	//CRect rcPOCC;
	//rcPOCC.right = rect.right-20;
	//rcPOCC.left = rcPOCC.right-80;
	//rcPOCC.top = rcSWSI.bottom + 40;
	//rcPOCC.bottom = rcPOCC.top+50;
	//m_rcPOCC = rcPOCC;
	//pDC->Rectangle(&rcPOCC);
	//CRect rcPOCCText;
	//rcPOCCText = rcPOCC;
	//rcPOCCText.top = rcPOCC.top+15;
	//pDC->DrawText(_T("用户POCC"), &rcPOCCText, DT_CENTER);

	//CRect rcIONET;
	//rcIONET.top = rcFDF.top;
	//rcIONET.bottom = rcSWSI.bottom;
	//rcIONET.right = rcFDF.left-120;
	//rcIONET.left = rcIONET.right-30;
	//m_rcIONET = rcIONET;
	//pDC->Rectangle(&rcIONET);
	//CRect rcIONETText;
	//rcIONETText = rcIONET;
	//rcIONETText.top += 60;
	//rcIONETText.bottom = rcIONETText.top+20;
	//rcIONETText.left += 8;
	//pDC->DrawText(_T("保"),&rcIONETText, DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("密"),&rcIONETText, DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("I"), &rcIONETText, DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("O"), &rcIONETText, DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("N"), &rcIONETText,DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("E"), &rcIONETText,DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("T"), &rcIONETText,DT_LEFT);


	//CRect rcDSER;
	//rcDSER.right = rcIONET.left-80;
	//rcDSER.left = rcDSER.right-80;
	//rcDSER.top = rcFDF.bottom;
	//rcDSER.bottom = rcDSER.top+50;
	//m_rcDSER = rcDSER;
	//pDC->Rectangle(&rcDSER);
	//CRect rcDSERText;
	//rcDSERText = rcDSER;
	//rcDSERText.top += 15;
	//pDC->DrawText(_T("DSER"), &rcDSERText, DT_CENTER);


	//CRect rcDASCON;
	//rcDASCON.bottom = rcIONET.bottom;
	//rcDASCON.top = rcDASCON.bottom-50;
	//rcDASCON.right = rcDSER.right;
	//rcDASCON.left = rcDSER.left;
	//m_rcDASCON = rcDASCON;
	//pDC->Rectangle(&rcDASCON);
	//CRect rcDASConText;
	//rcDASConText = rcDASCON;
	//rcDASConText.top += 15;
	//pDC->DrawText(_T("DASCON"), rcDASConText, DT_CENTER);

	////Termina
	//CRect rcTermina;
	//rcTermina.left = rect.left+20;
	//rcTermina.right = rcTermina.left+40;
	//rcTermina.top = rect.top+30;
	//rcTermina.bottom = rect.bottom-20;
	//m_rcTermina = rcTermina;
	//pDC->Rectangle(&rcTermina);
	//CRect rcTerminaText;
	//rcTerminaText = rcTermina;
	//rcTerminaText.top += 60;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//rcTerminaText.left += 8;
	//pDC->DrawText(_T("现"),&rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("有"),&rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("地"), &rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("面"), &rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("终"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("端"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("设"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("备"), &rcTerminaText,DT_LEFT);

	////DBF
	//CRect rcDBF;
	//rcDBF.top = rect.top+30;
	//rcDBF.bottom = rcDBF.top+150;
	//rcDBF.left = rcTermina.left+50;
	//rcDBF.right = rcDBF.left+150;
	//m_rcDBF = rcDBF;
	//pDC->Rectangle(&rcDBF);
	//CRect rcDBFText;
	//rcDBFText.top = rcDBF.top+4;
	//rcDBFText.bottom = rcDBFText.top+18;
	//rcDBFText.left = rcDBF.left+2;
	//rcDBFText.right = rcDBFText.left+30;
	//pDC->DrawText(_T("DBF"), rcDBFText, DT_LEFT);
	//CRect rcDBFSub1;
	//rcDBFSub1.left = rcDBF.left+50;
	//rcDBFSub1.right = rcDBFSub1.left+70;
	//rcDBFSub1.top = rcDBF.top+15;
	//rcDBFSub1.bottom = rcDBF.bottom-15;
	//pDC->Rectangle(&rcDBFSub1);
	//DrawMultiLine(pDC, rcDBFSub1, _T("波束形成器"));



	////DMG
	//CRect rcDMG;
	//rcDMG.top = rcDBF.top;
	//rcDMG.bottom = rcDBF.bottom;
	//rcDMG.left = rcDBF.right+10;
	//rcDMG.right = rcDMG.left+150;
	//m_rcDMG = rcDMG;
	//pDC->Rectangle(&rcDMG);
	//CRect rcDMGText;
	//rcDMGText.left = rcDMG.left+4;
	//rcDMGText.right = rcDMG.right;
	//rcDMGText.top = rcDMG.top+5;
	//rcDMGText.bottom = rcDMG.top+18;

	//pDC->DrawText(_T("DMG"),rcDMGText, DT_LEFT);
	//CRect rcDMGSub1;
	//rcDMGSub1.left = rcDMG.left+50;
	//rcDMGSub1.right = rcDMGSub1.left+70;
	//rcDMGSub1.top = rcDMG.top+15;
	//rcDMGSub1.bottom = rcDMG.bottom-15;
	//pDC->Rectangle(&rcDMGSub1);
	//DrawMultiLine(pDC, rcDMGSub1, _T("解调器"));


	////DARCM
	//CRect rcDARCM;
	//rcDARCM.left = rcDBF.left;
	//rcDARCM.right = rcDMG.right;
	//rcDARCM.top = rcDBF.bottom + 60;
	//rcDARCM.bottom = rcDARCM.top+80;
	//m_rcDARCM = rcDARCM;
	//pDC->Rectangle(&rcDARCM);
	//CRect rcDARCMText;
	//rcDARCMText.left = rcDARCM.left;
	//rcDARCMText.right = rcDARCM.right;
	//rcDARCMText.top = rcDARCM.top+3;
	//rcDARCMText.bottom = rcDARCM.top+20;
	//pDC->DrawText(_T("DARCM"), &rcDARCMText, DT_CENTER);
	//CRect rcDARCMICON;
	//rcDARCMICON.left = rcDARCM.left+18;
	//rcDARCMICON.right = rcDARCMICON.left+120;
	//rcDARCMICON.top = rcDARCM.top+20;
	//rcDARCMICON.bottom = rcDARCMICON.top+25;
	//pDC->Rectangle(&rcDARCMICON);
	//CRect rcDARCMICONText;
	//rcDARCMICONText = rcDARCMICON;
	//rcDARCMICONText.top += 3;
	//pDC->DrawText(_T("ICON"), &rcDARCMICONText, DT_CENTER);
	//CRect rcDARCMDCON;
	//rcDARCMDCON.right = rcDARCM.right-18;
	//rcDARCMDCON.left = rcDARCMDCON.right-120;
	//rcDARCMDCON.top = rcDARCMICON.top;
	//rcDARCMDCON.bottom = rcDARCMICON.bottom;
	//pDC->Rectangle(rcDARCMDCON);
	//CRect rcDARCMDCONText;
	//rcDARCMDCONText = rcDARCMDCON;
	//rcDARCMDCONText.top += 3;
	//pDC->DrawText(_T("DCON"), &rcDARCMDCONText, DT_CENTER);

	//CRect rcConDataInter;
	//rcConDataInter.top = rect.top+30;
	//rcConDataInter.bottom = rect.bottom-20;
	//rcConDataInter.left = rcDMG.right+50;
	//rcConDataInter.right = rcConDataInter.left + 30;
	//m_rcControl = rcConDataInter;
	//pDC->Rectangle(&rcConDataInter);
	//CRect rcConDataInterText;
	//rcConDataInterText = rcConDataInter;
	//rcConDataInterText.top += 60;
	//rcConDataInterText.bottom = rcConDataInterText.top+40;
	//rcConDataInterText.left += 8;
	//pDC->DrawText(_T("控"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top +40;
	//pDC->DrawText(_T("制"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top + 40;
	//pDC->DrawText(_T("数"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top + 40;
	//pDC->DrawText(_T("据"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top+40;
	//pDC->DrawText(_T("接"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top+40;
	//pDC->DrawText(_T("口"), &rcConDataInterText, DT_LEFT);

	//选择小字体
	pDC->SelectObject(pOldPen);
	CFont* pOldFont = pDC->SelectObject(&m_font);


	// put back the old objects
	//选回系统默认的刷子，取消蓝刷子
	pDC->SelectObject(pOlderBrush);
	pDC->SelectObject(pOldFont);

}


void CSysModuleIntroduction::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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

void CSysModuleIntroduction::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strMessage;
	if (m_rcFDF.PtInRect(point))
	{
		strMessage = _T("FDF为认可的用户飞行器任务提供捕获数据的生成，轨道的确定以及轨道机动支持。FDF验证并校正跟踪数据。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("FDF"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcNCC.PtInRect(point))
	{
		strMessage = _T("NCC可以管理TDRSS资源和构建未用时间表，在有TDRSS用户服务请求时通过Internet为DAS 提供TDRSS未用时间的调度信息，DAS利用该信息来评估规划DAF服务请求");
		m_pIntroModuleDlg->SetTitleAndContent(_T("NCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	/*else if (m_rcSWSI.PtInRect(point))
	{
		strMessage = _T("天基网网络服务接口（SN Web Services Interface，SWSI)是一个基于标准的、跨平台兼容的用户接口，它为用户提供与DAS系统的实时事件接口来执行服务计划、服务分配、实时操作、服务性能监控和数据获取。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("SWSI"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}*/
	else if (m_rcPOCC.PtInRect(point))
	{
		strMessage = _T("用户POCC是DAS的服务对象，也是DAS业务的发起者。在以下部分的描述中，我们默认DAS用户位于POCC。在DAS服务中，DAS用户通过POCC发送请求来完成整个DAS服务");
		m_pIntroModuleDlg->SetTitleAndContent(_T("POCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcIONET.PtInRect(point))
	{
		strMessage = _T("  IONET: IP Operational Network, IP业务网络。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("POCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDSER.PtInRect(point))
	{
		strMessage = _T("DSER系统由若干可编程遥测处理器，若干路由器，若干与保密IONet 以及本地用户接口连接的以太网集线器和转换开关组成。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DSER"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDASCON.PtInRect(point))
	{
		strMessage = _T("DASCON功能提供了DAS服务和DAS用户、NCC、FDF之间的主要控制接口，同时控制DAS设备。DASCON通过配置DSER实现对UP和DAS用户之间信息的格式化、缓冲和路由，完成对前向、返向命令的管理和DAS服务控制。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DASCON"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcControl.PtInRect(point))
	{
		strMessage = _T("控制数据接口");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DASCON"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDBF.PtInRect(point))
	{
		strMessage = _T("DBF在ICON的控制下完成DA DAR（Multiple Access Return，多址返向）链路的波束形成。DBF和现存的SGLT的波束形成设备有接口，并且可以形成大量的DAR链路天线波束");
		
		m_pIntroModuleDlg->SetTitleAndContent(_T("DBF"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDMG.PtInRect(point))
	{
		strMessage = _T("DMG组成元素需要在波束形成之后恢复DAR链路数据。这个系统接收来自波束形成器的中频输出，通过中频开关、信号解调和数据多路复用技术将数据送往DSER。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DMG"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDARCM.PtInRect(point))
	{
		strMessage = _T("DARCM返向链路功能完成了单个SGLT里的返向链路DAS设备所有的控制和监视。");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DARCM"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcSGLT.PtInRect(point))
	{
		strMessage = _T("SGLT: Space-Ground Link Terminal	空地链路终端");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DARCM"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}

// 	if(!strMessage.IsEmpty())
// 	{
// 		AfxMessageBox(strMessage);
// 	}
}