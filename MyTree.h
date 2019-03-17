#pragma once


// CMyTree

class CMyTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMyTree)

public:
	CMyTree();
	virtual ~CMyTree();
	virtual void showTree(char *query);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_imageList;
};


