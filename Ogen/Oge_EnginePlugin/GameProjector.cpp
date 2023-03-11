#include "StdAfx.h"
#include "GameProjector.h"

class GameProjectorFactory : public TGameObjectFactory<GameProjector>
{
};

M_REG_FACTORY(Projector, GameProjectorFactory, VisualObject);

GameProjector::GameProjector(void)
	: _projector(0)
{
}
//--------------------------------------------------------------------------------
GameProjector::~GameProjector(void)
{
}
//--------------------------------------------------------------------------------
const String& GameProjector::GetTypeName() const
{
	static String sName("Projector");
	return sName;
}
//--------------------------------------------------------------------------------
void GameProjector::BuildPropertyMap()
{
	PlacementObject::BuildPropertyMap();

	AddProp(Property_Material, Material, GameProjector, "Material", "Projector");

	AddProp(Property_Bool, ShowProjectorBoundingBox, GameProjector, "ShowProjectorBoundingBox", "Projector");
	AddProp(Property_Bool, ShowFrustum, GameProjector, "ShowFrustum", "Projector");

	AddProp(Property_Float, OrthoWindowWidth, GameProjector, "OrthoWindowWidth", "Projector");
	AddProp(Property_Bool, Perspective, GameProjector, "Perspective", "Projector");

	AddRangeFloatProp(FovDegree, 0, 180.0f, GameProjector, "FovDegree", "Projector");
}
//--------------------------------------------------------------------------------
void GameProjector::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateProjector();
}
//--------------------------------------------------------------------------------
void GameProjector::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyProjector();
}
//--------------------------------------------------------------------------------
void GameProjector::CreateProjector()
{
	if(!_projector)
	{
		_projector = OGRE_NEW Projector(GetGUID().toString());

		_projector->setTerrain(GameApp::getSingleton().GetGame()->GetTerrainGroup());
		_projector->MovableObject::setUserAny(Any(AnyGameObject(this)));
	}

	if(_node)
	{
		_node->attachObject(_projector);
		_node->setDirection(Vector3::NEGATIVE_UNIT_Y, Node::TS_WORLD);
	}
}
//--------------------------------------------------------------------------------
void GameProjector::DestroyProjector()
{
	if(_projector)
	{
		OGRE_DELETE _projector;
		_projector = 0;
	}
}
//--------------------------------------------------------------------------------
String GameProjector::GetMaterial() const
{
	if(_projector)
	{
		return _projector->getPatchMaterialName();
	}

	return StringUtil::BLANK;
}
//--------------------------------------------------------------------------------
void GameProjector::SetMaterial( const String& material )
{
	if(_projector)
	{
		return _projector->setPatchMaterial(material);
	}
}
//--------------------------------------------------------------------------------
bool GameProjector::GetShowProjectorBoundingBox() const
{
	if(_projector)
	{
		return _projector->getShowBoundingBox();
	}

	return false;
}
//--------------------------------------------------------------------------------
void GameProjector::SetShowProjectorBoundingBox( bool on )
{
	if(_projector)
	{
		return _projector->setShowFrustum(on);
	}
}
//--------------------------------------------------------------------------------
bool GameProjector::GetShowFrustum() const
{
	if(_projector)
	{
		return _projector->getShowFrustum();
	}

	return false;
}
//--------------------------------------------------------------------------------
void GameProjector::SetShowFrustum( bool on )
{
	if(_projector)
	{
		return _projector->setShowFrustum(on);
	}
}
//--------------------------------------------------------------------------------
float GameProjector::GetOrthoWindowWidth() const
{
	if(_projector)
	{
		return _projector->getOrthoWindowWidth();
	}

	return 100.0f;
}
//--------------------------------------------------------------------------------
void GameProjector::SetOrthoWindowWidth( float val )
{
	if(_projector)
	{
		_projector->setOrthoWindowWidth(val);
	}
}
//--------------------------------------------------------------------------------
bool GameProjector::GetPerspective() const
{
	if(_projector)
	{
		return _projector->getProjectionType() == PT_PERSPECTIVE;
	}

	return false;
}
//--------------------------------------------------------------------------------
void GameProjector::SetPerspective( bool on )
{
	if(_projector)
	{
		_projector->setProjectionType(on ? PT_PERSPECTIVE : PT_ORTHOGRAPHIC);
	}
}
//--------------------------------------------------------------------------------
float GameProjector::GetFovDegree() const
{
	if(_projector)
	{
		return _projector->getFOVy().valueDegrees();
	}

	return 90.0f;
}
//--------------------------------------------------------------------------------
void GameProjector::SetFovDegree( float val )
{
	if(_projector)
	{
		_projector->setFOVy(Radian(Degree(val)));
	}
}
//--------------------------------------------------------------------------------