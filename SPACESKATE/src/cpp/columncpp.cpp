//*************************************************************************************************
// 円柱処理 (column.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "keyboard.h"
#include "mode.h"
#include "modeGame.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "track.h"
#include "player.h"
#include "column.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define MODELNAME        "data/MODEL/column.x"           //読み込むモデルの名前

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
// 円柱のコンストラクタ
//*************************************************************************************************
CColumn::CColumn(int nPriority) :CSceneModel(SCENE_TYPE_COLUMN, nPriority)
{
    m_pVtxBuff = NULL;
}

//*************************************************************************************************
// 円柱のデストラクタ
//*************************************************************************************************
CColumn::~CColumn()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CColumn::Init(void)
{
    //Xファイルの読み込み
    CSceneModel::LoadX(MODELNAME);

    //シーンモデルクラスの初期化処理
    CSceneModel::Init();

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
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

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CColumn::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    //シーンモデルクラスの終了処理
    CSceneModel::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CColumn::Update(void)
{
    //シーンモデルクラスの更新処理
    CSceneModel::Update();

    m_Pos = CModeGame::GetPlayer()->GetPos();

    // 頂点情報を設定
    VERTEX_2D* pVtx;                                //仮想アドレス用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定(2D座標)
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //rhwの設定(必ず1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //頂点カラー(0.0f～1.0fの整数値)
    pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

    //頂点バッファのロックを解除
    m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CColumn::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //レンダーステート(ステンシル+1)
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);             //ステンシルバッファ　有効
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);             //zバッファ　無効
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);             //カラーバッファ　無効
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);      //
    pDevice->SetRenderState(D3DRS_STENCILREF, 1);                   //
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);  //インクリメントで計算　+1
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           //表面を描画

    //シーンモデルクラスの描画処理
    CSceneModel::Draw();

    //レンダーステート(ステンシル-1)
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);  //デクリメントで計算　-1
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);            //裏面を描画

    //シーンモデルクラスの描画処理(裏)
    CSceneModel::Draw();

    //レンダーステート
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           //表面を描画
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);           //カラーバッファ　有効
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);       //ステンシルで1のみを描画

                                                                    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        // 頂点バッファをデータストリームに設定
        pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

        // 頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_2D);

        // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
            0,                                        //オフセット(頂点数)
            NUM_POLYGON);                             //プリミティブ数
    }

    //レンダーステート(元に戻す)
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              //zバッファ　有効
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);            //ステンシルバッファ　無効
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CColumn *CColumn::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority)
{
    CColumn *pScenePlayer;                         //シーン２Dクラスのポインタ

    //動的確保
    pScenePlayer = new CColumn(nPriority);

    //各種初期化
    pScenePlayer->m_Pos = Pos;
    pScenePlayer->m_Scl = Scl;

    //初期化処理
    pScenePlayer->Init();

    return pScenePlayer;
}

//*************************************************************************************************
// 回転の取得
//*************************************************************************************************
D3DXVECTOR3 CColumn::GetPos(void)
{
    return m_Pos;
}