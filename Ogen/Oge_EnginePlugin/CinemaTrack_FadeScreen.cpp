#include "StdAfx.h"
#include "CinemaTrack_FadeScreen.h"
#include "Cinema.h"
#include "CinemaKeyFrame_Colour.h"

const String CinemaTrack_FadeScreenFactory::FactoryName = "FadeScreen";
//-----------------------------------------------------------------------------
// CinemaTrack_FadeScreen
//-----------------------------------------------------------------------------
CinemaTrack_FadeScreen::CinemaTrack_FadeScreen(Cinema* parent, IGameObject* object)
: AbstractCinemaTrack(parent, object)
, _screenRect(false)
{
	_needTarget =  false;

	_screenRect.setCorners(-1, 1, 1, -1);
	static int autoID = 0;
	String matName = formatString("__[%s]FadeScreen[%d]__",
		_parent->GetName(), autoID);
	++autoID;

	MaterialPtr mat = MaterialPtr(MaterialManager::getSingleton().getByName("Cinema/ScreenFade"));
	_material = mat->clone(matName);
	_material->setDiffuse(ColourValue(0, 0, 0));

	_screenRect.setMaterial(matName);
	_screenRect.setRenderQueueGroup(RENDER_QUEUE_MAX);
	_screenRect.setBoundingBox(AxisAlignedBox::BOX_INFINITE);
	_screenRect.setQueryFlags(0);

	RenderTarget* t = CinemaManager::getSingleton().GetCamera()->getViewport()->getTarget();
	t->addListener(this);
}
//-----------------------------------------------------------------------------
CinemaTrack_FadeScreen::~CinemaTrack_FadeScreen(void)
{
	RenderTarget* t = CinemaManager::getSingleton().GetCamera()->getViewport()->getTarget();
	t->removeListener(this);
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_FadeScreen::GetTypeName() const
{
	return CinemaTrack_FadeScreenFactory::FactoryName;
}
//-----------------------------------------------------------------------------
CinemaKeyFrame* CinemaTrack_FadeScreen::CreateKeyFrameImpl( float timePos )
{
	return OGRE_NEW CinemaKeyFrame_Colour(this, timePos);
}
//-----------------------------------------------------------------------------
void CinemaTrack_FadeScreen::SetTimePosition( float timePos )
{
	// Nothing to do if no keyframes or zero weight or no node
	if (_keyframes.empty())
		return;

// 	Real scl = 1.0f;
 	TimeIndex timeIndex(timePos);
 
 	CinemaKeyFrame_Colour kf(0, timeIndex.getTimePos());

	// Keyframe pointers
	CinemaKeyFrame *kBase1, *kBase2;
	CinemaKeyFrame_Colour *k1, *k2;
	unsigned short firstKeyIndex;

	Real t = GetKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
	k1 = static_cast<CinemaKeyFrame_Colour*>(kBase1);
	k2 = static_cast<CinemaKeyFrame_Colour*>(kBase2);

	if (t == 0.0)
	{
		// Just use k1
		ColourValue clr = k1->GetColor();
		kf.SetColor(clr);
	}
	else
	{
		ColourValue base = k1->GetColor();
		kf.SetColor( base + ((k2->GetColor() - base) * t) );
	}

	_material->setDiffuse(kf.GetColor());
}
//--------------------------------------------------------------------------------
void CinemaTrack_FadeScreen::BuildPropertyMap()
{
	AbstractCinemaTrack::BuildPropertyMap();
	//RemoveProperty("TargetObjct");
}
//--------------------------------------------------------------------------------
void CinemaTrack_FadeScreen::AutoUpdateKeyFrame( CinemaKeyFrame* kf )
{
}
//--------------------------------------------------------------------------------
void CinemaTrack_FadeScreen::postRenderTargetUpdate(const RenderTargetEvent& evt)
{
	if(_enable)
	{
		SceneManager* sm = CinemaManager::getSingleton().GetCamera()->getSceneManager();
		Pass* pass = _material->getBestTechnique()->getPass(0);
		sm->_injectRenderWithPass(pass, &_screenRect);
	}
}