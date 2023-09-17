//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー


//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
//ここ宣言する順番も関係してくるから注意
cbuffer global
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matNormal; //法線
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    bool isTexture; // テクスチャ貼ってあるかどうか
    
    float4x4 matW;
    float4 lightPos;
    float4 wLight;

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（輝度）

    float4 lightDir : TEXCOORD1;
    float lightLen : TEXCOORD2;
    float4 normal : TEXCOORD3;
};


//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float2 uv : TEXCOORD, float4 normal : NORMAL)
{
    //ピクセルシェーダーに渡す構造体データ
    VS_OUT outData;
    
    //ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
    //スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);

    //テクスチャデータをピクセルシェーダーへ
    outData.uv = uv;
    
    //ライト
    outData.lightDir = float4(lightPos.xyz, 1.0) - mul(pos.xyzw, matW);
    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

    //法線
    normal = float4(normal.xyz, 0);
    outData.normal = mul(normal, matW);
    
    //輝度情報をピクセルシェーダ―へ
    float4 light = mul(lightPos, matW);
    light = normalize(light);
    outData.color = clamp(dot(mul(normal, matW), outData.lightDir), 0, 1);

    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    const int MAX_LIGHTS = 2;
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse = 0; //拡散
    float4 ambient = 0; //環境光
    
    //ライトの座標
    float4 lightPos1[MAX_LIGHTS];
    lightPos1[0] = lightPos + float4(3, 3, 3, 0);
    lightPos1[1] = lightPos + float4(-3, 3, -3, 0);
    
    // ライトの数だけループ
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        // ライトが有効かどうかを確認（適切な条件に変更する必要があります）
        bool isLightValid = true; /* ここでライトが有効かどうかをチェックする条件を設定 */
        
        if (isLightValid)
        {   
            // ライトからの距離と方向を計算
            float3 lightDir = lightPos1[i].xyz - mul(inData.pos, matW).xyz;
            float lightDistance = length(lightDir);
            float3 lightDirection = normalize(lightDir);
            
            // 距離に応じた減衰を計算（例: 1 / (距離の二乗)）
            float attenuation = 1.0 / (lightDistance * lightDistance) + 0.3f;
    
            float4 lightDiffuse1 = (wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
            lightDiffuse1 *= attenuation;

            float4 lightAmbient1 = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
            lightAmbient1 *= attenuation;

            // ライトごとの拡散成分と環境光成分を加算
            diffuse += lightDiffuse1;
            ambient += lightAmbient1;
        }
    }
    
    // テクスチャがあるとき
    if (isTexture)
    {
        diffuse *= wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient *= wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    // テクスチャがないとき
    else
    {
        diffuse *= wLight * diffuseColor * inData.color;
        ambient *= wLight * diffuseColor * ambientSource;
    }
    
    return diffuse + ambient;
}


//───────────────────────────────────────
//点光源の実装について(アルゴリズム)
//───────────────────────────────────────
/*
    3D空間上の特定の位置から放射される光を表すための光源。
    光源からの光の強度と距離に基づいて、物体の表面の明るさを計算する。

    1.ライトの位置と強度の設定
    位置と強度をコンスタントバッファから取得する。

    2.光源からのベクトルの計算
    光源からの各頂点の位置への距離を計算することでベクトルを取得する。

    3.光の強度の計算（距離に応じた光の減衰）の適用
    光源と頂点の距離が大きくなればなるほど暗くなる
*/



//==============================================================================

//! コンスタントバッファ
cbuffer cbSceneParam : register(b1)
{
    float4 mtxView[3] : packoffset(c0); // ビュー行列
    float4 mtxProj[4] : packoffset(c3); // 投影行列
    float4 screenParam : packoffset(c7); // スクリーンパラメータ(near, far, ?, ?)
    float4 frustumCorner : packoffset(c8); // ビューフラスタムの角へのベクトル
    float4 mtxLastView[3] : packoffset(c9); // 前回のビュー行列
    float4 mtxLastProj[4] : packoffset(c12); // 前回の投影行列
};

struct PointLight
{
    float3 pos;
    float4 color;
    float4 attn; //attenuation 減衰
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

//! シェーダリソース
Texture2D texLightBuffer : register(t0);
Texture2D texShadowBuffer : register(t1);
Texture2D texAlbedo : register(t2);
Texture2D texHDAO : register(t3);
StructuredBuffer<PointLight> rLightBuffer : register(t4);
StructuredBuffer<uint> rLightIndices : register(t5);

//! サンプラ
SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);

const float kTileWidth = 16.0f;

//==============================================================================


//=====================================================================
// HDAOのサンプリング
float SampleHDAO(float2 uv)
{
    return texHDAO.SampleLevel(samPoint, uv, 0.0).r;
}

// タイルインデックスを求める
uint GetTileIndex(float2 screenPos)
{
    float2 framePos = screenPos * screenParam.zw;
    uint numCellX = (screenParam.z + kTileWidth - 1) / kTileWidth;
    return floor(framePos.x / kTileWidth) + floor(framePos.y / kTileWidth) * numCellX;
}

float4 CalcPointLightBRDF(float3 objPos, float3 objNormal, float3 litPos, float4 litColor, float4 litAttn, float gloss, float f0)
{
    float3 eyeVec = -normalize(objPos);

	// 減衰計算
    float3 litDir = litPos.xyz - objPos;
    float litLen = length(litDir);
    float litRate = litLen / litAttn.x;
    float attn = max(1.0 - litRate * litRate, 0.0);
    if (attn == 0.0)
    {
        return float4(0, 0, 0, 0);
    }
    litDir = normalize(litDir);

	// ライト計算
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

	// 出力
    return float4(litColor.rgb * nl * attn, spec * attn * attn);
}


[earlydepthstencil]
float4 RenderForwardPS( OutputVS inPixel ) : SV_TARGET
{
	// シャドウマップ用UVに変換する
	float2	shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// シャドウカラーを求める
	float	mapDepth = texShadowBuffer.Sample( samPoint, shadowUV ).r;
	float	modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
	float4	shadowColor = { 0, 0, 0, 1 };
	if( modelDepth < mapDepth + 0.01 )
	{
		shadowColor = float4( 1, 1, 1, 1 );
	}

	// スクリーンのUV座標
	float2	uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAOを取得
	float	hdao = SampleHDAO(uv);

	// ライト
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

	// ディフューズ + スペキュラ + シャドウ + HDAO
	float3	color = (diffuse * diffuseColor.rgb + specular) * shadowColor.rgb * hdao;
	return float4( color, 1.0f );
}

[earlydepthstencil]
float4 RenderForwardTexPS(OutputTexVS inPixel) : SV_TARGET
{
	// アルベドテクスチャをフェッチ
    float4 albedo = texAlbedo.Sample(samLinear, inPixel.uv);

	// シャドウマップ用UVに変換する
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// シャドウカラーを求める
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// スクリーンのUV座標
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAOを取得
    float hdao = SampleHDAO(uv);

	// ライト
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

	// ディフューズ + シャドウ + HDAO
    float3 color = albedo.rgb * diffuse.rgb * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}

[earlydepthstencil]
float4 RenderRawForwardPS(OutputVS inPixel) : SV_TARGET
{
	// シャドウマップ用UVに変換する
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// シャドウカラーを求める
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// スクリーンのUV座標
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAOを取得
    float hdao = SampleHDAO(uv);

	// ライト
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

	// ディフューズ + スペキュラ + シャドウ + HDAO
    float3 color = (diffuse * diffuseColor.rgb + specular) * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}

[earlydepthstencil]
float4 RenderRawForwardTexPS(OutputTexVS inPixel) : SV_TARGET
{
	// アルベドテクスチャをフェッチ
    float4 albedo = texAlbedo.Sample(samLinear, inPixel.uv);

	// シャドウマップ用UVに変換する
    float2 shadowUV = (inPixel.shadow_pos.xy / inPixel.shadow_pos.w) * float2(0.5, -0.5) + 0.5;

	// シャドウカラーを求める
    float mapDepth = texShadowBuffer.Sample(samPoint, shadowUV).r;
    float modelDepth = inPixel.shadow_pos.z / inPixel.shadow_pos.w;
    float4 shadowColor = { 0, 0, 0, 1 };
    if (modelDepth < mapDepth + 0.01)
    {
        shadowColor = float4(1, 1, 1, 1);
    }

	// スクリーンのUV座標
    float2 uv = (inPixel.use_pos.xy / inPixel.use_pos.w) * float2(0.5, -0.5) + 0.5;

	// HDAOを取得
    float hdao = SampleHDAO(uv);

	// ライト
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

	// ディフューズ + シャドウ + HDAO
    float3 color = albedo.rgb * diffuse.rgb * shadowColor.rgb * hdao;
    return float4(color, 1.0f);
}


