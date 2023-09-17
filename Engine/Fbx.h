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
		XMFLOAT4 diffuseColor;	// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
		bool 	 isTextured;	// �e�N�X�`���\���Ă��邩�ǂ���
		XMMATRIX matWorld;
		XMFLOAT4 lightPos;		//���C�g�̏ꏊ
		XMFLOAT4 wLight;		//���C�g���Ă������RGB�̒l
	};
	XMFLOAT4 lightPos = { 0,1,0,0 };

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

	VERTEX* pVertices_;
	int** ppIndex_;

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
	void Draw(Transform& transform, int type);
	void Release();

	void RayCast(RayCastData* rayData);
	void SetLightPosition(XMFLOAT3 pos) { lightPos = XMFLOAT4(pos.x, pos.y, pos.z, 0.0f); };

};