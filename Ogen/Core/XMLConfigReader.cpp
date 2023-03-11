#include "StdAfx.h"
#include "XMLConfigReader.h"
#include <tchar.h>




	XMLConfigReader::XMLConfigReader(void)
	{
	}
	//--------------------------------------------------------------------------------
	XMLConfigReader::XMLConfigReader(const String& fileName)
	{
		loadFromFile(fileName);
	}
	//--------------------------------------------------------------------------------
	XMLConfigReader::~XMLConfigReader(void)
	{
	}
	//--------------------------------------------------------------------------------
	bool XMLConfigReader::loadFromFile(const String& xmlFile)
	{
		if(_doc.loadFromFile(xmlFile))
		{
			_root = _doc.getRoot();
			return true;
		}
		else
		{
			assert(false && "[XMLConfigReader::loadFromFile]XML file loading failed!");
			return false;
		}
	}
	//--------------------------------------------------------------------------------
	bool XMLConfigReader::loadFromResourceSystem(const String& xmlFile)
	{
		DataStreamPtr ds = ResourceGroupManager::getSingleton().openResource(xmlFile);
		String str = ds->getAsString();
		if(_doc.loadFromXML(str))
		{
			_root = _doc.getRoot();
			return true;
		}
		else
		{
			assert(false && "[XMLConfigReader::loadFromResourceSystem]XML file loading failed!");
			return false;
		}
	}
	//--------------------------------------------------------------------------------
	String XMLConfigReader::getString(const String& xmlPath)
	{
		StringVector nodePath = StringUtil::split(xmlPath, "/");
		XMLElement c = _root;
		for (size_t i = 0; i != nodePath.size(); ++i)
		{
			const String& nodeName = nodePath[i];
			if(!c.isNull())
			{
				c = c.firstChildByName(nodeName);
			}
			else
			{
				assert(false && "[XMLConfigReader::getString] xml path is invalid!");
				return "";
			}
		}

		String val;
		if(!c.isNull())
		{
			val = c.value();
		}

		assert(!val.empty() && "[XMLConfigReader::getString] xml path is invalid, value not found!");
		return val;
	}
	//--------------------------------------------------------------------------------
	bool XMLConfigReader::getBool(const String& xmlPath)
	{
		String val = getString(xmlPath);
		return StringConverterBase::parseBool(val);
	}
	//--------------------------------------------------------------------------------
	float XMLConfigReader::getFloat(const String& xmlPath)
	{
		String val = getString(xmlPath);
		return StringConverterBase::parseFloat(val);
	}
	//--------------------------------------------------------------------------------
	int XMLConfigReader::getInt(const String& xmlPath)
	{
		String val = getString(xmlPath);
		return StringConverterBase::parseInt(val);
	}

