#include "stdafx.h"
#include "SpecialEffect.h"
#include "SpecialEffectsManager.h"

class SpecialEffectFactory : public TGameObjectFactory<SpecialEffect>
{
public:

};

M_REG_FACTORY(SpecialEffect, SpecialEffectFactory, VisualObject)

SpecialEffect::SpecialEffect()
: _effects(0)
{

}
//--------------------------------------------------------------------------------
SpecialEffect::~SpecialEffect()
{

}
//--------------------------------------------------------------------------------
const String& SpecialEffect::GetTypeName() const
{
	static String sName("SpecialEffect");
	return sName;
}
//--------------------------------------------------------------------------------
void SpecialEffect::BuildPropertyMap()
{
	PlacementObject::BuildPropertyMap();

	AddProp(Property_String, EffectTemplate, SpecialEffect, "SpecialEffect", "SpecialEffect");
}
//--------------------------------------------------------------------------------
void SpecialEffect::DestroyEffects()
{
	if(_effects)
	{
		SpecialEffectsManager::getSingleton().Destroy(_effects);
		_effects = 0;
	}
}
//--------------------------------------------------------------------------------
void SpecialEffect::SetEffectTemplate(const String& effTemp)
{
	_effectTemplate = effTemp;

	DestroyEffects();
	
	SpecialEffectsManager& mgr = SpecialEffectsManager::getSingleton();
	if(mgr.IsTemplateExists(effTemp))
	{
		_effects = mgr.Create(effTemp);
	}
	else
	{
		LogErrorIDf(SFXLogID, "[%s]Effect template not found[%s].",
			__FUNCTION__, effTemp.c_str());
	}
}
//--------------------------------------------------------------------------------
void SpecialEffect::Destroy()
{
	DestroyEffects();
	PlacementObject::Destroy();
}