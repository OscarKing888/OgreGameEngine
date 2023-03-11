// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-08-28
// *************************************************************************************

#include "stdafx.h"
#include "XML.h"
#include "File.h"
#include "OSAPI.h"

using namespace std;

//------------------------------------------------------------------------



//-------------------------------------------------------------------------------------
// xml attribute
//-------------------------------------------------------------------------------------
XMLAttribute::XMLAttribute( TiXmlAttribute * attribute )
: _attribute (attribute)
{

}
//------------------------------------------------------------------------
XMLAttribute::XMLAttribute(const XMLAttribute& src)
: _attribute (src._attribute)
{
	
}
//------------------------------------------------------------------------
XMLAttribute::~XMLAttribute()
{
}
//------------------------------------------------------------------------
String XMLAttribute::name()
{
	assert(_attribute);
	return (_attribute->Name());
}
//------------------------------------------------------------------------
String XMLAttribute::value()
{
	assert(_attribute);
	return (_attribute->Value());
}
//------------------------------------------------------------------------
bool XMLAttribute::hasNext()
{
	return _attribute->Next() != NULL;
}
//------------------------------------------------------------------------
XMLAttribute XMLAttribute::next()
{
	if(!isValid()) return XMLAttribute(NULL);
	return XMLAttribute (_attribute->Next());
}
//------------------------------------------------------------------------
bool XMLAttribute::isValid() const
{
	return _attribute != NULL;
}
//--------------------------------------------------------------------------------------
// xml node
//--------------------------------------------------------------------------------------
XMLElement::XMLElement( TiXmlElement * element )
: _element(element)
{
}
//------------------------------------------------------------------------
XMLElement::XMLElement(const XMLElement& src)
: _element(src._element)
{
}
//------------------------------------------------------------------------
XMLElement::~XMLElement()
{
}
//------------------------------------------------------------------------
XMLElement& XMLElement::operator=(const XMLElement& src)
{
	_element = src._element;
	return	*this;
}
//------------------------------------------------------------------------
XMLElement XMLElement::firstChild()
{
	assert(_element);
	TiXmlNode	*	pXMLNode = _element->FirstChild();

	while(pXMLNode != NULL
		&& pXMLNode->Type() != TiXmlNode::TINYXML_ELEMENT)
	{
		pXMLNode = pXMLNode->NextSibling();
	}

	if( pXMLNode )
	{
		return	XMLElement( pXMLNode->ToElement() );
	}
	
	return XMLElement();
}
//------------------------------------------------------------------------
XMLElement XMLElement::firstChildByName(const String& childName)
{
	TiXmlNode* pXMLNode = _element->FirstChild((childName.c_str()));
	while(pXMLNode != NULL
		&& pXMLNode->Type() != TiXmlNode::TINYXML_ELEMENT)
	{
		pXMLNode = pXMLNode->NextSibling((childName.c_str()));
	}

	if( pXMLNode )
	{
		return	XMLElement( pXMLNode->ToElement() );
	}

	return XMLElement();
}
//------------------------------------------------------------------------
XMLElement XMLElement::nextSibling()
{
	assert(_element);
	TiXmlElement *	element = _element->NextSiblingElement();

	if( element )
	{
		return	XMLElement( element );
	}
	
	return	XMLElement();
}
//------------------------------------------------------------------------
XMLElement XMLElement::nextSiblingByName(const String& childName)
{
	TiXmlNode* pXMLNode = _element->NextSibling((childName.c_str()));
	while(pXMLNode != NULL
		&& pXMLNode->Type() != TiXmlNode::TINYXML_ELEMENT)
	{
		pXMLNode = pXMLNode->NextSibling((childName.c_str()));
	}

	if( pXMLNode )
	{
		return	XMLElement( pXMLNode->ToElement() );
	}

	return XMLElement();
}
//------------------------------------------------------------------------
XMLElement XMLElement::parent()
{
	assert(_element);
	TiXmlNode	*	pXMLNode = _element->Parent();
	if( pXMLNode )
	{
		return	XMLElement( pXMLNode->ToElement() );
	}

	return	XMLElement();
}
//------------------------------------------------------------------------
String	XMLElement::name()
{
	assert(_element);
	return (_element->ValueStr().c_str());
}
//------------------------------------------------------------------------
String	XMLElement::value()
{
	assert(_element);

	if(_element->GetText())
	{
		String str = _element->GetText();
		Ogre::UTFString utfStr(str, true);
		str = utfStr.asMBCSStr();
		//str.resize(str.size());
		return str;
		//return _element->GetText();
	}
	else
	{
		return String();
	}
}
//------------------------------------------------------------------------
bool XMLElement::isNull()
{
	return	_element==NULL;
}
//------------------------------------------------------------------------
String	XMLElement::getAttribute(const String& attrName)
{
	assert(_element);
	const char * pCElement = _element->Attribute((attrName.c_str()));

	if( pCElement )
	{
		String str = pCElement;
		Ogre::UTFString utfStr(str, true);
		str = utfStr.asMBCSStr();
		//str.resize(str.size());
		return str;
		//return (pCElement);
	}

	return	("");
}
//------------------------------------------------------------------------
bool XMLElement::hasChildNodes() const
{
	assert(_element);
	return	!_element->NoChildren();
}
//------------------------------------------------------------------------
bool  XMLElement::hasAttributes() const
{
	assert(_element);
	return	_element->FirstAttribute() != NULL;
}
//------------------------------------------------------------------------
XMLAttribute XMLElement::firstAttribute()
{
	assert(_element);
	return XMLAttribute(_element->FirstAttribute());
}
//------------------------------------------------------------------------
void XMLElement::setAttribute(const String& attrName, const String& val)
{
	_element->SetAttribute(attrName, val);
}
//------------------------------------------------------------------------
void XMLElement::setValue(const String& val)
{
	_element->SetValue(val);
}
//------------------------------------------------------------------------
XMLElement XMLElement::addChild(const String& name)
{
	TiXmlNode* pNode = _element->InsertEndChild(TiXmlElement(name));

	return XMLElement(dynamic_cast<TiXmlElement*>(pNode));
}
//------------------------------------------------------------------------
void XMLElement::removeChild(XMLElement & src)
{
	assert(_element);

	_element->RemoveChild(src._element);
}
//--------------------------------------------------------------------------------------
// xml document
//--------------------------------------------------------------------------------------
XMLDoc::XMLDoc()
{
	_doc = OGRE_NEW TiXmlDocument();
}
//------------------------------------------------------------------------
XMLDoc::~XMLDoc()
{
	OGRE_DELETE _doc;
}
//------------------------------------------------------------------------
bool XMLDoc::loadFromFile(const String& file)
{
	return _doc->LoadFile((file.c_str()));
}
//------------------------------------------------------------------------
XMLElement XMLDoc::getRoot()
{
	return XMLElement(_doc->RootElement());
}
//------------------------------------------------------------------------
bool XMLDoc::loadFromXML(const String& xmlCode)
{
	_doc->Parse(xmlCode.c_str());

	return !_doc->Error();
}
//------------------------------------------------------------------------
bool XMLDoc::save(const String& fileName)
{
	if(File::isFileExist(fileName))
	{
		File::removeReadonly(fileName);
	}

	return _doc->SaveFile(fileName.c_str());
}
//------------------------------------------------------------------------
void XMLDoc::createRootNode(const String& name)
{
	_doc->InsertEndChild(TiXmlDeclaration(("1.0"), ("UTF-8"), ("")));

	_doc->InsertEndChild(TiXmlElement(name));
}
//------------------------------------------------------------------------
float getFloatAttribuate(const String& attrName, XMLElement& element)
{
	String val = element.getAttribute(attrName);
	return StringConverterBase::parseFloat(val);
}
//------------------------------------------------------------------------
int getIntAttribuate(const String& attrName,  XMLElement&  element)
{
	String val = element.getAttribute(attrName);
	return StringConverterBase::parseInt(val);
}
//------------------------------------------------------------------------
bool getBoolAttribuate(const String& attrName, XMLElement& element)
{
	String val = element.getAttribute(attrName);	
	return StringConverterBase::parseBool(val);
}
//------------------------------------------------------------------------
