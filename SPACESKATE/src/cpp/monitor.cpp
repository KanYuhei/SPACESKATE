//*************************************************************************************************
// モニター処理 (monitor.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "mode.h"
#include "scene.h"
#include "scene3D.h"
#include "monitor.h"

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
// モニターのコンストラクタ
//*************************************************************************************************
CMonitor::CMonitor(int nPriority) :CScene3D(SCENE_TYPE_MONITOR, nPriority)
{
    m_RenderTargetTexture = NULL;
    m_RenderTargetSurface = NULL;
    m_BackBufferSurface = NULL;
}

//*************************************************************************************************
// モニターのデストラクタ
//*************************************************************************************************
CMonitor::~CMonitor()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMonitor::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //頂点バッファの作成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_3D) * NUM_VERTEX,         //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                     //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                          //頂点フォーマット
            D3DPOOL_MANAGED,                        //メモリ管理方法
            &m_pVtxBuffer,                          //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }

        //テクスチャ生成
        D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture);

        //サーフェスの取得
        m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);

        //レンダーターゲット取得
        pDevice->GetRenderTarget(0, &m_BackBufferSurface);
    }

    m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f, 1.0f);
    m_Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMonitor::Uninit(void)
{
    // レンダーターゲットテクスチャの破棄
    if (m_RenderTargetTexture != NULL)
    {
        m_RenderTargetTexture->Release();
        m_RenderTargetTexture = NULL;
    }

    // レンダーターゲットサーフェスの破棄
    if (m_RenderTargetSurface != NULL)
    {
        m_RenderTargetSurface->Release();
        m_RenderTargetSurface = NULL;
    }

    // バックバッファサーフェスの破棄
    if (m_BackBufferSurface != NULL)
    {
        m_BackBufferSurface->Release();
        m_BackBufferSurface = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMonitor::Update(void)
{
    //頂点情報を設定
    VERTEX_3D* pVtx;                            //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(3D座標)
    pVtx[0].pos = D3DXVECTOR3(m_Pos.x,            m_Pos.y + m_Size.y, m_Pos.z);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, m_Pos.z);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x,            m_Pos.y,            m_Pos.z);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y,            m_Pos.z);

    //ライトベクトルの設定
    pVtx[0].normal = m_Normal;
    pVtx[1].normal = m_Normal;
    pVtx[2].normal = m_Normal;
    pVtx[3].normal = m_Normal;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[0].color = m_Color;
    pVtx[1].color = m_Color;
    pVtx[2].color = m_Color;
    pVtx[3].color = m_Color;

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMonitor::Draw(void)
{
    ////Direct3Dデバイスの取得
    //LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    ////Direct3DデバイスのNULLチェック
    //if (pDevice != NULL && m_RenderTargetTexture != NULL)
    //{
    //    //レンダーターゲットの設定(サーフェス)
    //    pDevice->SetRenderTarget(0, m_RenderTargetSurface);

    //    //レンダラクラスの描画処理開始
    //    CRenderer::DrawBegin();

    //    //モードクラスの描画処理
    //    CManager::GetMode()->Draw();

    //    //指定した優先度を全て描画
    //    CScene::DrawPriolityAll(3);

    //    //描画の終了
    //    pDevice->EndScene();

    //    // 頂点バッファをデータストリームに設定
    //    pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

    //    // 頂点フォーマットの設定
    //    pDevice->SetFVF(FVF_VERTEX_3D);

    //    // テクスチャの設定
    //    pDevice->SetTexture(0, m_RenderTargetTexture);

    //    // ポリゴンの描画
    //    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
    //        0,                                        //オフセット(頂点数)
    //        NUM_POLYGON);                             //プリミティブ数
    //}
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CMonitor *CMonitor::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, int nPriority)
{
    CMonitor *pMeshField;                     //モニタークラスのポインタ

    //動的確保
    pMeshField = new CMonitor(nPriority);

    //各種初期化
    pMeshField->m_Pos = Pos;
    pMeshField->m_Size = Size;
    pMeshField->m_Color = Color;

    //初期化処理
    pMeshField->Init();

    return pMeshField;
}