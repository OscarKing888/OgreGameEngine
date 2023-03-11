#include "EditorCorePrerequisites.h"
#include "MFCInputManager.h"
#include "EditorConfig.h"

///
/// Object EditState map manager, used for get object edit state when pick.
///
class _EditorCoreExport ObjectEditStateManager : public SingletonMyers<ObjectEditStateManager>
{
public:

	/// Add a edit state id mapping for a game object type
	void AddEditState(const String& objectTypeName, String editState);
	/// Get the edit state id, return -1 if not found.
	String GetEditStateID(IGameObject* obj) const;
	/// Active the state if has edit state
	void ActiveObjectEditState(IGameObject* obj);

protected:

	typedef map<String, String>::type ObjectEditStateMap;
	ObjectEditStateMap _objEditStateMap;
};

//--------------------------------------------------------------------------------

/**
* 动态编辑接口，如鼠标移动物体
* 将鼠标按下下弹起之间所有的移动整合成一个Command以方便undo/redo
*/
class _EditorCoreExport IActiveEdit : public RefObject
{
public:

	virtual ICommandXPtr GetCommand() = 0;
	virtual void Execute(const Any& contex) = 0;
};

SmartPointer(IActiveEdit);

//--------------------------------------------------------------------------------

///
/// AbstractActiveEdit
///
class _EditorCoreExport AbstractActiveEdit : public IActiveEdit
{
public:

	AbstractActiveEdit()
		: _isDirty(false)
	{
	}

protected:

	bool	_isDirty;
};

//--------------------------------------------------------------------------------


class _EditorCoreExport AbstractEditState
	: public IState
	, public IMFCKeyListener
	, public IMFCMouseListener
{
public:

	AbstractEditState(IStateManager* pMgr);
	virtual ~AbstractEditState();

	virtual void		OnActive();
	virtual void		OnInActive();

	Camera* GetActiveCamera();

	/// Create in main toolbar
	virtual void CreateToolbar(CMFCRibbonCategory* mainCategory) = 0;

	void AddCommand(ICommandXPtr cmd, bool callExec = true);

	void BeginActiveEdit(IActiveEditPtr activeEdit);
	void CommitActiveEdit();
	bool IsActiveEditing() const;
	void ExecuteActiveEdit(const Any& contex);

	void BeginBatchEdit();
	void EndBatchEdit();
	bool IsBatchEditMode() const;

	bool IsLMBDown() const;
	bool IsRMBDown() const;
	bool IsMMBDown() const;

	bool IsShiftDown() const;
	bool IsCtrlDown() const;

	void AddMemento(IMementoPtr m);
	void RemoveAllMemento();

	static void CheckLoadTerrain( const Vector3& pos);

protected:

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);

	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);


	IActiveEditPtr	_activeEdit;
	bool		_isLMBDown;
	bool		_isRMBDown;
	bool		_isMMBDown;
	CPoint		_lastMouseDownPos;
	String		_toolbarIconFile;
};