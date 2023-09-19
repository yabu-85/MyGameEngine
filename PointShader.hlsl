//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//ここはTextureクラスから送られてる

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
//ここ宣言する順番も関係してくるから注意、Fbxから送られてる
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

//VSの戻り値
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
    const int MAX_LIGHTS = 6;
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse = 0; //拡散
    float4 ambient = 0; //環境光
    
    //ライトの座標
    float4 lightPos1[MAX_LIGHTS];
    lightPos1[0] = lightPos + float4(30, 30, 30, 0);
    lightPos1[1] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[2] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[3] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[4] = lightPos + float4(-30, 30, -30, 0);
    lightPos1[5] = lightPos + float4(-30, 30, -30, 0);
    
    // ライトの数だけループ
    for (int i = 0; i < 3; ++i)
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
    
            float4 lightDiffuse = (wLight * g_texture.Sample(g_sampler, inData.uv)) / inData.lightLen;
            float4 lightAmbient = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
            lightDiffuse *= attenuation;
            lightAmbient *= attenuation;

            // ライトごとの拡散成分と環境光成分を加算
            diffuse += lightDiffuse;
            ambient = lightAmbient;

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



//------------------------------------------------------------------------------

//よさげなとこ
//https://dxlib.xsrv.jp/program/dxprogram_3DModelShaderBase.html

//DirectXー＞125回（複数ライト)
//https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC1210%E10%9B%9E

