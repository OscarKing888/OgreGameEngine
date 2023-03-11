#include "StdAfx.h"
#include "XAudioSound.h"
#include "SoundFileCodecManager.h"
#include "XAudioSoundManager.h"

namespace XAudio2Ex
{

typedef std::vector<float> VolumnVector;


XAudioSound::XAudioSound(const String& name, const String& fileName)
: _voice(0)
, _fileCodec(0)
, _name(name)
, _fileName(fileName)
, _isLoop(false)
, _lengthInSeconds(0.0)
, _currentOffset(0.0)
, _isPlaying(false)
, _isPaused(false)
{
	String fileNameT;
	String ext;
	StringUtil::splitBaseFilename(fileName, fileNameT, ext);
	StringUtil::toLowerCase(ext);

	_fileCodec = SoundFileCodecManager::getSingleton().GetCodec(ext);
	_fileStream = ResourceGroupManager::getSingleton().openResource(_fileName);

	size_t sz = _fileStream->size();
	unsigned char* buffer = OGE_ALLOC_T(unsigned char, sz);
	_fileStream->read(buffer, sz);

	DataStreamPtr bufferStream(OGRE_NEW MemoryDataStream(buffer, sz, true));
	_fileStream = bufferStream;
}

XAudioSound::~XAudioSound(void)
{
	Destroy();
	_fileStream->close();
}

void XAudioSound::ReadDataToBuffer()
{
	if(_dataBuffer.empty())
	{
		_fileStream->seek(0);
		_dataBuffer.clear();
		_fileCodec->ReadBuffer(_fileStream, _dataBuffer, _wavFmt);
		_lengthInSeconds = (double)_dataBuffer.size() / (double)_wavFmt.nAvgBytesPerSec;
	}

	if(!_dataBuffer.empty())
	{
		if(!_voice)
		{
			XAudioSoundManager::getSingleton().GetXAudio2()->CreateSourceVoice(&_voice, &_wavFmt,
				0, XAUDIO2_DEFAULT_FREQ_RATIO, (IXAudio2VoiceCallback*)this);
			_voice->Start(0);
			_isPlaying = true;
		}

		XAUDIO2_BUFFER xbuf = { 0 };
		xbuf.Flags = XAUDIO2_END_OF_STREAM;
		xbuf.pAudioData = (byte*)&_dataBuffer[0];
		xbuf.AudioBytes = (UINT32)_dataBuffer.size();
		_lastXBuffer = xbuf;
		_voice->SubmitSourceBuffer(&xbuf);
	}
	else
	{
		LogManager::getSingleton().logMessage("Can't play sound file, format not support:" + _fileName, LML_CRITICAL);
	}
}

void* XAudioSound::GetXAudio2Voice()
{
	return _voice;
}

const String& XAudioSound::GetName() const
{
	return _name;
}

const String& XAudioSound::GetFileName() const
{
	return _fileName;
}


void XAudioSound::Play(size_t operationSet)
{
	LogConsoleIDf("XAudio", "XAudioSound::Play [%s]", _name.c_str());

	if(!_voice)
	{
		ReadDataToBuffer();
	}
	else if(_voice)
 	{
		//if(IsPlaying())
		{
			_voice->Stop((UINT32)operationSet);
		}

		if(!_isPaused)
		{
			SetOffset(0.0);
		}

		_voice->Start(0, (UINT32)operationSet);
		_isPlaying = true;
		_isPaused = false;
 	}
}

void XAudioSound::Stop(size_t operationSet)
{
	LogConsoleIDf("XAudio", "XAudioSound::Stop %s", _name.c_str());
	if(_voice)
	{
		//_voice->Stop((UINT32)operationSet);
		//SetOffset(0.0);
		_voice->Stop((UINT32)operationSet);
		//_voice->FlushSourceBuffers();
		_isPlaying = false;
		_isPaused = false;
	}
}

void XAudioSound::Pause()
{
	if(_voice)
	{
		_voice->Stop(0);
		_isPaused = true;
		_isPlaying = false;
	}

	LogConsoleIDf("XAudio", "XAudioSound::Pause [%s]", _name.c_str());
}

bool XAudioSound::IsPaused() const
{
	return _isPaused;
}

void XAudioSound::SetLoop(bool loop)
{
	_isLoop = loop;

	/*if(!IsPlaying())
	{
		SetOffset(0.0);
	}

	if(!_voice)
	{
		ReadDataToBuffer();
	}

	_voice->Stop(0);
	_voice->FlushSourceBuffers();

	if(loop)
	{
		_lastXBuffer.pAudioData = (byte*)&_dataBuffer[0];
		_lastXBuffer.AudioBytes = (UINT32)_dataBuffer.size();
		_lastXBuffer.LoopBegin = 0;
		_lastXBuffer.LoopLength = _lastXBuffer.AudioBytes;
		_lastXBuffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
		_lastXBuffer.PlayBegin = 0;
		_lastXBuffer.PlayLength = _lastXBuffer.AudioBytes;
		_voice->SubmitSourceBuffer(&_lastXBuffer);
	}
	else
	{
		_lastXBuffer.LoopBegin = 0;
		_lastXBuffer.LoopLength = 0;
		_lastXBuffer.LoopCount = 0;
		_lastXBuffer.PlayBegin = 0;
		_lastXBuffer.PlayLength = 0;
		_voice->SubmitSourceBuffer(&_lastXBuffer);
	}

	_voice->Start(0);
	*/
}

void XAudioSound::SetOffset(double seconds)
{
	if(_dataBuffer.empty())
	{
		ReadDataToBuffer();
	}

	if(seconds > GetLengthInSeconds())
	{
		assert(false);
		LogManager::getSingleton().logMessage(LML_CRITICAL, "[XAudioSound::SetOffset] time out of range.");
		return;
	}

	if(_voice)
	{
		_voice->FlushSourceBuffers();
	}

	_currentOffset = seconds;

	size_t offset = (UINT32)(seconds * (double)_wavFmt.nSamplesPerSec);
	offset = std::min(_dataBuffer.size() - 1, offset);

	XAUDIO2_BUFFER xbuf = { 0 };
	xbuf.Flags = XAUDIO2_END_OF_STREAM;
	xbuf.pAudioData = (byte*)&_dataBuffer[offset];
	xbuf.AudioBytes = (UINT32)(_dataBuffer.size() - offset);
	_lastXBuffer = xbuf;
	_voice->SubmitSourceBuffer(&xbuf);
}

double XAudioSound::GetOffset() const
{
 	XAUDIO2_VOICE_STATE state;
 	_voice->GetState( &state );
 	double currentOffset = (double)state.SamplesPlayed / (double)_wavFmt.nSamplesPerSec;
	return fmod(currentOffset, GetLengthInSeconds());
}

void XAudioSound::Destroy()
{
	if(_voice)
	{
		_voice->Stop(0);
		_isPlaying = false;
		_voice->DestroyVoice();
	}

	_dataBuffer.clear();
}

void XAudioSound::SetVolumn(float volumn)
{
	if(!_voice)
	{
		ReadDataToBuffer();
	}
	
	if(_voice)
	{
		volumn = std::max(volumn, 0.0f);
		_voice->SetVolume(volumn);
	}

#if !defined(_PRODUCT_) && 1
	for (int i = 0; i < volumn * 20; ++i)
	{
		printf("_");
	}
	printf("\n");
#endif
	LogConsoleIDf("XAudio", "SetVolumn [%s] [%f]", _name.c_str(), volumn);
}

float XAudioSound::GetVolumn() const
{
	if(!_voice)
	{
		return 1.0f;
	}

	float v = 0.0f;
	_voice->GetVolume(&v);
	return v;
}

int XAudioSound::GetChannelCount() const
{
	return (int)_wavFmt.nChannels;
}

void XAudioSound::SetVolumn(int channelID, float volumn)
{
	if(channelID >= GetChannelCount())
	{
		LogManager::getSingleton().logMessage(LML_CRITICAL, "Channel id out of range:" + GetName());
		return;
	}

	if(!_voice)
	{
		ReadDataToBuffer();
	}

	if(!_voice)
	{
		return;
	}

	size_t channels = (UINT32)GetChannelCount();
	VolumnVector vv;
	vv.resize(channels, 0.0f);
	_voice->GetChannelVolumes((UINT32)channels, &vv[0]);

	volumn = std::max(volumn, 0.0f);
	vv[channelID] = volumn;

	_voice->SetChannelVolumes((UINT32)channels, &vv[0]);
}

float XAudioSound::GetVolumn(int channelID) const
{
	if(channelID >= GetChannelCount())
	{
		LogManager::getSingleton().logMessage(LML_CRITICAL, "Channel id out of range:" + GetName());
		return 0.0f;
	}

	if(!_voice)
	{
		return 0.0f;
	}

	size_t channels = (UINT32)GetChannelCount();
	VolumnVector vv;
	vv.resize(channels, 0.0f);
	_voice->GetChannelVolumes((UINT32)channels, &vv[0]);

	return vv[channelID];
}


void XAudioSound::SetPitch(float ratio)
{
	if(!_voice)
	{
		ReadDataToBuffer();
	}

	if(_voice)
	{
		_voice->SetFrequencyRatio(ratio);
	}
}

float XAudioSound::GetPitch() const
{
	if(_voice)
	{
		return 1.0f;
	}

	float f = 0.0f;
	_voice->GetFrequencyRatio(&f);
	return f;
}


bool XAudioSound::IsPlaying() const
{
	if(!_voice)
	{
		return false;
	}

	if(_isPaused)
	{
		return false;
	}

	return _isPlaying;

	/*XAUDIO2_VOICE_STATE state;
	_voice->GetState( &state );
	bool isRunning = (state.BuffersQueued > 0) != 0;
	return isRunning;*/
}


void XAudioSound::OnBufferEnd(void* p)
{	
	_currentOffset = 0.0;
	if(!_isLoop)
	{
		//Stop();
	}
	else
	{
		//Stop();
		Play();
	}
}

void XAudioSound::OnBufferStart(void* p)
{
	_isPlaying = true;
	_isPaused = false;
}

void XAudioSound::OnProcessingPassStart()
{

}

void XAudioSound::OnProcessingPassEnd()
{
// 	XAUDIO2_POSITION pos;
// 	if(_voice)
// 	{
// 		_voice->GetPosition(&pos);
// 		_currentOffset = (double)pos.SamplesPlayed / (double)_wavFmt.nSamplesPerSec;
// 	}
}

void XAudioSound::OnCriticalError(HRESULT Error)
{

}

} // namespace