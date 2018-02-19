//*************************************************************************************************
// プレイヤー処理 (player.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "keyboard.h"
#include "joystick.h"
#include "sound.h"
#include "renderTarget2D.h"
#include "mode.h"
#include "modeGame.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "countdawn.h"
#include "gear.h"
#include "ranking.h"
#include "time.h"
#include "scene3D.h"
#include "meshBox.h"
#include "meshField.h"
#include "sceneModel.h"
#include "player.h"
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
// 名前空間
//*************************************************************************************************
using namespace std;

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
int CPlayer::m_ID;

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************
inline void DeleteHeadSpace(string &buf);
inline void SetValue(string str, int *value);
inline void SetXYZ(string str, D3DXVECTOR3 *pos);
inline void SetKey(vector<string>::const_iterator it, CKey *key);
inline void SetKeyFrame(vector<string>::const_iterator it, CKeyFrame *keyframe, int numPart);

//*************************************************************************************************
// プレイヤーのコンストラクタ
//*************************************************************************************************
CPlayer::CPlayer ( int nPriority ):CSceneModel( SCENE_TYPE_PLAYER, nPriority )
{
    m_Rot = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
    m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_ItemNum = 0;
    m_Ranking = 0;
    m_HalfGoal = false;
    m_Goal = false;
    m_Cout = 0;
    m_GoalEffect = false;
}

//*************************************************************************************************
// プレイヤーのデストラクタ
//*************************************************************************************************
CPlayer::~CPlayer ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CPlayer::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // ファイルの読み込み
    string file;
    if (CSocket::GetID() == 1) file = "data/TEXT/motion.txt";
    else if (CSocket::GetID() == 2) file = "data/TEXT/motion2.txt";
    ifstream ifs(file);
    if (ifs.fail())
    {
        MessageBox(NULL, "プレイヤーモーションテキストの読み込みに失敗しました", "終了メッセージ", MB_OK);
        return E_FAIL;
    }

    //ファイルから文字列を読み込み
    vector<string> loadStr;             //読み込んだ文字列
    string str;                         //作業用文字列
    string::size_type commentStart = 0; //コメント開始位置
    while (getline(ifs, str))
    {
        // '#'以降はコメントとして無視
        if ((commentStart = str.find('#')) != string::size_type(-1))
            str = str.substr(0, commentStart);

        // 行頭のスペース，タブを削除
        DeleteHeadSpace(str);

        // 空行は無視
        if (str.empty()) continue;

        loadStr.push_back(str);
    }
    ifs.close();

    //ファイル文字列解析
    vector<string> filname;                 //読み込むモデル名のリスト
    int ModelNum = 0;                       //モデル数
    vector<CParts> Parts;                   //パーツ情報
    int NumParts = 0;                       //パーツ数
    vector<int> Index;                      //インデックス
    vector<int> Parent;                     //親子
    vector<D3DXVECTOR3> position;           //座標
    vector<D3DXVECTOR3> rotation;           //回転
    vector<int> Loop;                       //ループするかどうか[0:しない / 1:する]
    vector<int> NumKey;                     //キー数
    vector<CKeyFrame> keyFrame;             //キーフレームクラス
    for (vector<string>::const_iterator it = loadStr.begin(); it != loadStr.end(); it++)
    {
        //NUM_MODELが見つかった場合
        if (it->find("NUM_MODEL") != -1) SetValue(it->c_str(), &ModelNum);

        //MODEL_FILENAMEが見つかった場合
        if (it->find("MODEL_FILENAME") != -1)
        {
            int start = it->find("data");
            int end = it->length();
            filname.push_back(it->substr(start, end - start));
        }

        //キャラクター情報///////////////////////////////////////////
        //NUM_PARTSが見つかった場合
        if (it->find("NUM_PARTS") != -1) SetValue(it->c_str(), &NumParts);

        //INDEXが見つかった場合
        if (it->find("INDEX") != -1)
        {
            int index;
            SetValue(it->c_str(), &index);
            Index.push_back(index);
        }

        //PARENTが見つかった場合
        if (it->find("PARENT") != -1)
        {
            int parent;
            SetValue(it->c_str(), &parent);
            Parent.push_back(parent);
        }

        //POSが見つかった場合
        if (it->find("POS") != -1)
        {
            D3DXVECTOR3 Pos;
            SetXYZ(it->c_str(), &Pos);
            position.push_back(Pos);
        }

        //ROTが見つかった場合
        if (it->find("ROT") != -1)
        {
            D3DXVECTOR3 Rot;
            SetXYZ(it->c_str(), &Rot);
            rotation.push_back(Rot);
        }

        //モーション情報/////////////////////////////////////////////
        //LOOPが見つかった場合
        if (it->find("LOOP") != -1)
        {
            int loop;
            SetValue(it->c_str(), &loop);
            Loop.push_back(loop);
        }

        //NUM_KEYが見つかった場合
        if (it->find("NUM_KEY") != -1)
        {
            int num_key;
            SetValue(it->c_str(), &num_key);
            NumKey.push_back(num_key);
        }

        //KEYSETが見つかった場合
        if (it->find("KEYSET") != -1 && it->find("END_KEYSET") == -1)
        {
            CKeyFrame keyframe;
            SetKeyFrame(it, &keyframe, NumParts);
            keyFrame.push_back(keyframe);
        }
    }

    // パーツの作成
    for (int nCntPart = 0; nCntPart < NumParts; nCntPart++)
    {
        CParts *part = nullptr;
        if (Parent[nCntPart] == -1)     //(親)NULL
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], NULL);
        else                            //(子)
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], m_Part[Parent[nCntPart]]);
        m_Part.push_back(part);
    }

    // モーションの作成
    int num = 0;
    for (int nCntMotion = 0; nCntMotion < STATE_MAX; nCntMotion++)
    {
        vector<CKeyFrame> keyframe;
        for (int i = 0; i < NumKey[nCntMotion]; i++)
        {
            keyframe.push_back(keyFrame[num]);
            num++;
        }
        m_Motion[nCntMotion] = CMotion::Create(keyframe, NumKey[nCntMotion], Loop[nCntMotion]);
    }

    //実験
    CParts PARTS[10];
    for (int i = 0; i < 10; i++)
    {
        PARTS[i] = *m_Part[i];
    }

    CKey key[2][10];
    for (int i = 0; i < 10; i++)
    {
        key[0][i] = *m_Motion[WALK]->GetKeyFrame(0).GetKey(i);
        key[1][i] = *m_Motion[WALK]->GetKeyFrame(1).GetKey(i);
    }

    m_Play = false;
    m_ItemNum = 0;
    m_State = WAIT;
    m_BlendMotion = *m_Motion[m_State];
    m_PassingCheckPoint = 0;

    //シーンモデルクラスの初期化処理
    CSceneModel::Init ( );

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CPlayer::Uninit(void)
{
    //シーンモデルクラスの終了処理
    CSceneModel::Uninit ( );
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CPlayer::Update(void)
{
    //シーンモデルクラスの更新処理
    CSceneModel::Update ( );

    //前回の座標と状態を更新
    m_OldPos = m_Pos;
    m_OldState = m_State;

    //各種初期化
    m_Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //カメラ演出（回転処理）
    if (m_Cout <= 200)
        m_Cout++;
    if (m_Cout > 50 && m_Cout <= 150)
    {
        CModeGame::GetCamera() ->AddLength(1.5f);
        CModeGame::GetCamera()->AddAngleY(D3DX_PI * 0.01f);
        CModeGame::GetCamera()->AddAngleX(D3DX_PI * 0.0015f);
    }

    //カウントダウン中フリーズ
    if (CModeGame::GetCountdawn()->GetCountDawn() == 0) m_Play = true;

    //プレイヤー入力処理
    PlayerInput();

    //プレイヤー移動処理
    PlayerMove();

    //プレイヤー状態更新
    PlayerStateUpdate();

    //プレイヤーモーション処理
    PlayerMotion();

    //プレイヤーとトラックの衝突処理
    PlayerToTrackColision();

    //メッシュボックスクラス
    CModeGame::GetMeshBox()->SetPos(m_Pos);

    //プレイヤーランキング処理
    PlayerRanking();
    
    //プレイヤーとアイテムの衝突処理
    PlayerToItemColision();
    

    ////エフェクト処理
    //EMITTER Emitter;
    //Emitter.Pos = D3DXVECTOR3 ( 0.0f, 5.0f, 2300.0f + (float)(rand() % 1000));
    //Emitter.Speed = D3DXVECTOR3(0.0f, 0.7f, 0.0f);
    //Emitter.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    //Emitter.Size = D3DXVECTOR2(10.0f, 10.0f);
    //Emitter.Color = D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.7f);
    //Emitter.nLife = 60;
    //Emitter.nNum = 5;

    ////前回の座標と違う場合
    ///*if (m_OldPos != m_Pos)
    //{
    //    CEmitter *emitter = new CEmitter;
    //    emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());
    //}*/
    //CEmitter *emitter = new CEmitter;
    //emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());

    //パーティクルクラスの更新処理
    CModeGame::GetParticle()->Update();

    //カメラ注視点の設定
    CModeGame::GetCamera()->SetPosAt(m_Pos);

    //プレイヤーネットワーク処理
    PlayerNetwork();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CPlayer::Draw(void)
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
        D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_Part[part]->GetRot().y, m_Part[part]->GetRot().x, m_Part[part]->GetRot().z );
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

#ifdef _DEBUG
    char aStr[256];
    RECT rc = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
    sprintf(&aStr[0], "[プレイヤー]\n<座標>   ( X:%f Y:%f Z:%f )\n", m_Pos.x, m_Pos.y, m_Pos.z);
    CRenderer::GetFont()->DrawText(NULL, &aStr[0], -1, &rc, DT_LEFT, D3DCOLOR_RGBA(0, 255, 0, 255));
#endif //_DEBUG
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CPlayer *CPlayer::Create ( D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority )
{
    CPlayer *pPlayer;                           //自身クラスのポインタ

    //動的確保
    pPlayer = new CPlayer( nPriority );

    //2Pの場合
    if (CSocket::GetID() == 2) Pos = D3DXVECTOR3(0.0f, 0.0f, 3025.0f);

    //各種初期化
    pPlayer->m_Pos = Pos;
    pPlayer->m_Scl = Scl;

    //初期化処理
    pPlayer->Init ( );

    return pPlayer;
}

//*************************************************************************************************
// パーツの取得
//*************************************************************************************************
CParts *CPlayer::GetPart(int i)
{
    return m_Part[i];
}

//*************************************************************************************************
// モーションの取得
//*************************************************************************************************
CMotion *CPlayer::GetMotion(int i)
{
    return m_Motion[i];
}

//*************************************************************************************************
// 半分までゴールフラグ取得
//*************************************************************************************************
bool CPlayer::GetHalfGoal(void)
{
    return m_HalfGoal;
}

//*************************************************************************************************
// 通過したチェックポイントの取得
//*************************************************************************************************
int CPlayer::GetPassingCheckPoint(void)
{
    return m_PassingCheckPoint;
}

//*************************************************************************************************
// 最上座標の取得
//*************************************************************************************************
D3DXVECTOR3 CPlayer::GetPosUp(void)
{
    D3DXVECTOR3 PosUp = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );       //プレイヤー最上座標

    //  メッシュの頂点データ取得
    VERTEX *pVtx = NULL;

    //  パーツの最大数分繰り返す
    for (int i = 0; i < PART_MAX_NUM; i++)
    {
        //  各パーツの頂点数を取得
        int nNumVertex = m_Part[i]->GetMesh()->GetNumVertices();

        //  メッシュから頂点情報を取得
        m_Part[i]->GetMesh()->LockVertexBuffer(0, (void**)&pVtx);

        //  頂点数分のループ
        for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
        {
            if (PosUp.y <= pVtx->pos.y) PosUp = pVtx->pos;
            pVtx++;
        }

        //  頂点バッファのアンロック
        m_Part[i]->GetMesh()->UnlockVertexBuffer();
    }

    return PosUp;
}

//*************************************************************************************************
// 最下座標の取得
//*************************************************************************************************
D3DXVECTOR3 CPlayer::GetPosDown(void)
{
    D3DXVECTOR3 PosDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     //プレイヤー最下座標

    //  メッシュの頂点データ取得
    VERTEX *pVtx = NULL;

    //  パーツの最大数分繰り返す
    for (int i = 0; i < PART_MAX_NUM; i++)
    {
        //  各パーツの頂点数を取得
        int nNumVertex = m_Part[i]->GetMesh()->GetNumVertices();

        //  メッシュから頂点情報を取得
        m_Part[i]->GetMesh()->LockVertexBuffer(0, (void**)&pVtx);

        //  頂点数分のループ
        for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
        {
            if (PosDown.y >= pVtx->pos.y) PosDown = pVtx->pos;
            pVtx++;
        }

        //  頂点バッファのアンロック
        m_Part[i]->GetMesh()->UnlockVertexBuffer();
    }

    return PosDown;
}

//*************************************************************************************************
// 点Pと直線ABから線上最近点を求める
//*************************************************************************************************
D3DXVECTOR3 CPlayer::NearPosOnLine(D3DXVECTOR3 P, D3DXVECTOR3 A, D3DXVECTOR3 B)
{
    D3DXVECTOR3 AB, AP;//ベクトルAB AP

    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;
    AP.x = P.x - A.x;
    AP.y = P.y - A.y;
    AP.z = P.z - A.z;

    //ABの単位ベクトルを計算
    D3DXVECTOR3 nAB;
    D3DXVec3Normalize(&nAB, &AB);

    //Aから線上最近点までの距離（ABベクトルの後ろにあるときはマイナス値）
    float dist_AX = D3DXVec3Dot(&nAB, &AP);

    if (D3DXVec3Length(&AB) <= dist_AX)
    {
        return B;
    }
    else if (-D3DXVec3Length(&AB) >= dist_AX)
    {
        return A;
    }

    //線上最近点
    D3DXVECTOR3 ret;
    ret.x = A.x + (nAB.x * dist_AX);
    ret.y = A.y + (nAB.y * dist_AX);
    ret.z = A.z + (nAB.z * dist_AX);

    return ret;
}

//*************************************************************************************************
// プレイヤー入力処理
//*************************************************************************************************
void CPlayer::PlayerInput(void)
{
    m_Motion[WAIT]->SetPlay(true);
    m_Motion[WALK]->SetPlay(false);
    m_Motion[RUN]->SetPlay(false);

    if (m_Play != true) return;

    //カメラ操作(JoyStick用)
    /*D3DXVECTOR3 work;
    work.x = (float)CManager::GetInputJoystick()->GetRightStickDisposition().y;
    work.y = (float)CManager::GetInputJoystick()->GetRightStickDisposition().x;
    work.z = 0.0f;
    D3DXVec3Normalize(&work, &work);
    if (work.x != 0 || work.y != 0)
    {
        CModeGame::GetCamera()->AddAngleY(D3DX_PI * work.y * 0.005f);
        CModeGame::GetCamera()->AddAngleX(D3DX_PI * work.x * 0.005f);
    }*/

    //カメラベクトル取得
    D3DXVECTOR3 cameraVecDirect = CModeGame::GetCamera()->GetVecDirect();
    cameraVecDirect.y = 0.0f;                   //カメラY軸成分なし
    D3DXVec3Normalize(&cameraVecDirect, &cameraVecDirect);
    D3DXVECTOR3 cameraVecRightDirect;           //カメラ右方向ベクトル
    D3DXVec3Cross(&cameraVecRightDirect, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &cameraVecDirect);
    D3DXVec3Normalize(&cameraVecRightDirect, &cameraVecRightDirect);

    //移動
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))  //前
    {
        m_Acceleration += cameraVecDirect * 0.8f;
        m_Motion[WAIT]->SetPlay(false);
    }
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))  //後
    {
        m_Acceleration -= cameraVecDirect * 0.8f;
        m_Motion[WAIT]->SetPlay(false);
    }
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))  //右
    {
        m_Acceleration += cameraVecRightDirect * 0.8f;
        m_Motion[WAIT]->SetPlay(false);
    }
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))  //左
    {
        m_Acceleration -= cameraVecRightDirect * 0.8f;
        m_Motion[WAIT]->SetPlay(false);
    }

    //移動(JoyStick用)
    /*D3DXVECTOR3 work2;
    work2.x = (float)CManager::GetInputJoystick()->GetLeftStickDisposition().x;
    work2.y = 0.0f;
    work2.z = (float)CManager::GetInputJoystick()->GetLeftStickDisposition().y;
    D3DXVec3Normalize(&work2, &work2);

    if (work2.z != 0)
    {
        m_Acceleration += cameraVecDirect  * -(work2.z * 0.5f * (((m_ItemNum+1)/10)+1));
    }
    if (work2.x != 0)
    {
        m_Acceleration += cameraVecRightDirect  * (work2.x * 0.5f * (((m_ItemNum + 1) / 10) + 1));
    }

    if (work2.x != 0 || work2.z != 0) m_Motion[WAIT]->SetPlay(false);*/

    //ジャンプ
    if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE) ||
        CManager::GetInputJoystick()->GetJoyTrigger(CInputJoystick::DIJ_BUTTON, DIJ_CROSS))
    {
        //m_Acceleration.y += 200.0f;
    }

    //パンチ
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_P))  //P
    {
        m_Motion[PUNTCH]->SetPlay(true);
        m_Motion[PUNTCH]->SetFrame(0.0f);
        m_Motion[PUNTCH]->SetKey(0);
    }

    //加速
    if (CManager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT) /*||
        CManager::GetInputJoystick()->GetJoyPress(CInputJoystick::DIJ_BUTTON, DIJ_CROSS)*/)
    {
        CManager::GetRenderTarget2D()->IsFeedbackBlur(true);
    }
    else
    {
        CManager::GetRenderTarget2D()->IsFeedbackBlur(false);
    }
}

//*************************************************************************************************
// プレイヤー移動処理
//*************************************************************************************************
void CPlayer::PlayerMove(void)
{
    m_Acceleration.y = -0.98f;                  //重力
    m_Move += m_Acceleration;                   //移動量(速度)に加速度を足す

    if (m_Move.x >= 0.3f || m_Move.x <= -0.3f || m_Move.z >= 0.3f || m_Move.z <= -0.3f)
    {
        //方向
        D3DXMATRIX mtxRot;
        D3DXMatrixIdentity(&mtxRot);
        D3DXVec3Normalize(&m_Rot, &m_Move);
        m_Motion[WALK]->SetPlay(true);
        if (m_Move.x >= 20.0f || m_Move.x <= -20.0f || m_Move.z >= 20.0f || m_Move.z <= -20.0f)
        {
            m_Motion[RUN]->GetKeyFrame(0).SetFrame(30);
            m_Motion[RUN]->GetKeyFrame(1).SetFrame(30);
            m_Motion[RUN]->SetPlay(true);
        }
    }

    m_Pos += m_Move;                            //座標に移動量(速度)を足す
    m_Move *= 0.98f;                            //抵抗
}

//*************************************************************************************************
// プレイヤー状態更新
//*************************************************************************************************
void CPlayer::PlayerStateUpdate(void)
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
void CPlayer::PlayerMotion(void)
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
// プレイヤーランキング処理
//*************************************************************************************************
void CPlayer::PlayerRanking(void)
{
    //プレイヤーのチェックポイント設定
    for (int i = 0; i < CHECKPOINT_NUM; i++)
    {
        //チェックポイント（線分）
        D3DXVECTOR3 CheckPointStart = CModeGame::GetTrack()->GetCheckPointStart(i); //始点
        D3DXVECTOR3 CheckPointEnd = CModeGame::GetTrack()->GetCheckPointEnd(i);     //終点

                                                                                    //チェックポイントからの距離を計算
        float Player1length = D3DXVec3Length(&(NearPosOnLine(m_Pos, CheckPointStart, CheckPointEnd) - m_Pos));
        float Player2length = D3DXVec3Length(&(NearPosOnLine(CModeGame::GetPlayer2()->GetPos(), CheckPointStart, CheckPointEnd) - CModeGame::GetPlayer2()->GetPos()));

        //チェックポイントを通過した時
        if (Player1length <= 50.0f)
        {
            m_PassingCheckPoint = i;
            if (m_PassingCheckPoint == CHECKPOINT_NUM - 1 && m_HalfGoal == true) m_Goal = true;  //ゴール
            if (m_PassingCheckPoint == CHECKPOINT_NUM / 2) m_HalfGoal = true;//半分ゴール
            else if (m_PassingCheckPoint == CHECKPOINT_NUM - 1 && m_HalfGoal == false) m_PassingCheckPoint = 0;
        }
        if (Player2length <= 50.0f)
        {
            CModeGame::GetPlayer2()->SetPassingCheckPoint(i);
            if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM - 1 && CModeGame::GetPlayer2()->GetHalfGoal() == true) CModeGame::GetPlayer2()->SetGoal(true);      //ゴール
            if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM / 2) CModeGame::GetPlayer2()->SetHalfGoal(true);  //半分ゴール
            else if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM - 1 && CModeGame::GetPlayer2()->GetHalfGoal() == false) CModeGame::GetPlayer2()->SetPassingCheckPoint(0);
        }
    }

    //チェックポイント番号が大きい方が上位
    if (m_PassingCheckPoint >= CModeGame::GetPlayer2()->GetPassingCheckPoint())
    {
        m_Ranking = 0;
        //チェックポイントが同じ場合
        if (m_PassingCheckPoint == CModeGame::GetPlayer2()->GetPassingCheckPoint())
        {
            //チェックポイント（線分）
            D3DXVECTOR3 CheckPointStart = CModeGame::GetTrack()->GetCheckPointStart(m_PassingCheckPoint + 1); //始点
            D3DXVECTOR3 CheckPointEnd = CModeGame::GetTrack()->GetCheckPointEnd(m_PassingCheckPoint + 1);     //終点

                                                                                                              //チェックポイントからの距離を計算
            float Player1length = D3DXVec3Length(&(NearPosOnLine(m_Pos, CheckPointStart, CheckPointEnd) - m_Pos));
            float Player2length = D3DXVec3Length(&(NearPosOnLine(CModeGame::GetPlayer2()->GetPos(), CheckPointStart, CheckPointEnd) - CModeGame::GetPlayer2()->GetPos()));

            //チェックポイントからの距離が近い方が上位
            if (Player1length <= Player2length) m_Ranking = 0;
            else m_Ranking = 1;
        }
    }
    else
    {
        m_Ranking = 1;
    }

    //順位変動処理
    if (m_Ranking != CModeGame::GetRanking()->GetRanking()) CModeGame::GetRanking()->SetRanking(m_Ranking);
}

//*************************************************************************************************
// プレイヤーとトラックの衝突処理
//*************************************************************************************************
void CPlayer::PlayerToTrackColision(void)
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
            m_ItemNum -= 5;
            if (m_ItemNum <= 0) m_ItemNum = 0;
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
void CPlayer::PlayerToItemColision(void)
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
                m_ItemNum++;
                CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM);
            }
        }
        scene = scene->GetNext();
    }

    //アイテム取得個数カウントアップ
    CModeGame::GetGear()->SetItemNum(m_ItemNum);
}

//*************************************************************************************************
// プレイヤーネットワーク処理
//*************************************************************************************************
void CPlayer::PlayerNetwork(void)
{
    DATA data;

    data.Type = DATA_TYPE_POSITION;         //座標
    data.Position.x = m_Pos.x;
    data.Position.y = m_Pos.y;
    data.Position.z = m_Pos.z;
    CSocket::SendData(data);

    data.Type = DATA_TYPE_ROTATION;         //回転
    data.Rotation.x = m_Rot.x;
    data.Rotation.y = m_Rot.y;
    data.Rotation.z = m_Rot.z;
    CSocket::SendData(data);

    data.Type = DATA_TYPE_MOTION;           //モーション
    data.Motion.WaitPlay = m_Motion[WAIT]->GetPlay();
    data.Motion.WalkPlay = m_Motion[WALK]->GetPlay();
    data.Motion.RunPlay = m_Motion[RUN]->GetPlay();
    data.Motion.PuntchPlay = m_Motion[PUNTCH]->GetPlay();
    CSocket::SendData(data);

    int GameEnd = CModeGame::GetGameEnd();

    //ゲーム勝利
    if (GameEnd == GAME_PLAY)
    {
        //ゴールした時
        if (m_Goal == true && m_HalfGoal == true && m_GoalEffect == false)
        {
            data.Type = DATA_TYPE_EVENT;
            data.Event.Type = DATA_EVENT_TYPE_GOAL;

            CSocket::SendData(data);

            CModeGame::SetGameEnd(GAME_GOAL);

            //ゴールしたタイム記録を残す
            CManager::SetPlayerTime(1, CModeGame::GetTime()->GetTime());

            data.Type = DATA_TYPE_TIME;

            //ゴールした時のタイムを相手に送信
            data.Time.Time = (int)CModeGame::GetTime()->GetTime();

            CSocket::SendData(data);

            CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GOAL);
            m_GoalEffect = true;
        }
    }
}



//*************************************************************************************************
// 先頭の空白(スペース，タブ)を削除
//*************************************************************************************************
inline void DeleteHeadSpace(string &buf)
{
    size_t pos;
    while ((pos = buf.find_first_of(" 　\t")) == 0) {
        buf.erase(buf.begin());
        if (buf.empty()) break;
    }
}

//*************************************************************************************************
// 数値をにvalueセットする
//*************************************************************************************************
inline void SetValue(string str, int *value)
{
    int start = str.find_first_of("-0123456789");
    int end = str.length();
    *value = atoi(str.substr(start, end - start).c_str());
}

//*************************************************************************************************
// XYZ座標をD3DXVECTOR3にセットする
//*************************************************************************************************
inline void SetXYZ(string str, D3DXVECTOR3 *pos)
{
    int start = 0;
    int end = 0;
    float XYZ[3];
    for (int i = 0; i < 3; i++)
    {
        start = str.find_first_of("-0123456789", end);
        end = str.find_first_of(" ", start);
        XYZ[i] = atof(str.substr(start, end - start).c_str());
    }
    *pos = D3DXVECTOR3(XYZ[0], XYZ[1], XYZ[2]);
}

//*************************************************************************************************
// KEYをCKeyにセットする
//*************************************************************************************************
inline void SetKey(vector<string>::const_iterator it, CKey *key)
{
    //座標
    it++;
    D3DXVECTOR3 Pos;
    SetXYZ(it->c_str(), &Pos);
    key->SetPos(Pos);

    //回転
    it++;
    D3DXVECTOR3 Rot;
    SetXYZ(it->c_str(), &Rot);
    key->SetRot(Rot);
}

//*************************************************************************************************
// KEYFRAMEをCKeyFrameにセットする
//*************************************************************************************************
inline void SetKeyFrame(vector<string>::const_iterator it, CKeyFrame *keyframe, int numPart)
{
    it++;

    //FRAME
    int frame;
    SetValue(it->c_str(), &frame);
    keyframe->SetFrame(frame);
    it++;

    //KEY
    for (int i = 0; i < numPart; i++)
    {
        CKey key;
        SetKey(it, &key);
        keyframe->SetKey(key);
        it += 4;            //次のKEY文字位置へ
    }
}