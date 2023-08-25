#include "Stage.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Assets/BoxDefault.fbx");
	assert(hModel_ >= 0);
}

void Stage::Update()
{
}

void Stage::Draw()
{
    Transform blockTrans;
    int width_ = 15;
    int height_ = 15;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < width_; x++)
    {
        for (int z = 0; z < height_; z++)
        {
            blockTrans.position_.x = (float)x;
            blockTrans.position_.z = (float)z;

            //モデル描画
            Model::SetTransform(hModel_, blockTrans);
            Model::Draw(hModel_);
        }
    }

}

void Stage::Release()
{
}
