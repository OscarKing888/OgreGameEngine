#include "stdafx.h"
#include "Cmd_AddElement.h"
#include "AbstractSFXTimeblock.h"
#include "SFXEditorEvents.h"

Cmd_AddElement::Cmd_AddElement(SpecialEffects* sfx, const String& typeName,
							   const String& name,
							   const NameValuePairList& parms )
							   : _sfx(sfx)
							   , _elementType(typeName)
							   , _params(parms)
							   , _name(name)
{

}
//--------------------------------------------------------------------------------
Cmd_AddElement::~Cmd_AddElement( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_AddElement::Execute()
{
	IEffectsElement* elem =
		SpecialEffectsManager::getSingleton()
		.CreateEffectsElement(_elementType, _name, _sfx);

	int idx = _sfx->AddElement(elem);

	AbstractSFXTimeblockPtr atb = OGRE_NEW AbstractSFXTimeblock();
	atb->SetElement(elem);

	EventsManager::getSingleton().CreateEvent(
		SFXEditorEvents::UpdateUI_AddElement, Any(atb));

	return OGRE_NEW Cmd_RemoveElement(_sfx, idx);
}
//--------------------------------------------------------------------------------
Cmd_RemoveElement::Cmd_RemoveElement( SpecialEffects* sfx, int idx )
: _sfx(sfx)
, _idx(idx)
{

}
//--------------------------------------------------------------------------------
Cmd_RemoveElement::~Cmd_RemoveElement( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_RemoveElement::Execute()
{
	IEffectsElement* elem = _sfx->GetElementsByIndex(_idx);
	String name = elem->GetName();
	String typeName = elem->GetTypeName();
	NameValuePairList parms = elem->GetValues();
	_sfx->RemoveElement(_idx);
	
	EventsManager::getSingleton().CreateEvent(
		SFXEditorEvents::UpdateUI_RemoveElement, Any(_idx));

	return OGRE_NEW Cmd_AddElement(_sfx, typeName, name, parms);
}