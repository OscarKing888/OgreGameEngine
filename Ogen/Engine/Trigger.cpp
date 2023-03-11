// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************
#include "stdafx.h"
#include "Trigger.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "IGame.h"
#include "Property_String.h"
#include "Property_Bool.h"

class TriggerFactory : public TGameObjectFactory<Trigger>
{
};

M_REG_FACTORY(Trigger, TriggerFactory, Trigger)

const String Trigger::C_TypeName("Trigger");

Trigger::TriggerMap	Trigger::S_Triggers;

//--------------------------------------------------------------------------------
void ITriggerEventHandler::OnTouch( Trigger* trigger, IGameObject* object )
{

}

void ITriggerEventHandler::OnStay( Trigger* trigger, IGameObject* object )
{

}

void ITriggerEventHandler::OnLeave( Trigger* trigger, IGameObject* object )
{

}
//--------------------------------------------------------------------------------

Trigger::Trigger(void)
: _aabbBox(0)
, _size(100, 100, 100)
, _query(0)
, _showCollisionBox(true)
{
	_isEditorDislpayOnly = true;

	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();

	_query = sm->createAABBQuery(AxisAlignedBox::BOX_NULL);

	S_Triggers.insert(std::make_pair(this, this));
}

Trigger::~Trigger(void)
{
	TriggerMap::iterator it = S_Triggers.find(this);
	assert(it != S_Triggers.end());
	S_Triggers.erase(it);

	IGame* game = GameApp::getSingleton().GetGame();
	if(game)
	{
		SceneManager* sm = game->GetVisualScene();
		if(sm)
		{
			sm->destroyQuery(_query);
		}
	}
}

AxisAlignedBox Trigger::GetBoundingBox() const
{	
	Vector3 pos = Vector3::ZERO;
	if(_node)
	{
		pos = _node->_getDerivedPosition();
	}

	AxisAlignedBox box(Vector3(pos - _size * 0.5f), Vector3(pos + _size * 0.5f));
	return box;
}

void Trigger::SetSize(const Vector3& sz)
{
	_size = sz;
	if(_aabbBox)
	{
		_aabbBox->SetSize(sz);
	}
}

const Vector3& Trigger::GetSize() const
{
	return _size;
}

void Trigger::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();

	AddProp(Property_Vector3, Size, Trigger, "Size", "Trigger");
	AddProp(Property_String, Flag, Trigger, "Flag", "Trigger");
	AddProp(Property_Bool, ShowCollisionBox, Trigger, "ShowCollisionBox", "Trigger");

}

void Trigger::DestroyEntity()
{
	if(_aabbBox == 0)
	{
		return;
	}

	PreDestroyEntity();
	DestroyEntityImp();
	PostDestroyEntity();
}

void Trigger::CreateEntity()
{	
	PreCreateEntity();
	CreateEntityImp();
	PostCreateEntity();
}

void Trigger::DestroyEntityImp()
{
	if(GameApp::CurrentRunType == ERT_Editor)
	{
		OGRE_DELETE _aabbBox;
		_aabbBox = 0;
	}
}

void Trigger::CreateEntityImp()
{
	if(GameApp::CurrentRunType == ERT_Editor)
	{
		//IGame* game = GameApp::getSingleton().GetGame();
		//SceneManager* sm = game->GetVisualScene();
		_aabbBox = OGRE_NEW WireBox();
		_aabbBox->setUserAny(Any(AnyGameObject(this)));
		_node->attachObject(_aabbBox);
		_aabbBox->setCastShadows(false);
		_aabbBox->SetSize(_size);
		_aabbBox->setVisible(_showCollisionBox);
		_aabbBox->setQueryFlags(GetQueryFlag());
	}
}

void Trigger::PreCreateEntity()
{
}

void Trigger::PostCreateEntity()
{

}

void Trigger::PreDestroyEntity()
{	
}

void Trigger::PostDestroyEntity()
{

}

void Trigger::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateEntity();
}

void Trigger::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyEntity();
}

const String& Trigger::GetTypeName() const
{
	return C_TypeName;
}

STriggerResult Trigger::CheckCollision(ITriggerEventHandler* handler)
{
	STriggerResult results;
	results.TriggerObject = this;
	_query->setBox(GetBoundingBox());
	results.RawResult = _query->execute();
	IGame* game = GameApp::getSingleton().GetGame();

	for (SceneQueryResultMovableList::iterator i = results.RawResult.movables.begin();
		i != results.RawResult.movables.end(); ++i)
	{
		MovableObject* obj = *i;
		if(obj)
		{
			IGameObject* gameObject = 0;
			if(!obj->getUserAny().isEmpty())
			{
				gameObject = any_cast<AnyGameObject>(obj->getUserAny()).GameObject;
			}

			if(gameObject == 0)
			{
				XGUID guid;
				guid.fromString(obj->getName());
				gameObject = game->GetGameObject(guid);
			}

			if(gameObject && (gameObject != this))
			{
				if((gameObject->GetTriggerMask() & this->GetTriggerMask()) != 0)
				{
					results.TouchedObjects.push_back(gameObject);

					// 检查Trigger事件
					ObjectTriggerStateMap::const_iterator it = _triggerStateMap.find(gameObject);
					if(it != _triggerStateMap.end())
					{
						handler->OnStay(this, gameObject);
					}
					else
					{
						handler->OnTouch(this, gameObject);
						_triggerStateMap.insert(std::make_pair(gameObject, gameObject));
					} // if
				}
			} // if
		} // if
	} // for

	// 检查离开事件
	ObjectTriggerStateMap::iterator it = _triggerStateMap.begin();
	for (; it != _triggerStateMap.end();)
	{
		GameObjectList::const_iterator itFind = std::find(results.TouchedObjects.begin(),
			results.TouchedObjects.end(), it->first);
		
		if(itFind == results.TouchedObjects.end())
		{
			handler->OnLeave(this, it->first);
			it = _triggerStateMap.erase(it);
		}
		else
		{
			++it;
		}
	}

	return results;
}

void Trigger::SetShowCollisionBox(bool on)
{
	_showCollisionBox = on;
	if(_aabbBox)
	{
		_aabbBox->setVisible(_showCollisionBox);
	}
}

bool Trigger::GetShowCollisionBox() const
{
	return _showCollisionBox;
}
