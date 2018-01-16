//*************************************************************************************************
// ギア処理 (gear.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene2D.h"
#include "gear.h"
#include "number.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00   "data/TEXTURE/gear.png"                    //ギアテクスチャ
#define TEXTURENAME01   "data/TEXTURE/bar.png"                     //バーテクスチャ
#define NUM_SIZE_X ( SCREEN_WIDTH * 0.004f * 9 )                   //数値横サイズ
#define NUM_SIZE_Y ( SCREEN_HEIGHT * 0.004f * 16 )                 //数値縦サイズ

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// ギアのコンストラクタ
//*************************************************************************************************
CGear::CGear(int nPriolity) :CScene2D(SCENE_TYPE_GEAR, nPriolity)
{
    for (int i = 0; i < 2; i++)
        m_pTexture[i] = NULL;
    m_pVtxBuff = NULL;

    //タイム最高桁数分繰り返し
    for (int nCntNumber = 0; nCntNumber < GEAR_DISIT_MAX; nCntNumber++)
    {
        //数値クラスのNULL代入
        m_apNumber[nCntNumber] = NULL;
    }

    m_ItemNum = 0;
}

//*************************************************************************************************
// ギアのデストラクタ
//*************************************************************************************************
CGear::~CGear()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CGear::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        // テクスチャの生成
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME00, &m_pTexture[0])
            || D3DXCreateTextureFromFile(pDevice, TEXTURENAME01, &m_pTexture[1])))
        {
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
        }

        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_2D) * NUM_VERTEX * 3,     //作成したい頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                    //VertexBufferの使い方を指定
            FVF_VERTEX_2D,                         //頂点フォーマット
            D3DPOOL_MANAGED,                       //メモリ管理（誰が管理するか）
            &m_pVtxBuff,                           //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }
    }

    int AddPos_X = 0;

    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < GEAR_DISIT_MAX; nCntDisit++)
    {
        //数値の生成
        m_apNumber[nCntDisit] = CNumber::Create(D3DXVECTOR3(m_Pos.x- NUM_SIZE_X * (nCntDisit + 1) - AddPos_X, m_Pos.y, 0.0f),
            D3DXVECTOR3(NUM_SIZE_X, NUM_SIZE_Y, 0.0f));
    }
    m_ItemNum = 0;

    m_Size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
    angle = 0.0f;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CGear::Uninit(void)
{
    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < GEAR_DISIT_MAX; nCntDisit++)
    {
        //数値の描画処理
        m_apNumber[nCntDisit]->Uninit();
        delete m_apNumber[nCntDisit];
        m_apNumber[nCntDisit] = NULL;
    }

    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // テクスチャの破棄
    for (int i = 0; i < 2; i++)
    {
        if (m_pTexture != NULL)
        {
            m_pTexture[i]->Release();
            m_pTexture[i] = NULL;
        }
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CGear::Update(void)
{
    angle += 1.0f;

    m_Pos = D3DXVECTOR3(SCREEN_WIDTH / 19.0f, SCREEN_HEIGHT / 1.02f, 0.0f);
    m_Size = D3DXVECTOR3(SCREEN_WIDTH * 0.010f * 9, SCREEN_HEIGHT * 0.010f * 16, 0.0f);

    D3DXVECTOR2  Pos[4];
    rotation2D(&Pos[0].x, &Pos[0].y, m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.x, m_Pos.y, (angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[1].x, &Pos[1].y, m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.x, m_Pos.y, (angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[2].x, &Pos[2].y, m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.x, m_Pos.y, (angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[3].x, &Pos[3].y, m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.x, m_Pos.y, (angle / 180.0f) * D3DX_PI);

    // 頂点情報を設定
    VERTEX_2D* pVtx;                                //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(Pos[0].x, Pos[0].y, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(Pos[1].x, Pos[1].y, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(Pos[2].x, Pos[2].y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(Pos[3].x, Pos[3].y, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    rotation2D(&Pos[0].x, &Pos[0].y, (m_Pos.x + m_Pos.x*0.3f) - (m_Size.x*0.6f) / 2, (m_Pos.y - m_Pos.y * 0.11f) - (m_Size.y*0.6f) / 2, (m_Pos.x + m_Pos.x*0.3f), (m_Pos.y - m_Pos.y * 0.11f), -(angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[1].x, &Pos[1].y, (m_Pos.x + m_Pos.x*0.3f) + (m_Size.x*0.6f) / 2, (m_Pos.y - m_Pos.y * 0.11f) - (m_Size.y*0.6f) / 2, (m_Pos.x + m_Pos.x*0.3f), (m_Pos.y - m_Pos.y * 0.11f), -(angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[2].x, &Pos[2].y, (m_Pos.x + m_Pos.x*0.3f) - (m_Size.x*0.6f) / 2, (m_Pos.y - m_Pos.y * 0.11f) + (m_Size.y*0.6f) / 2, (m_Pos.x + m_Pos.x*0.3f), (m_Pos.y - m_Pos.y * 0.11f), -(angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[3].x, &Pos[3].y, (m_Pos.x + m_Pos.x*0.3f) + (m_Size.x*0.6f) / 2, (m_Pos.y - m_Pos.y * 0.11f) + (m_Size.y*0.6f) / 2, (m_Pos.x + m_Pos.x*0.3f), (m_Pos.y - m_Pos.y * 0.11f), -(angle / 180.0f) * D3DX_PI);

    //頂点座標の設定(2D座標)
    pVtx[4].pos = D3DXVECTOR3(Pos[0].x, Pos[0].y, 0.0f);
    pVtx[5].pos = D3DXVECTOR3(Pos[1].x, Pos[1].y, 0.0f);
    pVtx[6].pos = D3DXVECTOR3(Pos[2].x, Pos[2].y, 0.0f);
    pVtx[7].pos = D3DXVECTOR3(Pos[3].x, Pos[3].y, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[4].rhw =
    pVtx[5].rhw =
    pVtx[6].rhw =
    pVtx[7].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
    pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

    //テクスチャ座標の設定
    pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

    m_Pos = D3DXVECTOR3(SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 1.08f, 0.0f);
    m_Size = D3DXVECTOR3(SCREEN_WIDTH * 0.0224f * 9, SCREEN_HEIGHT * 0.005264f * 16, 0.0f);

    //頂点座標の設定(2D座標)
    pVtx[8].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, 0.0f);
    pVtx[9].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, 0.0f);
    pVtx[10].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, 0.0f);
    pVtx[11].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[8].rhw =
    pVtx[9].rhw =
    pVtx[10].rhw =
    pVtx[11].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャ座標の設定
    pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();

    int nItemNum = m_ItemNum;

    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < GEAR_DISIT_MAX; nCntDisit++)
    {
        //数値の設定
        m_apNumber[nCntDisit]->SetNumber(nItemNum % 10);

        //数値の描画処理
        m_apNumber[nCntDisit]->Update();

        nItemNum /= 10;
    }
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CGear::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        // 頂点バッファをデータストリームに設定
        pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

        // 頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_2D);

        // テクスチャの設定
        pDevice->SetTexture(0, m_pTexture[0]);

        // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            0,                                        //オフセット(頂点数)
            NUM_POLYGON);                             //プリミティブ数

       // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            4,                                        //オフセット(頂点数)
            NUM_POLYGON);                             //プリミティブ数

                                                      // テクスチャの設定
        pDevice->SetTexture(0, m_pTexture[1]);

        // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            8,                                        //オフセット(頂点数)
            NUM_POLYGON);                             //プリミティブ数

        int nItemNum = m_ItemNum;

        //描画する桁数カウント
        int Disit = 0;
        while (nItemNum >= 1)
        {
            nItemNum /= 10;
            Disit++;
        }

        //数値の描画処理
        m_apNumber[0]->Draw();

        //最高桁数分繰り返す
        for (int nCntDisit = 1; nCntDisit < Disit; nCntDisit++)
        {
            //数値の描画処理
            m_apNumber[nCntDisit]->Draw();
        }
    }
}

//***************************************************************************
// 自身を生成
//***************************************************************************
CGear *CGear::Create(D3DXVECTOR3 Pos, int nPriolity)
{
    CGear *pScene2D;                         //シーン２Dクラスのポインタ

    //動的確保
    pScene2D = new CGear(nPriolity);

    //各種初期化
    pScene2D->m_Pos = Pos;

    //テクスチャサイズを設定
    pScene2D->m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    pScene2D->m_TexSize = D3DXVECTOR2(1.0f, 1.0f);

    //初期化処理
    pScene2D->Init();

    return pScene2D;
}

//***************************************************************************
// 回転処理
//***************************************************************************
void CGear::rotation2D(float * xp, float * yp, float x, float y, float xc, float yc, float theta)
{
    *xp = (x - xc) * cosf(theta) - (y - yc) * sinf(theta) + xc;
    *yp = (x - xc) * sinf(theta) + (y - yc) * cosf(theta) + yc;
}

//***************************************************************************
// アイテム取得個数の設定
//***************************************************************************
void CGear::SetItemNum(int ItemNum)
{
    m_ItemNum = ItemNum;
}

//***************************************************************************
// アイテム取得個数の取得
//***************************************************************************
int CGear::GetItemNum(void)
{
    return m_ItemNum;
}