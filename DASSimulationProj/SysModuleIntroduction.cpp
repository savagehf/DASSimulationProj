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

		//CDC MemDC; //���ȶ���һ����ʾ�豸����
		//CBitmap MemBitmap;//����һ��λͼ����

		//CRect rect;
		//GetClientRect(rect);


		////���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
		//MemDC.CreateCompatibleDC(NULL);
		////��ʱ�����ܻ�ͼ����Ϊû�еط���
		////���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
		//MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

		////��λͼѡ�뵽�ڴ���ʾ�豸��
		////ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		////���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
		////��Ҳ�������Լ�Ӧ���õ���ɫ
		//MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

		////��ͼ
		//DrawClient(&MemDC);

		////���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
		//dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);

		////��ͼ��ɺ������
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

			//��λͼѡ�뵽�ڴ�DC��
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
	//��ɫˢ��ˢ�������ο�Ϊ��ɫ��
	CBrush whiteBlue(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBlue);

	// create and select a thick, black pen
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	//�������ɫ��DrawTextʱʹ�á���ˢ�ӵ���ɫһ����
	COLORREF clrOld = pDC->SetBkColor(RGB(153,204,255));


	//����ɫ���Ϊ��ɫ��
	pDC->FillRect(&rect, &whiteBlue);
	pDC->SelectObject(pOldBrush);

	//ѡ��ɫˢ��
	CBrush brushBlue(RGB(153,204,255));
	CBrush* pOlderBrush = pDC->SelectObject(&brushBlue);

	////FDF���ο�
	//CRect rcFDF;
	//rcFDF.right = rect.right-20;
	//rcFDF.left  = rcFDF.right-80;
	//rcFDF.top = rect.top+30;
	//rcFDF.bottom = rcFDF.top+50;
	//m_rcFDF = rcFDF;
	//pDC->Rectangle(&rcFDF);//���Ʒ�յ�shapeʱ��ʹ��Ĭ��ѡ���ˢ��ˢ��տռ��ڵı���ɫ��

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

	////SWSI���ο�
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
	//pDC->DrawText(_T("�滮����"), &rcSub2Text,DT_CENTER);


	////POCC���ο�
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
	//pDC->DrawText(_T("�û�POCC"), &rcPOCCText, DT_CENTER);

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
	//pDC->DrawText(_T("��"),&rcIONETText, DT_LEFT);
	//rcIONETText.top = rcIONETText.bottom;
	//rcIONETText.bottom = rcIONETText.top+20;
	//pDC->DrawText(_T("��"),&rcIONETText, DT_LEFT);
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
	//pDC->DrawText(_T("��"),&rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"),&rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText, DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText,DT_LEFT);
	//rcTerminaText.top = rcTerminaText.bottom;
	//rcTerminaText.bottom = rcTerminaText.top+20;
	//pDC->DrawText(_T("��"), &rcTerminaText,DT_LEFT);

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
	//DrawMultiLine(pDC, rcDBFSub1, _T("�����γ���"));



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
	//DrawMultiLine(pDC, rcDMGSub1, _T("�����"));


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
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top +40;
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top + 40;
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top + 40;
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top+40;
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);
	//rcConDataInterText.top = rcConDataInterText.bottom;
	//rcConDataInterText.bottom = rcConDataInterText.top+40;
	//pDC->DrawText(_T("��"), &rcConDataInterText, DT_LEFT);

	//ѡ��С����
	pDC->SelectObject(pOldPen);
	CFont* pOldFont = pDC->SelectObject(&m_font);


	// put back the old objects
	//ѡ��ϵͳĬ�ϵ�ˢ�ӣ�ȡ����ˢ��
	pDC->SelectObject(pOlderBrush);
	pDC->SelectObject(pOldFont);

}


void CSysModuleIntroduction::DrawMultiLine(CDC* pDC, CRect rect, CString strText)
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

void CSysModuleIntroduction::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strMessage;
	if (m_rcFDF.PtInRect(point))
	{
		strMessage = _T("FDFΪ�Ͽɵ��û������������ṩ�������ݵ����ɣ������ȷ���Լ��������֧�֡�FDF��֤��У���������ݡ�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("FDF"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcNCC.PtInRect(point))
	{
		strMessage = _T("NCC���Թ���TDRSS��Դ�͹���δ��ʱ�������TDRSS�û���������ʱͨ��InternetΪDAS �ṩTDRSSδ��ʱ��ĵ�����Ϣ��DAS���ø���Ϣ�������滮DAF��������");
		m_pIntroModuleDlg->SetTitleAndContent(_T("NCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	/*else if (m_rcSWSI.PtInRect(point))
	{
		strMessage = _T("������������ӿڣ�SN Web Services Interface��SWSI)��һ�����ڱ�׼�ġ���ƽ̨���ݵ��û��ӿڣ���Ϊ�û��ṩ��DASϵͳ��ʵʱ�¼��ӿ���ִ�з���ƻ���������䡢ʵʱ�������������ܼ�غ����ݻ�ȡ��");
		m_pIntroModuleDlg->SetTitleAndContent(_T("SWSI"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}*/
	else if (m_rcPOCC.PtInRect(point))
	{
		strMessage = _T("�û�POCC��DAS�ķ������Ҳ��DASҵ��ķ����ߡ������²��ֵ������У�����Ĭ��DAS�û�λ��POCC����DAS�����У�DAS�û�ͨ��POCC�����������������DAS����");
		m_pIntroModuleDlg->SetTitleAndContent(_T("POCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcIONET.PtInRect(point))
	{
		strMessage = _T("  IONET: IP Operational Network, IPҵ�����硣");
		m_pIntroModuleDlg->SetTitleAndContent(_T("POCC"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDSER.PtInRect(point))
	{
		strMessage = _T("DSERϵͳ�����ɿɱ��ң�⴦����������·�����������뱣��IONet �Լ������û��ӿ����ӵ���̫����������ת��������ɡ�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DSER"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDASCON.PtInRect(point))
	{
		strMessage = _T("DASCON�����ṩ��DAS�����DAS�û���NCC��FDF֮�����Ҫ���ƽӿڣ�ͬʱ����DAS�豸��DASCONͨ������DSERʵ�ֶ�UP��DAS�û�֮����Ϣ�ĸ�ʽ���������·�ɣ���ɶ�ǰ�򡢷�������Ĺ����DAS������ơ�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DASCON"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcControl.PtInRect(point))
	{
		strMessage = _T("�������ݽӿ�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DASCON"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDBF.PtInRect(point))
	{
		strMessage = _T("DBF��ICON�Ŀ��������DA DAR��Multiple Access Return����ַ������·�Ĳ����γɡ�DBF���ִ��SGLT�Ĳ����γ��豸�нӿڣ����ҿ����γɴ�����DAR��·���߲���");
		
		m_pIntroModuleDlg->SetTitleAndContent(_T("DBF"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDMG.PtInRect(point))
	{
		strMessage = _T("DMG���Ԫ����Ҫ�ڲ����γ�֮��ָ�DAR��·���ݡ����ϵͳ�������Բ����γ�������Ƶ�����ͨ����Ƶ���ء��źŽ�������ݶ�·���ü�������������DSER��");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DMG"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcDARCM.PtInRect(point))
	{
		strMessage = _T("DARCM������·��������˵���SGLT��ķ�����·DAS�豸���еĿ��ƺͼ��ӡ�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DARCM"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}
	else if (m_rcSGLT.PtInRect(point))
	{
		strMessage = _T("SGLT: Space-Ground Link Terminal	�յ���·�ն�");
		m_pIntroModuleDlg->SetTitleAndContent(_T("DARCM"), strMessage);
		m_pIntroModuleDlg->ShowWindow(SW_SHOW);
		m_pIntroModuleDlg->BringWindowToTop();
	}

// 	if(!strMessage.IsEmpty())
// 	{
// 		AfxMessageBox(strMessage);
// 	}
}