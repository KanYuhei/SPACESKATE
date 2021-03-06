﻿//*************************************************************************************************
// プレスエンター処理 (pressenter.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene2D.h"
#include "pressenter.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00   "data/TEXTURE/pressanybutton.png"      //プレスエンターテクスチャ

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
// プレスエンターのコンストラクタ
//*************************************************************************************************
CPressenter::CPressenter(int nPriolity) :CScene2D(SCENE_TYPE_PRESSENTER, nPriolity)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
}

//*************************************************************************************************
// プレスエンターのデストラクタ
//*************************************************************************************************
CPressenter::~CPressenter()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CPressenter::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        // テクスチャの生成
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME00, &m_pTexture)))
        {
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
        }

        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_2D) * NUM_VERTEX,        //作成したい頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                    //VertexBufferの使い方を指定
            FVF_VERTEX_2D,                         //頂点フォーマット
            D3DPOOL_MANAGED,                       //メモリ管理（誰が管理するか）
            &m_pVtxBuff,                           //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }
    }

    m_Pos = D3DXVECTOR3((SCREEN_WIDTH - (SCREEN_WIDTH * 9.0f * 0.0353f)) / 2.0f, (SCREEN_HEIGHT - (SCREEN_HEIGHT * 16.0f * 0.0033f)) / 1.3f, 0.0f);
    m_Size = D3DXVECTOR3(SCREEN_WIDTH * 9 * 0.0353f, SCREEN_HEIGHT * 16 * 0.0033f, 0.0f);
    m_ColorA = 1.0f;
    m_ColorA_fade = 0.01f;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CPressenter::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CPressenter::Update(void)
{
    m_ColorA -= m_ColorA_fade;
    if (m_ColorA <= 0.0f || m_ColorA >= 1.0f) m_ColorA_fade *= -1;

    // 頂点情報を設定
    VERTEX_2D* pVtx;                                //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CPressenter::Draw(void)
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
        pDevice->SetTexture(0, m_pTexture);

        // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            0,                                        //オフセット(頂点数)
            NUM_POLYGON);                             //プリミティブ数
    }
}

//***************************************************************************
// 自身を生成
//***************************************************************************
CPressenter *CPressenter::Create(D3DXVECTOR3 Pos, int nPriolity)
{
    CPressenter *pScene2D;                         //シーン２Dクラスのポインタ

                                                //動的確保
    pScene2D = new CPressenter(nPriolity);

    //各種初期化
    pScene2D->m_Pos = Pos;

    //テクスチャサイズを設定
    pScene2D->m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    pScene2D->m_TexSize = D3DXVECTOR2(1.0f, 1.0f);

    //初期化処理
    pScene2D->Init();

    return pScene2D;
}