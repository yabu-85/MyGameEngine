#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include <vector>
//ヘッダーでヘッダーあんまインクルードしないお互いがインクルードすると循環参照となってエラー吐く

using std::vector;

//MT -> MDに変更
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Texture;

struct RayCastData
{
	XMFLOAT4 start;
	XMFLOAT4 dir;
	bool hit;
	float dist;

	RayCastData() { dist = 0.0f; };
};

class Fbx
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;
		XMMATRIX matNormal;
		XMFLOAT4 diffuseColor;	// ディフューズカラー（マテリアルの色）
		bool 	 isTextured;	// テクスチャ貼ってあるかどうか
		XMMATRIX matWorld;
	};

	struct POINTLIGHT_BUFFER {
		XMFLOAT3 pos_;
		XMFLOAT4 color_;
		XMFLOAT3 leng_;
	};

	struct VERTEX
	{
		//ここFLOATだったのVECTORへ変更
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};

	VERTEX* pVertices_;
	int** ppIndex_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数
	vector <int> indexCount_;

	ID3D11Buffer* pVertexBuffer_; 
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;
	ID3D11Buffer* pointLightBuffer_;

	void IntConstantBuffer(); //コンスタントバッファを作成
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitMaterial(fbxsdk::FbxNode* pNode);

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform, int type);
	void Release();

	void RayCast(RayCastData* rayData);
};