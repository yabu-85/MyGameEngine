#pragma once
#include "Engine/GameObject.h"

class MiniChara : public GameObject
{
    int hModel_;

public:
    MiniChara(GameObject* parent);
    ~MiniChara();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

