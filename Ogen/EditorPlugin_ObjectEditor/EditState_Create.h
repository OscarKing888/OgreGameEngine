#pragma once

extern int EditStateID_Create;

class EditState_Create
	: public AbstractEditState
	, public IEventLisitener
{
public:

	EditState_Create(IStateManager* pMgr);
	virtual ~EditState_Create();
	
	virtual void		OnActive();
	virtual void		OnInActive();

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	virtual void CreateToolbar(CMFCRibbonCategory* mainCategory);

protected:

	String _createClassName;
};