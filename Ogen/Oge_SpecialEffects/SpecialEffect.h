#pragma once

#include "OgeSpecialEffectsPrerequisites.h"
#include <Engine/PlacementObject.h>

class ISpecialEffects;

///
/// Special effect object for edit, saving, loading in game.
///
class _OgeSFXExport SpecialEffect : public PlacementObject
{
public:

	SpecialEffect();
	virtual ~SpecialEffect();

	/// @copydoc IGameObject::getTypeName
	virtual const String& GetTypeName() const;
	/// Set the effect template name and create effect
	virtual void SetEffectTemplate(const String& effTemp);
	/// Return current effect template name
	virtual const String& GetEffectTemplate() const { return _effectTemplate; }

	/// @copydoc AbstractGameObject::Destroy
	virtual void Destroy();

protected:

	/// @copydoc PropertyObject::BuildPropertyMap
	virtual void BuildPropertyMap();

	String _effectTemplate;
	ISpecialEffects* _effects;
	void DestroyEffects();
};
