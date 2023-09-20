#pragma once
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

namespace PointLight
{
	struct PointLightData {
		XMFLOAT3 pos_;
		XMFLOAT4 color_;
		XMFLOAT3 leng_;
	};
	extern int pointLightNum_;
	extern std::vector<PointLightData*> vec;

	void PointInitialize();
	void SetPosition(int handle, XMFLOAT3 _pos);
	void Reset(int handle);
	void Update(int handle);

};