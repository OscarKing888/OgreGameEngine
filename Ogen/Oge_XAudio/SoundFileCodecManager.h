#pragma once
#include "EnginePrerequisites.h"
#include "ISoundFileCodec.h"

namespace XAudio2Ex
{
	class _EngineExport SoundFileCodecManager : public SingletonMyers<SoundFileCodecManager>
	{
	public:

		SoundFileCodecManager(void);
		virtual ~SoundFileCodecManager(void);

		void RegisterCodec(const String& codecName, ISoundFileCodec* codec);
		ISoundFileCodec* GetCodec(const String& codecName);

	protected:

		typedef std::map<String, ISoundFileCodec*>	CodecMap;
		CodecMap	_codecMap;
	};
} // namespace