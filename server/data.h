﻿//*************************************************************************************************
// データ (data.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _DATA_H_
#define _DATA_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
//データタイプ
#define DATA_TYPE_ID                1           //ID
#define DATA_TYPE_EVENT		        2           //イベント
#define DATA_TYPE_POSITION	        3           //座標
#define DATA_TYPE_ROTATION	        4           //回転
#define DATA_TYPE_MOTION            5           //モーション
#define DATA_TYPE_TIME              6           //タイム

//データイベントタイプ
#define DATA_EVENT_TYPE_START		1           //スタート
#define DATA_EVENT_TYPE_GOAL        2           //ゴール

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//データイベント情報
struct DATA_EVENT
{
    short Type;
};

//データ座標情報
struct DATA_POSITION
{
    float x, y, z;
};

//データ回転情報
struct DATA_ROTATION
{
    float x, y, z;
};

//データモーション情報
struct DATA_MOTION
{
    bool WaitPlay;                              //待機
    bool WalkPlay;                              //歩く
    bool RunPlay;                               //走る
    bool PuntchPlay;                            //パンチ
};

//データタイム情報
struct DATA_TIME
{
    int Time;
};

//データ情報
struct DATA
{
    int		ID;
    short	Type;

    union
    {
        DATA_EVENT		Event;
        DATA_POSITION	Position;
        DATA_ROTATION	Rotation;
        DATA_MOTION     Motion;
        DATA_TIME       Time;
    };
};

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

#endif //_DATA_H_
