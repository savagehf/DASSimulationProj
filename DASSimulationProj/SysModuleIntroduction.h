#pragma once


class CDlgInstroduceSystem;

class CSysModuleIntroduction : public CDialog
{
	DECLARE_DYNAMIC(CSysModuleIntroduction)

public:
	CSysModuleIntroduction(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSysModuleIntroduction();

// Dialog Data
	enum { IDD = IDD_DLG_INTRODUCE_MODULES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void DrawClient(CDC* pDC);
	void DrawMultiLine(CDC* pDC, CRect rect, CString strText);


protected:
	HICON m_hIcon;
	CFont m_font;

	/*CRect m_rcFDF;
	CRect m_rcNCC;
	CRect m_rcSWSI;
	CRect m_rcPOCC;
	CRect m_rcIONET;
	CRect m_rcDSER;
	CRect m_rcDASCON;
	CRect m_rcControl;
	CRect m_rcDBF;
	CRect m_rcDMG;
	CRect m_rcDARCM;
	CRect m_rcTermina;*/

	
	CDlgInstroduceSystem* m_pIntroModuleDlg;
	DECLARE_MESSAGE_MAP()
};
