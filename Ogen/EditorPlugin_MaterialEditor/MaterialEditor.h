#pragma once

extern const uint32 C_ViewportVisibilityFlag;

class MaterialEditor
	: public IEventLisitener
	, public OgeAlloc
	, public Singleton<MaterialEditor>
	, public ActionCommandManager
	, public IUpdateAble
{
public:

	MaterialEditor(void);
	virtual ~MaterialEditor(void);

	void SetMeshFile(const String& meshFile);
	const String& GetMeshFile() const;

	void EditMaterial(const String& templateName);

	bool GetShowBounding() const { return _showBounding; }
	void SetShowBounding(bool val);

	Material* GetCurrentMaterial() const { return _currentMaterial; }
	String GetCurrentTemplate() const { return _currentTemplate; }

	void AddCommand(ICommandXPtr cmd, bool execute = true);
	void Undo();
	void Redo();
	bool IsUndoEnable() const;
	bool IsRedoEnable() const;

	void SaveAllSFXTemplate();
	void DeleteCurrentSFXTemplate();

protected:

	Entity*			_mesh;
	String			_meshFile;

	Material*	_currentMaterial;
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
