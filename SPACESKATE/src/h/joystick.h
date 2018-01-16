//*****************************************************************************
// ジョイスティック処理 (joystick.h)
// Author : YUHEI KAN
//*****************************************************************************
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_JOY_MAX     (32)   // ジョイスティックボタンの最大数
// DirectInput Joystick (PS4)
#define DIJ_SQUARE           (0)
#define DIJ_CROSS            (1)
#define DIJ_CIRCLE           (2)
#define DIJ_TRIANGLE         (3)
#define DIJ_L1               (4)
#define DIJ_R1               (5)
#define DIJ_L2               (6)
#define DIJ_R2               (7)
#define DIJ_SHARE            (8)
#define DIJ_OPTIONS          (9)
#define DIJ_L3               (10)
#define DIJ_R3               (11)
#define DIJ_PS               (12)
#define DIJ_TOUCHPAD         (13)
#define DIJ_UP               (0)
#define DIJ_DOWN             (18000)
#define DIJ_LEFT             (27000)
#define DIJ_RIGHT            (9000)
#define DIJ_L_UP             (-1000)
#define DIJ_L_DOWN           (1000)
#define DIJ_L_LEFT           (-1000)
#define DIJ_L_RIGHT          (1000)
#define DIJ_R_UP             (-1000)
#define DIJ_R_DOWN           (1000)
#define DIJ_R_LEFT           (-990)
#define DIJ_R_RIGHT          (1000)

#define	PS4_STICK_ENABLE	 ( 300 )			// ジョイスティックボタンの最大数

//*****************************************************************************
// クラス
//*****************************************************************************
//ジョイスティック入力クラス
class CInputJoystick : public CInput
{
public:
    //ジョイスティックボタン種類
    typedef enum
    {
        DIJ_NONE,                               //インプット無し
        DIJ_BUTTON,                             //ボタン
        DIJ_POV,                                //方向キー
        DIJ_L_STICX,                            //左スティックX
        DIJ_L_STICY,                            //左スティックY
        DIJ_R_STICX,                            //右スティックX
        DIJ_R_STICY,                            //右スティックY
        DIJ_MAX
    }DIJ;

    CInputJoystick();                           //コンストラクタ
    ~CInputJoystick();                          //デコンストラクタ

    HRESULT Init(HINSTANCE hInst, HWND hWnd);   //初期化処理
    void Uninit(void);                          //終了処理
    void Update(void);                          //更新処理

    POINT GetLeftStickDisposition(void);		// 左スティックの傾き値取得
    POINT GetRightStickDisposition(void);		// 右スティックの傾き値取得
    BOOL GetJoyPress(DIJ Input,int nJoy);       //ジョイスティックデータ取得(プレス)
    BOOL GetJoyTrigger(DIJ Input,int nJoy);     //ジョイスティックデータ取得(トリガー)
    BOOL GetJoyRelease(DIJ Input,int nJoy);     //ジョイスティックデータ取得(リリース)
    BOOL GetJoyRepeat(DIJ Input,int nJoy);      //ジョイスティックデータ取得(リピート)
    void FlushJoyTrigger(DIJ Input,int nJoy);   //ジョイスティックデータ削除(トリガー)

private:
    static BOOL CALLBACK EnumJoystickCallback ( 
        const DIDEVICEINSTANCE* pdidInstance, 
        void* pContext );                       //ジョイスティックを列挙する関数
    static BOOL CALLBACK EnumAxesCallback ( 
        LPCDIDEVICEOBJECTINSTANCE lpddoi, 
        LPVOID pvRef );                         //軸ごとに呼び出される関数

    static LPDIRECTINPUTDEVICE8 m_pDIDevice;    //Deviceオブジェクト(入力に必要)
    static DIDEVCAPS m_diDevCaps;               //ジョイスティックの能力情報
    DIJOYSTATE2 m_aJoyState;                    //ジョイスティックの状態を受け取るワーク
    DIJOYSTATE2 m_aJoyStateTrigger;             //トリガーワーク
    DIJOYSTATE2 m_aJoyStateRelease;             //リリースワーク
    DIJOYSTATE2 m_aJoyStateRepeat;              //リピートワーク
    int  m_aJoyStateRepeatCnt;                  //リピートカウンタ
};

#endif //_JOYSTICK_H_