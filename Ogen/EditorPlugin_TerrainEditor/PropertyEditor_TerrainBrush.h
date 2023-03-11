#pragma once

class PropertyEditor_TerrainBrush : public PropertyEditorBase
{
	friend class PropertyGridEx;

	DECLARE_DYNAMIC(PropertyEditor_TerrainBrush)

	// Construction
public:
	PropertyEditor_TerrainBrush(IPropertyPtr prop);
	virtual ~PropertyEditor_TerrainBrush();
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual void OnDrawName(CDC* pDC, CRect rect);

protected:

	virtual void AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin);
	virtual void ResetOriginalValue();
};

class PropertyEditor_TerrainBrushFactory : public TTypeUIEditorFactor<PropertyEditor_TerrainBrush>
{
};


