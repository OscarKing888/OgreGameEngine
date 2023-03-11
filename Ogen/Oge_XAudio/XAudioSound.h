#pragma once
#include "IXAudioSound.h"
#include <xaudio2.h>
#include "XAudioPrerequisites.h"

namespace XAudio2Ex
{
	class ISoundFileCodec;

	class _EngineExport XAudioSound
		: public IXAudioSound
		, public IXAudio2VoiceCallback
		, public IXAudio2EngineCallback
		, public OgeAlloc
	{
	public:

		XAudioSound(const String& name, const String& fileName);
		virtual ~XAudioSound(void);

		virtual void* GetXAudio2Voice();

		virtual const String& GetName() const;
		virtual const String& GetFileName() const;

		virtual void Play(size_t operationSet = 0);
		virtual void Stop(size_t operationSet = 0);
		
		virtual void Pause();
		virtual bool IsPaused() const;

		virtual void SetLoop(bool loop);
		virtual bool IsLoop() const { return _isLoop; }

		virtual void SetOffset(double seconds);
		virtual double GetOffset() const;

		virtual void Destroy();

		virtual void SetVolumn(float volumn);
		virtual float GetVolumn() const;
		virtual int GetChannelCount() const;	
		virtual void SetVolumn(int channelID, float volumn);
		virtual float GetVolumn(int channelID) const;

		virtual void SetPitch(float ratio);
		virtual float GetPitch() const;

		virtual bool IsPlaying() const;
		virtual double GetLengthInSeconds() const{ return _lengthInSeconds; }

		virtual void BufferSounds(){}
		virtual bool IsStreamSound() const { return false; }

	protected:

		DataStreamPtr _fileStream;
		IXAudio2SourceVoice*		_voice;
		ISoundFileCodec*	_fileCodec;
		ByteBuffer				_dataBuffer;
		WAVEFORMATEX		_wavFmt;
		String		_name;
		String		_fileName;
		bool				_isLoop;
		double				_lengthInSeconds;
		double				_currentOffset;
		XAUDIO2_BUFFER		_lastXBuffer;
		bool				_isPlaying;
		bool				_isPaused;

		virtual void ReadDataToBuffer();

		void __stdcall OnVoiceProcessingPassStart() {}
		void __stdcall OnVoiceProcessingPassStart(UINT32) {}
		void __stdcall OnVoiceProcessingPassEnd() {}
		void __stdcall OnStreamEnd() {}
		void __stdcall OnBufferStart( void* );
		void __stdcall OnBufferEnd( void* );
		void __stdcall OnLoopEnd( void* ) {}
		void __stdcall OnVoiceError( void*, HRESULT ) {}

		STDMETHOD_(void, OnProcessingPassStart) ();
		STDMETHOD_(void, OnProcessingPassEnd) ();
		STDMETHOD_(void, OnCriticalError) (HRESULT Error);
	};

} // namespace