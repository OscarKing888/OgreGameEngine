#pragma once

#define DECL_PARTICLE_EVENT(x)	static String x;
#define IMPL_PARTICLE_EVENT(x)	String ParticleEditorEvents::x = #x;

struct ParticleEditorEvents
{
	DECL_PARTICLE_EVENT(AddEmitter)
	DECL_PARTICLE_EVENT(RemoveEmitter)
	DECL_PARTICLE_EVENT(UpdateRemoveEmitterUI)

	DECL_PARTICLE_EVENT(AddAffector)
	DECL_PARTICLE_EVENT(RemoveAffector)
	DECL_PARTICLE_EVENT(UpdateRemoveAffectorUI)

	DECL_PARTICLE_EVENT(ChangeParticleSysRenderer)

	DECL_PARTICLE_EVENT(SelectEmitter)
	DECL_PARTICLE_EVENT(SelectAffector)
	
	DECL_PARTICLE_EVENT(SelectParticleSysProp)
	DECL_PARTICLE_EVENT(SelectParticleSysRendererProp)

	DECL_PARTICLE_EVENT(NewParticleSystem)
	DECL_PARTICLE_EVENT(EditParticleSystem)
	DECL_PARTICLE_EVENT(EditParticleSystemInstance)

	DECL_PARTICLE_EVENT(Undo)
	DECL_PARTICLE_EVENT(Redo)

	DECL_PARTICLE_EVENT(DestroyParticleSysInstance)

	DECL_PARTICLE_EVENT(UpdateEmitterList)
	DECL_PARTICLE_EVENT(UpdateAffectorList)
	DECL_PARTICLE_EVENT(UpdateView)

	DECL_PARTICLE_EVENT(ShowParticleObjectProp)
	DECL_PARTICLE_EVENT(ClearObjectProp)
	DECL_PARTICLE_EVENT(UpdateTimeLineEditor)
};