#include "StdAfx.h"
#include "SoundFileCodecOgg.h"
#include "SoundFileCodecManager.h"

#pragma warning(push)

//warning C4244: 'argument' : conversion from 'ogg_int64_t' to 'long', possible loss of data
#pragma warning(disable : 4244)

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

using namespace XAudio2Ex;

static SoundFileCodecOgg g_SoundFileCodecOggInstance;

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

SoundFileCodecOgg::SoundFileCodecOgg(void)
{
	SoundFileCodecManager::getSingleton().RegisterCodec(GetName(), this);
}

SoundFileCodecOgg::~SoundFileCodecOgg(void)
{
}


const String& SoundFileCodecOgg::GetName()
{
	static String name = "ogg";
	return name;
}

size_t SoundFileCodecOgg::ReadBuffer(DataStreamPtr strm, ByteBuffer& buffer,
									 WAVEFORMATEX& outFmt, size_t readSize)
{
	assert(buffer.empty());
	
	size_t curSeek = strm->tell();

	ov_callbacks callbacks;
	callbacks.close_func = OgreALOggStreamClose;
	callbacks.tell_func = OgreALOggStreamTell;
	callbacks.read_func = OgreALOggStreamRead;
	callbacks.seek_func = OgreALOggStreamSeek;

	OggVorbis_File oggStream;
	vorbis_info* vorbisInfo = 0;

	// Open the DataStreamPtr
	ov_open_callbacks(strm.get(), &oggStream, NULL, 0, callbacks);
	//_tprintf(_T("ov_open_callbacks %d\n"), r);

	//curSeek = strm->tell();

	bool initRead = false;

	size_t channels = 0;
	size_t samplesPerSec = 0;

	double lengthInSeconds = 0.0f;
	ogg_int64_t samplesCount = 0;
	ogg_int64_t compressedBytes = 0;
	long bitRate = 0;

	if(curSeek == 0)
	{
		initRead = true;
		memset(&outFmt, 0, sizeof(	WAVEFORMATEX));
		outFmt.cbSize = sizeof(WAVEFORMATEX);
		outFmt.wFormatTag = WAVE_FORMAT_PCM;
		vorbisInfo = ov_info(&oggStream, -1);

		channels = vorbisInfo->channels;
		samplesPerSec = vorbisInfo->rate;

		lengthInSeconds = ov_time_total(&oggStream, -1);
		samplesCount = ov_pcm_total(&oggStream, -1);
		compressedBytes = ov_raw_total(&oggStream, -1);
		bitRate = ov_bitrate(&oggStream, -1);

		outFmt.nChannels = (WORD)channels;
		outFmt.nSamplesPerSec = (DWORD)samplesPerSec;
		outFmt.nBlockAlign = (WORD)(samplesPerSec % 4);

		// 		outFmt.nSamplesPerSec = 0x0000ac44;
		outFmt.nAvgBytesPerSec = 0x0002b110;
		outFmt.nBlockAlign = 0x0004;
		outFmt.wBitsPerSample = 0x0010;
	}
	else
	{
		strm->seek(curSeek);
	}

	if(readSize == 0)
	{
		//calculateFormatInfo();		

		// decode to buffer
		int bufferSize = (int)samplesPerSec;
		// IMPORTANT : The ByteBuffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= outFmt.nBlockAlign;

		char* data = OGE_ALLOC_T(char, bufferSize);
		int section, sizeRead = 0;

		// Read the rest of the file
		//time_t t = time(NULL);
		do
		{
			sizeRead = ov_read(&oggStream, data, bufferSize, 0, 2, 1, &section);
			buffer.insert(buffer.end(), data, data + sizeRead);
		}while(sizeRead > 0);

		//time_t t2 = time(NULL);

		//_tprintf(_T("Time %d"), t2 - t);

		OGE_FREE(data);

		ov_clear(&oggStream);

		outFmt.nAvgBytesPerSec = (int)((double)buffer.size() / lengthInSeconds + 0.5f);
		//outFmt.wBitsPerSample = outFmt.nAvgBytesPerSec * 8 / outFmt.nSamplesPerSec;

		return buffer.size();
	}
	else
	{
		//buffer.resize(readSize, 0);
		char* data = OGE_ALLOC_T(char, readSize);
		
		int section, sizeRead = 0;

		while(buffer.size() < readSize)
		{
			sizeRead = ov_read(&oggStream, data, (int)readSize, 0, 2, 1, &section);
			if(sizeRead <= 0)
			{
				break;
			}

			if(sizeRead > 0)
			{
				buffer.insert(buffer.end(), data, data + sizeRead);
			}
		}

		ov_clear(&oggStream);
		OGE_FREE(data);
		return sizeRead;
	}
}

#pragma warning(pop)
