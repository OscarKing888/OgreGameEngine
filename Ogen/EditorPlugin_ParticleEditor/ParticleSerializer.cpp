#include "stdafx.h"
#include "ParticleSerializer.h"
#include <OgreParticleSystemRenderer.h>

//--------------------------------------------------------------------------------
void ParticleSerializer::SaveParticle( const ParticleSystem* const ps, std::ofstream& of)
{
	of << "\n";
	of << "particle_system " << ps->getName() << "\n";
	of << "{" << "\n";

	ParameterList params = ps->getParameters();
	for (ParameterList::const_iterator i = params.begin(); i != params.end(); ++i)
	{
		of << "\t" << i->name << "\t" << ps->getParameter( i->name ) << "\n";
	}

	ParameterList rparams = ps->getRenderer()->getParameters();
	for (ParameterList::const_iterator it = rparams.begin(); it != rparams.end(); ++it)
	{
		// skip common_direction if not applicable
		if ( it->name == "common_direction" )
		{
			if ( ps->getRenderer()->getParameter("billboard_type") != "oriented_common" )
			{
				continue;
			}
		}
		of << "\t" << it->name << "\t" << ps->getRenderer()->getParameter( it->name ) << "\n";
	}

	// emitters
	for ( int i = 0; i < ps->getNumEmitters(); i++ )
	{
		SaveEmitter( ps->getEmitter(i), of );
	}

	// affectors
	for ( int i = 0; i < ps->getNumAffectors(); i++ )
	{
		SaveAffector( ps->getAffector(i), of );
	}

	of << "}\n";
}
//--------------------------------------------------------------------------------
void ParticleSerializer::SaveEmitter( const ParticleEmitter* const pe, std::ofstream& of )
{
	of << "\n";
	of << "\temitter " << pe->getType() << "\n";
	of << "\t{\n";

	ParameterList params = pe->getParameters();
	ParameterList::const_iterator i;
	for (i = params.begin(); i != params.end(); ++i)
	{
		of << "\t\t" << i->name << "\t" << pe->getParameter( i->name ) << "\n";
	}

	of << "\t}\n";
}
//--------------------------------------------------------------------------------
void ParticleSerializer::SaveAffector( const ParticleAffector* const pa, std::ofstream& of )
{
	of << "\n";
	of << "\taffector " << pa->getType() << "\n";
	of << "\t{\n";

	ParameterList params = pa->getParameters();
	ParameterList::const_iterator i;
	for (i = params.begin(); i != params.end(); ++i)
	{
		of << "\t\t" << i->name << "\t" << pa->getParameter( i->name ) << "\n";
	}

	of << "\t}\n";
}