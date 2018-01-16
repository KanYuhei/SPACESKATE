//*************************************************************************************************
// プレイヤー処理 (player.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include <vector>
#include "parts.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define PART_MAX_NUM        (10)                //パーツの最大数
#define MOTION_BLEND_FRAME  (30.0f)             //モーションブレンドフレーム数

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//キー情報（１パーツ）
struct KEY
{
    D3DXVECTOR3 Position;                           //座標
    D3DXVECTOR3 Rotation;                           //回転
};

//キーフレーム情報
struct KEY_FRAME
{
    int Frame;                                      //フレーム数
    KEY Key[PART_MAX_NUM];                          //キー情報
};

//モーション情報
struct MOTION
{
    std::vector<KEY_FRAME> KeyFrame;                //キーフレーム情報
    bool Play;                                      //再生(ON:true,OFF:false)
    float Frame;                                    //現在のフレーム数
    int Key;                                        //現在のキー
    int Key_Max;                                    //キーの数
    bool Loop;                                      //ループするかどうか
};

//*************************************************************************************************
// 前方宣言
//*************************************************************************************************
class CParts;

//*************************************************************************************************
// クラス
//*************************************************************************************************
//プレイヤークラス
class CPlayer:public CSceneModel
{
public:
    //状態の種類
    typedef enum
    {
        WAIT = 0,                                   //待機
        WALK,                                       //歩く
        RUN,                                        //走る
        PUNTCH,                                     //パンチ
        STATE_MAX
    }STATE;

    CPlayer ( int nPriority = 3 );                  //コンストラクタ
    ~CPlayer ( );                                   //デコンストラクタ

    HRESULT Init ( void );                          //初期化処理
    void Uninit ( void );                           //終了処理
    void Update ( void );                           //更新処理
    void Draw ( void );                             //描画処理

    static CPlayer *Create ( D3DXVECTOR3 Pos,
                             D3DXVECTOR3 Scl,
                             int nPriority = 3 );   //自身を生成

    MOTION GetMotion(int i);                        //モーションの取得
    bool GetHalfGoal(void);                         //半分までゴールフラグ取得
    int GetPassingCheckPoint(void);                 //通過したチェックポイントの取得
    D3DXVECTOR3 GetPosUp(void);                     //最上座標の取得
    D3DXVECTOR3 GetPosDown(void);                   //最下座標の取得

private:
    D3DXVECTOR3 NearPosOnLine(D3DXVECTOR3 P, 
                              D3DXVECTOR3 A, 
                              D3DXVECTOR3 B);       //点Pと直線ABから線上最近点を求める

    CParts m_Part[PART_MAX_NUM];                    //パーツ情報
    MOTION m_Motion[STATE_MAX];                     //モーション情報
    MOTION m_BlendMotion;                           //モーション情報（ブレンド用）
    float m_MotionBlendFrame;                       //モーションブレンドフレーム
    static int m_ID;                                //プレイヤーID
    D3DXVECTOR3 m_Move;                             //移動量(速度)
    D3DXVECTOR3 m_Acceleration;                     //加速度
    D3DXVECTOR3 m_OldPos;                           //前回の座標
    STATE m_State;                                  //状態
    STATE m_OldState;                               //前回の状態
    bool m_Play;                                    //動ける状態かどうか
    int m_ItemNum;                                  //アイテム取得個数
    int m_Ranking;                                  //順位
    bool m_HalfGoal;                                //半分までゴールフラグ
    bool m_Goal;                                    //ゴールフラグ
    int m_PassingCheckPoint;                        //通過したチェックポイント
    int m_Cout;                                     //カウント
    bool m_GoalEffect;                              //ゴールエフェクト
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_PLAYER_H_