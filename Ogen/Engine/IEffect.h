// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "IUpdateAble.h"

// namespace Fantasy
// {
	/**
	*	һ��������Ϸ�б��ֻ��ǻ���ʱ��Ĺ��ܵĽӿ�
	*/
	class _EngineExport IEffect : virtual public RefObject, public IUpdateAble
	{
	public:

		/**	�Ƿ��Ѿ������Ӧ������������ */
		virtual bool IsDead() const = 0;

		/**	��ʼ������ */
		virtual void Init() = 0;
		//virtual void Cancel() = 0;
	};

	SmartPointer(IEffect);

	/**
	*	IEffect����������������������
	*/
	class _EngineExport IEffectManager : public IUpdateAble , public OgeAlloc
	{
	public:

		/**	���һ���µ�IEffect���� */
		virtual void AddEffect(const IEffectPtr& spEffect) = 0;

		/**	�Ƴ����е�IEffect����Ӧ�û���������� */
		virtual void RemoveAll() = 0;

		/**	�Ƿ�Ϊ�գ�������ĳЩ���Ͻ����жϴ��� */
		virtual bool IsEmpty() const = 0;
		//virtual void CancelAll() = 0;
	};
// } // namespace