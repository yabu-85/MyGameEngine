#include "PointLight.h"
#include "Engine/Sprite.h"
#include "Engine/Input.h"

PointLight::PointLight(Fbx& mesh, float radius)
	:pos(0.0f,0.0f,0.0f,0.0f)
{
}

void PointLight::Reset() noexcept
{
	pos = { 0.0f, 0.0f, 0.0f, 0.0f };
}

void PointLight::Draw(Fbx& gfx) noexcept
{
	pos = { 5.0f, 3.0f, 5.0f, 0.0f };
}

void PointLight::Bind(Fbx& gfx) noexcept
{
}

void PointLight::Update()
{
	if (Input::IsKey(DIK_W))
	{
		pos.x += 0.2;
	}
	if (Input::IsKey(DIK_S))
	{
		pos.x += 0.2;
	}
	if (Input::IsKey(DIK_A))
	{
		pos.x += 0.2;
	}
	if (Input::IsKey(DIK_D))
	{
		pos.x += 0.2;
	}

}
