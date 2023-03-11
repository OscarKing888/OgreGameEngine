#include "stdafx.h"
#include "GameComponent.h"

GameComponent::GameComponent()
{

}
//--------------------------------------------------------------------------------
GameComponent::~GameComponent()
{

}
//--------------------------------------------------------------------------------
const Ogre::String& GameComponent::GetName() const
{
	return _name;
}
//--------------------------------------------------------------------------------
void GameComponent::SetName( const Ogre::String& val )
{
	_name = val;
}
//--------------------------------------------------------------------------------
//const Ogre::NameValuePairList& GameComponent::GetInitParams() const
//{
//	return _initParams;
//}
////--------------------------------------------------------------------------------
//void GameComponent::SetInitParams( const Ogre::NameValuePairList& val )
//{
//	_initParams = val;
//}