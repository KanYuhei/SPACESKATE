﻿//*************************************************************************************************
// シーン2D処理 (scene2D.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義
//*************************************************************************************************
// 頂点フォーマット
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//頂点情報構造体(2D)
typedef struct
{
    D3DXVECTOR3 pos;    //頂点座標
    float rhw;          //座標変換用係数(1.0fで固定)
    D3DCOLOR col;       //頂点カラー
    D3DXVECTOR2 tex;    //テクスチャ座標
} VERTEX_2D;

//*************************************************************************************************
// クラス
//*************************************************************************************************
//シーン2Dクラス
class CScene2D:public CScene
{
public:
    CScene2D (SCENE_TYPE type, int nPriolity = 3 );     //コンストラクタ
    ~CScene2D ( );                                      //デコンストラクタ

    HRESULT Init ( void );                              //初期化処理
    void Uninit ( void );                               //終了処理
    void Update ( void );                               //更新処理
    void Draw ( void );                                 //描画処理

    static CScene2D *Create ( char TexName[256],
                              D3DXVECTOR3 Pos,
                              D3DXVECTOR3 Size,
                              D3DXCOLOR Color,
                              D3DXVECTOR2 Tex,
                              D3DXVECTOR2 TexSize,
                              int nPriolity = 3 );      //自身を生成

private:
    LPDIRECT3DTEXTURE9      m_pTexture;                 //テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                 //頂点バッファへのポインタ
    char m_TextureName[256];                            //テクスチャの名前
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_SCENE2D_H_