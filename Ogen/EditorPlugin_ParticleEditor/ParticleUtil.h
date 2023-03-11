#pragma once

class ParticleUtil
{
public:

	static void ReplaceParticleTemplate(ParticleSystem* par, const String& templateName);
	static void SaveAllParticleTemplate(const String& fileName);
	static void RemoveParticleTemplate(const String& templateName);

	static NameValuePairList GetParameters(const StringInterface* obj);

};