#include "StdAfx.h"
#include "SoundFileCodecManager.h"

using namespace XAudio2Ex;

SoundFileCodecManager::SoundFileCodecManager(void)
{
}

SoundFileCodecManager::~SoundFileCodecManager(void)
{
}

void SoundFileCodecManager::RegisterCodec(const String& codecName, ISoundFileCodec* codec)
{
	CodecMap::iterator i = _codecMap.find(codecName);
	if(i != _codecMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			"Sound codec already exist" + codecName, "SoundFileCodecManager:RegisterCodec");
	}

	_codecMap.insert(std::make_pair(codecName, codec));
}

ISoundFileCodec* SoundFileCodecManager::GetCodec(const String& codecName)
{
	CodecMap::iterator i = _codecMap.find(codecName);
	if(i != _codecMap.end())
	{
		return i->second;
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		"Sound codec not found" + codecName, "SoundFileCodecManager:GetCodec");
}
