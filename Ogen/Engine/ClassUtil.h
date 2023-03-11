#pragma once
#include "EnginePrerequisites.h"
#include "IGameObject.h"

typedef map<String, int>::type SerializePriorityMap;

///
/// Global object serialization order priority
///
_EngineExport extern SerializePriorityMap g_serializePriorityMap;

///
/// Class for serialization objects with specified type mask
/// @see EDistributeFlag
///
class _EngineExport ISerializationMask
{
public:

	/// if the object can be saved into current file
	virtual bool IsObjectCouldSave(IGameObject* obj) = 0;
	// The porperty mask for selecting properties want to save
	virtual int GetPropertySaveMask() const = 0;
};

///
/// DefaultSerializationMask for general usage
///
class DefaultSerializationMask : public ISerializationMask
{
	int _objectSaveFlag;
	int _propertySaveFlag;

public:

	DefaultSerializationMask(int objSaveFlag, int propertySaveFlag = eDF_All)
		: _objectSaveFlag(objSaveFlag)
		, _propertySaveFlag(propertySaveFlag)
	{

	}

	virtual bool IsObjectCouldSave(IGameObject* obj)
	{
		return (obj->GetDistributeFlag() & _objectSaveFlag) != 0;
	}

	virtual int GetPropertySaveMask() const
	{
		return _propertySaveFlag;
	}
};

///
/// Interface for object serializer
///
class _EngineExport IClassSerializer
{
public:

	virtual void Save(const GameObjectList& objectList, const String& fileName, ISerializationMask* query = 0) = 0;
	virtual GameObjectList LoadDirect(const String& fileName) = 0;
	virtual GameObjectList LoadFromResourceSystem(const String& fileName, const String& groupName) = 0;
};

///
/// ClassXMLSerializer
///
class _EngineExport ClassXMLSerializer : public IClassSerializer
{
public:

	ClassXMLSerializer();
	virtual ~ClassXMLSerializer();

	virtual void Save(const GameObjectList& objectList, const String& fileName, ISerializationMask* query = 0);

	virtual GameObjectList LoadDirect(const String& fileName);
	virtual GameObjectList LoadFromResourceSystem(const String& fileName, const String& groupName);

	GameObjectList LoadFromXMLRootNode(XMLElement& objectsNode);
	XMLElement Save(const IGameObject* obj, XMLDoc& doc, ISerializationMask* query);
};

///
/// ClassBinarySerializer
///
class _EngineExport ClassBinarySerializer : public IClassSerializer, public Serializer
{
public:

	ClassBinarySerializer();
	virtual ~ClassBinarySerializer();

	virtual void Save(const GameObjectList& objectList, const String& fileName, ISerializationMask* query = 0);

	virtual GameObjectList LoadDirect(const String& fileName);
	virtual GameObjectList LoadFromResourceSystem(const String& fileName, const String& groupName);

	GameObjectList LoadFromDataStream(DataStreamPtr s);
};
