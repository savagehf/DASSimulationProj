#pragma once


// CDlgUnMar2DShow dialog

class CDlgUnMar2DShow : public CDialog
{
	DECLARE_DYNAMIC(CDlgUnMar2DShow)

public:
	CDlgUnMar2DShow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUnMar2DShow();

// Dialog Data
	enum { IDD = IDD_DLG_UNEXPETECTED_2D_SHOW };

	void SetDialogMode(BOOL bContinuPlay)
	{
		m_bContinuePlay = bContinuPlay;
	}
	BOOL m_bContinuePlay;

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
	BOOL m_bStep3;
	BOOL m_bStep4;

	HICON m_hIcon;

	CFont m_font;



	DECLARE_MESSAGE_MAP()
};
