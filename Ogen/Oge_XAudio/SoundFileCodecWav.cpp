#include "StdAfx.h"
#include "SoundFileCodecWav.h"
#include "SoundFileCodecManager.h"
#include <MMSystem.h>

using namespace XAudio2Ex;

static SoundFileCodecWav g_SoundFileCodecWavInstance;

struct RIFF_HEADER
{
	char	szRiffID[4];		// 'R','I','F','F'
	DWORD	dwRiffSize;
	char	szRiffFormat[4];	// 'W','A','V','E'
};

struct WAVE_FORMAT
{
	WORD	wFormatTag;
	WORD	wChannels;
	DWORD	dwSamplesPerSec;
	DWORD	dwAvgBytesPerSec;
	WORD	wBlockAlign;
	WORD	wBitsPerSample;
};

struct FMT_BLOCK
{
	char		szFmtID[4];	// 'f','m','t',' '
	DWORD		dwFmtSize;
	WAVE_FORMAT	wavFormat;
};

struct DATA_BLOCK
{
	char	szDataID[4];	// 'd','a','t','a'
	DWORD	dwDataSize;
};


bool IsValidRiffHeader(RIFF_HEADER* pHeader)
{
	RIFF_HEADER *pRiffHeader = pHeader;
	if(!(	(pRiffHeader->szRiffID[0] == 'R') &&
		(pRiffHeader->szRiffID[1] == 'I') &&
		(pRiffHeader->szRiffID[2] == 'F') &&
		(pRiffHeader->szRiffID[3] == 'F') ) )
		return false;

	// Verify Wave in Riff
	if(!(	(pRiffHeader->szRiffFormat[0] == 'W') &&
		(pRiffHeader->szRiffFormat[1] == 'A') &&
		(pRiffHeader->szRiffFormat[2] == 'V') &&
		(pRiffHeader->szRiffFormat[3] == 'E') ) )
		return false;

	return true;
}

// verifies fmt chunk. checks for 'fmt '
bool IsValidFmtChunk(FMT_BLOCK* pChunk)
{
	FMT_BLOCK *pFmtHeader = pChunk;
	if(!(	(pFmtHeader->szFmtID[0] == 'f')&&
		(pFmtHeader->szFmtID[1] == 'm')&&
		(pFmtHeader->szFmtID[2] == 't')&&
		(pFmtHeader->szFmtID[3] == ' ') ) )
		return false;

	return true;
}

// checks for 'data' 
bool IsValidDataBlock(DATA_BLOCK* pData)
{
	DATA_BLOCK *pDataBlock = pData;
	if(!(	(pDataBlock->szDataID[0] == 'd')&&
		(pDataBlock->szDataID[1] == 'a')&&
		(pDataBlock->szDataID[2] == 't')&&
		(pDataBlock->szDataID[3] == 'a') ) )	
		return false;

	return true;
}

void GetARiffChunk(RIFF_HEADER& rh)
{
	rh.dwRiffSize = sizeof(rh);
	rh.szRiffID[0] = 'R';
	rh.szRiffID[1] = 'I';
	rh.szRiffID[2] = 'F';
	rh.szRiffID[3] = 'F';

	// Lets add the wave tag id.
	rh.szRiffFormat[0] = 'W';
	rh.szRiffFormat[1] = 'A';
	rh.szRiffFormat[2] = 'V';
	rh.szRiffFormat[3] = 'E';

	rh.dwRiffSize = sizeof(rh);
}

void GetADataBlock(DATA_BLOCK& db)
{
	db.szDataID[0] = 'd';
	db.szDataID[1] = 'a';
	db.szDataID[2] = 't';
	db.szDataID[3] = 'a';
}

SoundFileCodecWav::SoundFileCodecWav(void)
{
	SoundFileCodecManager::getSingleton().RegisterCodec(GetName(), this);
}

SoundFileCodecWav::~SoundFileCodecWav(void)
{
}


const String& SoundFileCodecWav::GetName()
{
	static String name = "wav";
	return name;
}

size_t SoundFileCodecWav::ReadBuffer(DataStreamPtr strm, ByteBuffer& buffer,
											 WAVEFORMATEX& outFmt, size_t readSize)
{
	assert(buffer.empty());

	memset(&outFmt, 0, sizeof(	WAVEFORMATEX));
	outFmt.cbSize = sizeof(WAVEFORMATEX);
	outFmt.wFormatTag = WAVE_FORMAT_PCM;

	if(readSize == 0)
	{
		RIFF_HEADER hdr;
		FMT_BLOCK fmt;
		DATA_BLOCK blk;

		strm->read(&hdr, sizeof(RIFF_HEADER));
		if(!IsValidRiffHeader(&hdr))
		{
			return 0;
		}

		strm->read(&fmt, sizeof(FMT_BLOCK));
		if(!IsValidFmtChunk(&fmt))
		{
			return 0;
		}
		
		// only support the PCM format, some ADPCM will crash without decode
		if((fmt.wavFormat.wFormatTag != WAVE_FORMAT_PCM))
		{
			return 0;
		}

 		strm->read(&blk, sizeof(DATA_BLOCK));
 		if(!IsValidDataBlock(&blk)) // HACK:
		{
			strm->seek(58);
		}

		size_t sz = strm->size() - strm->tell();
		buffer.resize(sz);
		size_t szRead = strm->read((void*)&buffer[0], sz);
		assert(szRead == sz);		

		outFmt.nAvgBytesPerSec = fmt.wavFormat.dwAvgBytesPerSec;
		outFmt.nBlockAlign = fmt.wavFormat.wBlockAlign;
		outFmt.nChannels = fmt.wavFormat.wChannels;
		outFmt.nSamplesPerSec = fmt.wavFormat.dwSamplesPerSec;
		outFmt.wBitsPerSample = fmt.wavFormat.wBitsPerSample;
		outFmt.wFormatTag = fmt.wavFormat.wFormatTag;
		
		assert(!buffer.empty() && "No buffer data read!");

		return szRead;
	}
	else
	{
		return 0;
	}
}
