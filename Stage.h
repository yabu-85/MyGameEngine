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

    struct CellPos {
        int x;
        int y;
        int z;
        CellPos() : x(0), y(0), z(0) {};
    };

    CellPos start_;
    CellPos end_;
    bool isPressedR_;
    bool isPressedL_;
    int presseTime_;

    void SetBlockType(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    CellPos RayCastStage();
    
    BOOL Write(int data, HANDLE hFile, DWORD dwBytes);
    BOOL Read(int& data, HANDLE hFile, DWORD& dwBytes);
    
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    void Save();
    void Load();

};

