#include "Player.h"
#include "Engine/Model.h"
#include "Controller.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

namespace {
    bool keyChange_ = false;
    int upKey_ = 0;
    int downKey_ = 0;
    float posY_ = 0.0f;
    const float moveSpeed_ = 0.2f;
}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1)
{
    for (int i = 0; i < 10; i++) hPict_[i] = -1;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/Player.fbx");
    assert(hModel_ >= 0);

    for (int i = 0; i < 10; i++) {
        string fileName = "Assets/number/" + std::to_string(i) + ".png";
        hPict_[i] = Image::Load(fileName);
    }

    upKey_ = DIK_3;
    downKey_ = DIK_2;
}

void Player::Update()
{
    //キーコンフィグのとこ
    if (keyChange_) {
        int button = Input::KeyButton();
        if (button >= 2 && button <= 11) {
            upKey_ = button;
            keyChange_ = false;
        }
        return;
    }
    if (Input::IsKeyUp(DIK_ESCAPE)) {
        keyChange_ = true;
        return;
    }


    //移動させる
    if (Input::IsKey(upKey_)) {
        posY_ += moveSpeed_;
    }
    if (Input::IsKey(downKey_)) {
        posY_ -= moveSpeed_;
    }
    Controller* pController = (Controller*)FindObject("Controller");
    transform_.position_ = pController->GetPosition();
    transform_.position_.y = posY_;

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_, 2);

    //キー番号
    Direct3D::SetBlendMode(Direct3D::BLEND_ADD);

    Transform png;
    png.scale_ = { 0.1f, 0.1f, 1.0f};
    Image::SetTransform(hPict_[upKey_ - 1], png);
    Image::Draw(hPict_[upKey_ - 1]);

    png.position_ = { -0.3f, 0.0f, 0.2f };
    Image::SetTransform(hPict_[2], png);
    Image::Draw(hPict_[2]);

    Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);

}

void Player::Release()
{
    for (int i = 0; i < 10; i++) Image::Release(hPict_[i]);

}
