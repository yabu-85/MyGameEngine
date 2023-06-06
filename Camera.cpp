#include "Camera.h"

namespace Camera {
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
}

//初期化
void Camera::Initialize()
{
	/*
	XMVECTOR position = { 0, 3, -10, 0 };	//カメラの位置
	XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列 ///XMVectorSet=上はどっちか
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列 ///画角(ラジアン)・アスペクト比・クリッピング（＊２
	///Fovは視野角のこと（Field of View）今回はXM_PIDIV4 1π/4 だから４５度 狭いほどめちゃズーム(遠近感がなくなる
	///クリッピングの二つの値の差はなるべく小さく　そのサイズを圧縮するから　大きくするとZファイティングが起きる
	*/

	position_ = XMVectorSet(0, 3, -10, 0);	//カメラの位置
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点

	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

//更新
void Camera::Update()
{
	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

//位置を設定
void Camera::SetPosition(XMVECTOR position)
{
}

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}