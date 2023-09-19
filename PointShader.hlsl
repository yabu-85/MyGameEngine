//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������Texture�N���X���瑗���Ă�

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
//�����錾���鏇�Ԃ��֌W���Ă��邩�璍�ӁAFbx���瑗���Ă�
cbuffer global
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matNormal; //�@��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
    
    float4x4 matW;
    float4 lightPos;
    float4 wLight;

};

//VS�̖߂�l
//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i�P�x�j

    float4 lightDir : TEXCOORD1;
    float lightLen : TEXCOORD2;
    float4 normal : TEXCOORD3;
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
    
    //���C�g
    outData.lightDir = float4(lightPos.xyz, 1.0) - mul(pos.xyzw, matW);
    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

    //�@��
    normal = float4(normal.xyz, 0);
    outData.normal = mul(normal, matW);
    
    //�P�x�����s�N�Z���V�F�[�_�\��
    float4 light = mul(lightPos, matW);
    light = normalize(light);
    outData.color = clamp(dot(mul(normal, matW), outData.lightDir), 0, 1);

    return outData;
}


//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    const int MAX_LIGHTS = 6;
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse = 0; //�g�U
    float4 ambient = 0; //����
    
    //���C�g�̍��W
    float4 lightPos1[MAX_LIGHTS];
    lightPos1[0] = lightPos + float4(30, 30, 30, 0);
    lightPos1[1] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[2] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[3] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[4] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[5] = lightPos + float4(-30, 30, -30, 0);
    
    // ���C�g�̐��������[�v
    for (int i = 0; i < 3; ++i)
    {
        // ���C�g���L�����ǂ������m�F�i�K�؂ȏ����ɕύX����K�v������܂��j
        bool isLightValid = true; /* �����Ń��C�g���L�����ǂ������`�F�b�N���������ݒ� */
        
        if (isLightValid)
        {
            // ���C�g����̋����ƕ������v�Z
            float3 lightDir = lightPos1[i].xyz - mul(inData.pos, matW).xyz;
            float lightDistance = length(lightDir);
            float3 lightDirection = normalize(lightDir);
            
            // �����ɉ������������v�Z�i��: 1 / (�����̓��)�j
            float attenuation = 1.0 / (lightDistance * lightDistance) + 0.3f;
    
            float4 lightDiffuse = (wLight * g_texture.Sample(g_sampler, inData.uv)) / inData.lightLen;
            float4 lightAmbient = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
            lightDiffuse *= attenuation;
            lightAmbient *= attenuation;

            // ���C�g���Ƃ̊g�U�����Ɗ������������Z
            diffuse += lightDiffuse;
            ambient = lightAmbient;

        }
    }
    
    // �e�N�X�`��������Ƃ�
    if (isTexture)
    {
        diffuse *= wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient *= wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    // �e�N�X�`�����Ȃ��Ƃ�
    else
    {
        diffuse *= wLight * diffuseColor * inData.color;
        ambient *= wLight * diffuseColor * ambientSource;
    }
    
    return diffuse + ambient;
}



//------------------------------------------------------------------------------

//�悳���ȂƂ�
//https://dxlib.xsrv.jp/program/dxprogram_3DModelShaderBase.html

//DirectX�[��125��i�������C�g)
//https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC1210%E10%9B%9E

