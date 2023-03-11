#pragma once

class Cmd_AddElement : public ICommandX
{
public:

	Cmd_AddElement(SpecialEffects* sfx, const String& typeName,
		const String& name,
		const NameValuePairList& parms);
	virtual ~Cmd_AddElement(void);

	virtual ICommandXPtr Execute();

protected:

	SpecialEffects* _sfx;
	String _elementType;
	String _name;
	NameValuePairList _params;
};

//--------------------------------------------------------------------------------

class Cmd_RemoveElement : public ICommandX
{
public:

	Cmd_RemoveElement(SpecialEffects* sfx, int idx);
	virtual ~Cmd_RemoveElement(void);

	virtual ICommandXPtr Execute();

protected:

	SpecialEffects* _sfx;
	int _idx;
};
