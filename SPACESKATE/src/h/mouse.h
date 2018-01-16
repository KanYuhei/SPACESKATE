//*****************************************************************************
// マウス処理 (mouse.h)
// Author : YUHEI KAN
//*****************************************************************************
#ifndef _MOUSE_H_
#define _MOUSE_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_MB_MAX (8)                          // マウスボタン最大数

//*****************************************************************************
// クラス
//*****************************************************************************
//マウス入力クラス
class CInputMouse : public CInput
{
public:
    CInputMouse();                              //コンストラクタ
    ~CInputMouse();                             //デコンストラクタ

    HRESULT Init(HINSTANCE hInst, HWND hWnd);   //初期化処理
    void Uninit(void);                          //終了処理
    void Update(void);                          //更新処理

    BOOL GetLeftPress(void);                    //マウスデータ取得(レフトプレス)
    BOOL GetLeftTrigger(void);                  //マウスデータ取得(レフトトリガー)
    BOOL GetLeftRelease(void);                  //マウスデータ取得(レフトリリース)
    BOOL GetLeftRepeat(void);                   //マウスデータ取得(レフトリピート)
    BOOL GetRightPress(void);                   //マウスデータ取得(ライトプレス)
    BOOL GetRightTrigger(void);                 //マウスデータ取得(ライトトリガー)
    BOOL GetRightRelease(void);                 //マウスデータ取得(ライトリリース)
    BOOL GetRightRepeat(void);                  //マウスデータ取得(ライトリピート)
    LONG GetAxisX(void);                        //マウスX軸取得
    LONG GetAxisY(void);                        //マウスY軸取得
    LONG GetAxisZ(void);                        //マウスZ軸取得
    POINT GetMousePos(void);                    //マウス座標取得

private:
    LPDIRECTINPUTDEVICE8 m_pDIDevice;           //Deviceオブジェクト(入力に必要)
    DIMOUSESTATE2 m_mouseState;                 //プレスワーク
    DIMOUSESTATE2 m_mouseStateTrigger;          //トリガーワーク
    DIMOUSESTATE2 m_mouseStateRelease;          //リリースーワーク
    DIMOUSESTATE2 m_mouseStateRepeat;           //リピートワーク
    POINT m_posMouseWorld;                      //マウス座標
};

#endif //_MOUSE_H_