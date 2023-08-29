#include "Stage.h"
#include "Engine/Model.h"
#include <string>

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++)
        hModel_[i] = -1;

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            table_[x][z] = 0;
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
            table_[x][y] = (int)(x + y / 3);
        }
    }

}

void Stage::Update()
{
}

void Stage::Draw()
{
    Transform blockTrans;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < XSIZE; x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            blockTrans.position_.x = (float)x;
            blockTrans.position_.z = (float)z;

            int type = table_[x][z];

            //モデル描画
            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);
        }
    }

}

void Stage::Release()
{
}
