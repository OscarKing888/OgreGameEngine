#pragma once
#include "XAudioSound.h"
#include "IUpdateAble.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace XAudio2Ex
{
	class _EngineExport XAudioStreamSound
		: public XAudioSound
		, public IUpdateAble
	{
	public:

		XAudioStreamSound(const String& name, const String& fileName);
		virtual ~XAudioStreamSound(void);

		virtual void SetOffset(double seconds);
		virtual double GetOffset() const;

		virtual void SetLoop(bool loop);
		virtual void Stop(size_t operationSet = 0);
		virtual void Play(size_t operationSet = 0);

		virtual void BufferSounds();
		virtual bool IsStreamSound() const { return true; }

	protected:

		void __stdcall OnVoiceProcessingPassStart() {}
		void __stdcall OnVoiceProcessingPassStart(UINT32) {}
		void __stdcall OnVoiceProcessingPassEnd() {}
		void __stdcall OnStreamEnd() {}
		void __stdcall OnBufferStart( void* );
		void __stdcall OnBufferEnd( void* );
		void __stdcall OnLoopEnd( void* ) {}
		void __stdcall OnVoiceError( void*, HRESULT ) {}

		virtual void ReadDataToBuffer();

		virtual void Update(float deltaTime);

		int					_readBufferRequist;
		bool				_restart;

		ov_callbacks		_callbacks;
		OggVorbis_File		_oggStream;
		vorbis_info*		_vorbisInfo;
		bool				_seek;
		double				_seekPos;

		//OGE_AUTO_MUTEX;
	};

 } // namespace