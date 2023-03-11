#pragma once

extern const uint32 C_ViewportVisibilityFlag;

class IParticleRenderEditor;
class IParticleEmitterEditor;
class IParticleAffectorEditor;
class ParticleSystemProxy;

class ParticleSystemEditor
	: public IEventLisitener
	, public OgeAlloc
	, public Singleton<ParticleSystemEditor>
	, public ActionCommandManager
	, public IUpdateAble
{
public:

	ParticleSystemEditor(void);
	virtual ~ParticleSystemEditor(void);

	void SetMeshFile(const String& meshFile);
	const String& GetMeshFile() const;

	void EditParticle(const String& templateName);

	void UpdateRendererEditor();
	void CreateParticleSystemInstance(const String& templateName);
	void DestroyCurrentParticleSystemInstance();

	void EditEmitter(ushort idx);
	void EditAffector(int idx);

	bool GetShowBounding() const { return _showBounding; }
	void SetShowBounding(bool val);

	ParticleSystem* GetCurrentParticles() const { return _currentParticles; }
	String GetCurrentTemplate() const { return _currentTemplate; }

	void AddCommand(ICommandXPtr cmd, bool execute = true);
	void Undo();
	void Redo();
	bool IsUndoEnable() const;
	bool IsRedoEnable() const;

	IParticleRenderEditor* RenderEditor() const { return _renderEditor; }
	//void RenderEditor(IParticleRenderEditor* val) { _renderEditor = val; }

	IParticleEmitterEditor* EmitterEditor() const { return _emitterEditor; }
	//void EmitterEditor(IParticleEmitterEditor* val) { _emitterEditor = val; }

	IParticleAffectorEditor* AffectorEditor() const { return _affectorEditor; }
	//void AffectorEditor(IParticleAffectorEditor* val) { _affectorEditor = val; }

	IPropertyObject* ParticleEditor() const { return _particleEditor; }

	void SaveAllParticleTemplate();
	void DeleteCurrentParticleTemplate();

	NameValuePairList GetDefaultEmitterParams(const String& typeName);
	NameValuePairList GetDefaultAffectorParams(const String& typeName);
	NameValuePairList GetDefaultRendererParams(const String& typeName);

protected:

	Entity*			_mesh;
	String			_meshFile;

	ParticleSystem*	_currentParticles;
	SceneNode*		_node;
	String			_currentTemplate;
	bool			_showBounding;
	//ParticleSystemProxy* _particleSysProxy;

	IParticleRenderEditor* _renderEditor;
	IParticleEmitterEditor*	_emitterEditor;
	IParticleAffectorEditor* _affectorEditor;
	IPropertyObject*		_particleEditor;

	ushort			_selectedEmitter;
	ushort			_selectedAffector;

	CommandHistory	_cmdHistory;

	void CreateMesh();
	void DestroyMesh();

	virtual void OnEvents(const String& eventName,
		const Any& eventContex, bool& stopBroadCast);

	virtual void Update(float deltaTime);
};
