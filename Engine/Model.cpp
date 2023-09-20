#include "Model.h"

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

//3Dモデル（FBXファイル）を管理する
namespace Model
{
	//ロード済みのモデルデータ一覧
	std::vector<ModelData*> modelList;

}

int Model::Load(std::string fileName) {

	ModelData* pData = new ModelData;
	pData->filename_ = fileName;
	pData->pFbx_ = nullptr;

	//ファイルネームが同じだったら、読まない
	for (auto e : modelList) {
		if (e->filename_ == fileName) {
			pData->pFbx_ = e->pFbx_;
			break;
		
		}			
	}

	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);

	}
	modelList.push_back(pData);

	return (modelList.size() - 1);
	//読み込んだモデルのモデル番号を返す

}

void Model::SetTransform(int hModel, Transform transform) {
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel, int type) {
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_, type);

}

void Model::Release()
{
	bool isReffered = false; //参照されてる？
	for (int i = 0; i < modelList.size(); i++) {
		for (int j = i + 1; j < modelList.size(); j++) {
			if (modelList[i]->pFbx_ == modelList[j]->pFbx_) {
				isReffered = true;
				break;
			}
		}

		if (isReffered == false) {
			SAFE_DELETE(modelList[i]->pFbx_);
		}
		SAFE_DELETE(modelList[i]);

	}
	modelList.clear();

}

void Model::RayCast(int hModel, RayCastData& rayData)
{
	//⓪モデルのトランスフォームをカリキュレーション
	modelList[hModel]->transform_.Calclation();
	
	//①ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr, modelList[hModel]->transform_.GetWorldMatrix());
	
	//②レイの通過点を求める（モデル空間での例の方向ベクトルを求める）
	XMVECTOR vpass{ rayData.start.x + rayData.dir.x,
					rayData.start.y + rayData.dir.y,
					rayData.start.z + rayData.dir.z,
					rayData.start.w + rayData.dir.w };

	//③rayData.startをモデル空間に変換（①をかける）
	XMVECTOR vstart = XMLoadFloat4(&rayData.start);
	vstart = XMVector3TransformCoord(vstart, wInv); //tarnsformcoordはw要素を無視してくれるらしい
	XMStoreFloat4(&rayData.start, vstart);

	//④（視点から方向ベクトルをチョイ伸ばした先）通過点（②）に①をかける
	vpass = XMVector3TransformCoord(vpass, wInv);
	
	//⑤rayData.dirを③から④に向かうベクトルにする（引き算）
	vpass = vpass - vstart;
	XMStoreFloat4(&rayData.dir, vpass);

	//指定したモデル番号のFBXにレイキャスト
	modelList[hModel]->pFbx_->RayCast(&rayData);
}