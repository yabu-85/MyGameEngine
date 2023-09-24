#include "Stage.h"
#include "Engine/Model.h"
#include <string>
#include "Engine/Direct3D.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "resource.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), type_(1), model_(0), activeTimer_(0)
{
    for (int i = 0; i < MODEL_NUM; i++)
        hModel_[i] = -1;

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            table_[x][z] = { DEFAULT, 0 };
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
        }
    }

}

void Stage::Update()
{

    activeTimer_--;
    if (activeTimer_ > 0) return;

    if (Input::IsMouseButtonDown(0)) {
        RayCastStage();
    }

}

void Stage::Draw()
{
    Transform blockTrans;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < XSIZE; x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            for (int y = 0; y < table_[x][z].height_ + 1; y++)
            {
                //table[x][z]からオブジェクトのタイプを取り出して書く！
                int type = table_[x][z].type_;
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[type], trans);
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

//--------------private

void Stage::RayCastStage()
{
    float w = (float)Direct3D::scrWidth / 2.0f;
    float h = (float)Direct3D::scrHeight / 2.0f;

    //Offsetx,y は０
    //minZ = 0, maxZ = 1

    XMMATRIX vp = {
        w, 0, 0, 0,
        0, -h, 0, 0,
        0, 0, 1, 0,
        w, h, 0, 1
    };

    //ビューポート
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);

    //プロジェクション変換
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());

    //ビュー変換
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

    XMFLOAT3 mousePosFront = Input::GetMousePosition(); //マウスぽじげっと
    mousePosFront.z = 0.0f;

    XMFLOAT3 mousePosBack = Input::GetMousePosition(); //ごにょ
    mousePosBack.z = 1.0f;

    //1: mousePosFrontをベクトルに変換
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);

    //2: ①にinvVP、invPrj、invViewをかける
    vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);

    //3: mousePosBackをベクトルに変換
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);

    //4 ③にinvVP、invPrj、invViewをかける
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    const int maxLength = 999999;
    int hitPos[2] = { -1, -1 };
    float max = maxLength;

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            for (int y = 0; y < table_[x][z].height_ + 1; y++) {

                //5: ②から④に向かってレイを打つ（とりあえずモデル番号は0で
                //6: レイが当たったらブレイクポイントで止める
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);

                Transform trans;
                trans.position_ = { (float)x, (float)y, (float)z };
                Model::SetTransform(hModel_[0], trans);
                Model::RayCast(hModel_[0], data);

                if (data.hit) {

                    XMVECTOR pos = XMVectorSet(x, y, z, 0);
                    float distance = XMVectorGetX(XMVector3Length(pos - vMouseFront));

                    if (distance < max) {
                        max = distance;
                        hitPos[0] = x;
                        hitPos[1] = z;
                    }

                    break;
                }
            }
        }
    }

    if (!(max < maxLength)) return;

    //種類を変える
    if (type_ == 0) { 
        table_[hitPos[0]][hitPos[1]].type_ = (BLOCKTYPE)model_;
        return;
    }

    //高さを変える
    table_[hitPos[0]][hitPos[1]].height_+= type_;

}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"草原");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"砂地");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"水");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_SETCURSEL, 0, 0);
        return TRUE;

    case WM_COMMAND:
        activeTimer_ = 10;

        // コンボボックスの選択が変更されたとき
        if (HIWORD(wp) == CBN_SELCHANGE && LOWORD(wp) == IDC_COMBO)
        {
            type_ = 0;
            model_ = SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_GETCURSEL, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_CHANGE), BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_UNCHECKED, 0);
            return TRUE;
        }

        //ラジオボタン選択
        if (!HIWORD(wp) == BN_CLICKED) return FALSE;
        if (LOWORD(wp) == IDC_RADIO_UP) {
            type_ = 1;
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else if (LOWORD(wp) == IDC_RADIO_DOWN)
        {
            type_ = -1;
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else if (LOWORD(wp) == IDC_RADIO_CHANGE)
        {
            type_ = 0;
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_CHANGE), BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_UNCHECKED, 0);
        }

        return TRUE;
    }

    return FALSE;
}