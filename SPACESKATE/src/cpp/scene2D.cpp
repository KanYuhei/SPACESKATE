﻿//*************************************************************************************************
// シーン2D処理 (scene2D.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene2D.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************

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
// シーン2Dのコンストラクタ
//*************************************************************************************************
CScene2D::CScene2D ( SCENE_TYPE type, int nPriolity ):CScene( type, nPriolity )
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
}

//*************************************************************************************************
// シーン2Dのデストラクタ
//*************************************************************************************************
CScene2D::~CScene2D ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CScene2D::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        // テクスチャの生成
        if ( FAILED ( D3DXCreateTextureFromFile ( pDevice,  m_TextureName, &m_pTexture ) ) )
        {
            MessageBox ( NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK );
        }

        // 頂点バッファの生成
        if ( FAILED (pDevice->CreateVertexBuffer ( 
            sizeof ( VERTEX_2D ) * NUM_VERTEX,     //作成したい頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                    //VertexBufferの使い方を指定
            FVF_VERTEX_2D,                         //頂点フォーマット
            D3DPOOL_MANAGED,                       //メモリ管理（誰が管理するか）
            &m_pVtxBuff,                           //頂点バッファ管理インターフェースポインタのアドレス
            NULL ) ) )
        {
            return E_FAIL;
        }
    }

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
    pVtx[0].col = m_Color;
    pVtx[1].col = m_Color;
    pVtx[2].col = m_Color;
    pVtx[3].col = m_Color;

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CScene2D::Uninit(void)
{
    // 頂点バッファの破棄
    if ( m_pVtxBuff != NULL )
    {
        m_pVtxBuff->Release();
        m_pVtxBuff= NULL;
    }

    // テクスチャの破棄
    if ( m_pTexture != NULL )
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CScene2D::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CScene2D::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        // 頂点バッファをデータストリームに設定
        pDevice->SetStreamSource ( 0, m_pVtxBuff, 0, sizeof ( VERTEX_2D ) );

        // 頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_2D);

        // テクスチャの設定
        pDevice->SetTexture ( 0, m_pTexture );

        // ポリゴンの描画
        pDevice->DrawPrimitive ( D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            0,                                          //オフセット(頂点数)
            NUM_POLYGON );                              //プリミティブ数
    }
}

//***************************************************************************
// 自身を生成
//***************************************************************************
CScene2D *CScene2D::Create ( char TexName[256], D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, D3DXVECTOR2 Tex, D3DXVECTOR2 TexSize, int nPriolity )
{
    CScene2D *pScene2D;                         //シーン２Dクラスのポインタ

    //動的確保
    pScene2D = new CScene2D(SCENE_TYPE_2D, nPriolity );

    //各種初期化
    strcpy ( pScene2D->m_TextureName, TexName);
    pScene2D->m_Pos = Pos;
    pScene2D->m_Size = Size;
    pScene2D->m_Tex = Tex;
    pScene2D->m_TexSize = TexSize;
    pScene2D->m_Color = Color;

    //初期化処理
    pScene2D->Init ( );

    return pScene2D;
}