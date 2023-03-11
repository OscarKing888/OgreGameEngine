#pragma once
#include "UIControllerPrerequisites.h"

namespace Animations {

class _OgeCEGUIExports FrameAnimation_CEGUI
	: public FrameAnimation
{
public:

	explicit FrameAnimation_CEGUI(CEGUI::Window* imageHandle,
		const String& frameAniConfigName);

	explicit FrameAnimation_CEGUI(CEGUI::Window* imageHandle,
		const String& frameAniConfigName, const SAnimationInfo& aniInfo);

	virtual ~FrameAnimation_CEGUI(void);
	CEGUI::Window*			GetImageWindow(void){ return _imageHandle; }

protected:

	CEGUI::Window*		_imageHandle;

	virtual void				UpdateImage();
};

SmartPointer(FrameAnimation_CEGUI);

} // namespace