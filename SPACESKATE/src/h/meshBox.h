﻿//*************************************************************************************************
// メッシュボックス処理 (meshBox.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _MESHBOX_H_
#define _MESHBOX_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define MAX_TEXTURE_NUM (7)

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************
//メッシュボックスクラス
class CMeshBox :public CScene3D
{
public:
    CMeshBox(int nPriority = 3);                //コンストラクタ
    ~CMeshBox();                                //デコンストラクタ

    HRESULT Init(void);                         //初期化処理
    void Uninit(void);                          //終了処理
    void Update(void);                          //更新処理
    void Draw(void);                            //描画処理

    static CMeshBox *Create(D3DXVECTOR3 Pos,
                            D3DXVECTOR3 Size,
                            D3DXCOLOR Color,
                            int nPriority = 3); //自身を生成

protected:

private:
    LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_NUM];//テクスチャへポインタ
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_MESHBOX_H_