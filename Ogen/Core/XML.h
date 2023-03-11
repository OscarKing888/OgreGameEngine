// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-08-28
// *************************************************************************************

#pragma once

#include "CorePrerequisites.h"
#include "tinyxml.h"



class _CoreExport XMLAttribute
{
	friend class XMLDoc;
	friend class XMLElement;
public:
	XMLAttribute( TiXmlAttribute * element = NULL );
	XMLAttribute(const XMLAttribute& src);
	virtual ~XMLAttribute();
	String name();
	String value();
	bool hasNext();
	bool isValid() const;
	XMLAttribute next();
private:
	TiXmlAttribute* _attribute;
};

class _CoreExport XMLElement
{
	friend class XMLDoc;
public:

	XMLElement( TiXmlElement * element = NULL );
	XMLElement(const XMLElement& src);
	virtual ~XMLElement();
	XMLElement& operator=(const XMLElement& src);
	XMLElement firstChild();
	XMLElement nextSibling();
	XMLElement firstChildByName(const String& childName);
	XMLElement nextSiblingByName(const String& childName);
	XMLElement parent();
	String name();
	String value();
	void setValue(const String& val);
	bool		isNull();
	String getAttribute(const String& attrName);	
	bool		hasChildNodes() const;
	bool		hasAttributes() const;
	XMLAttribute firstAttribute();

	void setAttribute(const String& attrName, const String& val);
	XMLElement addChild(const String& name);
	void removeChild(XMLElement & src);

private:
	TiXmlElement* _element;
};

class _CoreExport XMLDoc
{
public:
	XMLDoc();
	virtual ~XMLDoc();
	// load from disk file.
	bool loadFromFile(const String& file);
	XMLElement getRoot();
	bool loadFromXML(const String& xmlCode);
	bool save(const String& fileName);
	void createRootNode(const String& name);

private:
	TiXmlDocument* _doc;
};

float _CoreExport extern getFloatAttribuate(const String& attrName, XMLElement& element);
int   _CoreExport extern getIntAttribuate(const String& attrName,  XMLElement&  element);
bool  _CoreExport extern getBoolAttribuate(const String& attrName, XMLElement& element);

