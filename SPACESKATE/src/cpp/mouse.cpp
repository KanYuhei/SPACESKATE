//*****************************************************************************
// マウス処理 (mouse.cpp)
// Author : YUHEI KAN
//*****************************************************************************
#include "mouse.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNT_WAIT_REPEAT   (20)                // リピート待ち時間

//*****************************************************************************
// 静的変数
//*****************************************************************************

//*****************************************************************************
// CInputMouseコンストラスタ
//*****************************************************************************
CInputMouse::CInputMouse()
{
    m_pDIDevice = NULL;

    // 各ワークのクリア
    m_posMouseWorld.x = 0;
    m_posMouseWorld.y = 0;
    ZeroMemory(&m_mouseState, sizeof (m_mouseState) );
    ZeroMemory(&m_mouseStateTrigger, sizeof (m_mouseStateTrigger) );
    ZeroMemory(&m_mouseStateRelease, sizeof (m_mouseStateRelease) );
    ZeroMemory(&m_mouseStateRepeat, sizeof (m_mouseStateRepeat) );
}

//*****************************************************************************
// CInputMouseデストラスタ
//*****************************************************************************
CInputMouse::~CInputMouse()
{
}

//*****************************************************************************
// マウスの初期化処理
//*****************************************************************************
HRESULT CInputMouse::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    //入力処理の初期化
    CInput::Init(hInst, hWnd);

    //デバイスオブジェクトを作成
    hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
    if (FAILED(hr) || m_pDIDevice == NULL)
    {
        MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    //データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    //協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    //デバイスの設定
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(dipdw);
    dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS)

    hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);

    // m_pDIDeviceアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//*****************************************************************************
// マウス入力処理の開放
//*****************************************************************************
void CInputMouse::Uninit(void)
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
// マウスの更新処理
//*****************************************************************************
void CInputMouse::Update(void)
{
    HRESULT hr;
    DIMOUSESTATE2 mouseState;

    if ( !m_pDIDevice )
    {
        return ;
    }

    //デバイスからデータを取得
    hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
    if (SUCCEEDED(hr))
    {
        m_mouseStateTrigger.lX = ( (m_mouseState.lX ^ mouseState.lX) & mouseState.lX );
        m_mouseStateTrigger.lY = ( (m_mouseState.lY ^ mouseState.lY) & mouseState.lY );
        m_mouseStateTrigger.lZ = ( (m_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ );
        m_mouseStateRelease.lX = ( (m_mouseState.lX ^ mouseState.lX) & m_mouseState.lX );
        m_mouseStateRelease.lY = ( (m_mouseState.lY ^ mouseState.lY) & m_mouseState.lY );
        m_mouseStateRelease.lZ = ( (m_mouseState.lZ ^ mouseState.lZ) & m_mouseState.lZ );
        m_mouseStateRepeat.lX = m_mouseStateTrigger.lX;
        m_mouseStateRepeat.lY = m_mouseStateTrigger.lY;
        m_mouseStateRepeat.lZ = m_mouseStateTrigger.lZ;

        for ( int nCntKey = 0; nCntKey < NUM_MB_MAX; nCntKey++ )
        {
            m_mouseStateTrigger.rgbButtons[nCntKey] = ( ( m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey] ) & mouseState.rgbButtons[nCntKey] );
            m_mouseStateRelease.rgbButtons[nCntKey] = ( ( m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey] ) & m_mouseState.rgbButtons[nCntKey] );
            m_mouseStateRepeat.rgbButtons[nCntKey] = m_mouseStateTrigger.rgbButtons[nCntKey];
        }

        m_mouseState = mouseState;

        //スクリーン座標を取得
        GetCursorPos ( &m_posMouseWorld );
    }
    else
    {
        m_pDIDevice->Acquire();
    }

    return ;
}

//*****************************************************************************
// マウスデータ取得(レフトプレス)
//*****************************************************************************
BOOL CInputMouse::GetLeftPress(void)
{
    return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(レフトトリガー)
//*****************************************************************************
BOOL CInputMouse::GetLeftTrigger(void)
{
    return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(レフトリリース)
//*****************************************************************************
BOOL CInputMouse::GetLeftRelease(void)
{
    return (m_mouseStateRelease.rgbButtons[0] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(レフトリピート)
//*****************************************************************************
BOOL CInputMouse::GetLeftRepeat(void)
{
    return (m_mouseStateRepeat.rgbButtons[0] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(ライトプレス)
//*****************************************************************************
BOOL CInputMouse::GetRightPress(void)
{
    return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(ライトトリガー)
//*****************************************************************************
BOOL CInputMouse::GetRightTrigger(void)
{
    return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(ライトリリース)
//*****************************************************************************
BOOL CInputMouse::GetRightRelease(void)
{
    return (m_mouseStateRelease.rgbButtons[1] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスデータ取得(ライトリピート)
//*****************************************************************************
BOOL CInputMouse::GetRightRepeat(void)
{
    return (m_mouseStateRepeat.rgbButtons[1] & 0x80) ? TRUE: FALSE;
}

//*****************************************************************************
// マウスX軸取得
//*****************************************************************************
LONG CInputMouse::GetAxisX(void)
{
    return m_mouseState.lX;
}

//*****************************************************************************
// マウスY軸取得
//*****************************************************************************
LONG CInputMouse::GetAxisY(void)
{
    return m_mouseState.lY;
}

//*****************************************************************************
// マウスZ軸取得
//*****************************************************************************
LONG CInputMouse::GetAxisZ(void)
{
    return m_mouseState.lZ;
}

//*****************************************************************************
// マウス座標取得
//*****************************************************************************
POINT CInputMouse::GetMousePos(void)
{
    return m_posMouseWorld;
}