//*************************************************************************************************
// プレイヤー2処理 (player2.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "keyboard.h"
#include "joystick.h"
#include "mode.h"
#include "modeGame.h"
#include "scene.h"
#include "scene3D.h"
#include "meshBox.h"
#include "sceneModel.h"
#include "player2.h"
#include "item.h"
#include "track.h"
#include "emitter.h"
#include "particle.h"
#include "socket.h"
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include "playerload.h"

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
int CPlayer2::m_ID;

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// プレイヤー2のコンストラクタ
//*************************************************************************************************
CPlayer2::CPlayer2(int nPriority) :CSceneModel(SCENE_TYPE_PLAYER2, nPriority)
{
    m_Rot = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
    m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_HalfGoal = false;
    m_Goal = false;
}

//*************************************************************************************************
// プレイヤー2のデストラクタ
//*************************************************************************************************
CPlayer2::~CPlayer2()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CPlayer2::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //プレイヤー読み込み処理
    CPlayerLoad *playerLoad = new CPlayerLoad();
    if (CSocket::GetID() == 1) 
        playerLoad->Load(2);
    else if(CSocket::GetID() == 2)
        playerLoad->Load(1);
    m_Part = playerLoad->GetParts();
    std::vector<CMotion*> motion = playerLoad->GetMotions();
    for (int nCntMotion = 0; nCntMotion < STATE_MAX; nCntMotion++)
        m_Motion[nCntMotion] = motion[nCntMotion];

    m_MotionBlendFrame = 0;
    m_State = WAIT;
    m_BlendMotion = *m_Motion[m_State];
    m_PassingCheckPoint = 0;

    //シーンモデルクラスの初期化処理
    CSceneModel::Init();

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CPlayer2::Uninit(void)
{
    //シーンモデルクラスの終了処理
    CSceneModel::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CPlayer2::Update(void)
{
    //シーンモデルクラスの更新処理
    CSceneModel::Update();

    //前回の座標と状態を更新
    m_OldPos = m_Pos;
    m_OldState = m_State;

    //プレイヤー状態更新
    PlayerStateUpdate();

    //プレイヤーモーション処理
    PlayerMotion();

    //プレイヤーとトラックの衝突処理
    PlayerToTrackColision();

    //アイテムとの衝突判定
    PlayerToItemColision();

    //エフェクト処理
    EMITTER Emitter;
    Emitter.Pos = m_Pos;
    Emitter.Speed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    Emitter.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    Emitter.Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Emitter.nLife = 30;
    Emitter.nNum = 5;

    //前回の座標と違う場合
    if (m_OldPos != m_Pos)
    {
        CEmitter *emitter = new CEmitter;
        //emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());
    }

    //パーティクルクラスの更新処理
    CModeGame::GetParticle()->Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CPlayer2::Draw(void)
{
    //パーティクルクラスの描画処理
    CModeGame::GetParticle()->Draw();

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
    if (pDevice == NULL) return;

    //現在デバイスに設定されているマテリアル情報を取得
    D3DMATERIAL9 matDef;
    pDevice->GetMaterial(&matDef);

    D3DXMATRIX mtxWorld;                    //ワールド行列
    D3DXMATRIX mtxSclModel;                 //拡大行列
    D3DXMATRIX mtxRotModel;                 //回転行列
    D3DXMATRIX mtxPosModel;                 //平行移動行列

    //単位行列・拡大行列・回転行列・平行移動行列を作成
    D3DXMatrixIdentity(&mtxWorld);
    D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);
    D3DXMatrixRotationQuaternion(&mtxRotModel, &m_Quaternion);
    D3DXMatrixTranslation(&mtxPosModel, m_Pos.x, m_Pos.y, m_Pos.z);

    //ワールドマトリクスに掛け算(拡大→回転→平行移動)
    mtxWorld *= mtxSclModel;
    mtxWorld *= mtxRotModel;
    mtxWorld *= mtxPosModel;

    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        D3DXMATRIX partMatrix = m_Part[part]->GetMatrix();

        //単位行列・拡大行列・回転行列・平行移動行列を作成
        D3DXMatrixIdentity(&partMatrix);
        D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);
        D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_Part[part]->GetRot().y, m_Part[part]->GetRot().x, m_Part[part]->GetRot().z);
        D3DXMatrixTranslation(&mtxPosModel, m_Part[part]->GetPos().x, m_Part[part]->GetPos().y, m_Part[part]->GetPos().z);

        //ワールドマトリクスに掛け算(拡大→回転→平行移動)
        partMatrix *= mtxSclModel;
        partMatrix *= mtxRotModel;
        partMatrix *= mtxPosModel;

        //親子構造構築
        if (m_Part[part]->GetParent() == NULL)  //(親)
            D3DXMatrixMultiply(&partMatrix, &partMatrix, &mtxWorld);
        else                                    //(子)
            D3DXMatrixMultiply(&partMatrix, &partMatrix, &m_Part[part]->GetParent()->GetMatrix());
        m_Part[part]->SetMatrix(partMatrix);

        //デバイスにワールドマトリクスを設定
        pDevice->SetTransform(D3DTS_WORLD, &partMatrix);

        D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_Part[part]->GetBuffMat()->GetBufferPointer();

        //モデルの描画
        for (int i = 0; i < (int)m_Part[part]->GetNumMat(); i++)
        {
            pDevice->SetMaterial(&pMat[i].MatD3D);
            pDevice->SetTexture(0, m_Part[part]->GetTexture(i));
            m_Part[part]->GetMesh()->DrawSubset(i);
        }

        //マテリアルの設定
        pDevice->SetMaterial(&matDef);

        //デバイスにワールドマトリクスを設定
        D3DXMatrixIdentity(&mtxWorld);
        pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
    }
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CPlayer2 *CPlayer2::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority)
{
    CPlayer2 *pPlayer2;                         //自身クラスのポインタ

    //動的確保
    pPlayer2 = new CPlayer2(nPriority);

    //2Pの場合
    if (CSocket::GetID() == 2) Pos = D3DXVECTOR3(0.0f, 0.0f, 2625.0f);

    //各種初期化
    pPlayer2->m_Pos = Pos;
    pPlayer2->m_Scl = Scl;

    //初期化処理
    pPlayer2->Init();

    return pPlayer2;
}

//*************************************************************************************************
// モーションの取得
//*************************************************************************************************
CMotion *CPlayer2::GetMotion(int i )
{
    return m_Motion[i];
}

//*************************************************************************************************
// 半分までゴールフラグ取得
//*************************************************************************************************
bool CPlayer2::GetHalfGoal(void)
{
    return m_HalfGoal;
}

//*************************************************************************************************
// 通過したチェックポイントの取得
//*************************************************************************************************
int CPlayer2::GetPassingCheckPoint(void)
{
    return m_PassingCheckPoint;
}

//*************************************************************************************************
// 通過したチェックポイントの設定
//*************************************************************************************************
void CPlayer2::SetPassingCheckPoint(int Num)
{
    m_PassingCheckPoint = Num;
}

//*************************************************************************************************
// 半分までゴールフラグの設定
//*************************************************************************************************
void CPlayer2::SetHalfGoal(bool HalfGoal)
{
    m_HalfGoal = HalfGoal;
}

//*************************************************************************************************
// ゴールフラグの設定
//*************************************************************************************************
void CPlayer2::SetGoal(bool Goal)
{
    m_Goal = Goal;
}

//*************************************************************************************************
// プレイヤー状態更新
//*************************************************************************************************
void CPlayer2::PlayerStateUpdate(void)
{
    if (m_Motion[WAIT]->GetPlay())           m_State = WAIT;   //待機
    else if (m_Motion[PUNTCH]->GetPlay())    m_State = PUNTCH; //パンチ
    else if (m_Motion[RUN]->GetPlay())       m_State = RUN;    //走る
    else if (m_Motion[WALK]->GetPlay())      m_State = WALK;   //歩く
    else m_State = WAIT;                                       //待機
}

//*************************************************************************************************
// プレイヤーモーション処理
//*************************************************************************************************
void CPlayer2::PlayerMotion(void)
{
    //前回のモーションを保存
    if (m_State != m_OldState)
    {
        m_BlendMotion = *m_Motion[m_OldState];
        m_BlendMotion.SetFrame(0.0f);
    }

    //フレーム計算
    float rate = (float)m_BlendMotion.GetFrame() / m_BlendMotion.GetKeyFrame(m_BlendMotion.GetKey()).GetFrame();
    float blendRate = (float)m_Motion[m_State]->GetFrame() / m_Motion[m_State]->GetKeyFrame(m_Motion[m_State]->GetKey()).GetFrame();
    float motionBlendRate = m_BlendMotion.GetFrame() / 30.0f;

    //モーション再生
    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        D3DXVECTOR3 Pos;    //前回のモーション座標
        D3DXVECTOR3 Rot;    //前回のモーション回転

                            //線形補間関数
        D3DXVec3Lerp(&Pos,                                                  //座標
            &m_BlendMotion.GetKeyFrame(m_BlendMotion.GetKey()).GetKey(part)->GetPos(),
            &m_BlendMotion.GetKeyFrame((m_BlendMotion.GetKey() + 1) % m_BlendMotion.GetNumKey()).GetKey(part)->GetPos(),
            rate);
        D3DXVec3Lerp(&Rot,                                                  //回転
            &m_BlendMotion.GetKeyFrame(m_BlendMotion.GetKey()).GetKey(part)->GetRot(),
            &m_BlendMotion.GetKeyFrame((m_BlendMotion.GetKey() + 1) % m_BlendMotion.GetNumKey()).GetKey(part)->GetRot(),
            rate);

        D3DXVECTOR3 BlendPos;
        D3DXVECTOR3 BlendRot;

        //線形補間関数
        D3DXVec3Lerp(&BlendPos,                                             //座標
            &m_Motion[m_State]->GetKeyFrame(m_Motion[m_State]->GetKey()).GetKey(part)->GetPos(),
            &m_Motion[m_State]->GetKeyFrame((m_Motion[m_State]->GetKey() + 1) % m_Motion[m_State]->GetNumKey()).GetKey(part)->GetPos(),
            blendRate);
        D3DXVec3Lerp(&BlendRot,                                             //回転
            &m_Motion[m_State]->GetKeyFrame(m_Motion[m_State]->GetKey()).GetKey(part)->GetRot(),
            &m_Motion[m_State]->GetKeyFrame((m_Motion[m_State]->GetKey() + 1) % m_Motion[m_State]->GetNumKey()).GetKey(part)->GetRot(),
            blendRate);

        //線形補間関数
        D3DXVec3Lerp(&BlendPos, &Pos, &BlendPos, motionBlendRate);
        D3DXVec3Lerp(&BlendRot, &Rot, &BlendRot, motionBlendRate);

        m_Part[part]->SetPos(BlendPos);
        m_Part[part]->SetRot(BlendRot);
    }

    //フレーム加算
    m_BlendMotion.SetFrame(m_BlendMotion.GetFrame() + 1.0f);   //モーションブレンド
    if (m_BlendMotion.GetFrame() >= MOTION_BLEND_FRAME) m_BlendMotion.SetFrame(MOTION_BLEND_FRAME);
    m_Motion[m_State]->SetFrame(m_Motion[m_State]->GetFrame() + 1.0f);  //キーフレーム
    if (m_Motion[m_State]->GetFrame() >= m_Motion[m_State]->GetKeyFrame(m_Motion[m_State]->GetKey()).GetFrame())
    {
        m_Motion[m_State]->SetFrame(0.0f);
        m_Motion[m_State]->SetKey(m_Motion[m_State]->GetKey() + 1.0f);

        //キー最大にならない時
        if (m_Motion[m_State]->GetKey() != m_Motion[m_State]->GetNumKey()) return;

        //ループする場合
        if (m_Motion[m_State]->GetLoop() == true) m_Motion[m_State]->SetKey(0);

        //ループしない場合
        if (m_Motion[m_State]->GetLoop() == false)
        {
            m_Motion[m_State]->SetPlay(false);
            m_Motion[m_State]->SetFrame(0.0f);
            m_Motion[m_State]->SetKey(0);
        }
    }
}

//*************************************************************************************************
// プレイヤーとトラックの衝突処理
//*************************************************************************************************
void CPlayer2::PlayerToTrackColision(void)
{
    //地面の高さを取得
    float Pos_y = CModeGame::GetTrack()->GetHeight(m_Pos);

    //地面に衝突した場合
    if (m_Pos.y - 37.0f <= Pos_y)
    {
        if (FLT_MAX == Pos_y)
        {
            //前回の座標を代入
            m_Pos = m_OldPos - (m_Rot * 20.0f);
            m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        }
        else
        {
            m_Pos.y = CModeGame::GetTrack()->GetHeight(m_Pos) + 37.0f;
            m_Move.y = 0.0f;
        }

        m_Rot.x *= -1;
        m_Rot.z *= -1;

        //面の法線を取得
        D3DXVECTOR3 VecN = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXVECTOR3 Vx, Vz;
        D3DXVec3Cross(&Vx, &VecN, &m_Rot);
        D3DXVec3Cross(&Vz, &Vx, &VecN);
        D3DXMATRIX mtxRot;                      //回転行列

        mtxRot._11 = Vx.x;
        mtxRot._12 = Vx.y;
        mtxRot._13 = Vx.z;
        mtxRot._14 = 0;
        mtxRot._21 = VecN.x;
        mtxRot._22 = VecN.y;
        mtxRot._23 = VecN.z;
        mtxRot._24 = 0;
        mtxRot._31 = Vz.x;
        mtxRot._32 = Vz.y;
        mtxRot._33 = Vz.z;
        mtxRot._34 = 0;
        mtxRot._41 = 0;
        mtxRot._42 = 0;
        mtxRot._43 = 0;
        mtxRot._44 = 1;

        D3DXQUATERNION quaternion;                  //クォータニオン

                                                    //回転行列からクォータニオンを作成する関数
        D3DXQuaternionRotationMatrix(&quaternion, &mtxRot);

        //2つのクォータニオンの補間関数
        D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quaternion, 0.5f);

        m_Rot.x *= -1;
        m_Rot.z *= -1;
    }
}

//*************************************************************************************************
// プレイヤーとアイテムの衝突処理
//*************************************************************************************************
void CPlayer2::PlayerToItemColision(void)
{
    //アイテムとの衝突判定
    CScene *scene = CScene::GetList(3);
    while (scene != NULL)
    {
        if (scene->GetType() == SCENE_TYPE_ITEM)
        {
            CItem *item = (CItem*)scene;
            D3DXVECTOR3 itemPos = item->GetPos();

            //衝突判定
            D3DXVECTOR3 direction = itemPos - m_Pos;
            float length = D3DXVec3Length(&direction);
            if (length < 50.0f)
            {
                item->Release();
            }
        }
        scene = scene->GetNext();
    }
}