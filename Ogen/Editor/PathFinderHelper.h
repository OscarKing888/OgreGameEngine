#pragma once
#include "Engine/DynamicRenderable.h"

class GridUintRenderable : public DynamicRenderable
{
public:

	GridUintRenderable(float unitScale);
	virtual ~GridUintRenderable();
	virtual void createVertexDeclaration();
	virtual void fillHardwareBuffers();

	void setPosition(int x, int y);

	virtual bool preRender(SceneManager* sm, RenderSystem* rsys);

	void update();

protected:

	int _x;
	int _y;
	float _unitScale;
	bool mDirty;
};