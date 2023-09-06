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

void Model::Draw(int hModel) {
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_);

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
	modelList[hModel]->pFbx_->RayCast(&rayData);
}
