#pragma once


// CDlgMAFMARTermFlash dialog

class CDlgMAFMARTermFlash : public CDialog
{
	DECLARE_DYNAMIC(CDlgMAFMARTermFlash)

public:
	CDlgMAFMARTermFlash(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMAFMARTermFlash();

// Dialog Data
	enum { IDD = IDD_DLG_MAFMAR_TERMINATION };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC );
	afx_msg void OnClose();


	void DrawClient(CDC* pDC);
	void DrawMultiLine(CDC* pDC, CRect rect, CString strText);
	int  DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
		double nArrowBorderLen, double xAngleInRadians, COLORREF cr);
	void DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd);
	void DrawTitleTips(CDC* pDC, CRect rcClient);

protected:
	BOOL m_bStep1;
	BOOL m_bStep2;

	BOOL m_bStep1Flash;
	BOOL m_bStep2Flash;

	int m_nStep;
	int m_nCurLineLen;
	int m_nTotalLineLen;

	HICON m_hIcon;

	CFont m_font;

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPause;

	afx_msg void OnBnClickedBtnPauseMafMarFlash();
};
