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

		/**	ȡ�õ�ǰ��Դ����ʱ�õ��ļ��� */
		virtual const String& GetFileName() const = 0;

		/**	ȡ��ID��ʶ */
		virtual const String& GetName() const = 0;

		/**	��ʼ���ŵ�ǰ��Դ */
		virtual void Play(size_t operationSet = 0) = 0;

		/**	ֹͣ���ţ������õ���ʼλ�� */
		virtual void Stop(size_t operationSet = 0) = 0;

		/**	��ͣ���� */
		virtual void Pause() = 0;

		/**	�Ƿ�����ͣ״̬ */
		virtual bool IsPaused() const = 0;

		/**	�����Ƿ�ѭ������ģʽ */
		virtual void SetLoop(bool loop) = 0;

		/**	�����Ƿ�Ϊѭ������ģʽ */
		virtual bool IsLoop() const = 0;
		
		/**	���õ�ǰ����ʱ��ƫ�� */
		virtual void SetOffset(double seconds) = 0;

		/**	ȡ�õ�ǰ�����˵�λ�� */
		virtual double GetOffset() const = 0;

		/**	�������� */
		virtual void SetVolumn(float volumn) = 0;

		/**	ȡ������ */
		virtual float GetVolumn() const = 0;

		/**	ȡ��ͨ������ */
		virtual int GetChannelCount() const = 0;

		/**	����ͨ������,ͨ����Ϊ0,˫������Ϊ0����Ϊ1 */
		virtual void SetVolumn(int channelID, float volumn) = 0;

		/**	ȡ��ͨ������,˫������Ϊ0����Ϊ1 */
		virtual float GetVolumn(int channelID) const = 0;

		/**	����Ƶ�ʱȣ����������� */
		virtual void SetPitch(float ratio) = 0;

		/**	ȡ��Ƶ�ʱȣ����������� */
		virtual float GetPitch() const = 0;

		/**	��ǰ�Ƿ��ڲ���״̬ */
		virtual bool IsPlaying() const = 0;

		/**	ȡ��������Чʱ�䳤�� */
		virtual double GetLengthInSeconds() const = 0;

		/**	�������ݣ����̻߳��� */
		virtual void BufferSounds() = 0;

		/**	�Ƿ�Ϊ��ʽ���ŵ����� */
		virtual bool IsStreamSound() const = 0;
	};

	typedef IXAudioSound* IXAudioSoundPtr;
} // namespace