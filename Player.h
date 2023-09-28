#pragma once
#include "Engine/GameObject.h"

class Player : public GameObject
{
    int hModel_;
    int hPict_[10];

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

