#pragma once

#include "AbstractGameObject.h"

class _EngineExport GameComponent
	: public PropertyObject
	, public OgeAlloc
{
public:

	GameComponent();
	virtual ~GameComponent();

// 	const Ogre::NameValuePairList& GetInitParams() const;
// 	void SetInitParams(const Ogre::NameValuePairList& val);

	const Ogre::String& GetName() const;
	void SetName(const Ogre::String& val);

protected:

// 	Ogre::NameValuePairList _initParams;
	Ogre::String _name;
};