//*************************************************************************************************
// モードゲーム処理 (modegame.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _MODEGAME_H_
#define _MODEGAME_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define GAME_PLAY 0
#define GAME_GOAL 1
#define GO_RESULT_TIME   ( 300 )                //リザルトに行くまでの待機時間

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// 前方宣言
//*************************************************************************************************
class CLight;
class CCamera;
class CMeshField;
class CMeshWall;
class CMeshDome;
class CMeshBox;
class CMeshEffect;
class CTrack;
class CPlayer;
class CPlayer2;
class CItem;
class CCountdawn;
class CTime;
class CGear;
class CRanking;
class CParticle;

//*************************************************************************************************
// クラス
//*************************************************************************************************
//モードゲームクラス
class CModeGame:public CMode
{
public:
    CModeGame ( );                              //コンストラクタ
    ~CModeGame ( );                             //デコンストラクタ

    HRESULT Init ( void );                      //初期化処理
    void Uninit ( void );                       //終了処理
    void Update ( void );                       //更新処理
    void Draw ( void );                         //描画処理

    static CCamera *GetCamera ( void );         //カメラクラスの取得
    static CMeshField *GetMeshField(void);      //メッシュフィールドクラスの取得
    static CMeshBox *GetMeshBox(void);          //メッシュボックスクラスの取得
    static CMeshEffect *GetMeshEffect(void);    //メッシュエフェクトクラスの取得
    static CTrack *GetTrack(void);              //トラッククラスの取得
    static CPlayer *GetPlayer ( void );         //プレイヤークラスの取得
    static CPlayer2 *GetPlayer2(void);          //プレイヤー２クラスの取得
    static CCountdawn *GetCountdawn(void);      //カウトダウンクラスの取得
    static CTime *GetTime(void);                //タイムクラスの取得
    static CGear *GetGear(void);                //ギアクラスの取得
    static CRanking *GetRanking(void);          //ランキングクラスの取得
    static CParticle *GetParticle ( void );     //パーティクルの取得
    static void SetGameEnd(int GameEnd);        //ゲームエンド設定
    static int GetGameEnd(void);                //ゲームエンド取得
    static void SetPlayer2GameEnd(int GameEnd); //プレイヤー２ゲームエンド設定

private:
    static CCamera *m_Camera;                   //カメラクラスのポインタ
    static CLight *m_CLight;                    //ライトクラスのポインタ
    static CMeshField *m_MeshField;             //メッシュフィールドクラスのポインタ
    CMeshWall *m_MeshWall;                      //メッシュウォールクラスのポインタ
    CMeshDome *m_MeshDome;                      //メッシュドームクラスのポインタ
    static CMeshBox *m_MeshBox;                 //メッシュボックスクラスのポインタ
    static CMeshEffect *m_MeshEffect;           //メッシュエフェクトクラスのポインタ
    static CTrack *m_Track;                     //トラッククラスのポインタ
    static CPlayer *m_Player;                   //プレイヤークラスのポインタ
    static CPlayer2 *m_Player2;                 //プレイヤー2クラスのポインタ
    CItem *m_Item;                              //アイテムクラスのポインタ
    static CCountdawn *m_CountDawn;             //カウントダウンクラスのポインタ
    static CTime *m_Time;                       //タイムクラスのポインタ
    static CGear *m_Gear;                       //ギアクラスのポインタ
    static CRanking *m_Ranking;                 //ランキングクラスのポインタ
    static CParticle *m_Particle;               //パーティクルクラスのポインタ
    static int m_GameEnd;                       //ゲームエンド判定用
    static int m_Player2GameEnd;                //プレイヤー２のゲームエンド判定
    bool m_CreateEndGoalResult;                 //ゴールリザルト生成終了フラグ
    int m_toResultTime;                         //リザルトまで行く時間
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_MODEGAME_H_