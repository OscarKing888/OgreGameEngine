#pragma once

#include "XAudioPrerequisites.h"
#include <xaudio2.h>

namespace XAudio2Ex
{
	/**	Sound file codec interface */
	class _EngineExport ISoundFileCodec
	{
	public:

		/**	The type name of file extension */
		virtual const String& GetName() = 0;

		/**	If readSize = 0, read all the data, return the size read */
		virtual size_t ReadBuffer(DataStreamPtr strm,
			ByteBuffer& buffer, WAVEFORMATEX& outFmt, size_t readSize = 0) = 0;
	};
} // namespace