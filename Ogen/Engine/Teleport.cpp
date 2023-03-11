#include "stdafx.h"
#include "Teleport.h"
#include "AnimationModel.h"
#include "GameApp.h"

class TeleportFactory : public TGameObjectFactory<Teleport>
{
public:

};

M_REG_FACTORY(Teleport, TeleportFactory, Scene)

Teleport::Teleport()
: _targetPosition(Vector3::ZERO)
, _entity(0)
, _meshFile(M_DEBUG_MESH_FILE)
{
	GameApp::getSingleton().AddUpdateable(this);
}

Teleport::~Teleport()
{

}


void Teleport::Destroy()
{
	DestroyDisplayModel();
	Trigger::Destroy();
	GameApp::getSingleton().RemoveUpdateable(this);
}

const String& Teleport::GetTypeName() const
{
	static String sName("Teleport");
	return sName;
}

void Teleport::BuildPropertyMap()
{
	Trigger::BuildPropertyMap();

	AddProp(Property_String, TargetSceneName, Teleport, "TargetSceneName", "Teleport");
	AddProp(Property_Vector3, TargetPosition, Teleport, "TargetPosition", "Teleport");
	AddProp(Property_MeshFile, MeshFile, Teleport, "MeshFile", "Teleport");
	//AddProp(Property_GUID, GUID, Teleport);
	AddProp(Property_StringVector, EnabledAnimations, Teleport, "EnabledAnimations", "Teleport");
	AddReadonlyProp(Property_StringVector, AllAnimations, Teleport, "AllAnimations", "Teleport");
}

void Teleport::SetMeshFile(const String& meshFile)
{
	_meshFile = meshFile;
	DestroyDisplayModel();
	CreateDisplayModel();
}


String Teleport::GetModelName()
{
	return GetGUID().toString() + ".DislplayModel";
}

void Teleport::PostCreateEntity()
{
	Trigger::PostCreateEntity();
	CreateDisplayModel();
}

void Teleport::PreDestroyEntity()
{
	DestroyDisplayModel();
	Trigger::PreDestroyEntity();
}

void Teleport::DestroyDisplayModel()
{
	if(_entity == 0)
	{
		return;
	}

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	sm->destroyEntity(_entity);
	_entity = 0;
}

void Teleport::CreateDisplayModel()
{
	if(_meshFile.empty())
	{
		return;
	}

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	_entity = sm->createEntity(GetModelName(), _meshFile);
	_entity->setUserAny(Any(AnyGameObject(this)));
	_node->attachObject(_entity);
	_entity->setQueryFlags(GetQueryFlag());
	_entity->setVisibilityFlags(_visibilityFlag);

	SetEnabledAnimations(_enabledAnimations);
}

StringVector Teleport::GetAllAnimations() const
{
	if(_entity)
	{
		return AnimationModel::GetAnimations(_entity);
	}

	return StringVector();
}

StringVector Teleport::GetEnabledAnimations() const
{
	StringVector lst;
	if(_entity)
	{
		if(_entity->getAllAnimationStates())
		{
			ConstEnabledAnimationStateIterator asi =
				_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();

			while (asi.hasMoreElements())
			{
				lst.push_back(asi.peekNext()->getAnimationName());
				asi.moveNext();
			}
		}
	}

	return lst;
}

void Teleport::SetEnabledAnimations(const StringVector& aniList)
{
	if(_entity)
	{
		AnimationStateSet* ass = _entity->getAllAnimationStates();
		if(ass)
		{
			for (size_t i = 0; i < aniList.size(); ++i)
			{
				const String& ani = aniList[i];
				if(ass->hasAnimationState(ani))
				{
					AnimationState* as = ass->getAnimationState(ani);
					as->setEnabled(true);
				}
				else
				{
					LogErrorIDf("GameLogic", "[Teleport::SetEnabledAnimations] animation [%s] not found!", ani.c_str());
				}
			}
		}
	}

	_enabledAnimations = aniList;
}

void Teleport::Update(float deltaTime)
{
	if (_entity && (_entity->getAllAnimationStates() != 0))
	{
		ConstEnabledAnimationStateIterator asi =
			_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
		while (asi.hasMoreElements())
		{
			asi.peekNext()->addTime(deltaTime);
			asi.moveNext();
		}
	}
}

void Teleport::SetVisibilityFlag(uint32 flag)
{
	Trigger::SetVisibilityFlag(flag);

	if(_entity)
	{
		_entity->setVisibilityFlags(_visibilityFlag);
	}
}

