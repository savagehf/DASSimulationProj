#pragma once

class CDlgInstroduceSystem : public CDialog
{
	DECLARE_DYNAMIC(CDlgInstroduceSystem)

public:
	CDlgInstroduceSystem(CWnd* pParent = NULL);  
	virtual ~CDlgInstroduceSystem();

	enum { IDD = IDD_DLG_INTRODUCE_SYSTEM };

	void SetTitleAndContent(CString strTitle, CString strContent)
	{
		m_strTitle = strTitle;
		SetWindowText(m_strTitle);
		m_strContent = strContent;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC );

	void DrawClient(CDC* pDC);
	void DrawMultiLine(CDC* pDC, CRect rect, CString strText);

protected:
	HICON m_hIcon;
	CFont m_font;
	CString m_strTitle;
	CString m_strContent;

	DECLARE_MESSAGE_MAP()
};
