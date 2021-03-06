﻿//*************************************************************************************************
// プレスエンター処理 (pressenter.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

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
//プレスエンタークラス
class CPressenter :public CScene2D
{
public:
    CPressenter(int nPriolity = 3);              //コンストラクタ
    ~CPressenter();                              //デコンストラクタ

    HRESULT Init(void);                         //初期化処理
    void Uninit(void);                          //終了処理
    void Update(void);                          //更新処理
    void Draw(void);                            //描画処理

    static CPressenter *Create(
        D3DXVECTOR3 Pos,
        int nPriolity = 3);                     //自身を生成

private:
    LPDIRECT3DTEXTURE9      m_pTexture;         //テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;         //頂点バッファへのポインタ
    float m_ColorA;                             //アルファ値
    float m_ColorA_fade;                        //カラーアルファ点滅間隔
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_PRESSENTER_H_