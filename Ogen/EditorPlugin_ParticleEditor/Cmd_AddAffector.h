#pragma once

class Cmd_AddAffector : public ICommandX
{
public:

	Cmd_AddAffector(ParticleSystem* ps, const String& affectorType, const NameValuePairList& parms);
	virtual ~Cmd_AddAffector(void);

	virtual ICommandXPtr Execute();

protected:

	ParticleSystem* _particleSys;
	String _affectorType;
	NameValuePairList _params;

};
