#pragma once

class Cmd_AddEmitter : public ICommandX
{
public:

	Cmd_AddEmitter(ParticleSystem* ps, const String& emitterType, const NameValuePairList& parms);
	virtual ~Cmd_AddEmitter(void);

	virtual ICommandXPtr Execute();

protected:

	ParticleSystem* _particleSys;
	String _emitterType;
	NameValuePairList _params;
};
