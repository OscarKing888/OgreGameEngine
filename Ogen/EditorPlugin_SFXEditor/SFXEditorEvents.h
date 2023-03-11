#pragma once

#define DECL_PARTICLE_EVENT(x)	static String x;
#define IMPL_PARTICLE_EVENT(x)	String SFXEditorEvents::x = #x;

struct SFXEditorEvents
{
	DECL_PARTICLE_EVENT(NewSFX)
	DECL_PARTICLE_EVENT(AddElement)
	
	DECL_PARTICLE_EVENT(UpdateUI_AddElement)
	DECL_PARTICLE_EVENT(UpdateUI_RemoveElement)
	DECL_PARTICLE_EVENT(UpdateUI_RemoveAllElement)

	DECL_PARTICLE_EVENT(ShowSFXObjectProp)
};