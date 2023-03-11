#include "StdAfx.h"
#include "CinemaTrack_Camera.h"

const String CinemaTrack_CameraFactory::FactoryName = "Camera";

//--------------------------------------------------------------------------------
// CinemaTrack_Camera
//--------------------------------------------------------------------------------
CinemaTrack_Camera::CinemaTrack_Camera(Cinema* parent, IGameObject* object)
: CinemaTrack_Movement(parent, object)
{
	_needTarget =  false;
}
//--------------------------------------------------------------------------------
CinemaTrack_Camera::~CinemaTrack_Camera(void)
{
}
//--------------------------------------------------------------------------------
const String& CinemaTrack_Camera::GetTypeName() const
{
	return CinemaTrack_CameraFactory::FactoryName;
}
//--------------------------------------------------------------------------------
void CinemaTrack_Camera::BuildPropertyMap()
{
	CinemaTrack_Movement::BuildPropertyMap();

	//RemoveProperty("TargetObjct");
}
//--------------------------------------------------------------------------------
void CinemaTrack_Camera::OnTransform( const Vector3& pos, const Vector3& scale, const Quaternion& rot )
{
	Ogre::Camera* cam = CinemaManager::getSingleton().GetCamera();
	cam->setPosition(pos);
	cam->setOrientation(rot);
}
//--------------------------------------------------------------------------------
void CinemaTrack_Camera::AutoUpdateKeyFrame( CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Transform* tkf = static_cast<CinemaKeyFrame_Transform*>(kf);
	Ogre::Camera* cam = CinemaManager::getSingleton().GetCamera();
	if(cam)
	{
		tkf->SetTranslate(cam->getPosition());
		tkf->SetOrientation(cam->getOrientation());
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"Camer not ready for camera transform track!", __FUNCTION__);
	}
}
//-----------------------------------------------------------------------------
String CinemaTrack_Camera::ToString() const
{
	String cap = formatString("%s [%s]", GetTypeName().c_str(), _comment.c_str());
	return cap;
}