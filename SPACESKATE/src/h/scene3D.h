﻿//*************************************************************************************************
// シーン3D処理 (scene3D.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
// 頂点フォーマット
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//頂点情報構造体(3D)
typedef struct
{
    D3DXVECTOR3 pos;    //頂点座標
    D3DXVECTOR3 normal; //法線
    D3DCOLOR color;     //頂点カラー
    D3DXVECTOR2 tex;    //テクスチャ座標
}VERTEX_3D;

//*************************************************************************************************
// クラス
//*************************************************************************************************
//シーン3Dクラス
class CScene3D:public CScene
{
public:
    CScene3D ( SCENE_TYPE type, int nPriolity = 3 );    //コンストラクタ
    ~CScene3D ( );                                      //デコンストラクタ

    HRESULT Init ( void );                              //初期化処理
    void Uninit ( void );                               //終了処理
    void Update ( void );                               //更新処理
    void Draw ( void );                                 //描画処理

    static CScene3D *Create ( D3DXVECTOR3 Pos,
                              D3DXVECTOR3 Size,
                              D3DXCOLOR Color,
                              D3DXVECTOR2 Tex,
                              D3DXVECTOR2 TexSize,
                              int nPriority = 3 );      //自身を生成

protected:
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;               //頂点バッファポインタ
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;                //インデックスバッファポインタ
    LPDIRECT3DTEXTURE9 m_pTexture;                      //テクスチャへポインタ
    D3DXVECTOR3 m_Normal;                               //法線

private:
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_SCENE3D_H_