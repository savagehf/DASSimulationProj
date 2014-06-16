#pragma once


// CDlgMARPhase6 dialog

class CDlgMARPhase6 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMARPhase6)

public:
	CDlgMARPhase6(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMARPhase6();

// Dialog Data
	enum { IDD = IDD_DLG_MAR_PHASE_6 };

	void SetDialogMode(BOOL bContinuPlay)
	{
		m_bContinuePlay = bContinuPlay;
	}
	BOOL m_bContinuePlay;

	void SetTitle(CString strTitle)
	{
		m_strTitle = strTitle;
	}

	BOOL m_bUnExpMar;
	void SetUnexpectMarflag(BOOL bUnexp)
	{
		m_bUnExpMar = bUnexp;
	}

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

	BOOL m_bStep1Flash;
	BOOL m_bStep2Flash;
	BOOL m_bStep3Flash;

	int m_nCurLineLen;
	int m_nCurLineLen1;
	int m_nTotalLen;
	int m_nTotalLen1;

	HICON m_hIcon;

	CFont m_font;

	CString m_strTitle;

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPause;
	afx_msg void OnBnClickedBtnMar6PauseFlash();
};
