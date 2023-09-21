//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

struct PointLight
{
    float3 pos;
    float4 color;
    float3 leng;
};
StructuredBuffer<PointLight> rLightBuffer : register(t1);

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matNormal; //法線
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    bool isTexture; // テクスチャ貼ってあるかどうか
    float4x4 matW;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（輝度）
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
    
    normal = float4(normal.xyz, 0);

    //PointLight light = rLightBuffer[0];
    
    const int lightNum = 5;
    float4 lightPoss[lightNum];
    lightPoss[0] = float4(0, 2, 0, 0);
    lightPoss[1] = float4(0, 2, 30, 0);
    lightPoss[2] = float4(30, 2, 0, 0);
    lightPoss[3] = float4(30, 2, 30, 0);
    lightPoss[4] = float4(15, 2, 15, 0);
    
    //各光源の届く距離を設定する配列
    float lightRanges[lightNum];
    lightRanges[0] = 10.0;
    lightRanges[1] = 10.0;
    lightRanges[2] = 20.0;
    lightRanges[3] = 25.0;
    lightRanges[4] = 5.0;
    
    outData.color = float4(0, 0, 0, 0);

    // ライト
    for (int i = 0; i < 5; i++)
    {
        float4 lightDir = lightPoss[i] - mul(pos.xyzw, matW);
        float4 lightLen = length(lightDir);
        lightDir = normalize(lightDir);
        
        // 輝度情報をピクセルシェーダーへ
        float4 light = normalize(lightPoss[i]);
        
        // 距離による減衰を計算して光の強度を調整
        float attenuation = saturate(1.0 - lightLen / lightRanges[i]);
        outData.color += clamp(dot(mul(normal, matW), lightDir), 0, 1) * attenuation;
    }

    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = float4(0.0, 0.0, 0.0, 1.0);
    float4 diffuse = 0; //拡散
    float4 ambient = 0; //環境光
    
    // テクスチャがあるとき
    if (isTexture)
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    // テクスチャがないとき
    else
    {
        diffuse = diffuseColor * inData.color;
        ambient = diffuseColor * ambientSource;
    }

    return diffuse + ambient;
}


//------------------------------------------------------------------------------

//よさげなとこ
//https://dxlib.xsrv.jp/program/dxprogram_3DModelShaderBase.html

//DirectXー＞125回（複数ライト)
//https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC1210%E10%9B%9E

