#include "StdAfx.h"
#include "Sound3D.h"
#include "ISound.h"

class Sound3DFactory : public TGameObjectFactory<Sound3D>
{
};

M_REG_FACTORY(Sound3D, Sound3DFactory, Sound);

Sound3D::Sound3D(void)
	: _sound3D(0)
{
}
//--------------------------------------------------------------------------------
Sound3D::~Sound3D(void)
{
}
//--------------------------------------------------------------------------------
const String& Sound3D::GetTypeName() const
{
	static String sName("Sound3D");
	return sName;
}
//--------------------------------------------------------------------------------
const String& Sound3D::GetSoundFileName() const
{
	return _soundFile;
}
//--------------------------------------------------------------------------------
void Sound3D::SetSoundFileName( const String& fileName )
{
	_soundFile = fileName;
	OnSoundFileChange();
}
//--------------------------------------------------------------------------------
void Sound3D::BuildPropertyMap()
{
	PlacementObject::BuildPropertyMap();

	AddProp(Property_SoundFile, SoundFileName, Sound3D, "SoundFileName", "Sound");
}
//--------------------------------------------------------------------------------
void Sound3D::OnSoundFileChange()
{
	DestroySoundObject();
	CreateSoundObject();
}
//--------------------------------------------------------------------------------
void Sound3D::PostCreateNode()
{
	PlacementObject::PostCreateNode();
}
//--------------------------------------------------------------------------------
void Sound3D::PreDestroyNode()
{
	PlacementObject::PreDestroyNode();
	DestroySoundObject();
}
//--------------------------------------------------------------------------------
void Sound3D::CreateSoundObject()
{
	if(!_soundFile.empty())
	{
		_sound3D = SoundManager::getSingleton().CreateSound3D(
			GetGUID().toString(), _soundFile);
	}
}
//--------------------------------------------------------------------------------
void Sound3D::DestroySoundObject()
{
	if(_sound3D)
	{
		SoundManager::getSingleton().DestroySound3D(_sound3D);
	}
}
//--------------------------------------------------------------------------------
Property_SoundFile::Property_SoundFile( SetFunType setFun, GetFunType getFun )
: Property_String(setFun, getFun)
{

}
//--------------------------------------------------------------------------------
Property_SoundFile::~Property_SoundFile()
{

}