#pragma once


// CDlgMAFMARPhase2 dialog
class CDlgCreateTaskWithUsabletime;
class CDlgMAFMARPhase2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMAFMARPhase2)

public:
	CDlgMAFMARPhase2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMAFMARPhase2();

// Dialog Data
	enum { IDD = IDD_DLG_MAFMAR_PHASE_2 };

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

	BOOL m_bStep1Flash;
	BOOL m_bStep2Flash;
	BOOL m_bStep3Flash;

	int m_nCurLineLen1;
	int m_nCurLineLen2;
	int m_nCurLineLen3;

	int m_nTotalLen1;
	int m_nTotalLen2;
	int m_nTotalLen3;

	HICON m_hIcon;

	CFont m_font;

	CDlgCreateTaskWithUsabletime* m_pparent;

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPause;
	afx_msg void OnBnClickedBtnMar2PauseFlash();
};
