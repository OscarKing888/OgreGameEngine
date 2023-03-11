#include "stdafx.h"
#include "OgePlugin.h"

OgePlugin::OgePlugin(const Ogre::String& name)
: _name(name)
{
	GameApp::getSingleton().AddPlugin(this);
}
//--------------------------------------------------------------------------------
OgePlugin::~OgePlugin()
{
	GameApp::getSingleton().RemovePlugin(this);
}
//--------------------------------------------------------------------------------
const Ogre::String& OgePlugin::getName() const
{
	return _name;
}
//--------------------------------------------------------------------------------
void OgePlugin::install()
{

}
//--------------------------------------------------------------------------------
void OgePlugin::uninstall()
{

}
//--------------------------------------------------------------------------------
void OgePlugin::initialise()
{

}
//--------------------------------------------------------------------------------
void OgePlugin::shutdown()
{

}
//--------------------------------------------------------------------------------