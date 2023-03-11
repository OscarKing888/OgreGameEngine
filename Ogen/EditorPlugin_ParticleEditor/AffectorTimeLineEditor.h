#pragma once

///
/// IAffectorTimeLineEditor
///
class _PaticleEditorExport IAffectorTimeLineEditor : public OgeAlloc
{
public:

	/// 取得编辑器的窗口对象
	virtual CWnd* GetEditorWnd() const = 0;
	/// 开始编辑Affector时调用
	virtual void OnEdit(Ogre::ParticleAffector* affector) = 0;
	/// 结束编辑时调用
	virtual void OnReset() = 0;
	/// 参数修改后调用
	virtual void OnAffectorParameterChanged() = 0;
};

///
/// IAffectorTimeLineEditorFactory
///
class _PaticleEditorExport IAffectorTimeLineEditorFactory
{
public:

	virtual IAffectorTimeLineEditor* CreateEditor(CWnd* parentPanel) = 0;
};

//------------------------------------------------------------------------------

template <class T>
class TAffectorTimeLineEditorFactory : public IAffectorTimeLineEditorFactory
{
	virtual IAffectorTimeLineEditor* CreateEditor(CWnd* parentPanel)
	{
		return OGRE_NEW T(parentPanel);
	}
};

///
/// AffectorTimeLineEditorFactoryManager
///
class _PaticleEditorExport AffectorTimeLineEditorFactoryManager
	: public SingletonMyers<AffectorTimeLineEditorFactoryManager>
{
public:

	typedef map<String, IAffectorTimeLineEditorFactory*>::type FactoryMap;

	AffectorTimeLineEditorFactoryManager();
	virtual ~AffectorTimeLineEditorFactoryManager();

	void registerFactory(const String& classTypeName, IAffectorTimeLineEditorFactory* factory,
		bool overrideExisting = false);

	IAffectorTimeLineEditorFactory* getFactory(const String& classTypeName);

	const FactoryMap& getFactoryMap() const { return _factoryMap; }

	virtual IAffectorTimeLineEditor* createAffectorTimeLineEditor(
		const String& classTypeName, CWnd* parentPanel);

protected:

	FactoryMap	_factoryMap;
};

//------------------------------------------------------------------------------

struct _PaticleEditorExport AffectorTimeLineEditorFactoryRegHelper
{
	explicit AffectorTimeLineEditorFactoryRegHelper(const String& propTypeName,
		IAffectorTimeLineEditorFactory* factory, bool overrideExisting = false)
	{
		AffectorTimeLineEditorFactoryManager::getSingleton()
			.registerFactory(propTypeName, factory, overrideExisting);
	}
};

#define M_REG_ATM_EDITOR_FACTORY_EX(propTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static AffectorTimeLineEditorFactoryRegHelper __##factoryClass##RegHelper(#propTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_ATM_EDITOR_FACTORY(propTypeName, factoryClass) M_REG_ATM_EDITOR_FACTORY_EX(propTypeName, factoryClass, false)
