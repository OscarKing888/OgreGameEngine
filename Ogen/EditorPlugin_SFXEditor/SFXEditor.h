#pragma once

extern const uint32 C_ViewportVisibilityFlag;

class SFXEditor
	: public IEventLisitener
	, public OgeAlloc
	, public Singleton<SFXEditor>
	, public ActionCommandManager
	, public IUpdateAble
{
public:

	SFXEditor(void);
	virtual ~SFXEditor(void);

	void SetMeshFile(const String& meshFile);
	const String& GetMeshFile() const;

	void EditSFX(const String& templateName);

	void CreateSFXInstance(const String& templateName);
	void DestroyCurrentSFXInstance();

	void CreateNewSFX(const String& templateName);
	void AddElement(const String& elementType);

	bool GetShowBounding() const { return _showBounding; }
	void SetShowBounding(bool val);

	SpecialEffects* GetCurrentSFX() const { return _currentSFX; }
	String GetCurrentTemplate() const { return _currentTemplate; }

	void AddCommand(ICommandXPtr cmd, bool execute = true);
	void Undo();
	void Redo();
	bool IsUndoEnable() const;
	bool IsRedoEnable() const;

	void SaveAllSFXTemplate();
	void DeleteCurrentSFXTemplate();

	void StepUpdate(float t, float step = 0.01f);

protected:

	Entity*			_mesh;
	String			_meshFile;

	SpecialEffects*	_currentSFX;
	SceneNode*		_node;
	String			_currentTemplate;
	bool			_showBounding;

	CommandHistory	_cmdHistory;

	void CreateMesh();
	void DestroyMesh();

	virtual void OnEvents(const String& eventName,
		const Any& eventContex, bool& stopBroadCast);

	virtual void Update(float deltaTime);
};
