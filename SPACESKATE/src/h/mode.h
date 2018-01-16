﻿//*************************************************************************************************
// モード処理 (mode.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _MODE_H_
#define _MODE_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************
//モードクラス
class CMode
{
public:
    CMode ( );                                  //コンストラクタ
    ~CMode ( );                                 //デコンストラクタ

    virtual HRESULT Init ( void ) = 0;          //初期化処理
    virtual void Uninit ( void ) = 0;           //終了処理
    virtual void Update ( void ) = 0;           //更新処理
    virtual void Draw ( void ) = 0;             //描画処理
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_MODE_H_