#include "Stage.h"
#include "Engine/Model.h"
#include <string>
#include "Engine/Direct3D.h"
#include "Engine/Input.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++)
        hModel_[i] = -1;

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            table_[x][z] = { DEFAULT, 1 };
        }
    }


}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    string modelName[] = {
        "BoxDefault.fbx",
        "BoxBrick.fbx",
        "BoxGrass.fbx",
        "BoxSand.fbx",
        "BoxWater.fbx"
    };

    string fnameBase = "Assets/";
    for (int i = 0; i < 5; i++) {
        hModel_[i] = Model::Load(fnameBase + modelName[i]);
        assert(hModel_[i] >= 0);
    }

    for (int y = 0; y < ZSIZE; y++) {
        for (int x = 0; x < XSIZE; x++) {
            SetBlockType(x, y, (BLOCKTYPE)(rand() % (int)TYPEMAX));
            SetBlockHeight(x, y, rand() % 3 -1);
        }
    }

}

void Stage::Update()
{
    float w = (float)(Direct3D::scrWidht) / 2;
    float h = (float)(Direct3D::scrHeight) / 2;
    //Offsetx,y は０
    //minZ = 0, maxZ = 1

    XMMATRIX vp = {
        w, 0, 0, 0,
        0, -w, 0, 0,
        0, 0, 1, 0,
        w, h, 0, 1
    };

    //ビューポート
    XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
    
    //プロジェクション変換
    XMMATRIX invPrj = {};

    //ビュー変換
    XMMATRIX invView = {};
    
    XMFLOAT3 mousePosFront = Input::GetMousePosition(); //マウスぽじげっと
    mousePosFront.z = 0.0f;

    XMFLOAT3 mousePosBack = {}; //ごにょ
    mousePosBack.z = 1.0f;
    
    //1: mousePosFrontをベクトルに変換
    //2: ①にinvVP、invPrj、invViewをかける
    //3: mousePosBackをベクトルに変換
    //4 ③にinvVP、invPrj、invViewをかける
    //5: ②から④に向かってレイを打つ（とりあえずモデル番号は0で
    //6: レイが当たったらブレイクポイントで止める

}

void Stage::Draw()
{
    Transform blockTrans;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < XSIZE; x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            for (int y = 0; y < table_[x][z].height_ ; y++)
            {
                blockTrans.position_.x = (float)x;
                blockTrans.position_.y = (float)y;
                blockTrans.position_.z = (float)z;

                int type = table_[x][z].type_;

                //モデル描画
                Model::SetTransform(hModel_[type], blockTrans);
                Model::Draw(hModel_[type]);

            }
        }
    }
}

void Stage::Release()
{
}

void Stage::SetBlockType(int _x, int _z, BLOCKTYPE _type)
{
    table_[_x][_z].type_ = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    table_[_x][_z].height_ = _height;
}
