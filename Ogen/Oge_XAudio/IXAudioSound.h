#pragma once
#include "EnginePrerequisites.h"

namespace XAudio2Ex
{
	enum _EngineExport EChannel
	{
		EC_LEFT		= 0,
		EC_RIGHT	= 1
	};

	class _EngineExport IXAudioSound
	{
	public:

		virtual void* GetXAudio2Voice() = 0;

		/**	取得当前音源创建时用的文件名 */
		virtual const String& GetFileName() const = 0;

		/**	取得ID标识 */
		virtual const String& GetName() const = 0;

		/**	开始播放当前音源 */
		virtual void Play(size_t operationSet = 0) = 0;

		/**	停止播放，并重置到开始位置 */
		virtual void Stop(size_t operationSet = 0) = 0;

		/**	暂停播放 */
		virtual void Pause() = 0;

		/**	是否是暂停状态 */
		virtual bool IsPaused() const = 0;

		/**	设置是否循环播放模式 */
		virtual void SetLoop(bool loop) = 0;

		/**	返回是否为循环播放模式 */
		virtual bool IsLoop() const = 0;
		
		/**	设置当前播放时间偏移 */
		virtual void SetOffset(double seconds) = 0;

		/**	取得当前播放了的位置 */
		virtual double GetOffset() const = 0;

		/**	设置音量 */
		virtual void SetVolumn(float volumn) = 0;

		/**	取得音量 */
		virtual float GetVolumn() const = 0;

		/**	取得通道数量 */
		virtual int GetChannelCount() const = 0;

		/**	设置通道音量,通道起为0,双声道左为0，右为1 */
		virtual void SetVolumn(int channelID, float volumn) = 0;

		/**	取得通道音量,双声道左为0，右为1 */
		virtual float GetVolumn(int channelID) const = 0;

		/**	设置频率比，即快慢倍率 */
		virtual void SetPitch(float ratio) = 0;

		/**	取得频率比，即快慢倍率 */
		virtual float GetPitch() const = 0;

		/**	当前是否处于播放状态 */
		virtual bool IsPlaying() const = 0;

		/**	取得整首音效时间长度 */
		virtual double GetLengthInSeconds() const = 0;

		/**	加载数据，多线程缓存 */
		virtual void BufferSounds() = 0;

		/**	是否为流式播放的声音 */
		virtual bool IsStreamSound() const = 0;
	};

	typedef IXAudioSound* IXAudioSoundPtr;
} // namespace