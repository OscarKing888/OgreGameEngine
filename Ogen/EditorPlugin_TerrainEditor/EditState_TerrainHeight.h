#pragma once
#include "AbstractTerrainEditState.h"
#include "CmdTypes.h"

extern int EditStateID_TerrainHeight;


enum EHeightEditType
{
	EHET_DynamicElevate,
	EHET_Smooth,
	EHET_FixedHeight
};

class HeightEditType
	: public AbstractEnumDescriptor
	, public Singleton<HeightEditType>
{
public:

	HeightEditType();
	virtual ~HeightEditType();	
};

//------------------------------------------------------------------------

class AbstractEditHeightState : public IState
{
public:

	AbstractEditHeightState(IStateManager* pMgr);
	virtual ~AbstractEditHeightState();

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:

	TerrainHFData _oldData;
	bool _isPainting;

	void AddOldHF(Terrain* terrain, long x, long y, float val);
	void CommitHFChange();
};

SmartPointer(AbstractEditHeightState);

//------------------------------------------------------------------------

class EditState_TerrainHeight
	: public AbstractTerrainEditState
	, public Singleton<EditState_TerrainHeight>
{
public:

	EditState_TerrainHeight(IStateManager* pMgr);
	virtual ~EditState_TerrainHeight();

	//virtual const String& GetTypeName() const;
	virtual void CreateToolbar(CMFCRibbonCategory* mainCategory);

	virtual void		OnActive();
	virtual void		OnInActive();

	AbstractEditHeightState* GetActiveState();

	EHeightEditType GetHeightEditType() const { return _heightEditType; }
	void SetHeightEditType(EHeightEditType e);

	float GetFixedHeight() const { return _fixedHeight; }
	void SetFixedHeight(float v){ _fixedHeight = v; }

	float GetStrength() const { return _strength; }
	void SetStrength(float v){ _strength = v; }

	bool IsElevUp() const { return _isUpElev; }
	void SetElevUp(bool on) { _isUpElev = on; }

protected:

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	// properties
	EHeightEditType		_heightEditType;
	StateManager		_stateManager;
	float				_fixedHeight;
	float				_strength;
	bool				_isUpElev;

	virtual void BuildPropertyMap();
};

///
/// EditState_SubEditHFMsgHandler
///

class EditState_SubEditHFMsgHandler
	: public CCmdTarget
	, public Singleton<EditState_SubEditHFMsgHandler>
{
public:

	EditState_SubEditHFMsgHandler(EditState_TerrainHeight* state);
	virtual ~EditState_SubEditHFMsgHandler();

	DECLARE_DYNAMIC(EditState_SubEditHFMsgHandler);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnActiveSubEditState(UINT id);
	afx_msg void OnUpdateSubEditState(CCmdUI* ui);

protected:

	EditState_TerrainHeight* _state;
};