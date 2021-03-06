﻿//*************************************************************************************************
// アイテム処理 (item.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************
//アイテムクラス
class CItem:public CSceneModel
{
public:
    CItem ( int nPriority = 3 );                    //コンストラクタ
    ~CItem ( );                                     //デコンストラクタ

    HRESULT Init ( void );                          //初期化処理
    void Uninit ( void );                           //終了処理
    void Update ( void );                           //更新処理
    void Draw ( void );                             //描画処理

    static CItem *Create ( D3DXVECTOR3 Pos,
                           D3DXVECTOR3 Scl,
                           int nPriority = 3 );     //自身を生成

    D3DXVECTOR3 GetPos ( void );                    //座標の取得

private:

};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_ITEM_H_