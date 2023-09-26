#include "PointLight.h"
#include "Engine/Input.h"

namespace PointLight {
	std::vector <PointLightData*> vec;
	int pointLightNum_;
}
	
void PointLight::PointInitialize()
{

}

void PointLight::SetPosition(int handle, XMFLOAT3 _pos)
{
	vec[handle]->pos_ = _pos;
}
	
void PointLight::Reset(int handle)
{
	vec[handle]->pos_ = { 0.0f, 0.0f, 0.0f };
}
	
void PointLight::Update(int handle)
{
}

	


