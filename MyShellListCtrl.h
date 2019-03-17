#pragma once


// CMyShellListCtrl

class CMyShellListCtrl : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CMyShellListCtrl)

public:
	CMyShellListCtrl();
	virtual ~CMyShellListCtrl();
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);

protected:
	DECLARE_MESSAGE_MAP()
};


