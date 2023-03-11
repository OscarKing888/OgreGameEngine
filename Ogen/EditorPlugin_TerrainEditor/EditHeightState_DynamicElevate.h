#include "EditState_TerrainHeight.h"

class EditHeightState_DynamicElevate : public AbstractEditHeightState
{
public:

	EditHeightState_DynamicElevate(IStateManager* pMgr);
	virtual ~EditHeightState_DynamicElevate();

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	void PaintHeight( Terrain* terrain, const Vector3& position, float brushRadius,
		float strength, ITerrainBrush* brush, bool useSubtract );

	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);

};