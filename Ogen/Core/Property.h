// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "IProperty.h"

template<typename SetType, typename GetType>
class /*_CoreExport*/ Property : public IProperty
{
public:

	typedef std::tr1::function<void (SetType)> SetFunType;
	typedef std::tr1::function<GetType (void)> GetFunType;

	Property(SetFunType setFun, GetFunType getFun)
		: _setFun(setFun)
		, _getFun(getFun)
		, _distributeFlag(eDF_All)
		, _visibleRange(500.0f)
	{
	}

	virtual ~Property(void){}

	virtual const String	GetTypeName() const { return "Property"; }
	virtual const String&	GetName() const { return _strName; }
	virtual void				SetName(const String& strName){ _strName = strName; }

	virtual const String&	GetDisplayName() const { return _strDisplayName; }
	virtual void				SetDisplayName(const String& strName){ _strDisplayName = strName; }

	virtual const String&	GetPropertyCategory() const { return _strCategory; }
	virtual void				SetPropertyCategory(const String& strCategory){_strCategory = strCategory; }

	virtual const String&	GetDescription() const { return _strDescription; }
	virtual void				SetDescription(const String& strDescription){_strDescription = strDescription; }

	virtual bool				IsSerializeable() const { return (_propertyFlag & ePropertyFlagSerializable) != 0; }

	virtual bool				IsReadOnly() const { return (_propertyFlag & ePropertyFlagReadonly) != 0; }

	virtual size_t				GetFlag() const { return _propertyFlag; }
	virtual void				SetFlag(size_t uFlag){ _propertyFlag = uFlag; }

	bool						IsVisibleToEditor() const { return (_propertyFlag & ePropertyFlagEditable) != 0; }

	void SetValue(SetType value)
	{ 
		if(!IsReadOnly())
		{
			_setFun(value);
		}

		//LogErrorIDf("Property", "Property[%s][%s] is read only.", _strCategory.c_str(), _strName.c_str());
	}

	GetType GetValue() const { return _getFun(); }

	virtual void SetSerializeable(bool on)
	{
		if(on)
		{
			_propertyFlag |= ePropertyFlagSerializable;
		}
		else
		{
			_propertyFlag &= ~ePropertyFlagSerializable;
		}
	}

	virtual void SetReadOnly(bool on)
	{
		if(on)
		{
			_propertyFlag |= ePropertyFlagReadonly;
		}
		else
		{
			_propertyFlag &= ~ePropertyFlagReadonly;
		}
	}

	virtual void SetVisibleToEditor(bool on)
	{
		if(on)
		{
			_propertyFlag |= ePropertyFlagEditable;
		}
		else
		{
			_propertyFlag &= ~ePropertyFlagEditable;
		}
	}

	virtual int GetSerializingPriority() const{ return _priority; }
	virtual void SetSerializingPriority(int priority) { _priority = priority; }

	virtual int GetDistributeFlag() const { return _distributeFlag; }
	virtual void SetDistributeFlag(int flag) { _distributeFlag = flag; }

	virtual float GetVisibleRange() const { return _visibleRange; }
	virtual void SetVisibleRange(float range) { _visibleRange = range; }

protected:

	String			_strName;			// property string key, by name
	String			_strDisplayName;	// property display string
	String			_strCategory;		// category using for sort
	String			_strDescription;

	size_t					_propertyFlag;
	int						_priority;

	GetFunType				_getFun;
	SetFunType				_setFun;
	int						_distributeFlag;
	float					_visibleRange;
};

