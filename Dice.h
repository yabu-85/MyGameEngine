#pragma once
#include "Quad.h"

class Dice :
    public Quad
{
public:
	Dice();
	~Dice() override;

	HRESULT InitializeVertex() override;
	HRESULT InitializeIndex() override;
	void DrawIndex(XMMATRIX& worldMatrix) override;

};

