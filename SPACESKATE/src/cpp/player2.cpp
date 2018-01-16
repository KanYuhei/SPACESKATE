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
    //パーツ情報の初期化
    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        m_Part[part].pMesh = NULL;
        m_Part[part].pBuffMat = NULL;
        for (int i = 0; i < MDEL_TEXTURE_NUM; i++)
        {
            m_Part[part].pTexture[i] = NULL;
        }
        m_Part[part].nNumMat = (DWORD)0;
    }

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

    char fileName[PART_MAX_NUM][256];
    if (CSocket::GetID() == 1)
    {
        sprintf(fileName[0], "data/MODEL/00_RoboBlue_body.x");    //（体）
        sprintf(fileName[1], "data/MODEL/01_RoboBlue_head.x");    //（頭）
        sprintf(fileName[2], "data/MODEL/02_RoboBlue_arm_L.x");   //（左腕）
        sprintf(fileName[3], "data/MODEL/03_RoboBlue_hand_L.x");  //（左手）
        sprintf(fileName[4], "data/MODEL/04_RoboBlue_arm_R.x");   //（右腕）
        sprintf(fileName[5], "data/MODEL/05_RoboBlue_hand_R.x");  //（右手）
        sprintf(fileName[6], "data/MODEL/06_RoboBlue_leg_L.x");   //（左腿）
        sprintf(fileName[7], "data/MODEL/07_RoboBlue_foot_L.x");  //（左足）
        sprintf(fileName[8], "data/MODEL/08_RoboBlue_leg_R.x");   //（右腿）
        sprintf(fileName[9], "data/MODEL/09_RoboBlue_foot_R.x");  //（右足）
    }
    else if (CSocket::GetID() == 2)
    {
        sprintf(fileName[0], "data/MODEL/00_Robo_body.x");       //（体）
        sprintf(fileName[1], "data/MODEL/01_Robo_head.x");       //（頭）
        sprintf(fileName[2], "data/MODEL/02_Robo_arm_L.x");      //（左腕）
        sprintf(fileName[3], "data/MODEL/03_Robo_hand_L.x");     //（左手）
        sprintf(fileName[4], "data/MODEL/04_Robo_arm_R.x");      //（右腕）
        sprintf(fileName[5], "data/MODEL/05_Robo_hand_R.x");     //（右手）
        sprintf(fileName[6], "data/MODEL/06_Robo_leg_L.x");      //（左腿）
        sprintf(fileName[7], "data/MODEL/07_Robo_foot_L.x");     //（左足）
        sprintf(fileName[8], "data/MODEL/08_Robo_leg_R.x");      //（右腿）
        sprintf(fileName[9], "data/MODEL/09_Robo_foot_R.x");     //（右足）
    }

    //体
    D3DXLoadMeshFromX(fileName[0], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[0].pBuffMat, NULL, &m_Part[0].nNumMat, &m_Part[0].pMesh);
    m_Part[0].Position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
    m_Part[0].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[0].Parent = NULL;

    //頭
    D3DXLoadMeshFromX(fileName[1], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[1].pBuffMat, NULL, &m_Part[1].nNumMat, &m_Part[1].pMesh);
    m_Part[1].Position = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
    m_Part[1].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[1].Parent = &m_Part[0];

    //左腕
    D3DXLoadMeshFromX(fileName[2], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[2].pBuffMat, NULL, &m_Part[2].nNumMat, &m_Part[2].pMesh);
    m_Part[2].Position = D3DXVECTOR3(9.0f, 30.0f, 0.0f);
    m_Part[2].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[2].Parent = &m_Part[0];

    //左手
    D3DXLoadMeshFromX(fileName[3], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[3].pBuffMat, NULL, &m_Part[3].nNumMat, &m_Part[3].pMesh);
    m_Part[3].Position = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
    m_Part[3].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[3].Parent = &m_Part[2];

    //右腕
    D3DXLoadMeshFromX(fileName[4], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[4].pBuffMat, NULL, &m_Part[4].nNumMat, &m_Part[4].pMesh);
    m_Part[4].Position = D3DXVECTOR3(-9.0f, 30.0f, 0.0f);
    m_Part[4].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[4].Parent = &m_Part[0];

    //右手
    D3DXLoadMeshFromX(fileName[5], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[5].pBuffMat, NULL, &m_Part[5].nNumMat, &m_Part[5].pMesh);
    m_Part[5].Position = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
    m_Part[5].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[5].Parent = &m_Part[4];

    //左足
    D3DXLoadMeshFromX(fileName[6], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[6].pBuffMat, NULL, &m_Part[6].nNumMat, &m_Part[6].pMesh);
    m_Part[6].Position = D3DXVECTOR3(6.0f, 6.0f, 0.0f);
    m_Part[6].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[6].Parent = &m_Part[0];

    //左足先
    D3DXLoadMeshFromX(fileName[7], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[7].pBuffMat, NULL, &m_Part[7].nNumMat, &m_Part[7].pMesh);
    m_Part[7].Position = D3DXVECTOR3(1.5f, -20.0f, -1.5f);
    m_Part[7].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[7].Parent = &m_Part[6];

    //右足
    D3DXLoadMeshFromX(fileName[8], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[8].pBuffMat, NULL, &m_Part[8].nNumMat, &m_Part[8].pMesh);
    m_Part[8].Position = D3DXVECTOR3(-6.0f, 6.0f, 0.0f);
    m_Part[8].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[8].Parent = &m_Part[0];

    //右足先
    D3DXLoadMeshFromX(fileName[9], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[9].pBuffMat, NULL, &m_Part[9].nNumMat, &m_Part[9].pMesh);
    m_Part[9].Position = D3DXVECTOR3(-1.5f, -20.0f, -1.5f);
    m_Part[9].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[9].Parent = &m_Part[8];


    //モーション情報の初期化
    for (int i = 0; i < STATE_MAX; i++)
    {
        m_Motion[i] = CModeGame::GetPlayer()->GetMotion(i);
        m_Motion[i].Frame = 0.0f;
        m_Motion[i].Key = 0;
        m_Motion[i].Play = false;
    }

    m_MotionBlendFrame = 0;
    m_State = WAIT;
    m_BlendMotion = m_Motion[m_State];
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

    //プレイヤー状態の更新///////////////////////////////////////////
    if (m_Motion[WAIT].Play == true)        m_State = WAIT;   //待機
    else if (m_Motion[PUNTCH].Play == true) m_State = PUNTCH; //パンチ
    else if (m_Motion[RUN].Play == true)    m_State = RUN;    //走る
    else if (m_Motion[WALK].Play == true)   m_State = WALK;   //歩く
    else m_State = WAIT;                                      //待機

    
    //プレイヤー状態毎のモーション再生///////////////////////////////

    //前回のモーションを保存
    if (m_State != m_OldState)
    {
        m_BlendMotion = m_Motion[m_OldState];
        m_MotionBlendFrame = 0;
    }

    //フレーム計算
    float rate = (float)m_BlendMotion.Frame / m_BlendMotion.KeyFrame[m_BlendMotion.Key].Frame;
    float blendRate = (float)m_Motion[m_State].Frame / m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Frame;
    float motionBlendRate = m_MotionBlendFrame / 30.0f;

    //モーション再生
    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        D3DXVECTOR3 Pos;    //前回のモーション座標
        D3DXVECTOR3 Rot;    //前回のモーション回転

                            //線形補間関数
        D3DXVec3Lerp(&Pos,                                                  //座標
            &m_BlendMotion.KeyFrame[m_BlendMotion.Key].Key[part].Position,
            &m_BlendMotion.KeyFrame[(m_BlendMotion.Key + 1) % m_BlendMotion.Key_Max].Key[part].Position,
            rate);
        D3DXVec3Lerp(&Rot,                                                  //回転
            &m_BlendMotion.KeyFrame[m_BlendMotion.Key].Key[part].Rotation,
            &m_BlendMotion.KeyFrame[(m_BlendMotion.Key + 1) % m_BlendMotion.Key_Max].Key[part].Rotation,
            rate);

        D3DXVECTOR3 BlendPos;
        D3DXVECTOR3 BlendRot;

        //線形補間関数
        D3DXVec3Lerp(&BlendPos,                                             //座標
            &m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Key[part].Position,
            &m_Motion[m_State].KeyFrame[(m_Motion[m_State].Key + 1) % m_Motion[m_State].Key_Max].Key[part].Position,
            blendRate);
        D3DXVec3Lerp(&BlendRot,                                             //回転
            &m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Key[part].Rotation,
            &m_Motion[m_State].KeyFrame[(m_Motion[m_State].Key + 1) % m_Motion[m_State].Key_Max].Key[part].Rotation,
            blendRate);

        //線形補間関数
        D3DXVec3Lerp(&m_Part[part].Position, &Pos, &BlendPos, motionBlendRate);
        D3DXVec3Lerp(&m_Part[part].Rotation, &Rot, &BlendRot, motionBlendRate);
    }

    //フレーム加算
    m_MotionBlendFrame++;   //モーションブレンド
    if (m_MotionBlendFrame >= MOTION_BLEND_FRAME) m_MotionBlendFrame = MOTION_BLEND_FRAME;
    m_Motion[m_State].Frame++;  //キーフレーム
    if (m_Motion[m_State].Frame >= m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Frame)
    {
        m_Motion[m_State].Frame = 0.0f;
        m_Motion[m_State].Key++;
        //キー最大になった時
        if (m_Motion[m_State].Key == m_Motion[m_State].Key_Max)
        {
            //ループする場合
            if (m_Motion[m_State].Loop == true) m_Motion[m_State].Key = 0;

            //ループしない場合
            if (m_Motion[m_State].Loop == false)
            {
                //パンチ
                if (m_State == PUNTCH)
                {
                    m_Motion[PUNTCH].Play = false;
                    m_Motion[PUNTCH].Frame = 0;
                    m_Motion[PUNTCH].Key = 0;
                }
            }
        }
    }

    //トラック////////////////////////////////////////////////////////
    //地面の高さを取得
    float Pos_y = CModeGame::GetTrack()->GetHeight(m_Pos);

    //地面に衝突した場合
    if (m_Pos.y - 37.0f <= Pos_y)
    {
        if (FLT_MAX == Pos_y)
        {
            //前回の座標を代入
            //m_Pos = m_OldPos;
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

    //シーンモデルクラスの描画処理
    //CSceneModel::Draw ( );

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        D3DMATERIAL9 matDef;

        //現在デバイスに設定されているマテリアル情報を取得
        pDevice->GetMaterial(&matDef);

        D3DXMATRIX mtxWorld;                    //ワールド行列
        D3DXMATRIX mtxSclModel;                 //拡大行列
        D3DXMATRIX mtxRotModel;                 //回転行列
        D3DXMATRIX mtxPosModel;                 //平行移動行列

        //単位行列を作る関数
        D3DXMatrixIdentity(&mtxWorld);

        //拡大行列を作る関数
        D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);

        //回転行列を作る関数
        D3DXMatrixRotationQuaternion(&mtxRotModel, &m_Quaternion);

        //平行移動行列を作る関数
        D3DXMatrixTranslation(&mtxPosModel, m_Pos.x, m_Pos.y, m_Pos.z);

        //ワールドマトリクスに掛け算(拡大→回転→平行移動)
        mtxWorld *= mtxSclModel;
        mtxWorld *= mtxRotModel;
        mtxWorld *= mtxPosModel;

        for (int part = 0; part < PART_MAX_NUM; part++)
        {
            //単位行列を作る関数
            D3DXMatrixIdentity(&m_Part[part].Matrix);

            //拡大行列を作る関数
            D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);

            //回転行列を作る関数
            D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_Part[part].Rotation.y, m_Part[part].Rotation.x, m_Part[part].Rotation.z);

            //平行移動行列を作る関数
            D3DXMatrixTranslation(&mtxPosModel, m_Part[part].Position.x, m_Part[part].Position.y, m_Part[part].Position.z);

            //ワールドマトリクスに掛け算(拡大→回転→平行移動)
            m_Part[part].Matrix *= mtxSclModel;
            m_Part[part].Matrix *= mtxRotModel;
            m_Part[part].Matrix *= mtxPosModel;

            //親子構造構築
            if (m_Part[part].Parent == NULL)
            {
                D3DXMatrixMultiply(&m_Part[part].Matrix, &m_Part[part].Matrix, &mtxWorld);
            }
            else
            {
                D3DXMatrixMultiply(&m_Part[part].Matrix, &m_Part[part].Matrix, &m_Part[part].Parent->Matrix);
            }

            //デバイスにワールドマトリクスを設定
            pDevice->SetTransform(D3DTS_WORLD, &m_Part[part].Matrix);

            D3DXMATERIAL* pMat;
            pMat = (D3DXMATERIAL*)m_Part[part].pBuffMat->GetBufferPointer();

            //マテリアル情報の数分繰り返す
            for (int i = 0; i < (int)m_Part[part].nNumMat; i++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[i].MatD3D);

                //テクスチャの設定
                pDevice->SetTexture(0, m_Part[part].pTexture[i]);

                //メッシュの描画
                m_Part[part].pMesh->DrawSubset(i);
            }

            //マテリアルの設定
            pDevice->SetMaterial(&matDef);

            //単位行列を作る関数
            D3DXMatrixIdentity(&mtxWorld);

            //デバイスにワールドマトリクスを設定
            pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
        }
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
MOTION *CPlayer2::GetMotion(int i )
{
    return &m_Motion[i];
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