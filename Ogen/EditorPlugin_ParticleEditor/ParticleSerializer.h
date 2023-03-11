#pragma once

class ParticleSerializer
{
public:

	void SaveParticle( const ParticleSystem* const ps, std::ofstream& of);
	void SaveEmitter(const ParticleEmitter* const pe, std::ofstream& of);
	void SaveAffector(const ParticleAffector* const pa, std::ofstream& of);

};