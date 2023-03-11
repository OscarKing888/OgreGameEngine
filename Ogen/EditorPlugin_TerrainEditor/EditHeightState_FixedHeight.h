#include "EditState_TerrainHeight.h"

class EditHeightState_FixedHeight : public AbstractEditHeightState
{
public:

	EditHeightState_FixedHeight(IStateManager* pMgr);
	virtual ~EditHeightState_FixedHeight();

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	void PaintHeight( Terrain* terrain, const Vector3& position, float brushRadius, ITerrainBrush* brush, float fixHeight );
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
};