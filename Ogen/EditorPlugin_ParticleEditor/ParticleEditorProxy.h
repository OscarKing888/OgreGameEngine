#pragma once

class IParticleComponentEditor
	: public PropertyObject
	, public OgeAlloc
{
public:

	virtual const String& GetTypeName() const = 0;
	virtual void ShowEditorProxyHelper(bool on) = 0;
};

class IParticleEmitterEditor : public IParticleComponentEditor
{
public:

	virtual void SetEmitter(ParticleEmitter* emitter) = 0;
	virtual ParticleEmitter* GetEmitter() const = 0;

};

class IParticleAffectorEditor : public IParticleComponentEditor
{
public:

	virtual void SetAffector(ParticleAffector* affector) = 0;
	virtual ParticleAffector* GetAffector() const = 0;

	virtual CWnd* GetEditorWnd() const = 0;
	virtual void OnAffectorParameterChanged() = 0;
};


class IParticleRenderEditor : public IParticleComponentEditor
{
public:

	virtual void SetRender(ParticleSystemRenderer* emitter) = 0;
	virtual ParticleSystemRenderer* GetRender() const = 0;
};

//------------------------------------------------------------------------

class IParticleComponentEditorFactory
{
public:

	// 返回创建时的默认参数列表
	virtual NameValuePairList GetInitDefaultParameters();
};

class IParticleEmitterEditorFactory : public IParticleComponentEditorFactory
{
public:

	virtual IParticleEmitterEditor* Create(ParticleEmitter* emitter) = 0;
	virtual void Destroy(IParticleEmitterEditor* obj) = 0;

	/// 保持和相应的Emitter一致的类名字
	virtual String GetTypeName() const = 0;
};

class IParticleAffectorEditorFactory : public IParticleComponentEditorFactory
{
public:

	virtual IParticleAffectorEditor* Create(ParticleAffector* affector) = 0;
	virtual void Destroy(IParticleAffectorEditor* obj) = 0;

	/// 保持和相应的Affector一致的类名字
	virtual String GetTypeName() const = 0;
};

class IParticleRenderEditorFactory : public IParticleComponentEditorFactory
{
public:

	virtual IParticleRenderEditor* Create(ParticleSystemRenderer* render) = 0;
	virtual void Destroy(IParticleRenderEditor* obj) = 0;

	/// 保持和相应的Renderer一致的类名字
	virtual String GetTypeName() const = 0;
};

//------------------------------------------------------------------------

template<class T>
class TEmitterEditorFactory : public IParticleEmitterEditorFactory
{
public:

	template<class Y>
	struct SDestroy
	{
		SDestroy(IParticleEmitterEditor* obj)
		{
			OGRE_DELETE dynamic_cast<Y*>(obj);
		}
	};

	virtual void Destroy(IParticleEmitterEditor* obj)
	{
		SDestroy<T> d(obj);
	}

	virtual IParticleEmitterEditor* Create(ParticleEmitter* emitter)
	{
		IParticleEmitterEditor* obj = OGRE_NEW T();
		obj->SetEmitter(emitter);
		return obj;
	}


	virtual Ogre::NameValuePairList GetInitDefaultParameters()
	{
		NameValuePairList parms;

		parms["colour"] = StringConverter::toString(ColourValue::Black);
		parms["colour_range_start"] = StringConverter::toString(ColourValue::Black);
		parms["colour_range_end"] = StringConverter::toString(ColourValue::Black);

		return parms;
	}
};

template<class T>
class TAffectorEditorFactory : public IParticleAffectorEditorFactory
{
public:

	template<class Y>
	struct SDestroy
	{
		SDestroy(IParticleAffectorEditor* obj)
		{
			OGRE_DELETE dynamic_cast<Y*>(obj);
		}
	};

	virtual void Destroy(IParticleAffectorEditor* obj)
	{
		SDestroy<T> d(obj);
	}

	virtual IParticleAffectorEditor* Create(ParticleAffector* affector)
	{
		IParticleAffectorEditor* obj = OGRE_NEW T();
		obj->SetAffector(affector);
		return obj;
	}
};

template<class T>
class TParticleRenderEditorFactory : public IParticleRenderEditorFactory
{
public:

	template<class Y>
	struct SDestroy
	{
		SDestroy(IParticleRenderEditor* obj)
		{
			OGRE_DELETE dynamic_cast<Y*>(obj);
		}
	};

	virtual void Destroy(IParticleRenderEditor* obj)
	{
		SDestroy<T> d(obj);
	}

	virtual IParticleRenderEditor* Create(ParticleSystemRenderer* render)
	{
		IParticleRenderEditor* obj = OGRE_NEW T();
		obj->SetRender(render);
		return obj;
	}
};

//------------------------------------------------------------------------

class ParticleEditorFactoryManager : public SingletonMyers<ParticleEditorFactoryManager>
{
public:

	void RegisterParticleRendererEditorFactory(IParticleRenderEditorFactory* render);
	void RegisterEmitterEditorFactory(IParticleEmitterEditorFactory* emitter);
	void RegisterAffectorEditorFactory(IParticleAffectorEditorFactory* affector);

	IParticleRenderEditor* CreateParticleRenderEditor(const String& typeName, ParticleSystemRenderer* render);
	IParticleEmitterEditor* CreateEmitterEditor(const String& typeName, ParticleEmitter* emitter);
	IParticleAffectorEditor* CreateAffectorEditor(const String& typeName, ParticleAffector* affector);

	void DestroyParticleRenderEditor(IParticleRenderEditor* renderEditor);
	void DestroyEmitterEditor(IParticleEmitterEditor* emitterEditor);
	void DestroyAffectorEditor(IParticleAffectorEditor* affectorEditor);

	IParticleRenderEditorFactory* GetRendererFactory(const String& typeName) const;
	IParticleEmitterEditorFactory* GetEmitterFactory(const String& typeName) const;
	IParticleAffectorEditorFactory* GetAffectorFactory(const String& typeName) const;
	
protected:

	typedef map<String, IParticleRenderEditorFactory*>::type ParticleRenderEditorFactoryMap;
	typedef map<String, IParticleEmitterEditorFactory*>::type EmitterEditorFactoryMap;
	typedef map<String, IParticleAffectorEditorFactory*>::type AffectorEditorFactoryMap;

	ParticleRenderEditorFactoryMap _renderEditorFactory;
	EmitterEditorFactoryMap		_emitterEditorFactory;
	AffectorEditorFactoryMap	_affectorEditorFactory;
};

//------------------------------------------------------------------------
struct ParticleRenderEditorFactoryRegHelper
{
	explicit ParticleRenderEditorFactoryRegHelper(const String& classTypeName, IParticleRenderEditorFactory* factory, bool overrideExisting = false)
	{
		ParticleEditorFactoryManager::getSingleton().RegisterParticleRendererEditorFactory(factory);
	}
};

#define M_REG_RENDER_EDITOR_FACTORY_EX(classTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static ParticleRenderEditorFactoryRegHelper __##classTypeName##Factory(#classTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_RENDER_EDITOR_FACTORY(classTypeName, factoryClass) M_REG_RENDER_EDITOR_FACTORY_EX(classTypeName, factoryClass, false)



struct EmitterEditorFactoryRegHelper
{
	explicit EmitterEditorFactoryRegHelper(const String& classTypeName, IParticleEmitterEditorFactory* factory, bool overrideExisting = false)
	{
		ParticleEditorFactoryManager::getSingleton().RegisterEmitterEditorFactory(factory);
	}
};

#define M_REG_EMITTER_EDITOR_FACTORY_EX(classTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static EmitterEditorFactoryRegHelper __##classTypeName##Factory(#classTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_EMITTER_EDITOR_FACTORY(classTypeName, factoryClass) M_REG_EMITTER_EDITOR_FACTORY_EX(classTypeName, factoryClass, false)


struct AffectorEditorFactoryRegHelper
{
	explicit AffectorEditorFactoryRegHelper(const String& classTypeName, IParticleAffectorEditorFactory* factory, bool overrideExisting = false)
	{
		ParticleEditorFactoryManager::getSingleton().RegisterAffectorEditorFactory(factory);
	}
};

#define M_REG_AFFECTOR_FACTORY_EDITOR_EX(classTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static AffectorEditorFactoryRegHelper __##classTypeName##Factory(#classTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_AFFECTOR_EDITOR_FACTORY(classTypeName, factoryClass) M_REG_AFFECTOR_FACTORY_EDITOR_EX(classTypeName, factoryClass, false)

//--------------------------------------------------------------------------------
// Declare/implement particle affector/emitter/renderer parameter get/set function
//--------------------------------------------------------------------------------

#define DECL_PARTICLE_EDITOR_FUN(fun, t) void Set##fun(t val); \
	t Get##fun() const;

#define IMPL_PARTICLE_EDITOR_FUN(obj, c, t, fun) void c##::Set##fun( t val ) \
{ \
	obj->setParameter(#fun, StringConverter::toString(val)); \
} \
	t c##::Get##fun( void ) const \
{ \
	return StringConverter::parse##t( \
	obj->getParameter(#fun)); \
}

//
// Affector
//
#define DECL_AFFECTOR_EDITOR_FUN	DECL_PARTICLE_EDITOR_FUN
#define IMPL_AFFECTOR_EDITOR_FUN(c, t, fun) IMPL_PARTICLE_EDITOR_FUN(_affector, c, t, fun)

//
// Emitter
//
#define DECL_EMITTER_EDITOR_FUN		DECL_PARTICLE_EDITOR_FUN
#define IMPL_EMITTER_EDITOR_FUN(c, t, fun)  IMPL_PARTICLE_EDITOR_FUN(_emitter, c, t, fun)

//
// Renderer
//
#define DECL_RENDERER_EDITOR_FUN	DECL_PARTICLE_EDITOR_FUN
#define IMPL_RENDERER_EDITOR_FUN(c, t, fun)  IMPL_PARTICLE_EDITOR_FUN(_render, c, t, fun)

//------------------------------------------------------------------------
class ParticleAttributeEditor
	: public PropertyObject
	, public OgeAlloc
{
public:

	ParticleAttributeEditor();
	virtual ~ParticleAttributeEditor();

	void SetParticleSystem(ParticleSystem* ps);

	void SetRendererName(const String& typeName);
	const String& GetRendererName(void) const;

	size_t GetParticleQuota(void) const;
	void SetParticleQuota(size_t quota);

	size_t GetEmittedEmitterQuota(void) const;
	void SetEmittedEmitterQuota(size_t quota);

	void SetMaterialName(const String& name);
	const String& GetMaterialName(void) const;

	//void fastForward(Real time, Real interval = 0.1);
	void SetSpeedFactor(Real speedFactor);
	Real GetSpeedFactor(void) const;

	void SetIterationInterval(Real iterationInterval);
	Real GetIterationInterval(void) const;

	void SetNonVisibleUpdateTimeout(Real timeout);
	Real GetNonVisibleUpdateTimeout(void) const;

	void SetDefaultWidth(Real width);
	Real GetDefaultWidth(void) const;
	void SetDefaultHeight(Real height);
	Real GetDefaultHeight(void) const;
	bool GetCullIndividually(void) const;
	void SetCullIndividually(bool cullIndividual);
	
	//const String& GetOrigin(void) const;
	//void _notifyOrigin(const String& origin) { mOrigin = origin; }

	void SetSortingEnabled(bool enabled);
	bool GetSortingEnabled(void) const;
	
	//void SetBoundsAutoUpdated(bool autoUpdate, Real stopIn = 0.0f);
	
	void SetKeepParticlesInLocalSpace(bool keepLocal);
	bool GetKeepParticlesInLocalSpace(void) const;

	virtual const String& GetTypeName() const;

protected:

	ParticleSystem* _particleSystem;
	virtual void BuildPropertyMap();
};