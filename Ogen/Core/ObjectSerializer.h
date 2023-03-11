// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-09-04
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"

namespace Fantasy
{
	class CORE_API IObjectSerializer
	{
	public:

		virtual const String& getVersion() const = 0;
		virtual bool save(OutStream& os) = 0;
		virtual bool load(InStream& is) = 0;
	};

	class CORE_API ObjectSerializer : public IObjectSerializer
	{
	public:
		
		ObjectSerializer(void);
		virtual ~ObjectSerializer(void);

		virtual const String& getVersion() const;
		virtual bool save(OutStream& os);
		virtual bool load(InStream& is);

		bool canReadVersion(const String& version) const;
		IObjectSerializer* getSerializer(const String& version) const;

	protected:
		
		typedef std::map<String, IObjectSerializer*> SerializerVersionMap;
		SerializerVersionMap _serializerMap;

		String _currentVersion;

		void addSerializer(const String& version, IObjectSerializer* ser);
	};

} // namespace