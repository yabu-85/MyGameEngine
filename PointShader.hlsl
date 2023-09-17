//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[


//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
//�����錾���鏇�Ԃ��֌W���Ă��邩�璍��
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
    const int MAX_LIGHTS = 2;
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse = 0; //�g�U
    float4 ambient = 0; //����
    
    //���C�g�̍��W
    float4 lightPos1[MAX_LIGHTS];
    lightPos1[0] = lightPos + float4(3, 3, 3, 0);
    lightPos1[1] = lightPos + float4(-3, 3, -3, 0);
    
    // ���C�g�̐��������[�v
    for (int i = 0; i < MAX_LIGHTS; ++i)
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
    
            float4 lightDiffuse1 = (wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
            lightDiffuse1 *= attenuation;

            float4 lightAmbient1 = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
            lightAmbient1 *= attenuation;

            // ���C�g���Ƃ̊g�U�����Ɗ������������Z
            diffuse += lightDiffuse1;
            ambient += lightAmbient1;
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


//������������������������������������������������������������������������������
//�_�����̎����ɂ���(�A���S���Y��)
//������������������������������������������������������������������������������
/*
    3D��ԏ�̓���̈ʒu������˂�������\�����߂̌����B
    ��������̌��̋��x�Ƌ����Ɋ�Â��āA���̂̕\�ʂ̖��邳���v�Z����B

    1.���C�g�̈ʒu�Ƌ��x�̐ݒ�
    �ʒu�Ƌ��x���R���X�^���g�o�b�t�@����擾����B

    2.��������̃x�N�g���̌v�Z
    ��������̊e���_�̈ʒu�ւ̋������v�Z���邱�ƂŃx�N�g�����擾����B

    3.���̋��x�̌v�Z�i�����ɉ��������̌����j�̓K�p
    �����ƒ��_�̋������傫���Ȃ�΂Ȃ�قǈÂ��Ȃ�
*/



//==============================================================================

//! �R���X�^���g�o�b�t�@
cbuffer cbSceneParam : register(b1)
{
    float4 mtxView[3] : packoffset(c0); // �r���[�s��
    float4 mtxProj[4] : packoffset(c3); // ���e�s��
    float4 screenParam : packoffset(c7); // �X�N���[���p�����[�^(near, far, ?, ?)
    float4 frustumCorner : packoffset(c8); // �r���[�t���X�^���̊p�ւ̃x�N�g��
    float4 mtxLastView[3] : packoffset(c9); // �O��̃r���[�s��
    float4 mtxLastProj[4] : packoffset(c12); // �O��̓��e�s��
};

struct PointLight
{
    float3 pos;
    float4 color;
    float4 attn; //attenuation ����
};

struct OutputVS
{
    float4 pos : SV_POSITION;
    float4 use_pos : POSITION;
    float4 shadow_pos : SHADOW_POSITION;
    float4 posView : POSITION_IN_VIEW;
    float3 normal : NORMAL;
};

struct OutputTexVS
{
    float4 pos : SV_POSITION;
    float4 use_pos : POSITION;
    float4 shadow_pos : SHADOW_POSITION;
    float4 posView : POSITION_IN_VIEW;
    float3 normal : NORMAL;
    float2 uv : UV;
};

//! �V�F�[�_���\�[�X
Texture2D texLightBuffer : register(t0);
Texture2D texShadowBuffer : register(t1);
Texture2D texAlbedo : register(t2);
Texture2D texHDAO : register(t3);
StructuredBuffer<PointLight> rLightBuffer : register(t4);
StructuredBuffer<uint> rLightIndices : register(t5);

//! �T���v��
SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);

const float kTileWidth = 16.0f;

//==============================================================================


//=====================================================================
// HDAO�̃T���v�����O
float SampleHDAO(float2 uv)
{
    return texHDAO.SampleLevel(samPoint, uv, 0.0).r;
}

// �^�C���C���f�b�N�X�����߂�
uint GetTileIndex(float2 screenPos)
{
    float2 framePos = screenPos * screenParam.zw;
    uint numCellX = (screenParam.z + kTileWidth - 1) / kTileWidth;
    return floor(framePos.x / kTileWidth) + floor(framePos.y / kTileWidth) * numCellX;
}

float4 CalcPointLightBRDF(float3 objPos, float3 objNormal, float3 litPos, float4 litColor, float4 litAttn, float gloss, float f0)
{
    float3 eyeVec = -normalize(objPos);

	// �����v�Z
    float3 litDir = litPos.xyz - objPos;
    float litLen = length(litDir);
    float litRate = litLen / litAttn.x;
    float attn = max(1.0 - litRate * litRate, 0.0);
    if (attn == 0.0)
    {
        return float4(0, 0, 0, 0);
    }
    litDir = normalize(litDir);

	// ���C�g�v�Z
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = saturate(dot(objNormal, halfVec));
    float power = litAttn.w;
    float spec = 0.0;
    if (power > 0.0)
    {
        float nv = dot(objNormal, eyeVec);
        float sn = pow(2.0, 13.0 * gloss) * power;
        float D = (sn + 2.0) * pow(nh, sn) / (2.0 * 3.1415926);
        float F = f0 + (1.0 - f0) * pow((1.0 - nv), 5.0);
        float dv = dot(eyeVec, (eyeVec + litDir));
        float V = 4.0 / (dv * dv);
        float nl_nv = nl * nv;
        float G = V * nl_nv;
        spec = max(F * G * D / (4.0 * nl_nv), 0.0);
    }

	// �o��
    return float4(litColor.rgb * nl * attn, spec * attn * attn);
}


[earlydepthstencil]
float4 RenderForwardPS( OutputVS inPixel ) : SV_TARGET
{
	// �V���h�E�}�b�v�pUV�ɕϊ�����
	float2	shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// �V���h�E�J���[�����߂�
	float	mapDepth = texShadowBuffer.Sample( samPoint, shadowUV ).r;
	float	modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
	float4	shadowColor = { 0, 0, 0, 1 };
	if( modelDepth < mapDepth + 0.01 )
	{
		shadowColor = float4( 1, 1, 1, 1 );
	}

	// �X�N���[����UV���W
	float2	uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAO���擾
	float	hdao = SampleHDAO(uv);

	// ���C�g
    uint numLights, dummy;
    rLightBuffer.GetDimensions( numLights, dummy );
	uint tileIndex = GetTileIndex( uv );
	uint lightStart = tileIndex * numLights;
	uint lightEnd = lightStart + numLights;
	float3 normal = normalize( inPixel.normal );
	float3 diffuse = (float3)0.0;
	float specular = 0.0;
	[loop]
	for (uint lightRefIndex = lightStart; lightRefIndex < lightEnd; ++lightRefIndex)
	{
		uint lightIndex = rLightIndices[lightRefIndex];
		[branch] if (lightIndex == 0xffffffff) { break; }

		PointLight light = rLightBuffer[lightIndex];
		float4 lightResult = CalcPointLightBRDF( inPixel.posView.xyz, normal, light.pos.xyz, light.color, light.attn, 0.4, 0.5 );
		diffuse += lightResult.rgb;
		specular += lightResult.a;
	}

	// �f�B�t���[�Y + �X�y�L���� + �V���h�E + HDAO
	float3	color = (diffuse * diffuseColor.rgb + specular) * shadowColor.rgb * hdao;
	return float4( color, 1.0f );
}

[earlydepthstencil]
float4 RenderForwardTexPS(OutputTexVS inPixel) : SV_TARGET
{
	// �A���x�h�e�N�X�`�����t�F�b�`
    float4 albedo = texAlbedo.Sample(samLinear, inPixel.uv);

	// �V���h�E�}�b�v�pUV�ɕϊ�����
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// �V���h�E�J���[�����߂�
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// �X�N���[����UV���W
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAO���擾
    float hdao = SampleHDAO(uv);

	// ���C�g
    uint numLights, dummy;
    rLightBuffer.GetDimensions(numLights, dummy);
    uint tileIndex = GetTileIndex(uv);
    uint lightStart = tileIndex * numLights;
    uint lightEnd = lightStart + numLights;
    float3 normal = normalize(inPixel.normal);
    float3 diffuse = (float3) 0.0;
	[loop]
    for (uint lightRefIndex = lightStart; lightRefIndex < lightEnd; ++lightRefIndex)
    {
        uint lightIndex = rLightIndices[lightRefIndex];
		[branch]
        if (lightIndex == 0xffffffff)
        {
            break;
        }

        PointLight light = rLightBuffer[lightIndex];
        float4 attn = float4(light.attn.rgb, 0.0);
        float4 lightResult = CalcPointLightBRDF(inPixel.posView.xyz, normal, light.pos.xyz, light.color, attn, 0.0, 0.0);
        diffuse += lightResult.rgb;
    }

	// �f�B�t���[�Y + �V���h�E + HDAO
    float3 color = albedo.rgb * diffuse.rgb * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}

[earlydepthstencil]
float4 RenderRawForwardPS(OutputVS inPixel) : SV_TARGET
{
	// �V���h�E�}�b�v�pUV�ɕϊ�����
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// �V���h�E�J���[�����߂�
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// �X�N���[����UV���W
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAO���擾
    float hdao = SampleHDAO(uv);

	// ���C�g
    uint numLights, dummy;
    rLightBuffer.GetDimensions(numLights, dummy);
    float3 normal = normalize(inPixel.normal);
    float3 diffuse = (float3) 0.0;
    float specular = 0.0;
	[loop]
    for (uint lightIndex = 0; lightIndex < numLights; ++lightIndex)
    {
        PointLight light = rLightBuffer[lightIndex];
        float4 lightResult = CalcPointLightBRDF(inPixel.posView.xyz, normal, light.pos.xyz, light.color, light.attn, 0.4, 0.5);
        diffuse += lightResult.rgb;
        specular += lightResult.a;
    }

	// �f�B�t���[�Y + �X�y�L���� + �V���h�E + HDAO
    float3 color = (diffuse * diffuseColor.rgb + specular) * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}

[earlydepthstencil]
float4 RenderRawForwardTexPS(OutputTexVS inPixel) : SV_TARGET
{
	// �A���x�h�e�N�X�`�����t�F�b�`
    float4 albedo = texAlbedo.Sample(samLinear, inPixel.uv);

	// �V���h�E�}�b�v�pUV�ɕϊ�����
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// �V���h�E�J���[�����߂�
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// �X�N���[����UV���W
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAO���擾
    float hdao = SampleHDAO(uv);

	// ���C�g
    uint numLights, dummy;
    rLightBuffer.GetDimensions(numLights, dummy);
    float3 normal = normalize(inPixel.normal);
    float3 diffuse = (float3) 0.0;
	[loop]
    for (uint lightIndex = 0; lightIndex < numLights; ++lightIndex)
    {
        PointLight light = rLightBuffer[lightIndex];
        float4 attn = float4(light.attn.rgb, 0.0);
        float4 lightResult = CalcPointLightBRDF(inPixel.posView.xyz, normal, light.pos.xyz, light.color, attn, 0.0, 0.0);
        diffuse += lightResult.rgb;
    }

	// �f�B�t���[�Y + �V���h�E + HDAO
    float3 color = albedo.rgb * diffuse.rgb * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}


