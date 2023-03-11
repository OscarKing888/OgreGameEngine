#pragma once
#include "PlacementObject.h"

class IGameSound3D;

///
/// 3D Sound object to place in scene
///
class _EngineExport Sound3D : public PlacementObject
{
public:

	Sound3D(void);
	virtual ~Sound3D(void);

	virtual const String& GetTypeName() const;
	
	const String& GetSoundFileName() const;
	void SetSoundFileName(const String& fileName);

protected:

	String _soundFile;
	IGameSound3D* _sound3D;

	virtual void BuildPropertyMap();
	virtual void OnSoundFileChange();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();
	
	void CreateSoundObject();
	void DestroySoundObject();
};
//--------------------------------------------------------------------------------
class _EngineExport Property_SoundFile : public Property_String
{
public:

	
	Property_SoundFile(SetFunType setFun, GetFunType getFun);
	virtual ~Property_SoundFile();

	virtual const String GetTypeName() const { return "SoundFile"; }

protected:

};

SmartPointer(Property_SoundFile);