//*****************************************************************************
// ジョイスティック処理 (joystick.cpp)
// Author : YUHEI KAN
//*****************************************************************************
#include "input.h"
#include "joystick.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNT_WAIT_REPEAT   (20)                // リピート待ち時間

//*****************************************************************************
// 静的変数
//*****************************************************************************
LPDIRECTINPUTDEVICE8 CInputJoystick::m_pDIDevice;
DIDEVCAPS CInputJoystick::m_diDevCaps;

//*****************************************************************************
// CInputJoystickコンストラスタ
//*****************************************************************************
CInputJoystick::CInputJoystick()
{
    m_pDIDevice = NULL;

    // 各ワークのクリア
    /*ZeroMemory(m_aJoyState, sizeof m_aJoyState);
    ZeroMemory(m_aJoyStateTrigger, sizeof m_aJoyStateTrigger);
    ZeroMemory(m_aJoyStateRelease, sizeof m_aJoyStateRelease);
    ZeroMemory(m_aJoyStateRepeat, sizeof m_aJoyStateRepeat);
    ZeroMemory(m_aJoyStateRepeatCnt, sizeof m_aJoyStateRepeatCnt);*/
}

//*****************************************************************************
// CInputJoystickデストラスタ
//*****************************************************************************
CInputJoystick::~CInputJoystick()
{
}

//*****************************************************************************
// ジョイスティックの初期化処理
//*****************************************************************************
HRESULT CInputJoystick::Init(HINSTANCE hInst, HWND hWnd)
{
    // 入力処理の初期化
    CInput::Init(hInst, hWnd);

    // デバイスの作成
    if (FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY)) 
        || m_pDIDevice == NULL )                                                 //ジョイスティック
    {
        MessageBox(NULL,"DirectInputDevice8オブジェクトの作成に失敗","警告",MB_OK);
        //return E_FAIL;
    }
    else
    {
        // データフォーマットを設定
        if(FAILED(m_pDIDevice->SetDataFormat(&c_dfDIJoystick2)))                      //ジョイスティック
        {
            MessageBox(hWnd, "ジョイスティックのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
            return E_FAIL;
        }

        // 協調モードを設定（フォアグラウンド＆非排他モード）
        if(FAILED(m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))  //ジョイスティック
        {
            MessageBox(hWnd, "ジョイスティックの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
            return E_FAIL;
        }

        //コールバック関数を使って各軸のモードを設定
        if(FAILED(m_pDIDevice->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS ) ) )
        {
            MessageBox(hWnd, "各軸モードの設定に失敗しました", "警告！", MB_ICONWARNING);
            return E_FAIL;
        }

        // 入力制御開始
        m_pDIDevice->Acquire();
    }

    return S_OK;
}

//*****************************************************************************
// ジョイスティック入力処理の開放
//*****************************************************************************
void CInputJoystick::Uninit(void)
{
    if (m_pDIDevice)
    {// デバイスオブジェクトの開放
        m_pDIDevice->Unacquire();

        m_pDIDevice->Release();
        m_pDIDevice = NULL;
    }

    // 入力処理の開放
    CInput::Uninit();
}

//*****************************************************************************
// ジョイスティックの更新処理
//*****************************************************************************
void CInputJoystick::Update(void)
{
    DIJOYSTATE2 aJoyState;

    //初期化処理（ゲームパッドがない時は必要）
    m_aJoyStateTrigger.rgdwPOV[0] = 1;
    m_aJoyStateRelease.rgdwPOV[0] = 1;

    if(m_pDIDevice != NULL)
    {
        // ジョイスティックデバイスからデータを取得
        if ( SUCCEEDED ( m_pDIDevice->GetDeviceState ( sizeof ( DIJOYSTATE2 ), &aJoyState ) ) )
        {
            //ジョイトリガー・リリース情報の取得
            if ( m_aJoyState.rgdwPOV[0] != aJoyState.rgdwPOV[0] )
            {
                m_aJoyStateTrigger.rgdwPOV[0] = aJoyState.rgdwPOV[0];
                m_aJoyStateRelease.rgdwPOV[0] = m_aJoyState.rgdwPOV[0];
            }
            else
            {
                m_aJoyStateTrigger.rgdwPOV[0] = 1;
                m_aJoyStateRelease.rgdwPOV[0] = 1;
            }
            m_aJoyStateTrigger.lY = ( m_aJoyState.lY != aJoyState.lY ) * aJoyState.lY;
            m_aJoyStateTrigger.lX = ( m_aJoyState.lX != aJoyState.lX ) * aJoyState.lX;
            m_aJoyStateRelease.lY = ( m_aJoyState.lY != aJoyState.lY ) * m_aJoyState.lY;
            m_aJoyStateRelease.lX = ( m_aJoyState.lX != aJoyState.lX ) * m_aJoyState.lX;
            m_aJoyStateTrigger.lRz = ( m_aJoyState.lRz != aJoyState.lRz ) * aJoyState.lRz;
            m_aJoyStateRelease.lRz = ( m_aJoyState.lRz != aJoyState.lRz ) * m_aJoyState.lRz;
            m_aJoyStateTrigger.lZ = ( m_aJoyState.lZ != aJoyState.lZ ) * aJoyState.lZ;
            m_aJoyStateRelease.lZ = ( m_aJoyState.lZ != aJoyState.lZ ) * m_aJoyState.lZ;

            for ( int nCnJoy = 0; nCnJoy < NUM_JOY_MAX; nCnJoy++ )
            {
                //ジョイトリガー・リリース情報の取得
                m_aJoyStateTrigger.rgbButtons[nCnJoy] = ( m_aJoyState.rgbButtons[nCnJoy] ^ aJoyState.rgbButtons[nCnJoy] ) & aJoyState.rgbButtons[nCnJoy];
                m_aJoyStateRelease.rgbButtons[nCnJoy] = ( m_aJoyState.rgbButtons[nCnJoy] ^ aJoyState.rgbButtons[nCnJoy] ) & m_aJoyState.rgbButtons[nCnJoy];

                 //ジョイリピート情報の取得
                 if ( aJoyState.rgbButtons[nCnJoy] & 0x80 )
                 {
                     if ( m_aJoyStateRepeatCnt < COUNT_WAIT_REPEAT )
                     {
                         m_aJoyStateRepeatCnt++;
                         if ( m_aJoyStateRepeatCnt == 1 || m_aJoyStateRepeatCnt >= COUNT_WAIT_REPEAT )
                         {
                             m_aJoyStateRepeat.rgbButtons[nCnJoy] = aJoyState.rgbButtons[nCnJoy];
                         }
                         else
                         {
                             m_aJoyStateRepeat.rgbButtons[nCnJoy] = 0;
                         }
                     }
                 }
                 else
                 {
                     m_aJoyStateRepeat.rgbButtons[nCnJoy] = 0;
                     m_aJoyStateRepeatCnt = 0;
                 }

                 // ジョイプレス情報を保存
                 m_aJoyState.rgbButtons[nCnJoy] = aJoyState.rgbButtons[nCnJoy];
            }

            // ジョイプレス情報を保存
            m_aJoyState.rgdwPOV[0] = aJoyState.rgdwPOV[0];
            m_aJoyState.lY = aJoyState.lY;
            m_aJoyState.lX = aJoyState.lX;
            m_aJoyState.lRz = aJoyState.lRz;
            m_aJoyState.lZ = aJoyState.lZ;
        }
        else
        {
            // ジョイスティックへのアクセス権を取得
            m_pDIDevice->Acquire();
        }
    }
}

//*************************************************************************************************
// ジョイスティックを列挙する関数
//*************************************************************************************************
BOOL CALLBACK CInputJoystick::EnumJoystickCallback ( const DIDEVICEINSTANCE* pdidInstance, void* pContext )
{
    UNREFERENCED_PARAMETER(pdidInstance);
    UNREFERENCED_PARAMETER(pContext);

    static GUID pad_discrimination;             //各デバイスの識別子を格納
    //DIDEVCAPS diDevCaps;						//ジョイパッドの能力情報

    // 既に取得済みのデバイスだった場合はもう一回実行する
    if ( pad_discrimination == pdidInstance->guidInstance )
    {
        return DIENUM_CONTINUE;
    }

    // 列挙されたジョイスティックへのインターフェイスを取得する。
    if (FAILED(m_pDInput->CreateDevice(pdidInstance->guidInstance, &m_pDIDevice, NULL)))
    {
        return DIENUM_CONTINUE;
    }

    // ジョイスティックの能力を調べる
    m_diDevCaps.dwSize = sizeof(DIDEVCAPS);

    if (FAILED(m_pDIDevice->GetCapabilities(&m_diDevCaps)))
    {
        // ジョイスティック能力の取得に失敗
        m_pDIDevice->Release ( );
        m_pDIDevice = NULL;
    }

    return DIENUM_STOP;
}

//*************************************************************************************************
// 軸ごとに呼び出される関数
//*************************************************************************************************
BOOL CALLBACK CInputJoystick::EnumAxesCallback ( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef )
{
    if ( lpddoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        ZeroMemory ( &diprg, sizeof ( diprg ) );
        diprg.diph.dwSize           = sizeof ( diprg );
        diprg.diph.dwHeaderSize     = sizeof ( diprg.diph );
        diprg.diph.dwObj            = lpddoi->dwType;
        diprg.diph.dwHow            = DIPH_BYID;
        diprg.lMin                  = 0 - 1000;
        diprg.lMax                  = 0 + 1000;

        if ( FAILED ( m_pDIDevice->SetProperty ( DIPROP_RANGE, &diprg.diph ) ) )
        {
            return DIENUM_STOP;
        }
    }

    return DIENUM_CONTINUE;
}

//*****************************************************************************
// ジョイスティックデータ取得(プレス)
//*****************************************************************************
BOOL CInputJoystick::GetJoyPress(DIJ Input,int nJoy)
{
    switch ( Input )
    {
    case DIJ_BUTTON:
        return (m_aJoyState.rgbButtons[nJoy] & 0x80) ? true: false;

    case DIJ_POV:
        return (m_aJoyState.rgdwPOV[0] == nJoy) ? true: false;

    case DIJ_L_STICX:
        return ( nJoy - 500 <= m_aJoyState.lX && m_aJoyState.lX <= nJoy ) ? true: false;

    case DIJ_L_STICY:
        return ( nJoy - 500 <= m_aJoyState.lY && m_aJoyState.lY <= nJoy ) ? true: false;

    case DIJ_R_STICX:
        return (nJoy - 500 <= m_aJoyState.lZ && m_aJoyState.lZ <= nJoy) ? true: false;

    case DIJ_R_STICY:
        return (nJoy - 500 <= m_aJoyState.lRz && m_aJoyState.lRz <= nJoy) ? true: false;
    }
    return false;
}

//*****************************************************************************
// ジョイスティックデータ取得(トリガー)
//*****************************************************************************
BOOL CInputJoystick::GetJoyTrigger(DIJ Input,int nJoy)
{
    switch ( Input )
    {
    case DIJ_BUTTON:
        return (m_aJoyStateTrigger.rgbButtons[nJoy] & 0x80) ? true: false;

    case DIJ_POV:
        return (m_aJoyStateTrigger.rgdwPOV[0] == nJoy) ? true: false;

    case DIJ_L_STICX:
        return ( nJoy - 500 <= m_aJoyStateTrigger.lX && m_aJoyStateTrigger.lX <= nJoy ) ? true: false;

    case DIJ_L_STICY:
        return ( nJoy - 500 <= m_aJoyStateTrigger.lY && m_aJoyStateTrigger.lY <= nJoy ) ? true: false;

    case DIJ_R_STICX:
        return (nJoy - 500 <= m_aJoyStateTrigger.lZ && m_aJoyStateTrigger.lZ <= nJoy ) ? true: false;

    case DIJ_R_STICY:
        return (nJoy - 500 <= m_aJoyStateTrigger.lRz && m_aJoyStateTrigger.lRz <= nJoy ) ? true: false;
    }
    return false;
}

//*****************************************************************************
// ジョイスティックデータ取得(リリース)
//*****************************************************************************
BOOL CInputJoystick::GetJoyRelease(DIJ Input,int nJoy)
{
    switch ( Input )
    {
    case DIJ_BUTTON:
        return (m_aJoyStateRelease.rgbButtons[nJoy] & 0x80) ? true: false;

    case DIJ_POV:
        return (m_aJoyStateRelease.rgdwPOV[0] == nJoy) ? true: false;

    case DIJ_L_STICX:
        return (nJoy - 500 <= m_aJoyStateRelease.lX && m_aJoyStateRelease.lX <= nJoy) ? true: false;

    case DIJ_L_STICY:
        return (nJoy - 500 <= m_aJoyStateRelease.lY && m_aJoyStateRelease.lY <= nJoy) ? true: false;

    case DIJ_R_STICX:
        return (nJoy - 500 <= m_aJoyStateRelease.lZ && m_aJoyStateRelease.lZ <= nJoy) ? true: false;

    case DIJ_R_STICY:
        return (nJoy - 500 <= m_aJoyStateRelease.lRz && m_aJoyStateRelease.lRz <= nJoy) ? true: false;
    }
    return false;
}

//*****************************************************************************
// ジョイスティックデータ取得(リピート)
//*****************************************************************************
BOOL CInputJoystick::GetJoyRepeat(DIJ Input,int nJoy)
{
    return (m_aJoyStateRepeat.rgbButtons[nJoy] & 0x80) ? true: false;
}

//*****************************************************************************
// ジョイスティックデータ削除(トリガー)
//*****************************************************************************
void CInputJoystick::FlushJoyTrigger(DIJ Input,int nJoy)
{
    switch ( Input )
    {
    case DIJ_BUTTON:
        m_aJoyStateTrigger.rgbButtons[nJoy] = 0;

    case DIJ_POV:
        m_aJoyStateTrigger.rgdwPOV[0] =0;

    case DIJ_L_STICX:
        m_aJoyStateTrigger.lX = 0;

    case DIJ_L_STICY:
        m_aJoyStateTrigger.lY = 0;

    case DIJ_R_STICX:
        m_aJoyStateTrigger.lZ = 0;

    case DIJ_R_STICY:
        m_aJoyStateTrigger.lRz = 0;
    }
}

//*****************************************************************************
//	左スティックの傾き値取得（上下左右共に-1000～+1000）
//*****************************************************************************
POINT CInputJoystick::GetLeftStickDisposition(void)
{
    POINT LeftStick = { NULL,NULL };

    DIJOYSTATE2 DevJoystick;

    // スティックの傾き値を取得
    m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &DevJoystick);

    LeftStick.x = DevJoystick.lX;
    LeftStick.y = DevJoystick.lY;

    if (LeftStick.x >= -PS4_STICK_ENABLE && LeftStick.x <= PS4_STICK_ENABLE)
    {
        LeftStick.x = 0;
    }

    if (LeftStick.y >= -PS4_STICK_ENABLE && LeftStick.y <= PS4_STICK_ENABLE)
    {
        LeftStick.y = 0;
    }

    return LeftStick;
}

//*****************************************************************************
//	右スティックの傾き値取得（上下左右共に-1000～+1000）
//*****************************************************************************
POINT CInputJoystick::GetRightStickDisposition(void)
{
    POINT RightStick = { NULL,NULL };

    DIJOYSTATE2 DevJoystick;

    // スティックの傾き値を取得
    m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &DevJoystick);

    RightStick.x = DevJoystick.lZ;
    RightStick.y = DevJoystick.lRz;

    if (RightStick.x >= -PS4_STICK_ENABLE && RightStick.x <= PS4_STICK_ENABLE)
    {
        RightStick.x = 0;
    }

    if (RightStick.y >= -PS4_STICK_ENABLE && RightStick.y <= PS4_STICK_ENABLE)
    {
        RightStick.y = 0;
    }

    return RightStick;
}