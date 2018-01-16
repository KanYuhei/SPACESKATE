﻿//*************************************************************************************************
// シーン3D処理 (scene3D.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene3D.h"

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
// シーン3Dのコンストラクタ
//*************************************************************************************************
CScene3D::CScene3D ( SCENE_TYPE type, int nPriolity ):CScene( type, nPriolity )
{
    m_pVtxBuffer = NULL;
    m_pIdxBuffer = NULL;
    m_pTexture = NULL;
    m_Normal = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
}

//*************************************************************************************************
// シーン3Dのデストラクタ
//*************************************************************************************************
CScene3D::~CScene3D ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CScene3D::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        //頂点バッファの作成
        if ( FAILED (pDevice->CreateVertexBuffer ( 
            sizeof ( VERTEX_3D ) * NUM_VERTEX,      //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                     //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                          //頂点フォーマット
            D3DPOOL_MANAGED,                        //メモリ管理方法
            &m_pVtxBuffer,                          //頂点バッファ管理インターフェースポインタのアドレス
            NULL ) ) )
        {
            return E_FAIL;
        }
    }

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CScene3D::Uninit(void)
{
    //テクスチャインターフェースの解放
    if ( m_pTexture != NULL )
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }

    //インデックスバッファインターフェースの解放
    if ( m_pIdxBuffer != NULL )
    {
        m_pIdxBuffer->Release();
        m_pIdxBuffer= NULL;
    }

    //頂点バッファ管理インターフェースの解放
    if ( m_pVtxBuffer != NULL )
    {
        m_pVtxBuffer->Release();
        m_pVtxBuffer= NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CScene3D::Update(void)
{
    //頂点情報を設定
    VERTEX_3D* pVtx;                            //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock ( 0, 0, (void**)&pVtx, 0 );

    //頂点座標の設定(3D座標)
    pVtx[ 0 ].pos = D3DXVECTOR3 ( m_Pos.x,            m_Pos.y, m_Pos.z + m_Size.z );
    pVtx[ 1 ].pos = D3DXVECTOR3 ( m_Pos.x + m_Size.x, m_Pos.y, m_Pos.z + m_Size.z );
    pVtx[ 2 ].pos = D3DXVECTOR3 ( m_Pos.x,            m_Pos.y, m_Pos.z );
    pVtx[ 3 ].pos = D3DXVECTOR3 ( m_Pos.x + m_Size.x, m_Pos.y, m_Pos.z );

    //ライトベクトルの設定
    pVtx[ 0 ].normal = m_Normal;
    pVtx[ 1 ].normal = m_Normal;
    pVtx[ 2 ].normal = m_Normal;
    pVtx[ 3 ].normal = m_Normal;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[ 0 ].color = m_Color;
    pVtx[ 1 ].color = m_Color;
    pVtx[ 2 ].color = m_Color;
    pVtx[ 3 ].color = m_Color;

    //テクスチャ座標の設定
    pVtx[ 0 ].tex = D3DXVECTOR2 ( m_Tex.x,               m_Tex.y );
    pVtx[ 1 ].tex = D3DXVECTOR2 ( m_Tex.x + m_TexSize.x, m_Tex.y );
    pVtx[ 2 ].tex = D3DXVECTOR2 ( m_Tex.x,               m_Tex.y + m_TexSize.y );
    pVtx[ 3 ].tex = D3DXVECTOR2 ( m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y );

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock ( );
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CScene3D::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        // 頂点バッファをデータストリームに設定
        pDevice->SetStreamSource ( 0, m_pVtxBuffer, 0, sizeof ( VERTEX_3D ) );

        // 頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_3D);

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
CScene3D *CScene3D::Create ( D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, D3DXVECTOR2 Tex, D3DXVECTOR2 TexSize, int nPriority )
{
    CScene3D *pScene3D;                         //シーン２Dクラスのポインタ

    //動的確保
    pScene3D = new CScene3D( SCENE_TYPE_3D, nPriority );

    //各種初期化
    pScene3D->m_Pos = Pos;
    pScene3D->m_Size = Size;
    pScene3D->m_Tex = Tex;
    pScene3D->m_TexSize = TexSize;
    pScene3D->m_Color = Color;

    //初期化処理
    pScene3D->Init ( );

    return pScene3D;
}