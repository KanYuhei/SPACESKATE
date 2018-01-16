﻿//*************************************************************************************************
// 数値処理 (number.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "Manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "number.h"


//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME01   "data/TEXTURE/num2.png"  //読み込むテクスチャの名前

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;  //テクスチャへのポインタ

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// 数値のコンストラクタ
//*************************************************************************************************
CNumber::CNumber()
{
    m_pVtxBuff = NULL;
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(0.0f, 0.0f);
}

//*************************************************************************************************
// 数値のデストラクタ
//*************************************************************************************************
CNumber::~CNumber()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CNumber::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_2D) * NUM_VERTEX,     //作成したい頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                 //VertexBufferの使い方を指定
            FVF_VERTEX_2D,                      //頂点フォーマット
            D3DPOOL_MANAGED,                    //メモリ管理（誰が管理するか）
            &m_pVtxBuff,                        //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CNumber::Uninit(void)
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
void CNumber::Update(void)
{
    VERTEX_2D* pVtx;                            //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y , 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x , m_pos.y + m_size.y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //頂点カラー(0.0f～1.0)
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y );
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x , m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CNumber::Draw(void)
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

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CNumber *CNumber::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
    CNumber *pNumber;                             //数値クラスのポインタ

    //動的確保
    pNumber = new CNumber;

    //座標を設定
    pNumber->m_pos = Pos;

    //サイズを設定
    pNumber->m_size = Size;

    //テクスチャサイズを設定
    pNumber->m_TexSize = D3DXVECTOR2(0.2f, 0.5f);

    //初期化処理
    pNumber->Init();

    return pNumber;
}

//*************************************************************************************************
// テクスチャのロード
//*************************************************************************************************
HRESULT CNumber::Load(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        if (m_pTexture == NULL)               //←これだけでは不安かも
        {
            // テクスチャの生成
            if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME01, &m_pTexture)))
            {
                MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
            }
        }
    }

    return S_OK;
}

//*************************************************************************************************
// テクスチャのアンロード
//*************************************************************************************************
void CNumber::Unload(void)
{
    //テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//*************************************************************************************************
// 数値の設定
//*************************************************************************************************
void CNumber::SetNumber(int nNumber)
{
    VERTEX_2D* pVtx;                            //仮想アドレス用ポインタ

    //テクスチャ座標を設定
    m_Tex.x = 0.2f * nNumber;
    m_Tex.y = 0.5f * (nNumber >= 5);

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y );
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x , m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();
}