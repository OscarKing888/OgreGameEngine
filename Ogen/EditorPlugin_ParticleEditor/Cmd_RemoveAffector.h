#pragma once

class Cmd_RemoveAffector : public ICommandX
{
public:

	Cmd_RemoveAffector(ParticleSystem* ps, ushort idx);
	virtual ~Cmd_RemoveAffector(void);
	
	virtual ICommandXPtr Execute();

protected:

	ParticleSystem* _particleSys;
	ushort _idx;
};
