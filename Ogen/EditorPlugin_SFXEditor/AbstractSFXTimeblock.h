#pragma once

class AbstractSFXTimeblock : public AbstractTimeblock
{
public:
	AbstractSFXTimeblock();
	virtual ~AbstractSFXTimeblock();

	IEffectsElement* GetElement() const { return _element; }
	void SetElement(IEffectsElement* val);

	void UpdateUI();
protected:

	IEffectsElement* _element;
};

SmartPointer(AbstractSFXTimeblock);