#pragma once
#include "Engine/GameObject.h"

class Fbx;

class MiniChara : public GameObject
{
    Fbx* pFbx_;

public:
    MiniChara(GameObject* parent);
    ~MiniChara();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

