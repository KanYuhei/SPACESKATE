//*************************************************************************************************
// トラック処理 (track.cpp)
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
#include "scene3D.h"
#include "meshField.h"
#include "sceneModel.h"
#include "player.h"
#include "player2.h"
#include "track.h"
#include "emitter.h"
#include "particle.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define MODELNAME        "data/MODEL/track.x"           //読み込むモデルの名前
#define MODELNAME2       "data/MODEL/trackColision.x"   //読み込むモデルの名前

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
// トラックのコンストラクタ
//*************************************************************************************************
CTrack::CTrack(int nPriority) :CSceneModel(SCENE_TYPE_TRACK, nPriority)
{
    for (int i = 0; i < CHECKPOINT_NUM; i++)
    {
        m_CheckPointStart[i] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
        m_CheckPointEnd[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }
    m_EmitterCount = 0;
}

//*************************************************************************************************
// トラックのデストラクタ
//*************************************************************************************************
CTrack::~CTrack()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CTrack::Init(void)
{
    //Xファイルの読み込み
    CSceneModel::LoadX(MODELNAME);

    //シーンモデルクラスの初期化処理
    CSceneModel::Init();

    //////////////////////////////////////////////////////////
    //  頂点座標の取得
    //////////////////////////////////////////////////////////

    //  メッシュの頂点データ取得
    VERTEX *pVtx = NULL;

    int nNumVertex = m_pMesh->GetNumVertices();

    //  メッシュから頂点情報を取得
    m_pMesh->LockVertexBuffer(0, (void**)&pVtx);

    //  頂点数分のループ
    for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
    {
        //  座標を一時格納
        m_vtxPos[nCntVtx] = pVtx->pos;

        pVtx++;
    }
    
    //  頂点バッファのアンロック
    m_pMesh->UnlockVertexBuffer();

    //////////////////////////////////////////////////////////
    //  頂点インデックスの取得
    //////////////////////////////////////////////////////////

    int nNumFace = m_pMesh->GetNumFaces();

    WORD* pIndexBuffer;

    //  インデックスバッファのロック
    m_pMesh->LockIndexBuffer(0, (void**)&pIndexBuffer);

    //  頂点数分のループ
    for (int nCntFace = 0; nCntFace < nNumFace; nCntFace++)
    {
        //  インデックス番号を一時格納
        m_vtxIndex[nCntFace * 3 + 0] = pIndexBuffer[nCntFace * 3 + 0];
        m_vtxIndex[nCntFace * 3 + 1] = pIndexBuffer[nCntFace * 3 + 1];
        m_vtxIndex[nCntFace * 3 + 2] = pIndexBuffer[nCntFace * 3 + 2];
    }

    //  インデックスバッファのアンロック
    m_pMesh->UnlockIndexBuffer();


    //チェックポイント用モデル読み込み///////////////////////////////
    LPD3DXMESH pMesh;                           //メッシュ情報インターフェース
    LPD3DXBUFFER pBuffMat;                      //マテリアル情報
    DWORD nNumMat;                              //マテリアル情報の数

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //モデルの読み込み
        if (FAILED(D3DXLoadMeshFromX(
            MODELNAME2,                         //読み込むXファイル名
            D3DXMESH_MANAGED,                   //頂点/インデックス・バッファをシステム・メモリ上に作る
            pDevice,                            //メッシュ描画に使うデバイス
            NULL,                               //Xファイルの隣接情報
            &pBuffMat,                          //Xファイルのマテリアル情報
            NULL,                               //
            &nNumMat,                           //マテリアル情報の数
            &pMesh)))                           //メッシュ情報インターフェース
        {
            MessageBox(NULL, "モデルの読み込みに失敗しました", "終了メッセージ", MB_OK);
            return E_FAIL;
        }

        int Cnt = 0;
        nNumVertex = pMesh->GetNumVertices();

        //  メッシュから頂点情報を取得
        pMesh->LockVertexBuffer(0, (void**)&pVtx);

        //  頂点数分のループ
        for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx += 4, Cnt++, pVtx++)
        {
            //チェックポイント初期化
            m_CheckPointStart[Cnt] = pVtx->pos;
            pVtx++;
            pVtx++;
            pVtx++;
            m_CheckPointEnd[Cnt] = pVtx->pos;
        }

        //  頂点バッファのアンロック
        pMesh->UnlockVertexBuffer();
    }

    //ゴール
    m_CheckPointStart[CHECKPOINT_NUM-1] = D3DXVECTOR3(0.0f, 41.0f, 3300.0f);
    m_CheckPointStart[CHECKPOINT_NUM-1] = D3DXVECTOR3(0.0f, 41.0f, 2250.0f);

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CTrack::Uninit(void)
{
    //シーンモデルクラスの終了処理
    CSceneModel::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CTrack::Update(void)
{
    //シーンモデルクラスの更新処理
    CSceneModel::Update();

    //エフェクト処理
    EMITTER Emitter;
    Emitter.Pos = D3DXVECTOR3(0.0f, 0.0f, 2300.0f + (float)(rand() % 1000));
    Emitter.Speed = D3DXVECTOR3(0.0f, 0.6f, 0.0f);
    Emitter.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    Emitter.Size = D3DXVECTOR2(10.0f, 10.0f);
    Emitter.Color = D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.7f);
    Emitter.nLife = 30;
    Emitter.nNum = 5;

    //前回の座標と違う場合
    if (m_EmitterCount == 3)
    {
        /*CEmitter *emitter = new CEmitter;
        emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());
        m_EmitterCount = 0;*/
    }
    m_EmitterCount++;
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CTrack::Draw(void)
{
    //シーンモデルクラスの描画処理
    CSceneModel::Draw();
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CTrack *CTrack::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority)
{
    CTrack *pScenePlayer;                         //シーン２Dクラスのポインタ

    //動的確保
    pScenePlayer = new CTrack(nPriority);

    //各種初期化
    pScenePlayer->m_Pos = Pos;
    pScenePlayer->m_Scl = Scl;

    //初期化処理
    pScenePlayer->Init();

    return pScenePlayer;
}

//*************************************************************************************************
// 座標の取得
//*************************************************************************************************
D3DXVECTOR3 CTrack::GetPos(void)
{
    return m_Pos;
}

//*************************************************************************************************
// 地面の高さ取得
//*************************************************************************************************
float CTrack::GetHeight(D3DXVECTOR3 Pos)
{
    D3DXVECTOR3 vec1(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vec2(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 cross(0.0f, 0.0f, 0.0f);

    D3DXVECTOR3 vec3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vec4(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 cross2(0.0f, 0.0f, 0.0f);

    D3DXVECTOR3 vec5(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vec6(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 cross3(0.0f, 0.0f, 0.0f);

    D3DXVECTOR3 normal(0.0f, 0.0f, 0.0f);

    int nNumFace = m_pMesh->GetNumFaces();

    const float fRange = 400.0f;

#pragma omp parallel for
    //  頂点数分のループ
    for (int nCntFace = 0; nCntFace < nNumFace; nCntFace++)
    {
        vec1 = m_vtxPos[m_vtxIndex[nCntFace * 3 + 0]] - m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]];
        vec2 = Pos - m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]];

        //  2つのベクトルのなす直交ベクトルを求める
        D3DXVec3Cross(&cross, &vec1, &vec2);

        //  単位ベクトル化して法線ベクトルに変換
        D3DXVec3Normalize(&cross, &cross);

        //  法線が上向き方向の場合
        if (cross.y >= 0.0f)
        {
            vec3 = m_vtxPos[m_vtxIndex[nCntFace * 3 + 1]] - m_vtxPos[m_vtxIndex[nCntFace * 3 + 0]];
            vec4 = Pos - m_vtxPos[m_vtxIndex[nCntFace * 3 + 0]];

            //  2つのベクトルのなす直交ベクトルを求める
            D3DXVec3Cross(&cross2, &vec3, &vec4);

            //  単位ベクトル化して法線ベクトルに変換
            D3DXVec3Normalize(&cross2, &cross2);

            //  法線が上向き方向の場合
            if (cross2.y >= 0.0f)
            {
                vec5 = m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]] - m_vtxPos[m_vtxIndex[nCntFace * 3 + 1]];
                vec6 = Pos - m_vtxPos[m_vtxIndex[nCntFace * 3 + 1]];

                //  2つのベクトルのなす直交ベクトルを求める
                D3DXVec3Cross(&cross3, &vec5, &vec6);

                //  単位ベクトル化して法線ベクトルに変換
                D3DXVec3Normalize(&cross3, &cross3);

                //  法線が上向き方向の場合
                if (cross3.y >= 0.0f)
                {
                    D3DXVECTOR3 workVec = m_vtxPos[m_vtxIndex[nCntFace * 3 + 1]] - m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]];

                    //  外積計算からそのポリゴンの法線ベクトルを求める
                    D3DXVec3Cross(&normal, &vec1, &workVec);
                    D3DXVec3Normalize(&normal, &normal);

                    float fHeight = m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]].y -
                        ((Pos.x - m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]].x) * normal.x +
                        (Pos.z - m_vtxPos[m_vtxIndex[nCntFace * 3 + 2]].z) * normal.z) / normal.y;

                    if (Pos.y >= fHeight - 100.0f && Pos.y <= fHeight + 100.0f)
                    {
                        return fHeight + 1.0f;
                    }    
                }
            }
        }
    }

    return FLT_MAX;
}

//*************************************************************************************************
// チェックポイント(始点)の取得
//*************************************************************************************************
D3DXVECTOR3 CTrack::GetCheckPointStart(int Num)
{
    return m_CheckPointStart[Num];
}

//*************************************************************************************************
// チェックポイント(終点)の取得
//*************************************************************************************************
D3DXVECTOR3 CTrack::GetCheckPointEnd(int Num)
{
    return m_CheckPointEnd[Num];
}