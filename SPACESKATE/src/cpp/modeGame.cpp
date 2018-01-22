//*************************************************************************************************
// モードゲーム処理 (modegame.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include"manager.h"
#include "keyboard.h"
#include "sound.h"
#include "light.h"
#include "camera.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "modeResult.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshDome.h"
#include "meshBox.h"
#include "sceneModel.h"
#include "meshEffect.h"
#include "monitor.h"
#include "track.h"
#include "player.h"
#include "player2.h"
#include "item.h"
#include "column.h"
#include "number.h"
#include "countdawn.h"
#include "time.h"
#include "gear.h"
#include "ranking.h"
#include "particle.h"

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
CCamera *CModeGame::m_Camera = NULL;            //カメラクラスのポインタ
CLight *CModeGame::m_CLight = NULL;             //ライトクラスのポインタ
CMeshField *CModeGame::m_MeshField = NULL;      //メッシュフィールドクラスのポインタ
CMeshBox *CModeGame::m_MeshBox = NULL;          //メッシュボックスクラスのポインタ
CMeshEffect *CModeGame::m_MeshEffect = NULL;    //メッシュエフェクトクラスのポインタ
CTrack *CModeGame::m_Track = NULL;              //トラッククラスのポインタ
CPlayer *CModeGame::m_Player = NULL;            //プレイヤークラスのポインタ
CPlayer2 *CModeGame::m_Player2 = NULL;          //プレイヤー２クラスのポインタ
CCountdawn *CModeGame::m_CountDawn = NULL;      //カウントダウンクラスのポインタ
CTime *CModeGame::m_Time = NULL;                //タイムクラスのポインタ
CGear *CModeGame::m_Gear = NULL;                //ギアクラスのポインタ
CRanking *CModeGame::m_Ranking = NULL;          //ランキングクラスのポインタ
CParticle *CModeGame::m_Particle = NULL;        //パーティクルクラスのポインタ
int CModeGame::m_GameEnd;                       //ゲームエンド判定用
int CModeGame::m_Player2GameEnd;                //プレイヤー２のゲームエンド判定

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// モードゲームのコンストラクタ
//*************************************************************************************************
CModeGame::CModeGame ( )
{
    m_GameEnd = GAME_PLAY;
    m_Player2GameEnd = GAME_PLAY;
    m_CreateEndGoalResult = false;
    m_toResultTime = 0;
}

//*************************************************************************************************
// モードゲームのデストラクタ
//*************************************************************************************************
CModeGame::~CModeGame ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CModeGame::Init( void )
{
    //ライトクラスの生成と初期化
    m_CLight = new CLight;
    if ( FAILED ( m_CLight->Init ( ) ) )
    {
        return E_FAIL;
    }

    //カメラクラスの生成と初期化
    m_Camera = new CCamera;
    if ( FAILED ( m_Camera->Init ( ) ) )
    {
        return E_FAIL;
    }

    //数値のロード
    CNumber::Load();

    //メッシュボックスクラスの生成と初期化
    m_MeshBox = CMeshBox::Create( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  D3DXVECTOR3( 50000.0f, 50000.0f, 0.0f ),
                                  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) );

    //トラッククラスの生成と初期化
    m_Track = CTrack::Create( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
                              D3DXVECTOR3(1.0f, 1.0f, 1.0f) );

    //円柱クラスの生成と初期化
    CColumn::Create(D3DXVECTOR3(100.0f, 0.0f, 2625.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

    //プレイヤークラスの生成と初期化
    m_Player = CPlayer::Create ( D3DXVECTOR3 ( 0.0f, 0.0f, 2625.0f ),
                                 D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f ) );
    
    //プレイヤー２クラスの生成と初期化
    m_Player2 = CPlayer2::Create(D3DXVECTOR3( 0.0f, 0.0f, 3025.0f),
                                 D3DXVECTOR3(1.0f, 1.0f, 1.0f));

    //アイテムクラスの生成と初期化
    D3DXVECTOR3 Pos;

    // ファイルから読み込む
    FILE* fp;
    if ((fp = fopen("data/TEXT/item.txt", "rt")) != NULL)
    {
        while (fscanf(fp, "%f%f%f", &Pos.x, &Pos.y, &Pos.z) != EOF)
        {
            CItem::Create( Pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f) );
        }
    }
    fclose(fp);

    //エフェクトクラスの生成と初期化
    //m_MeshEffect = CMeshEffect::Create();

    //パーティクルクラスの生成と初期化
    m_Particle = CParticle::Create();

    //モニターの生成と初期化
    CMonitor::Create(D3DXVECTOR3(0.0f, 0.0f, 3625.0f), 
                     D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, 0.0f),
                     D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    //シーン2Dクラスの生成
    CScene2D::Create("data/TEXTURE/GAME/TIME.png",
        D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * 0.005f * 9) * 10), SCREEN_WIDTH * 0.005f, 0.0f),
        D3DXVECTOR3(SCREEN_WIDTH * 9 * 0.01079f, SCREEN_HEIGHT * 16 * 0.00401f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        4);

    //シーン2Dクラスの生成
    CScene2D::Create("data/TEXTURE/GAME/Timedabule.png",
        D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * 0.005f * 9) * 5.25f), SCREEN_WIDTH * 0.007f, 0.0f),
        D3DXVECTOR3(SCREEN_WIDTH * 9 * 0.00059f, SCREEN_HEIGHT * 16 * 0.00332f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        4);

    //シーン2Dクラスの生成
    CScene2D::Create("data/TEXTURE/GAME/Timedot.png",
        D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * 0.005f * 9) * 3.15f), SCREEN_WIDTH * 0.03f, 0.0f),
        D3DXVECTOR3(SCREEN_WIDTH * 9 * 0.00059f, SCREEN_HEIGHT * 16 * 0.00108f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        4);

    //カウントダウンクラスの生成と初期化
    m_CountDawn = CCountdawn::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f), 4);

    //タイムクラスの生成と初期化
    m_Time = CTime::Create ( D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f ), 4 );

    //タイムの設定
    m_Time->SetTime(0.0f);

    //ギアクラスの生成と初期化
    m_Gear = CGear::Create(D3DXVECTOR3(SCREEN_WIDTH / 5.0f, SCREEN_HEIGHT / 1.12f, 0.0f), 4 );

    //ランキングクラスの生成と初期化
    m_Ranking = CRanking::Create(D3DXVECTOR3(SCREEN_WIDTH / 1.1f, SCREEN_HEIGHT / 1.2f, 0.0f), 4);

    //ゲームBGMを再生する
    CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CModeGame::Uninit( void )
{
    //シーンクラスの全ての終了処理
    CScene::ReleaseAll ( );

    //パーティクルの終了処理
    if (m_Particle != NULL )
    {
        m_Particle->Uninit();
        delete m_Particle;
    }

    //数値のアンロード
    CNumber::Unload();

    //カメラクラスの終了処理
    if ( m_Camera != NULL )
    {
        m_Camera->Uninit();
        delete m_Camera;
    }

    //ライトクラスの終了処理
    if ( m_CLight != NULL )
    {
        m_CLight->Uninit();
        delete m_CLight;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CModeGame::Update( void )
{
    //ライトクラスの更新
    m_CLight->Update ( );

    //カメラクラスの更新
    m_Camera->Update ( );

    //シーン2Dクラスの生成(ゴール文字)
    if (m_GameEnd == GAME_GOAL && m_CreateEndGoalResult == false)
    {
        CScene2D::Create("data/TEXTURE/goal.png",
            D3DXVECTOR3((SCREEN_WIDTH - (SCREEN_WIDTH * 9.0f * 0.03316f)) / 2.0f, (SCREEN_HEIGHT - (SCREEN_HEIGHT * 16.0f * 0.01062f)) / 2.0f, 0.0f),
            D3DXVECTOR3(SCREEN_WIDTH * 9.0f * 0.03316f, SCREEN_HEIGHT * 16.0f * 0.01062f, 0.0f),
            D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
            D3DXVECTOR2(0.0f, 0.0f),
            D3DXVECTOR2(1.0f, 1.0f),
            4);
        m_CreateEndGoalResult = true;
    }

    //どちらもゴールした場合リザルトへ
    if (m_GameEnd == GAME_GOAL || m_Player2GameEnd == GAME_GOAL)
    {
        m_toResultTime++;   //リザルトまでの時間の経過
    }

    //モードの設定
    if ( m_toResultTime >= GO_RESULT_TIME)
    {
        //ゲームBGMを停止する
        CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_GAME);

        CManager::SetMode ( new CModeTitle );
    }
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CModeGame::Draw( void )
{
    //カメラをセットする
    m_Camera->Set ( );
}

//*************************************************************************************************
// カメラクラスの取得
//*************************************************************************************************
CCamera *CModeGame::GetCamera ( void )
{
    return m_Camera;
}

//*************************************************************************************************
// メッシュボックスクラスの取得
//*************************************************************************************************
CMeshBox *CModeGame::GetMeshBox(void)
{
    return m_MeshBox;
}

//*************************************************************************************************
// メッシュフィールドクラスの取得
//*************************************************************************************************
CMeshField *CModeGame::GetMeshField(void)
{
    return m_MeshField;
}

//*************************************************************************************************
// エフェクトクラスの取得
//*************************************************************************************************
CMeshEffect *CModeGame::GetMeshEffect(void)
{
    return m_MeshEffect;
}

//*************************************************************************************************
// トラッククラスの取得
//*************************************************************************************************
CTrack *CModeGame::GetTrack(void)
{
    return m_Track;
}

//*************************************************************************************************
// プレイヤークラスの取得
//*************************************************************************************************
CPlayer *CModeGame::GetPlayer ( void )
{
    return m_Player;
}

//*************************************************************************************************
// プレイヤー２クラスの取得
//*************************************************************************************************
CPlayer2 *CModeGame::GetPlayer2(void)
{
    return m_Player2;
}

//*************************************************************************************************
// カウトダウンクラスの取得
//*************************************************************************************************
CCountdawn *CModeGame::GetCountdawn(void)
{
    return m_CountDawn;
}

//*************************************************************************************************
// タイムクラスの取得
//*************************************************************************************************
CTime *CModeGame::GetTime(void)
{
    return m_Time;
}

//*************************************************************************************************
// ギアクラスの取得
//*************************************************************************************************
CGear *CModeGame::GetGear(void)
{
    return m_Gear;
}

//*************************************************************************************************
// ランキングクラスの取得
//*************************************************************************************************
CRanking *CModeGame::GetRanking(void)
{
    return m_Ranking;
}

//*************************************************************************************************
// パーティクルの取得
//*************************************************************************************************
CParticle *CModeGame::GetParticle( void )
{
    return m_Particle;
}

//*************************************************************************************************
// ゲームエンド設定
//*************************************************************************************************
void CModeGame::SetGameEnd(int GameEnd)
{
    m_GameEnd = GameEnd;
}

//*************************************************************************************************
// プレイヤー２ゲームエンド設定
//*************************************************************************************************
void CModeGame::SetPlayer2GameEnd(int GameEnd)
{
    m_Player2GameEnd = GameEnd;
}

//*************************************************************************************************
// ゲームエンド取得
//*************************************************************************************************
int CModeGame::GetGameEnd(void)
{
    return m_GameEnd;
}