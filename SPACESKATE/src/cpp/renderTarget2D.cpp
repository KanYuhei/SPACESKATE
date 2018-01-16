//*************************************************************************************************
// レンダーターゲット2D処理 (renderTarget2D.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "renderTarget2D.h"

//*************************************************************************************************
// 定数定義
//*************************************************************************************************

//*************************************************************************************************
// 列挙体
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************

//*************************************************************************************************
// レンダーターゲット2Dのコンストラクタ
//*************************************************************************************************
CRenderTarget2D::CRenderTarget2D()
{
    m_RenderTargetTexture = NULL;
    m_RenderTargetTexture1 = NULL;
    m_RenderTargetTexture2 = NULL;
    m_RenderTargetSurface = NULL;
    m_RenderTargetSurface1 = NULL;
    m_RenderTargetSurface2 = NULL;
    m_BackBufferSurface = NULL;
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*************************************************************************************************
// レンダーターゲット2Dのデストラクタ
//*************************************************************************************************
CRenderTarget2D::~CRenderTarget2D()
{
}

//*************************************************************************************************
//初期化処理
//*************************************************************************************************
HRESULT CRenderTarget2D::Init(void)
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

    m_pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);
    m_size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    VERTEX_2D* pVtx;                            //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

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
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();

    //テクスチャ生成
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture);
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture1);
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture2);

    //サーフェスの取得
    m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);
    m_RenderTargetTexture1->GetSurfaceLevel(0, &m_RenderTargetSurface1);
    m_RenderTargetTexture2->GetSurfaceLevel(0, &m_RenderTargetSurface2);

    //レンダーターゲット取得
    pDevice->GetRenderTarget(0, &m_BackBufferSurface);

    m_FeedbackBlur = false;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CRenderTarget2D::Uninit(void)
{
    // レンダーターゲットテクスチャ2の破棄
    if (m_RenderTargetTexture2 != NULL)
    {
        m_RenderTargetTexture2->Release();
        m_RenderTargetTexture2 = NULL;
    }

    // レンダーターゲットテクスチャ1の破棄
    if (m_RenderTargetTexture1 != NULL)
    {
        m_RenderTargetTexture1->Release();
        m_RenderTargetTexture1 = NULL;
    }

    // レンダーターゲットテクスチャの破棄
    if (m_RenderTargetTexture != NULL)
    {
        m_RenderTargetTexture->Release();
        m_RenderTargetTexture = NULL;
    }

    // レンダーターゲットサーフェス2の破棄
    if (m_RenderTargetSurface2 != NULL)
    {
        m_RenderTargetSurface2->Release();
        m_RenderTargetSurface2 = NULL;
    }

    // レンダーターゲットサーフェス1の破棄
    if (m_RenderTargetSurface1 != NULL)
    {
        m_RenderTargetSurface1->Release();
        m_RenderTargetSurface1 = NULL;
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

    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CRenderTarget2D::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CRenderTarget2D::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //フィードバックブラー
        if (m_FeedbackBlur)
        {
            VERTEX_2D* pVtx;                            //仮想アドレス用ポインタ

            //頂点バッファをロックして、仮想アドレスを取得する
            m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

            //全画面より少し大きく
            D3DXVECTOR3 size = D3DXVECTOR3(m_size.x * 1.01f, m_size.y * 1.01f, 0.0f);

            //頂点座標の設定(2D座標)
            pVtx[0].pos = D3DXVECTOR3(m_pos.x - size.x / 2, m_pos.y - size.y / 2, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(m_pos.x + size.x / 2, m_pos.y - size.y / 2, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(m_pos.x - size.x / 2, m_pos.y + size.y / 2, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(m_pos.x + size.x / 2, m_pos.y + size.y / 2, 0.0f);

            //頂点カラー(0.0f～1.0)
            pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);

            //頂点バッファのロックを解除
            m_pVtxBuff->Unlock();

            //レンダーターゲットの設定(サーフェス2)
            pDevice->SetRenderTarget(0, m_RenderTargetSurface2);

            //レンダラクラスの描画処理開始
            CRenderer::DrawBegin();

            //モードクラスの描画処理
            CManager::GetMode()->Draw();

            //シーンクラスの全ての描画処理
            CScene::DrawAll();

            // 頂点バッファをデータストリームに設定
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // 頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_2D);

            // テクスチャの設定
            pDevice->SetTexture(0, m_RenderTargetTexture1);

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                0,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数

            //描画の終了
            pDevice->EndScene();


            //頂点バッファをロックして、仮想アドレスを取得する
            m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

            //頂点座標の設定(2D座標)
            pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

            //頂点カラー(0.0f～1.0)
            pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            //頂点バッファのロックを解除
            m_pVtxBuff->Unlock();

            //レンダーターゲットの設定(バックバッファ)
            pDevice->SetRenderTarget(0, m_BackBufferSurface);

            //レンダラクラスの描画処理開始
            CRenderer::DrawBegin();

            // 頂点バッファをデータストリームに設定
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // 頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_2D);

            // テクスチャの設定
            pDevice->SetTexture(0, m_RenderTargetTexture2);

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); //アルファブレンド(OFF)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  //アルファテスト(OFF)

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                0,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //アルファブレンド(ON)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);  //アルファテスト(ON)

            //レンダラクラスの描画処理終了
            CRenderer::DrawEnd();

            //テクスチャ入れ替え
            LPDIRECT3DTEXTURE9 texture = m_RenderTargetTexture1;
            m_RenderTargetTexture1 = m_RenderTargetTexture2;
            m_RenderTargetTexture2 = texture;

            //サーフェス入れ替え
            LPDIRECT3DSURFACE9 surface = m_RenderTargetSurface1;
            m_RenderTargetSurface1 = m_RenderTargetSurface2;
            m_RenderTargetSurface2 = surface;
        }
        else
        {
            //レンダーターゲットの設定(サーフェス)
            pDevice->SetRenderTarget(0, m_RenderTargetSurface);

            //レンダラクラスの描画処理開始
            CRenderer::DrawBegin();

            //モードクラスの描画処理
            CManager::GetMode()->Draw();

            //シーンクラスの全ての描画処理
            CScene::DrawAll();

            //描画の終了
            pDevice->EndScene();

            //レンダーターゲットの設定(バックバッファ)
            pDevice->SetRenderTarget(0, m_BackBufferSurface);

            //レンダラクラスの描画処理開始
            CRenderer::DrawBegin();

            // 頂点バッファをデータストリームに設定
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // 頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_2D);

            // テクスチャの設定
            pDevice->SetTexture(0, m_RenderTargetTexture);

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); //アルファブレンド(OFF)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  //アルファテスト(OFF)

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                0,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //アルファブレンド(ON)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);  //アルファテスト(ON)

            //レンダラクラスの描画処理終了
            CRenderer::DrawEnd();
        }
    }
}

//*************************************************************************************************
// フィードバックブラーON/OFF
//*************************************************************************************************
void CRenderTarget2D::IsFeedbackBlur(bool flag)
{
    m_FeedbackBlur = flag;
}

//*************************************************************************************************
// フィードバックブラーフラグの取得
//*************************************************************************************************
bool CRenderTarget2D::GetFeedbackBlur(void)
{
    return m_FeedbackBlur;
}