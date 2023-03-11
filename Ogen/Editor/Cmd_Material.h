#pragma once
#if 0
class Cmd_AddTextureUnit : public ICommandX
{
public:

	Cmd_AddTextureUnit(Pass* pass, TextureUnitState* tus);
	virtual ~Cmd_AddTextureUnit();
	virtual ICommandXPtr Execute();

protected:

	Pass*			_pass;
	TextureUnitState* _tus;
};

//------------------------------------------------------------------------

class Cmd_RemoveTextureUnit : public ICommandX
{
public:

	Cmd_RemoveTextureUnit(Pass* pass, TextureUnitState* tus);
	virtual ~Cmd_RemoveTextureUnit();
	virtual ICommandXPtr Execute();

protected:

	Pass*			_pass;
	TextureUnitState* _tus;
};


//------------------------------------------------------------------------

 class Cmd_AddPass : public ICommandX
 {
 public:
 
 	Cmd_AddPass(Pass* pass, Technique* tech);
 	virtual ~Cmd_AddPass();
 	virtual ICommandXPtr Execute();
 
 protected:
 
 	Technique*			_tech;
 	Pass*				_pass;
 };

 //------------------------------------------------------------------------

 class Cmd_RemovePass : public ICommandX
 {
 public:

	 Cmd_RemovePass(Pass* pass, Technique* tech);
	 virtual ~Cmd_RemovePass();
	 virtual ICommandXPtr Execute();

 protected:

	 Technique*			_tech;
	 Pass*				_pass;
 };

#endif