#pragma once

class Cmd_SetParticleRenderer : public ICommandX
{
public:

	Cmd_SetParticleRenderer(ParticleSystem* ps, const String& renderName,
		const NameValuePairList& parms = NameValuePairList());

	virtual ~Cmd_SetParticleRenderer(void);

	virtual ICommandXPtr Execute();

protected:

	ParticleSystem* _particleSys;
	String _renderName;
	NameValuePairList _params;
};
