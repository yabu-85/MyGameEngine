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
                //��ނ�ς���
                table_[start_.x][start_.z].type_ = (BLOCKTYPE)model_;
            }
            else {
                //������ς���
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

    //���Stage�I�u�W�F�N�g�Ō����\��������
    for (int z = ZSIZE - 1; z >= 0; z--) {
        for (int x = 0; x < XSIZE; x++) {
            for (int y = 0; y < table_[z][x].height_ + 1; y++)
            {
                //table[x][z]����I�u�W�F�N�g�̃^�C�v�����o���ď����I
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

    //Offsetx,y �͂O
    //minZ = 0, maxZ = 1

    XMMATRIX vp = {
        w, 0, 0, 0,
        0, -h, 0, 0,
        0, 0, 1, 0,
        w, h, 0, 1
    };

    //�r���[�|�[�g
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);

    //�v���W�F�N�V�����ϊ�
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());

    //�r���[�ϊ�
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

    XMFLOAT3 mousePosFront = Input::GetMousePosition(); //�}�E�X�ۂ�������
    mousePosFront.z = 0.0f;

    XMFLOAT3 mousePosBack = Input::GetMousePosition(); //���ɂ�
    mousePosBack.z = 1.0f;

    //1: mousePosFront���x�N�g���ɕϊ�
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);

    //2: �@��invVP�AinvPrj�AinvView��������
    vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);

    //3: mousePosBack���x�N�g���ɕϊ�
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);

    //4 �B��invVP�AinvPrj�AinvView��������
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    const int maxLength = 999999;
    int hitPos[2] = { -1, -1 };
    float max = maxLength;

    for (int z = ZSIZE - 1; z >= 0; z--) {
        for (int x = 0; x < XSIZE; x++) {
            for (int y = 0; y < table_[z][x].height_ + 1; y++) {

                //5: �A����C�Ɍ������ă��C��łi�Ƃ肠�������f���ԍ���0��
                //6: ���C������������u���C�N�|�C���g�Ŏ~�߂�
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
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"����");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"���n");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_SETCURSEL, 0, 0);
        return TRUE;

    case WM_COMMAND:
        // �R���{�{�b�N�X�̑I�����ύX���ꂽ�Ƃ�
        if (HIWORD(wp) == CBN_SELCHANGE && LOWORD(wp) == IDC_COMBO)
        {
            type_ = 0;
            model_ = SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_GETCURSEL, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_CHANGE), BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_UNCHECKED, 0);
            return TRUE;
        }

        //���W�I�{�^���I��
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
    //--------------�t�@�C���ۑ�--------------

    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        "map.txt",             //�t�@�C����
        GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        CREATE_ALWAYS,           //�쐬���@   CREATE_ALWAYS(�㏑���j
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j

    //���s�����Ƃ�
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

    DWORD dwBytes = 0;  //�������݈ʒu
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
    //--------------�t�@�C���J��--------------
    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        "map.txt",             //�t�@�C����
        GENERIC_READ,           //�A�N�Z�X���[�h
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,           //�쐬���@   CREATE_ALWAYS(�㏑���j
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j

    //���s�����Ƃ�
    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    // �ǂݎ��f�[�^���i�[���邽�߂̕ϐ�
    OutputDebugString("\n");

    int** dataType = new int* [ZSIZE];
    int** dataHeight = new int* [ZSIZE];
    for (int y = ZSIZE - 1; y >= 0; y--) {
        dataType[y] = new int[XSIZE];
        dataHeight[y] = new int[XSIZE];

        //�f�[�^���ꂿ�Ⴄ
        for (int x = 0; x < XSIZE; x++) {
            dataType[y][x] = table_[y][x].type_;
            dataHeight[y][x] = table_[y][x].height_;
        }
    }

    DWORD dwBytes = 0;  //�������݈ʒu

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
        hFile,                   //�t�@�C���n���h��
        &data,                   //�ۑ�����f�[�^�i������j
        sizeof(data),            //�������ޕ�����
        &dwBytes,                //�������񂾃T�C�Y������ϐ�
        NULL);                   //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    if (res == 0) {
        std::wcout << L"�t�@�C���������݂Ɏ��s" << GetLastError() << std::endl;
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
        std::wcout << L"�t�@�C���ǂݎ��Ɏ��s" << GetLastError() << std::endl;
        return false;
    }

    return true;
}
