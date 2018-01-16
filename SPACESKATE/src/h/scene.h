//*************************************************************************************************
// シーン処理 (scene.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "main.h"
#include "renderer.h"

//*************************************************************************************************
// マクロ定義
//*************************************************************************************************
#define PRIORITY_NUM ( 8 )                      //優先度数

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//頂点
typedef struct
{
    D3DXVECTOR3 pos;						//  XYZ座標
    D3DXVECTOR3 normal;						//  法線
    float tu, tv;
}VERTEX;

//*************************************************************************************************
// 列挙体
//*************************************************************************************************
//シーンタイプの種類
enum SCENE_TYPE
{
    SCENE_TYPE_2D,                              //2D
    SCENE_TYPE_COUNTDAWN,                       //カウントダウン
    SCENE_TYPE_GEAR,                            //ギア
    SCENE_TYPE_TIME,                            //タイム
    SCENE_TYPE_RANKING,                         //ランキング
    SCENE_TYPE_PRESSENTER,                      //プレスエンター
    SCENE_TYPE_3D,                              //3D
    SCENE_TYPE_MESHBOX,                         //メッシュボックス
    SCENE_TYPE_MESHDOME,                        //メッシュドーム
    SCENE_TYPE_MESHFIELD,                       //メッシュフィールド
    SCENE_TYPE_MESHWALL,                        //メッシュウォール
    SCENE_TYPE_MESHEFFECT,                      //メッシュエフェクト
    SCENE_TYPE_MONITOR,                         //モニター
    SCENE_TYPE_TRACK,                           //トラック
    SCENE_TYPE_PLAYER,                          //プレイヤー
    SCENE_TYPE_PLAYER2,                         //プレイヤー２
    SCENE_TYPE_ITEM,                            //アイテム
    SCENE_TYPE_COLUMN,                          //円柱
    SCENE_TYPE_PARTICLE                         //パーティクル
};

//*************************************************************************************************
// クラス
//*************************************************************************************************
//シーンクラス
class CScene
{
public:
    CScene ( SCENE_TYPE type, 
             int nPriolity = 3 );               //コンストラクタ
    ~CScene ( );                                //デコンストラクタ

    virtual HRESULT Init ( void ) = 0;          //初期化処理
    virtual void Uninit ( void ) = 0;           //終了処理
    virtual void Update ( void ) = 0;           //更新処理
    virtual void Draw ( void ) = 0;             //描画処理

    static void UpdataAll ( void );             //全ての更新処理
    static void DrawAll ( void );               //全ての描画処理
    static void DrawPriolityAll(int nPriolity); //指定した優先度を全て描画
    static void ReleaseAll ( void );            //全ての終了処理
    void Release ( void );                      //自身を終了

    static CScene *GetList ( int nPriolity );   //リストの取得
    CScene *GetNext ( void );                   //シーンクラスの次のポインター
    SCENE_TYPE GetType ( void );                //シーンタイプの取得

    D3DXVECTOR3 GetPos ( void );                //座標の取得
    D3DXVECTOR3 GetSize ( void );               //サイズの取得
    D3DXVECTOR2 GetTex ( void );                //テクスチャ座標の取得
    D3DXVECTOR2 GetTexSize ( void );            //テクスチャサイズの取得
    D3DXCOLOR   GetColor ( void );              //色の取得

    void SetPos ( D3DXVECTOR3 Pos );            //座標の設定
    void SetSize ( D3DXVECTOR3 Size );          //サイズの設定
    void SetTex ( D3DXVECTOR2 Tex );            //テクスチャ座標の設定
    void SetTexSize ( D3DXVECTOR2 TexSize );    //テクスチャサイズの設定
    void SetColor ( D3DXCOLOR Color );          //色の設定
    
protected:
    D3DXVECTOR3 m_Pos;                          //座標
    D3DXVECTOR3 m_Size;                         //サイズ
    D3DXVECTOR2 m_Tex;                          //テクスチャ座標
    D3DXVECTOR2 m_TexSize;                      //テクスチャサイズ
    D3DXCOLOR   m_Color;                        //色
    
private:
    static CScene *m_Top[PRIORITY_NUM];         //シーンクラスのトップ
    CScene *m_Next;                             //シーンクラスの次のポインター
    SCENE_TYPE m_Type;                          //シーンタイプの種類
    bool m_Delete;                              //削除フラグ
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_SCENE_H_