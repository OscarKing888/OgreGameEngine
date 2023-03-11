#pragma once

#include "ISoundFileCodec.h"

namespace XAudio2Ex
{
	class _EngineExport SoundFileCodecWav :
		public ISoundFileCodec
	{
	public:

		SoundFileCodecWav(void);
		virtual ~SoundFileCodecWav(void);

		/**	The type name of file extension */
		virtual const String& GetName();

		/**	If readSize = 0, read all the data */
		virtual size_t ReadBuffer(DataStreamPtr strm,
			ByteBuffer& buffer, WAVEFORMATEX& outFmt, size_t readSize = 0);
	};

} // namespace