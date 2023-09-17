#pragma once
#include "Engine/Fbx.h"

template <class EBI> class PixelConstantBuffer {};

class PointLight
{
public:
	PointLight(Fbx& mesh, float radius = 0.5f);
	PointLight() : pos{ 0,0,0,0 } {};

	void Reset() noexcept;
	void Draw(Fbx& gfx) noexcept; //noexcept(!IS_DEBUG);
	void Bind(Fbx& gfx) noexcept;

	void Update();

private:
	struct PointLightCBuf
	{
		XMFLOAT4 pos;
		float padding;
	};

	XMFLOAT4 pos;

	mutable Fbx mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;

};