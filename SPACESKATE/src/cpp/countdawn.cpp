//*************************************************************************************************
// カウントダウン処理 (countdawn.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "sound.h"
#include "scene.h"
#include "scene2D.h"
#include "countdawn.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00   "data/TEXTURE/num3.png"                    //カウントダウンテクスチャ
#define TEXTURENAME01   "data/TEXTURE/GO.png"                      //カウントダウンテクスチャ

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
// カウントダウンのコンストラクタ
//*************************************************************************************************
CCountdawn::CCountdawn(int nPriolity) :CScene2D(SCENE_TYPE_COUNTDAWN, nPriolity)
{
    m_CountDawn = 0;
    for (int i = 0; i < 2; i++)
        m_pTexture[i] = NULL;
    m_pVtxBuff = NULL;
    m_FrameCount = 0;
    m_ColorA = 1.0f;
    m_CountDawnStart = 0;
}

//*************************************************************************************************
// カウントダウンのデストラクタ
//*************************************************************************************************
CCountdawn::~CCountdawn()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CCountdawn::Init(void)
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
            sizeof(VERTEX_2D) * NUM_VERTEX * 2,     //作成したい頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                    //VertexBufferの使い方を指定
            FVF_VERTEX_2D,                         //頂点フォーマット
            D3DPOOL_MANAGED,                       //メモリ管理（誰が管理するか）
            &m_pVtxBuff,                           //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }
    }

    m_CountDawn = 3;
    m_Size = D3DXVECTOR3(12200.0f, 12200.0f, 0.0f);
    m_bCount = false;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CCountdawn::Uninit(void)
{
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
void CCountdawn::Update(void)
{
    m_CountDawnStart++;
    if (m_CountDawnStart == 200)
    {
        CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDAWN);
        m_bCount = true;
    }
    if (m_CountDawnStart >= 200)
    {
        m_FrameCount++;
        m_Size.x -= 800.0f;
        m_Size.y -= 800.0f;
    }
    if (m_Size.x <= 100.0f)
    {
        m_Size = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
        m_ColorA -= 0.01f;
    }

    if (m_FrameCount == 60)
    {
        if (m_bCount == true)
        {
            m_CountDawn--;
            m_Size = D3DXVECTOR3(12200.0f, 12200.0f, 0.0f);
            m_ColorA = 1.0f;
            if (m_CountDawn == 0)
            {
                CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_START);
                m_bCount = false;
            }
            else
            {
                CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDAWN);
            }
        }
        else if (m_ColorA <= 0.0f)
        {
            Release();
        }
        m_FrameCount = 0;
    }

    // 頂点情報を設定
    VERTEX_2D* pVtx;                                //仮想アドレス用ポインタ

    //テクスチャ座標を設定
    m_Tex.x = 0.2f * m_CountDawn;
    m_Tex.y = 0.5f * (m_CountDawn >= 5);

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x/2, m_Pos.y - m_Size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y/2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x/2, m_Pos.y + m_Size.y/2, 0.0f);

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

    //頂点座標の設定(2D座標)
    pVtx[4].pos = D3DXVECTOR3(m_Pos.x - m_Size.x * 1.8f / 2, m_Pos.y - m_Size.y * 0.794f / 2, 0.0f);
    pVtx[5].pos = D3DXVECTOR3(m_Pos.x + m_Size.x* 1.8f / 2, m_Pos.y - m_Size.y* 0.794f / 2, 0.0f);
    pVtx[6].pos = D3DXVECTOR3(m_Pos.x - m_Size.x* 1.8f / 2, m_Pos.y + m_Size.y* 0.794f / 2, 0.0f);
    pVtx[7].pos = D3DXVECTOR3(m_Pos.x + m_Size.x * 1.8f / 2, m_Pos.y + m_Size.y* 0.794f / 2, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[4].rhw =
    pVtx[5].rhw =
    pVtx[6].rhw =
    pVtx[7].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);
    pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ColorA);

    //テクスチャ座標の設定
    pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CCountdawn::Draw(void)
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
        if (m_bCount == true)
        {
            // テクスチャの設定
            pDevice->SetTexture(0, m_pTexture[0]);

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                0,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数
        }
        else
        {
            // テクスチャの設定
            pDevice->SetTexture(0, m_pTexture[1]);

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                4,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数
        }
    }
}

//***************************************************************************
// 自身を生成
//***************************************************************************
CCountdawn *CCountdawn::Create( D3DXVECTOR3 Pos, int nPriolity)
{
    CCountdawn *pScene2D;                         //シーン２Dクラスのポインタ

    //動的確保
    pScene2D = new CCountdawn(nPriolity);

    //各種初期化
    pScene2D->m_Pos = Pos;

    //テクスチャサイズを設定
    pScene2D->m_TexSize = D3DXVECTOR2(0.2f, 0.5f);

    //初期化処理
    pScene2D->Init();

    return pScene2D;
}

//***************************************************************************
// カウントダウン数値の取得
//***************************************************************************
int CCountdawn::GetCountDawn(void)
{
    return m_CountDawn;
}