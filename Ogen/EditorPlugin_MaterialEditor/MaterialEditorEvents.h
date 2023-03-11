#pragma once

#define DECL_PARTICLE_EVENT(x)	static String x;
#define IMPL_PARTICLE_EVENT(x)	String MaterialEditorEvents::x = #x;

struct MaterialEditorEvents
{
	DECL_PARTICLE_EVENT(AddElement)
};