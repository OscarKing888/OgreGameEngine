#include "stdafx.h"
#include "Cmd_Material.h"

//------------------------------------------------------------------------
// Cmd_AddTextureUnit
//------------------------------------------------------------------------
//Cmd_AddTextureUnit::Cmd_AddTextureUnit(Pass* pass, TextureUnitState* tus)
//: _pass(pass)
//, _tus(tus)
//{
//}
//
//Cmd_AddTextureUnit::~Cmd_AddTextureUnit()
//{
//}
//
//ICommandXPtr Cmd_AddTextureUnit::Execute()
//{
//	_tus->setParent(0);
//	_pass->addTextureUnitState(_tus);
//	return OGRE_NEW Cmd_RemoveTextureUnit(_pass, _tus);
//}

//------------------------------------------------------------------------
// Cmd_AddTextureUnit
//------------------------------------------------------------------------
//Cmd_RemoveTextureUnit::Cmd_RemoveTextureUnit(Pass* pass, TextureUnitState* tus)
//: _pass(pass)
//, _tus(tus)
//{
//}
//
//Cmd_RemoveTextureUnit::~Cmd_RemoveTextureUnit()
//{
//}
//
//ICommandXPtr Cmd_RemoveTextureUnit::Execute()
//{
//	TextureUnitState* tus = OGRE_NEW TextureUnitState(_pass, *_tus);
//
//	_pass->removeTextureUnitState(
//		_pass->getTextureUnitStateIndex(_tus), false);
//	_tus = tus;
//	tus->setParent(0);
//	return OGRE_NEW Cmd_AddTextureUnit(_pass, tus);
//}

//------------------------------------------------------------------------
// Cmd_AddPass
//------------------------------------------------------------------------
//Cmd_AddPass::Cmd_AddPass(Pass* pass, Technique* tech)
//: _pass(pass)
//, _tech(tech)
//{
//}
//
//Cmd_AddPass::~Cmd_AddPass()
//{
//}
//
//ICommandXPtr Cmd_AddPass::Execute()
//{
//	_tech->addPass(_pass);
//	return OGRE_NEW Cmd_RemovePass(_pass, _tech);
//}

//------------------------------------------------------------------------
// Cmd_RemovePass
//------------------------------------------------------------------------
//Cmd_RemovePass::Cmd_RemovePass(Pass* pass, Technique* tech)
//: _pass(pass)
//, _tech(tech)
//{
//}
//
//Cmd_RemovePass::~Cmd_RemovePass()
//{
//}
//
//ICommandXPtr Cmd_RemovePass::Execute()
//{
//	Pass* pass = OGRE_NEW Pass(_tech, _pass->getIndex(), *_pass);
//	_tech->removePass(_pass->getIndex(), false);
//	_pass = pass;
//	return OGRE_NEW Cmd_AddPass(_pass, _tech);
//}