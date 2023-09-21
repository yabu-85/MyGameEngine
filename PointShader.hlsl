//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

struct PointLight
{
    float3 pos;
    float4 color;
    float3 leng;
};
StructuredBuffer<PointLight> rLightBuffer : register(t1);

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matNormal; //�@��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
    float4x4 matW;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i�P�x�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float2 uv : TEXCOORD, float4 normal : NORMAL)
{
    //�s�N�Z���V�F�[�_�[�ɓn���\���̃f�[�^
    VS_OUT outData;

    //���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
    //�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);

    //�e�N�X�`���f�[�^���s�N�Z���V�F�[�_�[��
    outData.uv = uv;
    
    normal = float4(normal.xyz, 0);

    //PointLight light = rLightBuffer[0];
    
    const int lightNum = 5;
    float4 lightPoss[lightNum];
    lightPoss[0] = float4(0, 2, 0, 0);
    lightPoss[1] = float4(0, 2, 30, 0);
    lightPoss[2] = float4(30, 2, 0, 0);
    lightPoss[3] = float4(30, 2, 30, 0);
    lightPoss[4] = float4(15, 2, 15, 0);
    
    //�e�����̓͂�������ݒ肷��z��
    float lightRanges[lightNum];
    lightRanges[0] = 10.0;
    lightRanges[1] = 10.0;
    lightRanges[2] = 20.0;
    lightRanges[3] = 25.0;
    lightRanges[4] = 5.0;
    
    outData.color = float4(0, 0, 0, 0);

    // ���C�g
    for (int i = 0; i < 5; i++)
    {
        float4 lightDir = lightPoss[i] - mul(pos.xyzw, matW);
        float4 lightLen = length(lightDir);
        lightDir = normalize(lightDir);
        
        // �P�x�����s�N�Z���V�F�[�_�[��
        float4 light = normalize(lightPoss[i]);
        
        // �����ɂ�錸�����v�Z���Č��̋��x�𒲐�
        float attenuation = saturate(1.0 - lightLen / lightRanges[i]);
        outData.color += clamp(dot(mul(normal, matW), lightDir), 0, 1) * attenuation;
    }

    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = float4(0.0, 0.0, 0.0, 1.0);
    float4 diffuse = 0; //�g�U
    float4 ambient = 0; //����
    
    // �e�N�X�`��������Ƃ�
    if (isTexture)
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    // �e�N�X�`�����Ȃ��Ƃ�
    else
    {
        diffuse = diffuseColor * inData.color;
        ambient = diffuseColor * ambientSource;
    }

    return diffuse + ambient;
}


//------------------------------------------------------------------------------

//�悳���ȂƂ�
//https://dxlib.xsrv.jp/program/dxprogram_3DModelShaderBase.html

//DirectX�[��125��i�������C�g)
//https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC1210%E10%9B%9E

