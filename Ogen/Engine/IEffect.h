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
	*	一种用于游戏中表现或是基于时间的功能的接口
	*/
	class _EngineExport IEffect : virtual public RefObject, public IUpdateAble
	{
	public:

		/**	是否已经完成相应的作用生存期 */
		virtual bool IsDead() const = 0;

		/**	初始化操作 */
		virtual void Init() = 0;
		//virtual void Cancel() = 0;
	};

	SmartPointer(IEffect);

	/**
	*	IEffect管理器，控制其生存周期
	*/
	class _EngineExport IEffectManager : public IUpdateAble , public OgeAlloc
	{
	public:

		/**	添加一个新的IEffect对象 */
		virtual void AddEffect(const IEffectPtr& spEffect) = 0;

		/**	移除所有的IEffect对象，应该会调用其析构 */
		virtual void RemoveAll() = 0;

		/**	是否为空，可用于某些场合进行判断处理 */
		virtual bool IsEmpty() const = 0;
		//virtual void CancelAll() = 0;
	};
// } // namespace