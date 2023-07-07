#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include <vector>
//�w�b�_�[�Ńw�b�_�[����܃C���N���[�h���Ȃ����݂����C���N���[�h����Əz�Q�ƂƂȂ��ăG���[�f��

using std::vector;

//MT -> MD�ɕύX
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Texture;

class Fbx
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;
		int 		isTextured;
	};

	struct VERTEX
	{
		//����FLOAT��������VECTOR�֕ύX
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};
	
	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�
	vector <int> indexCount_;

	ID3D11Buffer* pVertexBuffer_; 
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;

	void IntConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitMaterial(fbxsdk::FbxNode* pNode);

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();

};