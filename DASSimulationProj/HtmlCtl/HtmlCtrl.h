////////////////////////////////////////////////////////////////
// ˵��: �Ի����ڲ���ʾHTML��ҳ
// ���ߣ�����(airen3339@163.com)
// ���ڣ�2005.12
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxhtml.h>

class CHtmlCtrl : public CHtmlView {
public:
	CHtmlCtrl() { }
	~CHtmlCtrl() { }

	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

	// Normally, CHtmlView destroys itself in PostNcDestroy,
	// but we don't want to do that for a control since a control
	// is usually implemented as a stack object in a dialog.
	//
	virtual void PostNcDestroy() {  }

	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CHtmlCtrl)
};

