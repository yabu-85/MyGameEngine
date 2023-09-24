#pragma once
#include "Engine/GameObject.h"
#include <Windows.h>

namespace {
    const int MODEL_NUM{ 5 };
    const int XSIZE{ 30 };
    const int ZSIZE{ 30 };
    const int YSIZE{ 15 }; //これMAXの値

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
    int hModel_[MODEL_NUM];    //モデル番号
    int type_;
    int model_;

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

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

};

