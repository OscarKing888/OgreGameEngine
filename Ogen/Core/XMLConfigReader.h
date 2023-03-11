#pragma once
#include "XML.h"

/**
* XMLConfigReader用于读取XML配置，根据传入的XMLPATH，从ROOT下的节点开始找，对于：
* <root>
*	<net>
*	<port>9800</port>
*	<ip>127.0.0.1</ip>
* </root>
* 相应的XMLPATH为：
*
* "net/port"
* "net/ip"
*
* 注意root是不需要填写的，良好的XML格式只需要一个ROOT节点。读取配置时如下：
*
* XMLConfigReader config;
* config.load("xxx.xml");
* int port = config.getInt("net/port");
* 
* 配置中不能有同级同名的节点，查找时只会找第一个名字符合的节点。
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

