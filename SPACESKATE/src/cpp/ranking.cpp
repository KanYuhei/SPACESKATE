//*************************************************************************************************
// ランキング処理 (ranking.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene2D.h"
#include "ranking.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00   "data/TEXTURE/GAME/rankingNum.png"               //ランキングテクスチャ

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
// ランキングのコンストラクタ
//*************************************************************************************************
CRanking::CRanking(int nPriolity) :CScene2D(SCENE_TYPE_RANKING, nPriolity)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    m_Ranking = 0;
    m_ColorA = 0.8f;
    m_CountDawnStart = 0;
    m_PlayChange = false;
    m_angle = 0.0f;
}

//*************************************************************************************************
// ランキングのデストラクタ
//*************************************************************************************************
CRanking::~CRanking()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CRanking::Init(void)
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

    m_Ranking = 0;
    m_Size = D3DXVECTOR3(SCREEN_WIDTH * 0.018f * 9, SCREEN_HEIGHT * 0.018f * 16, 0.0f);
    m_bCount = false;
    m_PlayChange = false;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CRanking::Uninit(void)
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
void CRanking::Update(void)
{
    //カメラがプレイヤーの後ろに周るまでの時間
    m_CountDawnStart++;
    if (m_CountDawnStart == 200) m_bCount = true;

    //順位変動アニメーション
    if (m_PlayChange == true)
    {
        m_Size.x *= 0.7f;
        m_Size.y *= 0.7f;
        m_ColorA -= 0.01f;
        m_angle += 20.0f;
        if (m_Size.x <= SCREEN_WIDTH * 0.0009f * 9)
        {
            m_PlayChange = false;
        }
    }
    else
    {
        m_Size.x *= 1.3f;
        m_Size.y *= 1.3f;
        m_ColorA += 0.01f;
        m_angle = 0.0f;
        if (m_Size.x >= SCREEN_WIDTH * 0.018f * 9)
        {
            m_Size = D3DXVECTOR3(SCREEN_WIDTH * 0.018f * 9, SCREEN_HEIGHT * 0.018f * 16, 0.0f);
            m_ColorA = 1.0f;
        }
    }

    // 頂点情報を設定
    VERTEX_2D* pVtx;                                //仮想アドレス用ポインタ

    //テクスチャ座標を設定
    m_Tex.x = 0.2f * m_Ranking;
    m_Tex.y = 0.5f * (m_Ranking >= 5);

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    D3DXVECTOR2  Pos[4];
    rotation2D(&Pos[0].x, &Pos[0].y, m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.x, m_Pos.y, (m_angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[1].x, &Pos[1].y, m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.x, m_Pos.y, (m_angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[2].x, &Pos[2].y, m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.x, m_Pos.y, (m_angle / 180.0f) * D3DX_PI);
    rotation2D(&Pos[3].x, &Pos[3].y, m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.x, m_Pos.y, (m_angle / 180.0f) * D3DX_PI);

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
void CRanking::Draw(void)
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
            pDevice->SetTexture(0, m_pTexture);

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                0,                                        //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数
        }
    }
}

//***************************************************************************
// 自身を生成
//***************************************************************************
CRanking *CRanking::Create(D3DXVECTOR3 Pos, int nPriolity)
{
    CRanking *pScene2D;                         //シーン２Dクラスのポインタ

    //動的確保
    pScene2D = new CRanking(nPriolity);

    //各種初期化
    pScene2D->m_Pos = Pos;

    //テクスチャサイズを設定
    pScene2D->m_TexSize = D3DXVECTOR2(0.2f, 0.5f);

    //初期化処理
    pScene2D->Init();

    return pScene2D;
}

//***************************************************************************
// ランキング数値の取得
//***************************************************************************
int CRanking::GetRanking(void)
{
    return m_Ranking;
}

//***************************************************************************
// ランキング数値の取得
//***************************************************************************
void CRanking::SetRanking(int Num)
{
    m_Ranking = Num;
    m_PlayChange = true;
}

//***************************************************************************
// 回転処理
//***************************************************************************
void CRanking::rotation2D(float * xp, float * yp, float x, float y, float xc, float yc, float theta)
{
    *xp = (x - xc) * cosf(theta) - (y - yc) * sinf(theta) + xc;
    *yp = (x - xc) * sinf(theta) + (y - yc) * cosf(theta) + yc;
}