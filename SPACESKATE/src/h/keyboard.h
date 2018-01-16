//*****************************************************************************
// キーボード処理 (keyboard.h)
// Author : YUHEI KAN
//*****************************************************************************
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)                       // キー最大数

//*****************************************************************************
// クラス
//*****************************************************************************
//キーボード入力クラス
class CInputKeyboard : public CInput
{
public:
    CInputKeyboard();                           //コンストラクタ
    ~CInputKeyboard();                          //デコンストラクタ

    HRESULT Init(HINSTANCE hInst, HWND hWnd);   //初期化処理
    void Uninit(void);                          //終了処理
    void Update(void);                          //更新処理

    BOOL GetKeyPress(int nKey);                 //キーボードデータ取得(プレス)
    BOOL GetKeyTrigger(int nKey);               //キーボードデータ取得(トリガー)
    BOOL GetKeyRelease(int nKey);               //キーボードデータ取得(リリース)
    BOOL GetKeyRepeat(int nKey);                //キーボードデータ取得(リピート)
    void FlushKeyTrigger(int nKey);             //キーボードデータ削除(トリガー)

private:
    LPDIRECTINPUTDEVICE8 m_pDIDevice;           //Deviceオブジェクト(入力に必要)
    BYTE m_aKeyState[NUM_KEY_MAX];              //キーボードの状態を受け取るワーク
    BYTE m_aKeyStateTrigger[NUM_KEY_MAX];       //トリガーワーク
    BYTE m_aKeyStateRelease[NUM_KEY_MAX];       //リリースワーク
    BYTE m_aKeyStateRepeat[NUM_KEY_MAX];        //リピートワーク
    int  m_aKeyStateRepeatCnt[NUM_KEY_MAX];     //リピートカウンタ
};

#endif //_KEYBOARD_H_