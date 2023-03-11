#include "stdafx.h"
#include "CEGUIAnimation.h"

namespace Animations {

FrameAnimation_CEGUI::FrameAnimation_CEGUI(CEGUI::Window* imageHandle, const String& frameAniConfigName)
	: FrameAnimation(frameAniConfigName)
	, _imageHandle(imageHandle)
{
}

FrameAnimation_CEGUI::FrameAnimation_CEGUI(CEGUI::Window* imageHandle, const String& frameAniConfigName,
	const SAnimationInfo& aniInfo)
	: FrameAnimation(frameAniConfigName, aniInfo)
	, _imageHandle(imageHandle)
{
}

FrameAnimation_CEGUI::~FrameAnimation_CEGUI()
{
}

void FrameAnimation_CEGUI::UpdateImage()
{
	assert(_imageHandle);

	if(_isOnceDead)
	{
		assert(_imageHandle);
		if (!_isEndShow)
		{
			_imageHandle->setProperty(("Image"), (""));
		}
		GameApp::getSingleton().RemoveUpdateable(this);
	}
	else
	{
		_imageHandle->setProperty(("Image"), _aniInfo.GetFrameImageName(_currentFrame).c_str());
	}
}

} // namespace
