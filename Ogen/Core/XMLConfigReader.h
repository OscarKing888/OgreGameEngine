#pragma once
#include "XML.h"

/**
* XMLConfigReader���ڶ�ȡXML���ã����ݴ����XMLPATH����ROOT�µĽڵ㿪ʼ�ң����ڣ�
* <root>
*	<net>
*	<port>9800</port>
*	<ip>127.0.0.1</ip>
* </root>
* ��Ӧ��XMLPATHΪ��
*
* "net/port"
* "net/ip"
*
* ע��root�ǲ���Ҫ��д�ģ����õ�XML��ʽֻ��Ҫһ��ROOT�ڵ㡣��ȡ����ʱ���£�
*
* XMLConfigReader config;
* config.load("xxx.xml");
* int port = config.getInt("net/port");
* 
* �����в�����ͬ��ͬ���Ľڵ㣬����ʱֻ���ҵ�һ�����ַ��ϵĽڵ㡣
*/
class _CoreExport XMLConfigReader
{
public:

	XMLConfigReader(void);
	/// Constructor with file name
	XMLConfigReader(const String& fileName);
	~XMLConfigReader(void);
	/// Load xml from file
	bool loadFromFile(const String& xmlFile);
	/// Load file from resource system
	bool loadFromResourceSystem(const String& xmlFile);
	/// Get string value from xpath
	String getString(const String& xmlPath);
	/// Get bool value from xpath
	bool getBool(const String& xmlPath);
	/// get float value from xpath
	float getFloat(const String& xmlPath);
	/// get int value from xpath
	int getInt(const String& xmlPath);

protected:

	XMLDoc	_doc;
	XMLElement _root;
};

