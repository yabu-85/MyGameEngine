#pragma once
#include "Engine/GameObject.h"

class Controller :
    public GameObject
{
    XMFLOAT3 cameraPos_;
    XMFLOAT3 cameraTar_;

public:
    Controller(GameObject* parent);
    ~Controller();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

