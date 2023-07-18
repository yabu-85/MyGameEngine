#pragma once
#include "GameObject.h"

class RootJob :
    public GameObject
{
public:
    RootJob(GameObject* parent); //一番上はnullptrにする、じゃないとどこが終わりかわからないmainでやってる
    ~RootJob();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

