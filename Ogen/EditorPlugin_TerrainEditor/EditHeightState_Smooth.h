#include "EditState_TerrainHeight.h"

class EditHeightState_Smooth : public AbstractEditHeightState
{
public:

	EditHeightState_Smooth(IStateManager* pMgr);
	virtual ~EditHeightState_Smooth();

	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
};