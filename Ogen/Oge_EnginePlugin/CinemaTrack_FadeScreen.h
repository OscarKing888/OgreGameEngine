#pragma once
#include "ICinemaTrack.h"

class _OgeEnginePluginExport CinemaTrack_FadeScreen
	: public AbstractCinemaTrack
	, public RenderTargetListener
{
public:

	CinemaTrack_FadeScreen(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_FadeScreen(void);

	virtual const String& GetTypeName() const;

	virtual void SetTimePosition(float timePos);

	virtual void BuildPropertyMap();

	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf);

protected:

	Ogre::Rectangle2D _screenRect;
	Ogre::MaterialPtr _material;
	virtual CinemaKeyFrame* CreateKeyFrameImpl(float timePos);

	virtual void postRenderTargetUpdate(const RenderTargetEvent& evt);
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_FadeScreenFactory : public TCinemaTrackFactory<CinemaTrack_FadeScreen>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};