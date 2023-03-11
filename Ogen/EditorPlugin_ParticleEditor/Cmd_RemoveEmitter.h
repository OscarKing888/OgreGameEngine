#pragma once

class Cmd_RemoveEmitter : public ICommandX
{
public:

	Cmd_RemoveEmitter(ParticleSystem* ps, ushort idx);
	virtual ~Cmd_RemoveEmitter(void);

	virtual ICommandXPtr Execute();

protected:

	ParticleSystem* _particleSys;
	ushort _idx;
};
