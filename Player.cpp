#include "Player.h"
#include "Engine/Model.h"
#include "Controller.h"
#include "Engine/Input.h"

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
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/Player.fbx");
    assert(hModel_ >= 0);

    upKey_ = DIK_E;
    downKey_ = DIK_Q;
}

void Player::Update()
{
    if (keyChange_) {
        int button = Input::KeyButton();
        if (button >= 1) {
            upKey_ = button;
            keyChange_ = false;
        }
        return;
    }

    if (Input::IsKeyUp(DIK_3)) {
        keyChange_ = true;
        return;
    }

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

}

void Player::Release()
{
}
