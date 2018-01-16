//*************************************************************************************************
// マネージャ処理 (manager.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "manager.h"
#include "renderer.h"
#include "renderTarget2D.h"
#include "keyboard.h"
#include "mouse.h"
#include "joystick.h"
#include "sound.h"
#include "socket.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "modeResult.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshDome.h"
#include "sceneModel.h"
#include "player.h"
#include "item.h"

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
CSocket *CManager::m_Socket = NULL;                 //ソケットクラスのポインタ
CInputKeyboard *CManager::m_CInputKeyboard = NULL;  //キーボード入力クラスのポインタ
CInputMouse *CManager::m_CInputMouse = NULL;        //マウス入力クラスのポインタ
CInputJoystick *CManager::m_CInputJoystick = NULL;  //ジョイスティック入力クラスのポインタ
CSound *CManager::m_Sound = NULL;                   //Soundクラスのポインタ
CMode *CManager::m_Mode = NULL;                     //モードクラスのポインタ
CMode *CManager::m_ModeNext = NULL;                 //モードネクストクラスのポインタ
CRenderTarget2D *CManager::m_RenderTarget2D = NULL; //レンダーターゲット2Dクラスのポインタ
int CManager::Player1Time = 0;                      //プレイヤー１のゴールタイム
int CManager::Player2Time = 0;                      //プレイヤー２のゴールタイム

bool CManager::m_WireFrame = false;                 //ワイヤーフレームフラグ

//*************************************************************************************************
// マネージャのコンストラクタ
//*************************************************************************************************
CManager::CManager ()
{
}

//*************************************************************************************************
// マネージャのデストラクタ
//*************************************************************************************************
CManager::~CManager ()
{
}

//*************************************************************************************************
//初期化処理
//*************************************************************************************************
HRESULT CManager::Init ( HINSTANCE hInstance, HWND hWnd, BOOL bWindow )
{
    //レンダラの初期化処理
    if ( FAILED ( CRenderer::Init ( hInstance, hWnd, bWindow ) ) )
    {
        return E_FAIL;
    }

    //ソケットクラスの生成と初期化
    m_Socket = new CSocket;
    if( FAILED ( m_Socket->Init ( ) ) )
    {
        return E_FAIL;
    }

    // データ送信
    DATA data;
    data.Type = DATA_TYPE_ID;         //ID
    CSocket::SendData(data);

    //キーボード入力クラスの生成と初期化
    m_CInputKeyboard = new CInputKeyboard;
    if ( FAILED ( m_CInputKeyboard->Init ( hInstance, hWnd ) ) )
    {
        return E_FAIL;
    }

    //マウス入力クラスの生成と初期化
    m_CInputMouse = new CInputMouse;
    if ( FAILED ( m_CInputMouse->Init ( hInstance, hWnd ) ) )
    {
        return E_FAIL;
    }

    //ジョイスティック入力クラスの生成と初期化
    m_CInputJoystick = new CInputJoystick;
    if (FAILED(m_CInputJoystick->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    //Xactクラスの生成と初期化
    m_Sound = new CSound;
    if ( FAILED (m_Sound->Init ( hWnd ) ) )
    {
        return E_FAIL;
    }

    //モードクラスの生成と初期化処理
    SetMode ( new CModeTitle );
    m_Mode->Init();

    //レンダーターゲット2Dクラスの生成と初期化処理
    m_RenderTarget2D = new CRenderTarget2D;
    if (FAILED(m_RenderTarget2D->Init()))
    {
        return E_FAIL;
    }

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CManager::Uninit ( void )
{
    //シーンクラスの全ての終了処理
    CScene::ReleaseAll ( );

    //レンダーターゲット2Dクラスの終了処理
    if (m_RenderTarget2D != NULL)
    {
        m_RenderTarget2D->Uninit();
        delete m_RenderTarget2D;
    }

    //モードクラスの終了処理
    if ( m_Mode != NULL )
    {
        m_Mode->Uninit();
        delete m_Mode;
    }

    //Xactクラスの終了処理
    if (m_Sound != NULL)
    {
        m_Sound->Uninit();
        delete m_Sound;
    }

    //ジョイスティック入力クラスの終了
    if (m_CInputJoystick != NULL)
    {
        m_CInputJoystick->Uninit();
        delete m_CInputJoystick;
    }

    //マウス入力クラスの終了
    if ( m_CInputMouse != NULL )
    {
        m_CInputMouse->Uninit ( );
        delete m_CInputMouse;
    }

    //キーボード入力クラスの終了
    if ( m_CInputKeyboard != NULL )
    {
        m_CInputKeyboard->Uninit ( );
        delete m_CInputKeyboard;
    }

    //ソケットクラスの終了
    if ( m_Socket != NULL )
    {
        m_Socket->Uninit ( );
        delete m_Socket;
    }

    //レンダラクラスの終了処理
    CRenderer::Uninit ( );
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CManager::Update ( void )
{
    //レンダラクラスの更新処理
    CRenderer::Updata ( );

    //キーボード入力クラスの更新
    m_CInputKeyboard->Update ( );

    //マウス入力クラスの更新
    m_CInputMouse->Update ( );

    //ジョイスティック入力クラスの更新
    m_CInputJoystick->Update();

    //シーンクラスの全ての更新処理
    CScene::UpdataAll ( );

    //モードクラスの更新処理
    m_Mode->Update();

    //レンダーターゲット2Dクラスの更新処理
    m_RenderTarget2D->Update();

#ifdef _DEBUG
    //ワイヤーフレームON/OFF
    if ( m_CInputKeyboard->GetKeyTrigger ( DIK_F1 ) )
    {
        if ( m_WireFrame == false )
        {
            m_WireFrame = true;
        }
        else
        {
            m_WireFrame = false;
        }
    }
#endif //_DEBUG
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CManager::Draw ( void )
{
    //フィードバックブラー
    /*if (m_RenderTarget2D->GetFeedbackBlur())
    {*/
        //レンダーターゲット2Dクラスの描画処理
        m_RenderTarget2D->Draw();
    /*}
    else
    {

    }*/

#ifdef _DEBUG
    //Debug描画処理
    DrawDebug();
#endif //_DEBUG
}

//*************************************************************************************************
// キーボード入力クラスの取得
//*************************************************************************************************
CInputKeyboard *CManager::GetInputKeyboard ( void )
{
    return m_CInputKeyboard;
}

//*************************************************************************************************
// マウス入力クラスの取得
//*************************************************************************************************
CInputMouse *CManager::GetInputMouse ( void )
{
    return m_CInputMouse;
}

//*************************************************************************************************
// ジョイスティック入力クラスの取得
//*************************************************************************************************
CInputJoystick *CManager::GetInputJoystick(void)
{
    return m_CInputJoystick;
}

//*************************************************************************************************
// サウンドクラスの取得
//*************************************************************************************************
CSound *CManager::GetSound(void)
{
    return m_Sound;
}

//*************************************************************************************************
// モードの取得
//*************************************************************************************************
CMode *CManager::GetMode(void)
{
    return m_Mode;
}

//*************************************************************************************************
// レンダーターゲット2Dクラスの取得
//*************************************************************************************************
CRenderTarget2D *CManager::GetRenderTarget2D(void)
{
    return m_RenderTarget2D;
}

//*************************************************************************************************
// モードの設定
//*************************************************************************************************
void CManager::SetMode ( CMode *Mode )
{
    //現在のモードの終了処理
    if ( m_Mode != NULL )
    {
        m_Mode->Uninit();
        delete m_Mode;
    }

    //引数でもらったモードを現在のモードに設定
    m_Mode = Mode;

    //現在のモードの初期化処理
    if ( m_Mode != NULL )
    {
        m_Mode->Init();
    }
}

//*************************************************************************************************
// フェードの処理
//*************************************************************************************************
void CManager::Fade ( CMode *ModeNext )
{
}

//*************************************************************************************************
// ワイヤーフレームフラグ取得
//*************************************************************************************************
bool CManager::GetWireFrame ( void )
{
    return m_WireFrame;
}

//*************************************************************************************************
// プレイヤーのタイムを取得
//*************************************************************************************************
int CManager::GetPlayerTime(int Player)
{
    if (Player == 1) return Player1Time;
    if (Player == 2) return Player2Time;
}

//*************************************************************************************************
// プレイヤーのタイムを取得
//*************************************************************************************************
void CManager::SetPlayerTime(int Player, int Time)
{
    if (Player == 1) Player1Time = Time;
    if (Player == 2) Player2Time = Time;
}

#ifdef _DEBUG
//*************************************************************************************************
// Debug描画処理
//************************************************************************************************
void CManager::DrawDebug ( void )
{
    //変数宣言
    LPD3DXFONT Font = CRenderer::GetFont ( );
    char aStr[256];

    //FPS描画
    int nCountFPS = GetFPS ( );
    sprintf ( &aStr[0], "FPS:%d\n", nCountFPS );
    RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    Font->DrawText ( NULL, &aStr[0] , -1, &rc, DT_LEFT, D3DCOLOR_RGBA ( 0, 255, 0 , 255 ) );

    //ワイヤーフレーム描画
    if ( m_WireFrame == false )
    {
        sprintf ( &aStr[0], "WIREFRAME[F1]:OFF" );
    }
    else
    {
        sprintf ( &aStr[0], "WIREFRAME[F1]:ON" );
    }
    RECT rc2 = { 70, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    Font->DrawText ( NULL, &aStr[0] , -1, &rc2, DT_LEFT, D3DCOLOR_RGBA ( 0, 255, 0, 255 ) );
    
    //プレイヤー
    //CMode *Mode = m_Mode;
    ////if (m_Mode == CModeGame())
    //{
    //    CPlayer *Player = CModeGame::GetPlayer();
    //    D3DXVECTOR3 Pos = Player->GetPos();
    //    sprintf(&aStr[0], "[プレイヤー]\n<座標>   ( X:%f Y:%f Z:%f )\n", Pos.x, Pos.y, Pos.z);
    //    RECT rc3 = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
    //    Font->DrawText(NULL, &aStr[0], -1, &rc3, DT_LEFT, D3DCOLOR_RGBA(0, 255, 0, 255));
    //}

    //カメラ描画
    //CCamera *m_Camera = CModeGame::GetCamera();
    //D3DXVECTOR3 posEye = m_Camera->GetPos ( );
    //D3DXVECTOR3 posAt = m_Camera->GetPosAt ( );
    //sprintf ( &aStr[0], "[カメラ]\n<視点>   ( X:%f Y:%f Z:%f )\n<注視点> ( X:%f Y:%f Z:%f )\n", 
    //                        posEye.x, posEye.y, posEye.z, posAt.x, posAt.y, posAt.z );
    //RECT rc3 = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
    //Font->DrawText( NULL, &aStr[0] , -1, &rc3, DT_LEFT, D3DCOLOR_RGBA( 0, 0, 0, 255 ) );// -1は全部
}
#endif //_DEBUG
