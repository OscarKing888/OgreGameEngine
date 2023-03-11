#include "StdAfx.h"
#include "XAudioStreamSound.h"
#include "ISoundFileCodec.h"
#include "XAudioSoundManager.h"
#include "GameApp.h"

namespace XAudio2Ex
{
	/*
	** These next four methods are custom accessor functions to allow the Ogg Vorbis
	** libraries to be able to stream audio data directly from an DataStreamPtr
	*/
	size_t OgreALOggStreamRead(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);
		return dataStream->read(ptr, size);
	}

	int OgreALOggStreamSeek(void *datasource, ogg_int64_t offset, int whence)
	{
		DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);
		switch(whence)
		{
		case SEEK_SET:
			dataStream->seek(offset);
			break;
		case SEEK_END:
			dataStream->seek(dataStream->size());
			// Falling through purposefully here
		case SEEK_CUR:
			dataStream->skip(offset);
			break;
		}

		return 0;
	}

	int OgreALOggStreamClose(void *datasource)
	{
		return 0;
	}

	long OgreALOggStreamTell(void *datasource)
	{
		DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);
		return static_cast<long>(dataStream->tell());
	}

	//-------------------------------------------------------------------------------------------------

	XAudioStreamSound::XAudioStreamSound(const String& name, const String& fileName)
		: XAudioSound(name, fileName)
		, _vorbisInfo(0)
		, _restart(false)
		, _readBufferRequist(0)
		, _seekPos(0.0f)
		, _seek(false)
	{
#if !XAUDIO_THREADED
		GameApp::getSingleton().AddUpdateable(this);
#endif

		if(_vorbisInfo == 0)
		{
			_callbacks.close_func = OgreALOggStreamClose;
			_callbacks.tell_func = OgreALOggStreamTell;
			_callbacks.read_func = OgreALOggStreamRead;
			_callbacks.seek_func = OgreALOggStreamSeek;

			ov_open_callbacks(_fileStream.get(), &_oggStream, NULL, 0, _callbacks);

			size_t channels = 0;
			size_t samplesPerSec = 0;

			_lengthInSeconds = 0.0f;
			ogg_int64_t samplesCount = 0;
			ogg_int64_t compressedBytes = 0;
			long bitRate = 0;
			memset(&_wavFmt, 0, sizeof(	WAVEFORMATEX));
			_wavFmt.cbSize = sizeof(WAVEFORMATEX);
			_wavFmt.wFormatTag = WAVE_FORMAT_PCM;
			_vorbisInfo = ov_info(&_oggStream, -1);

			channels = _vorbisInfo->channels;
			samplesPerSec = _vorbisInfo->rate;

			_lengthInSeconds = ov_time_total(&_oggStream, -1);
			samplesCount = ov_pcm_total(&_oggStream, -1);
			compressedBytes = ov_raw_total(&_oggStream, -1);
			bitRate = ov_bitrate(&_oggStream, -1);

			_wavFmt.nChannels = (WORD)channels;
			_wavFmt.nSamplesPerSec = (DWORD)samplesPerSec;
			_wavFmt.nBlockAlign = (WORD)(samplesPerSec % 4);
			// 		_wavFmt.nSamplesPerSec = 0x0000ac44;
			_wavFmt.nAvgBytesPerSec = 0x0002b110;
			_wavFmt.nBlockAlign = 0x0004;
			_wavFmt.wBitsPerSample = 0x0010;
		}

		if(!_voice)
		{
			XAudioSoundManager::getSingleton().GetXAudio2()->CreateSourceVoice(&_voice, &_wavFmt,
				0, XAUDIO2_DEFAULT_FREQ_RATIO, (IXAudio2VoiceCallback*)this);			
		}
	}

	XAudioStreamSound::~XAudioStreamSound(void)
	{
#if !XAUDIO_THREADED
		GameApp::getSingleton().RemoveUpdateable(this);
#endif
		//OGE_AUTO_SCOPED_LOCK;

		ov_clear(&_oggStream);
		_fileStream->close();
	}

	void XAudioStreamSound::OnBufferStart(void* p)
	{
		//OGE_AUTO_SCOPED_LOCK;
		_isPlaying = true;
		_isPaused = false;
		//_tprintf(_T("+++OnBufferStart"));
		//_dataBuffer.clear();

		++_readBufferRequist;
		//ReadDataToBuffer();
	}

	void XAudioStreamSound::OnBufferEnd(void* p)
	{
		//_tprintf(_T("---OnBufferEnd"));
 		ByteBuffer* buf = reinterpret_cast<ByteBuffer*>(p);
		buf->clear();
		OGRE_DELETE buf;

		bool end = (_oggStream.offset == _oggStream.end);
		if(end && _isLoop)
		{
			//OGE_AUTO_SCOPED_LOCK;
			_restart = true;
			end = false;
			++_readBufferRequist;
			//ReadDataToBuffer();
		}
		else
		{
			//_isPlaying = false;
		}

		if(!end)
		{
			//ReadDataToBuffer();
		}
	}

	void XAudioStreamSound::ReadDataToBuffer()
	{
		//size_t curSeek = _fileStream->tell();
		//OGE_AUTO_SCOPED_LOCK;

		ByteBuffer* buf = OGE_NEW_T(ByteBuffer);
		ByteBuffer& dataBuffer = *buf;
		//ByteBuffer& dataBuffer = _dataBuffer;

		int bufferSize = (int)_wavFmt.nSamplesPerSec;
		// IMPORTANT : The ByteBuffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= _wavFmt.nBlockAlign;

#if 0
		//_fileStream->seek(0);
		_fileCodec->ReadBuffer(_fileStream, dataBuffer, _wavFmt, bufferSize);

#else
		char* data = OGE_ALLOC_T(char, bufferSize);

		int section, sizeRead = 0;
		while((int)dataBuffer.size() < bufferSize)
		{
			sizeRead = ov_read(&_oggStream, data, bufferSize, 0, 2, 1, &section);
			if(sizeRead <= 0)
			{
				break;
			}

			if(sizeRead > 0)
			{
				dataBuffer.insert(dataBuffer.end(), data, data + sizeRead);
			}
		}

		OGE_FREE(data);

#endif
		if(!dataBuffer.empty())
		{
			/*if(!_voice)
			{
				XAudioSoundManager::getSingleton().GetXAudio2()->CreateSourceVoice(&_voice, &_wavFmt,
				0, XAUDIO2_DEFAULT_FREQ_RATIO, (IXAudio2VoiceCallback*)this);
				_voice->Start(0);
			}*/
			_voice->Start(0);

			XAUDIO2_BUFFER xbuf = { 0 };
			xbuf.Flags = XAUDIO2_END_OF_STREAM;
			xbuf.pAudioData = (byte*)&dataBuffer[0];
			xbuf.AudioBytes = (UINT32)dataBuffer.size();
			xbuf.pContext = (void*)&dataBuffer;
			_lastXBuffer = xbuf;
			_voice->SubmitSourceBuffer(&xbuf);
		}
		else
		{
			LogManager::getSingleton().logMessage("Can't play stream sound file, format not support:" 
				+ _fileName, LML_CRITICAL);
			//++_readBufferRequist;
		}
	}

	void XAudioStreamSound::SetOffset(double seconds)
	{
		//OGE_AUTO_SCOPED_LOCK;

		_seek = true;
		_seekPos = seconds;

		//ov_time_seek(&_oggStream, seconds);
		if(_voice)
		{
			_voice->Stop(0);
			_voice->FlushSourceBuffers();
		}

		++_readBufferRequist;
		//ReadDataToBuffer();
		_voice->Start(0);
		_isPaused = false;
		_isPlaying = true;

		//EnableLogID("XAudio", true);

		LogConsoleIDf("XAudio", "XAudioStreamSound::SetOffset [%s] %f", _name.c_str(), seconds);
	}

	double XAudioStreamSound::GetOffset() const
	{
		//OGE_AUTO_SCOPED_LOCK;
		return ov_time_tell((OggVorbis_File*)&_oggStream);
	}

	void XAudioStreamSound::SetLoop(bool loop)
	{
		_isLoop = loop;
	}

	void XAudioStreamSound::Stop(size_t operationSet)
	{
		LogConsoleIDf("XAudio", "XAudioStreamSound::Stop %s", _name.c_str());
		//OGE_AUTO_SCOPED_LOCK;

		if(_voice)
		{
			_isLoop = false;
			_voice->Stop((UINT32)operationSet);
			_voice->FlushSourceBuffers();
			_restart = true;
			//ov_time_seek(&_oggStream, 0);
			_isPlaying = false;
			_isPaused = false;
		}
	}

	void XAudioStreamSound::Play(size_t operationSet)
	{
		LogConsoleIDf("XAudio", "XAudioStreamSound::Play %s", _name.c_str());
		//OGE_AUTO_SCOPED_LOCK;

		if(!_voice)
		{
			ReadDataToBuffer();
		}

		if(_voice)
		{
			_voice->Start(0, (UINT32)operationSet);
			_isPlaying = true;
			_isPaused = false;
			//ReadDataToBuffer();
			++_readBufferRequist;
		}
	}

	void XAudioStreamSound::Update(float deltaTime)
	{
		BufferSounds();		
	}
	
	void XAudioStreamSound::BufferSounds()
	{
		//OGE_AUTO_SCOPED_LOCK;
		if(_isPaused)
		{
			return;
		}

		if(_restart)
		{
			ov_time_seek(&_oggStream, 0);
			_restart = false;
		}
		else if(_seek)
		{
			ov_time_seek(&_oggStream, _seekPos);
			_seek = false;
		}

		if(_isPlaying)
		{
			while(_readBufferRequist > 0)
			{
				--_readBufferRequist;
				ReadDataToBuffer();
			}
		}
		else
		{
			_readBufferRequist = 0;
		}
	}
} // namespace