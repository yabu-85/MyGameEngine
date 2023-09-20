#pragma once
#include "Engine/GameObject.h"

namespace {
    const int MODEL_NUM{ 5 };
    const int XSIZE{ 30 };
    const int ZSIZE{ 30 };
    const int YSIZE{ 15 }; //Ç±ÇÍMAXÇÃíl

    enum BLOCKTYPE {
        DEFAULT = 0,
        BRICK,
        GRASS,
        SAND,
        WATER,
        TYPEMAX,
    };
}

class Stage :
    public GameObject
{
    int hModel_[MODEL_NUM];    //ÉÇÉfÉãî‘çÜ
    
    struct {
        BLOCKTYPE type_;
        int height_;
    }table_[XSIZE][ZSIZE];

    void SetBlockType(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    void RayCastStage();

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

