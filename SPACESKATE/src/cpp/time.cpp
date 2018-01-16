﻿//*************************************************************************************************
// タイム処理 (time.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene2D.h"
#include "time.h"
#include "number.h"
#include "countdawn.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define NUM_SIZE_X ( SCREEN_WIDTH * 0.005f * 9 )                   //数値横サイズ
#define NUM_SIZE_Y ( SCREEN_HEIGHT * 0.005f * 16 )                 //数値縦サイズ

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// タイムのコンストラクタ
//*************************************************************************************************
CTime::CTime ( int nPriority ):CScene2D( SCENE_TYPE_TIME, nPriority )
{
    //タイム最高桁数分繰り返し
    for (int nCntNumber = 0; nCntNumber < TIME_DISIT_MAX; nCntNumber++)
    {
        //数値クラスのNULL代入
        m_apNumber[nCntNumber] = NULL;
    }
}

//*************************************************************************************************
// タイムのデストラクタ
//*************************************************************************************************
CTime::~CTime()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CTime::Init(void)
{
    float AddPos_X = 0;

    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < TIME_DISIT_MAX; nCntDisit++)
    {
        if (nCntDisit == 3|| nCntDisit == 5)
            AddPos_X += NUM_SIZE_X/8;

        //数値の生成
        m_apNumber[nCntDisit] = CNumber::Create(D3DXVECTOR3(m_Pos.x + SCREEN_WIDTH - NUM_SIZE_X * (nCntDisit+1)- AddPos_X, m_Pos.y, 0.0f),
                                                D3DXVECTOR3(NUM_SIZE_X, NUM_SIZE_Y, 0.0f));
    }

    m_Start = false;
    m_Time = 0;
    m_Minute = 0;

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CTime::Uninit(void)
{
    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < TIME_DISIT_MAX; nCntDisit++)
    {
        //数値の描画処理
        m_apNumber[nCntDisit]->Uninit();
        delete m_apNumber[nCntDisit];
        m_apNumber[nCntDisit] = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CTime::Update(void)
{
    int CountDawn;

    CScene *scene = CScene::GetList(4);
     while (scene != NULL)
     {
         if (scene->GetType() == SCENE_TYPE_COUNTDAWN)
         {
             CCountdawn *pCountDawn = (CCountdawn*)scene;
             CountDawn = pCountDawn->GetCountDawn();
             if (CountDawn == 0)
                m_Start = true;
         }
         scene = scene->GetNext();
     }

    //タイム増算
     if (m_Start == true)
    m_Time += 1000.0f/60.0f;

    if ((int)m_Time%100000 >= 60000)
    {
        m_Time = 0.0f;
        m_Minute += 100000;
        m_Time += (float)m_Minute;
    }

    int nTime = (int)m_Time;

    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < TIME_DISIT_MAX; nCntDisit++)
    {
        //数値の設定
        m_apNumber[nCntDisit]->SetNumber(nTime % 10);

        //数値の描画処理
        m_apNumber[nCntDisit]->Update();

        nTime /= 10;
    }
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CTime::Draw(void)
{
    //最高桁数分繰り返す
    for (int nCntDisit = 0; nCntDisit < TIME_DISIT_MAX; nCntDisit++)
    {
        //数値の描画処理
        m_apNumber[nCntDisit]->Draw();
    }
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CTime *CTime::Create ( D3DXVECTOR3 Pos, int nPriolity )
{
    CTime *pTime;                               //タイムクラスのポインタ

    //動的確保
    pTime = new CTime ( nPriolity );

    //座標の設定
    pTime->m_Pos = Pos;

    //初期化処理
    pTime->Init();

    return pTime;
}

//*************************************************************************************************
// タイムの設定
//*************************************************************************************************
void CTime::SetTime ( float Time )
{
    m_Time = Time;
}

//*************************************************************************************************
// タイムの取得
//*************************************************************************************************
float CTime::GetTime ( void )
{
    m_Time = 0.0f;
    m_Time += (float)m_Minute;
    return m_Time;
}