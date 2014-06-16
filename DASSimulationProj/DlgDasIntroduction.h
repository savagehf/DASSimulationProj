#if !defined(AFX_DLGDASINTRODUCTION_H__914B3717_F82D_4579_A4E8_83AED8C833D8__INCLUDED_)
#define AFX_DLGDASINTRODUCTION_H__914B3717_F82D_4579_A4E8_83AED8C833D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDasIntroduction.h : header file

/////////////////////////////////////////////////////////////////////////////
// CDlgDasIntroduction dialog

#include "Resource.h"


#include "HtmlCtl\HtmlCtrl.h"
#include "HtmlCtl\HtmlTool.h"

class CDlgDasIntroduction : public CDialog
{
// Construction
public:
	CHtmlTool *pHtmlTool;
	CHtmlCtrl pHtmlPage;

	CString m_strHtmlBasePath ;
	CDlgDasIntroduction(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDasIntroduction)
	enum { IDD = IDD_DLG_INTRODUCE_MAF_MAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDasIntroduction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDasIntroduction)
	virtual BOOL OnInitDialog();
	afx_msg void OnDasMaf();
	afx_msg void OnDasMar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDASINTRODUCTION_H__914B3717_F82D_4579_A4E8_83AED8C833D8__INCLUDED_)
