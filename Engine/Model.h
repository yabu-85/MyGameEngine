#pragma once
#include "Fbx.h"
#include "GameObject.h"

namespace Model {
	struct ModelData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string filename_;

	};

	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel, int type = 0);
	void Release();

	void RayCast(int hModel, RayCastData& rayData);
	void SetLightPosition(int hModel, XMFLOAT3 pos);

}
