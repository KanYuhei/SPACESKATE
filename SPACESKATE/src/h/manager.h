﻿//*************************************************************************************************
// マネージャ処理 (manager.h)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードガード
//*************************************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "main.h"

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
// 前方宣言
//*************************************************************************************************
class CSocket;
class CInputKeyboard;
class CInputMouse;
class CInputJoystick;
class CSound;
class CMode;
class CScene;
class CRenderTarget2D;

//*************************************************************************************************
// クラス
//*************************************************************************************************
//マネージャクラス
class CManager
{
public:
    CManager ( );                                       //コンストラクタ
    ~CManager ( );                                      //デコンストラクタ

    static HRESULT Init ( HINSTANCE hInstance, 
                          HWND hWnd, 
                          BOOL bWindow );               //初期化処理
    static void Uninit ( void );                        //終了処理
    static void Update ( void );                        //更新処理
    static void Draw ( void );                          //描画処理

    static CInputKeyboard *GetInputKeyboard ( void );   //キーボード入力クラスの取得
    static CInputMouse *GetInputMouse ( void );         //マウス入力クラスの取得
    static CInputJoystick *GetInputJoystick ( void );   //ジョイスティック入力クラスの取得
    static CSound *GetSound ( void );                   //サウンドクラスの取得
    static CMode *GetMode(void);                        //モードの取得
    static CRenderTarget2D *GetRenderTarget2D(void);    //レンダーターゲット2Dクラスの取得
    static bool GetWireFrame ( void );                  //ワイヤーフレームフラグの取得
    static int GetPlayerTime(int Player);               //プレイヤーのタイムを取得

    static void SetMode ( CMode *Mode );                //モードの設定
    static void Fade ( CMode *ModeNext );               //フェード処理
    static void SetPlayerTime(int Player, int Time);    //プレイヤーのタイムを設定

#ifdef _DEBUG
    static void DrawDebug ( void );             //Debug描画処理
#endif //_DEBUG

private:
    static CSocket *m_Socket;                   //ソケットクラスのポインタ
    static CInputKeyboard *m_CInputKeyboard;    //キーボード入力クラスのポインタ
    static CInputMouse *m_CInputMouse;          //マウス入力クラスのポインタ
    static CInputJoystick *m_CInputJoystick;    //ジョイスティック入力クラスのポインタ
    static CSound *m_Sound;                     //サウンドクラスのポインタ
    static bool m_WireFrame;                    //ワイヤーフレームフラグ
    static CMode *m_Mode;                       //モードクラスのポインタ
    static CMode *m_ModeNext;                   //モードネクストクラスのポインタ
    static CRenderTarget2D *m_RenderTarget2D;   //レンダーターゲット2Dクラスのポインタ
    static int Player1Time;                     //プレイヤー１のゴールタイム
    static int Player2Time;                     //プレイヤー２のゴールタイム
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_MANAGER_H_