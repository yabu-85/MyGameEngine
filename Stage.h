#pragma once
#include "Engine/GameObject.h"
#include <Windows.h>

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
        TYPEMAX,
    };
}

class Stage :
    public GameObject
{
    int hModel_[MODEL_NUM];    //ÉÇÉfÉãî‘çÜ
    int type_;
    int model_;

    struct Cell{
        BLOCKTYPE type_;
        int height_;
    }table_[XSIZE][ZSIZE];

    void SetBlockType(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    void RayCastStage();
    
//    BOOL Write(int** data, HANDLE hFile, DWORD dwBytes);
//    BOOL Read(int& data, HANDLE hFile, DWORD& dwBytes);
    BOOL Read(Cell& data, HANDLE hFile, DWORD& dwBytes);

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    void Save();

};

