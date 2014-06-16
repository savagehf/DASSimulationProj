#pragma once


// CDlgMAFMARPhase3 dialog

class CDlgMAFMARPhase3 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMAFMARPhase3)

public:
	CDlgMAFMARPhase3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMAFMARPhase3();

// Dialog Data
	enum { IDD = IDD_DLG_MAFMAR_PHASE_3 };

	void SetDialogMode(BOOL bContinuPlay)
	{
		m_bContinuePlay = bContinuPlay;
	}
	BOOL m_bContinuePlay;

	void SetTitle(CString strTitle)
	{
		m_strTitle = strTitle;
	}
	void SetStartGapTime(UINT nTimeGap)
	{
		m_nTaskStartGap = nTimeGap;
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
	void DrawOneLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, COLORREF cr);
	int  DrawArrowLine(HDC hDC, LPPOINT lpPointStart, LPPOINT lpPointEnd, 
		double nArrowBorderLen, double xAngleInRadians, COLORREF cr);
	void DrawDASHLine(CDC* pDC, CPoint ptStart, CPoint ptEnd);
	void DrawTitleTips(CDC* pDC, CRect rcClient);

protected:
	BOOL m_bStep1;
	BOOL m_bStep2;
	BOOL m_bStep3;
	BOOL m_bStep4;
	BOOL m_bStep5;

	BOOL m_bStep1Flash;
	BOOL m_bStep2Flash;
	BOOL m_bStep4Flash;

	int m_nCurLineLen;
	int m_nTotalLen;

	HICON m_hIcon;

	CFont m_font;

	CString m_strTitle;
	//距离任务开始的时间。
	UINT m_nTaskStartGap;

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPause;
	afx_msg void OnBnClickedBtnMafMar3PauseFlash();
};
