// DlgDasIntroduction.cpp : implementation file
//

#include "stdafx.h"
//#include "dassimu.h"
#include "DlgDasIntroduction.h"
#include "MyTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDasIntroduction dialog


CDlgDasIntroduction::CDlgDasIntroduction(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDasIntroduction::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDasIntroduction)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDasIntroduction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDasIntroduction)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDasIntroduction, CDialog)
	//{{AFX_MSG_MAP(CDlgDasIntroduction)
	ON_BN_CLICKED(ID_DAS_MAF, OnDasMaf)
	ON_BN_CLICKED(ID_DAS_MAR, OnDasMar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDasIntroduction message handlers

BOOL CDlgDasIntroduction::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pHtmlPage.CreateFromStatic(IDC_HTML_PAGE, this);

	m_strHtmlBasePath = MyTools::GetCurrentPath();
	m_strHtmlBasePath = m_strHtmlBasePath + _T("\\Database\\html");

	return TRUE;  
}

void CDlgDasIntroduction::OnDasMaf() 
{
	pHtmlPage.Navigate2(m_strHtmlBasePath + _T("\\maf.htm"));	
}

void CDlgDasIntroduction::OnDasMar() 
{
	pHtmlPage.Navigate2(m_strHtmlBasePath + _T("\\mar.htm"));	
}
