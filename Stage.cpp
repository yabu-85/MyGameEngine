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
    //Offsetx,y �͂O
    //minZ = 0, maxZ = 1

    XMMATRIX vp = {
        w, 0, 0, 0,
        0, -w, 0, 0,
        0, 0, 1, 0,
        w, h, 0, 1
    };

    //�r���[�|�[�g
    XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
    
    //�v���W�F�N�V�����ϊ�
    XMMATRIX invPrj = {};

    //�r���[�ϊ�
    XMMATRIX invView = {};
    
    XMFLOAT3 mousePosFront = Input::GetMousePosition(); //�}�E�X�ۂ�������
    mousePosFront.z = 0.0f;

    XMFLOAT3 mousePosBack = {}; //���ɂ�
    mousePosBack.z = 1.0f;
    
    //1: mousePosFront���x�N�g���ɕϊ�
    //2: �@��invVP�AinvPrj�AinvView��������
    //3: mousePosBack���x�N�g���ɕϊ�
    //4 �B��invVP�AinvPrj�AinvView��������
    //5: �A����C�Ɍ������ă��C��łi�Ƃ肠�������f���ԍ���0��
    //6: ���C������������u���C�N�|�C���g�Ŏ~�߂�

}

void Stage::Draw()
{
    Transform blockTrans;

    //���Stage�I�u�W�F�N�g�Ō����\��������
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

                //���f���`��
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
