﻿//*************************************************************************************************
// エミッター処理 (emitter.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "emitter.h"

//*************************************************************************************************
// 定数定義
//*************************************************************************************************

//*************************************************************************************************
// 列挙体
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************
int Comp(const void * a, const void * b);

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************


//*************************************************************************************************
// エミッターのコンストラクタ
//*************************************************************************************************
CEmitter::CEmitter()
{
}

//*************************************************************************************************
// エミッターのデストラクタ
//*************************************************************************************************
CEmitter::~CEmitter()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CEmitter::Init(void)
{
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CEmitter::Uninit(void)
{
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CEmitter::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CEmitter::Draw(void)
{
}

////*************************************************************************************************
//// 自身を生成
////*************************************************************************************************
//CEmitter *CEmitter::Create( D3DXVECTOR3 Pos, D3DXVECTOR3 Speed, D3DXVECTOR3 Acceleration, D3DXCOLOR Color, int nLife, int nNum )
//{
//    m_Emitter
//}

//*************************************************************************************************
// エミッターからパーティクルに設定
//*************************************************************************************************
void CEmitter::EmitParticle(EMITTER *pEmitter, PARTICLE *pParticle)
{
    int nSetCnt;                                //1フレームの個数カウント

    //1フレームの個数カウント設定
    nSetCnt = pEmitter->nNum;

    //エミッターの設定
    for (int nCntEmitter = 0; nCntEmitter < PARTICLE_MAX; nCntEmitter++)
    {
        if (pParticle[nCntEmitter].bUse == false)
        {
            //パラメータ設定
            pParticle[nCntEmitter].Pos = pEmitter->Pos;
            pParticle[nCntEmitter].Speed.x = pEmitter->Speed.x * ((rand() % 201 - 100) * 0.01f);
            pParticle[nCntEmitter].Speed.y = pEmitter->Speed.y * ((rand() % 201 - 100) * 0.01f);
            pParticle[nCntEmitter].Speed.z = pEmitter->Speed.z * ((rand() % 201 - 100) * 0.01f);
            pParticle[nCntEmitter].Acceleration = pEmitter->Acceleration;
            pParticle[nCntEmitter].Color = pEmitter->Color;
            pParticle[nCntEmitter].Size = pEmitter->Size;
            pParticle[nCntEmitter].nLife = pEmitter->nLife;
            pParticle[nCntEmitter].bUse = true;

            //1フレームの個数カウントマイナス
            nSetCnt--;
            if (nSetCnt == 0)
            {
                break;
            }
        }
    }
}