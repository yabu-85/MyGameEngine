#pragma once
#include "Engine/GameObject.h"

namespace {
    const int MODEL_NUM{ 5 };
    const int XSIZE{ 15 };
    const int ZSIZE{ 15 };
    enum BLOCKTYPE {
        DEFAULT = 0,
        BRICK,
        GRASS,
        SAND,
        WATER,
    };
}

class Stage :
    public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�
    int table_[XSIZE][ZSIZE];      //���f���̃e�[�u��

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

