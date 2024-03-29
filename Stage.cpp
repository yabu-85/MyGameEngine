#include "Stage.h"
#include "Engine/Model.h"
#include <string>
#include "Engine/Direct3D.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "resource.h"
#include <iostream>

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), type_(1), model_(0), isPressedL_(false), isPressedR_(false), presseTime_(0)
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
    



    if (isPressedL_) {
        presseTime_++;
        if (presseTime_ % 10 == 1) {
            start_ = RayCastStage();
            
            if (type_ == 0) {
                //種類を変える
                table_[start_.x][start_.z].type_ = (BLOCKTYPE)model_;
            }
            else {
                //高さを変える
                table_[start_.x][start_.z].height_ += type_;
            }
        }
    }

    if (Input::IsMouseButtonDown(0)) isPressedL_ = true;
    if (Input::IsMouseButtonUp(0)) isPressedL_ = false;

    if (isPressedR_ && Input::IsMouseButtonDown(0)) isPressedR_ = false;

    if (Input::IsMouseButtonDown(1) && !Input::IsKey(DIK_E)) {
        start_ = RayCastStage();
        isPressedL_ = false;
        isPressedR_ = true;
    }
    if (isPressedR_ && Input::IsMouseButtonUp(1)) {
        end_ = RayCastStage();
        isPressedR_ = false;

        int s[2] = { start_.x, start_.z }; // 0 = x, 1 = z
        int e[2] = { end_.x, end_.z };
        if (start_.x > end_.x) {
            s[0] = end_.x; e[0] = start_.x;
        }
        if (start_.z > end_.z) {
            s[1] = end_.z; e[1] = start_.z;
        }

        if (type_ == 0) {
            for (int z = s[1]; z < e[1]; z++) {
                for (int x = s[0]; x < e[0]; x++) {
                    for (int y = 0; y < table_[z][x].height_ + 1; y++) {
                        table_[x][z].type_ = (BLOCKTYPE)type_;
                    }
                }
            }
        }
        else {
            for (int z = s[1]; z < e[1]; z++) {
                for (int x = s[0]; x < e[0]; x++) {
                    table_[x][z].height_ += type_;
                }
            }
        }

    }

}

void Stage::Draw()
{
    Transform blockTrans;

    //一つのStageオブジェクトで個数分表示させる
    for (int z = ZSIZE - 1; z >= 0; z--) {
        for (int x = 0; x < XSIZE; x++) {
            for (int y = 0; y < table_[z][x].height_ + 1; y++)
            {
                //table[x][z]からオブジェクトのタイプを取り出して書く！
                int type = table_[z][x].type_;
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

Stage::CellPos Stage::RayCastStage()
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

    //2: �@にinvVP、invPrj、invViewをかける
    vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);

    //3: mousePosBackをベクトルに変換
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);

    //4 �BにinvVP、invPrj、invViewをかける
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    const int maxLength = 999999;
    int hitPos[2] = { -1, -1 };
    float max = maxLength;

    for (int z = ZSIZE - 1; z >= 0; z--) {
        for (int x = 0; x < XSIZE; x++) {
            for (int y = 0; y < table_[z][x].height_ + 1; y++) {

                //5: �Aから�Cに向かってレイを打つ（とりあえずモデル番号は0で
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
                        hitPos[0] = z;
                        hitPos[1] = x;
                    }

                    break;
                }
            }
        }
    }

    if (!(max < maxLength)) return CellPos();

    CellPos cell;
    cell.x = hitPos[0];
    cell.z = hitPos[1];
    return cell;

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

void Stage::Save()
{
    //--------------ファイル保存--------------

    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        "map.txt",             //ファイル名
        GENERIC_WRITE,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        CREATE_ALWAYS,           //作成方法   CREATE_ALWAYS(上書き）
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）

    //失敗したとき
    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    int dataType[XSIZE][ZSIZE];
    int dataHeight[XSIZE][ZSIZE];
    for (int y = ZSIZE -1; y >= 0; y--) {
        for (int x = 0; x < XSIZE; x++) {
            dataType[y][x] = table_[y][x].type_;
            dataHeight[y][x] = table_[y][x].height_;
        }
    }

    DWORD dwBytes = 0;  //書き込み位置
    for (int y = ZSIZE - 1; y >= 0; y--) {
        for (int x = 0; x < XSIZE; x++) {
            if (!Write(dataType[y][x], hFile, dwBytes)) return;
        }
    }

    for (int y = ZSIZE - 1; y >= 0; y--) {
        for (int x = 0; x < XSIZE; x++) {
            if (!Write(dataHeight[y][x], hFile, dwBytes)) return;
        }
    }

    CloseHandle(hFile);

}

void Stage::Load()
{
    //--------------ファイル開く--------------
    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        "map.txt",             //ファイル名
        GENERIC_READ,           //アクセスモード
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        OPEN_EXISTING,           //作成方法   CREATE_ALWAYS(上書き）
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）

    //失敗したとき
    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    // 読み取るデータを格納するための変数
    OutputDebugString("\n");

    int** dataType = new int* [ZSIZE];
    int** dataHeight = new int* [ZSIZE];
    for (int y = ZSIZE - 1; y >= 0; y--) {
        dataType[y] = new int[XSIZE];
        dataHeight[y] = new int[XSIZE];

        //データ入れちゃう
        for (int x = 0; x < XSIZE; x++) {
            dataType[y][x] = table_[y][x].type_;
            dataHeight[y][x] = table_[y][x].height_;
        }
    }

    DWORD dwBytes = 0;  //書き込み位置

    //Type
    for (int y = ZSIZE - 1; y >= 0; y--) {
        if (!Read(*dataType[y], hFile, dwBytes)) return;

        for (int x = 0; x < XSIZE; x++) {
            table_[y][x].type_ = (BLOCKTYPE)dataType[y][x];
        }
    }

    //Height
    for (int y = ZSIZE - 1; y >= 0; y--) {
        if (!Read(*dataHeight[y], hFile, dwBytes)) return;

        for (int x = 0; x < XSIZE; x++) {
            table_[y][x].height_ = dataHeight[y][x];
        }
    }

    CloseHandle(hFile);
}

BOOL Stage::Write(int data, HANDLE hFile, DWORD dwBytes)
{
    BOOL res = WriteFile(
        hFile,                   //ファイルハンドル
        &data,                   //保存するデータ（文字列）
        sizeof(data),            //書き込む文字数
        &dwBytes,                //書き込んだサイズを入れる変数
        NULL);                   //オーバーラップド構造体（今回は使わない）

    if (res == 0) {
        std::wcout << L"ファイル書き込みに失敗" << GetLastError() << std::endl;
        return false;
    }

    return true;
}

BOOL Stage::Read(int& data, HANDLE hFile, DWORD& dwBytes)
{
    BOOL res = ReadFile(
        hFile,
        &data,
        sizeof(int[XSIZE]),
        &dwBytes,
        NULL);

    if (res == 0) {
        std::wcout << L"ファイル読み取りに失敗" << GetLastError() << std::endl;
        return false;
    }

    return true;
}
