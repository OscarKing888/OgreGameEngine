#pragma once
#include "EditorCorePrerequisites.h"


class _EditorCoreExport ResListTreeCtrl : public CTreeCtrl
{
	// Construction
public:
	ResListTreeCtrl();

	// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// Implementation
public:
	virtual ~ResListTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};

//------------------------------------------------------------------------

class _EditorCoreExport ResViewPanel
	: public CDockablePane
	, public IAppListener
	, public WorkQueue::ResponseHandler
	, public WorkQueue::RequestHandler
{
	DECLARE_DYNAMIC(ResViewPanel)
	friend ResListTreeCtrl;

public:
	ResViewPanel(bool hasTreeList = true);
	virtual ~ResViewPanel();

	bool IsStringCanDisplay(const CString& str);
	void ClearListItem();
	void ClearTreeItem();

	const CString& GetSelectedTreeItemText() const { return _selectedTreeItemName; }
	const CString& GetSelectedListItemText() const { return _selectedListItemName; }

	virtual void OnRefreshTreeItem(void);
	virtual void OnRefreshListItem(void);

	virtual CString GetPanelTitle() const = 0;

	void Show();

	void AddLoadingRequest(const Any& req,
		uint16 requestType = WORKQUEUE_THUMB_REQUEST, bool forceSynchronous = false);

	void WaitForLoadingProcesses();

	/// Return the game object class type name to create when drop
	const String& GetDragdropCreateGameObjectTypeName() const;
	/// Return if this resource view can create with drag drop
	bool CanDragCreate() const { return _canDragCreate; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	// Resource name filter edit box
	CComboBox _filterBox;
	CStatic _filterBoxCaption;
	ResListTreeCtrl _treeCtrl;
	CListCtrl _listCtrl;
	CString _selectedTreeItemName;
	CString _selectedListItemName;
	CString _currentFilter;
	bool _hasTreeList;
	float _leftCtlrWidthRatio;
	CFont _defaultFont;

	struct SItemData
	{
		CString Text;
		int ImageIndex;
	};

	typedef std::list<SItemData> HideItemList;
	HideItemList	_hideItemList;

	CImageList _iconImages;
	CImageList _iconImagesSmall;
	typedef std::map<String, int> ImageListMap;
	ImageListMap _iconMap;

	uint16 _workQueueChannel;
	bool _isLoading;
	
	String _dragdropGameObjectTypeName;
	bool _canDragCreate;
	
	static const uint16 WORKQUEUE_THUMB_REQUEST;

	void AdjustLayout(void);

	virtual void OnInitTreeItem(void);

	virtual void OnInitListItem(void);

	virtual void OnDBClickTreeItem(HTREEITEM item, const CString& resName);
	virtual void OnClickTreeItem(HTREEITEM item, const CString& resName);

	virtual void OnDBClickListItem(int item, const CString& resName);
	virtual void OnClickListItem(int item, const CString& resName);

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnBeforeShowPaneMenu(CMenu& m);
	virtual BOOL OnAfterShowPaneMenu(int nMenuResult);

	virtual void OnAppInitCompleted();
	virtual void OnAppShutdown();

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFilterChange();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDBClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFilterLostFocus();
	afx_msg void OnListItemChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListItemDBClick(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnViewBigIconList();
	afx_msg void OnViewSmallIconList();
	afx_msg void OnViewList();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void CreateImageList();
	int AddToImageList(const String& fileName);
	void AddToSmallImageList(CBitmap* bmp);

	/// WorkQueue::RequestHandler override
	virtual WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	virtual void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	virtual bool canShowInTree(const String& resGroup) const;
};

//------------------------------------------------------------------------

class _EditorCoreExport  ResPanel_ResGrouped : public ResViewPanel
{
public:

	virtual void OnInitTreeItem(void);
	virtual void OnRefreshTreeItem(void);
};

//------------------------------------------------------------------------