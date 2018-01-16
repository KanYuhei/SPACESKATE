//*************************************************************************************************
// プレイヤー処理 (player.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "manager.h"
#include "keyboard.h"
#include "joystick.h"
#include "sound.h"
#include "renderTarget2D.h"
#include "mode.h"
#include "modeGame.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "countdawn.h"
#include "gear.h"
#include "ranking.h"
#include "time.h"
#include "scene3D.h"
#include "meshBox.h"
#include "meshField.h"
#include "sceneModel.h"
#include "player.h"
#include "player2.h"
#include "item.h"
#include "track.h"
#include "emitter.h"
#include "particle.h"
#include "socket.h"
#include <math.h>

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
int CPlayer::m_ID;

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// プレイヤーのコンストラクタ
//*************************************************************************************************
CPlayer::CPlayer ( int nPriority ):CSceneModel( SCENE_TYPE_PLAYER, nPriority )
{
    //パーツ情報の初期化
    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        m_Part[part].pMesh = NULL;
        m_Part[part].pBuffMat = NULL;
        for (int i = 0; i < MDEL_TEXTURE_NUM; i++)
        {
            m_Part[part].pTexture[i] = NULL;
        }
        m_Part[part].nNumMat = (DWORD)0;
    }

    m_Rot = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
    m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_ItemNum = 0;
    m_Ranking = 0;
    m_HalfGoal = false;
    m_Goal = false;
    m_Cout = 0;
    m_GoalEffect = false;
}

//*************************************************************************************************
// プレイヤーのデストラクタ
//*************************************************************************************************
CPlayer::~CPlayer ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CPlayer::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    char fileName[PART_MAX_NUM][256];
    if (CSocket::GetID() == 1)
    {
        sprintf(fileName[0], "data/MODEL/00_Robo_body.x" );      //（体）
        sprintf(fileName[1], "data/MODEL/01_Robo_head.x");       //（頭）
        sprintf(fileName[2], "data/MODEL/02_Robo_arm_L.x");      //（左腕）
        sprintf(fileName[3], "data/MODEL/03_Robo_hand_L.x");     //（左手）
        sprintf(fileName[4], "data/MODEL/04_Robo_arm_R.x");      //（右腕）
        sprintf(fileName[5], "data/MODEL/05_Robo_hand_R.x");     //（右手）
        sprintf(fileName[6], "data/MODEL/06_Robo_leg_L.x");      //（左腿）
        sprintf(fileName[7], "data/MODEL/07_Robo_foot_L.x");     //（左足）
        sprintf(fileName[8], "data/MODEL/08_Robo_leg_R.x");      //（右腿）
        sprintf(fileName[9], "data/MODEL/09_Robo_foot_R.x");     //（右足）
    }
    else if (CSocket::GetID() == 2)
    {
        sprintf(fileName[0], "data/MODEL/00_RoboBlue_body.x");    //（体）
        sprintf(fileName[1], "data/MODEL/01_RoboBlue_head.x");    //（頭）
        sprintf(fileName[2], "data/MODEL/02_RoboBlue_arm_L.x");   //（左腕）
        sprintf(fileName[3], "data/MODEL/03_RoboBlue_hand_L.x");  //（左手）
        sprintf(fileName[4], "data/MODEL/04_RoboBlue_arm_R.x");   //（右腕）
        sprintf(fileName[5], "data/MODEL/05_RoboBlue_hand_R.x");  //（右手）
        sprintf(fileName[6], "data/MODEL/06_RoboBlue_leg_L.x");   //（左腿）
        sprintf(fileName[7], "data/MODEL/07_RoboBlue_foot_L.x");  //（左足）
        sprintf(fileName[8], "data/MODEL/08_RoboBlue_leg_R.x");   //（右腿）
        sprintf(fileName[9], "data/MODEL/09_RoboBlue_foot_R.x");  //（右足）
    }

    CParts parts;
    LPD3DXMESH mesh;     
    LPD3DXBUFFER buffMat;
    DWORD numMat;
    LPDIRECT3DTEXTURE9 m_Texture[PARTS_TEXTURE_NUM];
    D3DXVECTOR3 position;
    D3DXVECTOR3 rotation;
    CParts *pParts;
    
    //体
    D3DXLoadMeshFromX(fileName[0], D3DXMESH_MANAGED, pDevice, NULL, &buffMat, NULL, &numMat, &mesh);
    position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
    rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pParts = NULL;

    //マテリアル情報バッファのポインタ取得
    D3DXMATERIAL* pMat = (D3DXMATERIAL*)buffMat->GetBufferPointer();

    //各マテリアル情報を保存
    for (int j = 0; j < (int)numMat; j++)
    {
        if (pMat[j].pTextureFilename == NULL) continue;

        //テクスチャの読み込み
        if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[j].pTextureFilename, &m_Texture[j])))
        {
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
            return E_FAIL;
        }
    }

    //頭
    D3DXLoadMeshFromX(fileName[1], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[1].pBuffMat, NULL, &m_Part[1].nNumMat, &m_Part[1].pMesh);
    m_Part[1].Position = D3DXVECTOR3 ( 0.0f, 35.0f, 0.0f );
    m_Part[1].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[1].Parent = &m_Part[0];

    //左腕
    D3DXLoadMeshFromX(fileName[2], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[2].pBuffMat, NULL, &m_Part[2].nNumMat, &m_Part[2].pMesh);
    m_Part[2].Position = D3DXVECTOR3(9.0f, 30.0f, 0.0f);
    m_Part[2].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[2].Parent = &m_Part[0];

    //左手
    D3DXLoadMeshFromX(fileName[3], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[3].pBuffMat, NULL, &m_Part[3].nNumMat, &m_Part[3].pMesh);
    m_Part[3].Position = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
    m_Part[3].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[3].Parent = &m_Part[2];

    //右腕
    D3DXLoadMeshFromX(fileName[4], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[4].pBuffMat, NULL, &m_Part[4].nNumMat, &m_Part[4].pMesh);
    m_Part[4].Position = D3DXVECTOR3(-9.0f, 30.0f, 0.0f);
    m_Part[4].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[4].Parent = &m_Part[0];

    //右手
    D3DXLoadMeshFromX(fileName[5], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[5].pBuffMat, NULL, &m_Part[5].nNumMat, &m_Part[5].pMesh);
    m_Part[5].Position = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
    m_Part[5].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[5].Parent = &m_Part[4];

    //左足
    D3DXLoadMeshFromX(fileName[6], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[6].pBuffMat, NULL, &m_Part[6].nNumMat, &m_Part[6].pMesh);
    m_Part[6].Position = D3DXVECTOR3(6.0f, 6.0f, 0.0f);
    m_Part[6].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[6].Parent = &m_Part[0];

    //左足先
    D3DXLoadMeshFromX(fileName[7], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[7].pBuffMat, NULL, &m_Part[7].nNumMat, &m_Part[7].pMesh);
    m_Part[7].Position = D3DXVECTOR3(1.5f, -20.0f, -1.5f);
    m_Part[7].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[7].Parent = &m_Part[6];

    //右足
    D3DXLoadMeshFromX(fileName[8], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[8].pBuffMat, NULL, &m_Part[8].nNumMat, &m_Part[8].pMesh);
    m_Part[8].Position = D3DXVECTOR3(-6.0f, 6.0f, 0.0f);
    m_Part[8].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[8].Parent = &m_Part[0];

    //右足先
    D3DXLoadMeshFromX(fileName[9], D3DXMESH_MANAGED, pDevice, NULL, &m_Part[9].pBuffMat, NULL, &m_Part[9].nNumMat, &m_Part[9].pMesh);
    m_Part[9].Position = D3DXVECTOR3(-1.5f, -20.0f, -1.5f);
    m_Part[9].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Part[9].Parent = &m_Part[8];

    KEY_FRAME keyFrame;

    //待機モーション
    m_Motion[WAIT].Key_Max = 2;
    m_Motion[WAIT].Loop = true;
    keyFrame =
    {
        30,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, -1.3f) },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.1f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 1.3f) },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(6.0f, 6.0f, 0.0f),     D3DXVECTOR3(0.0f, 0.0f, 0.1f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-6.0f, 6.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, -0.1f) },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) }
        }
    };
    m_Motion[WAIT].KeyFrame.push_back(keyFrame);
    keyFrame =
    {
        30,
        {
            { D3DXVECTOR3(0.0f, 9.0f, 0.8f),     D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, -1.3f) },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 1.3f) },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(6.0f, 6.0f, 0.0f),     D3DXVECTOR3(0.1f, 0.0f, 0.1f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(-0.1f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-6.0f, 6.0f, 0.0f),    D3DXVECTOR3(0.1f, 0.0f, -0.1f) },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(-0.1f, 0.0f, 0.0f) }
        }
    };
    m_Motion[WAIT].KeyFrame.push_back(keyFrame);


    //歩くモーション
    m_Motion[WALK].Key_Max = 2;
    m_Motion[WALK].Loop = true;
    keyFrame =
    {
        20,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.1f, 0.0f)   },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(0.0f, -0.1f, 0.0f)  },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(0.6f, 0.0f, -1.3f)  },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.2f, 0.0f)   },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(-0.6f, 0.0f, 1.3f)  },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 0.0f)   },
            { D3DXVECTOR3(6.0f, 6.0f, 0.0f),     D3DXVECTOR3(-1.0f, -0.1f, 0.0f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f)   },
            { D3DXVECTOR3(-6.0f, 6.0f, 0.0f),    D3DXVECTOR3(1.0f, -0.1f, 0.0f)  },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(-0.8f, 0.0f, 0.0f)  }
        }
    };
    m_Motion[WALK].KeyFrame.push_back(keyFrame);
    keyFrame =
    {
        20,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f),    D3DXVECTOR3(0.0f, -0.1f, 0.0f)  },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.1f, 0.0f)   },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(-0.6f, 0.0f, -1.3f) },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f)   },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(0.6f, 0.0f, 1.3f)   },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.2f, 0.0f)   },
            { D3DXVECTOR3(6.0f, 6.0f, 0.0f),     D3DXVECTOR3(1.0f, 0.1f, 0.0f)   },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(-0.8f, 0.0f, 0.0f)  },
            { D3DXVECTOR3(-6.0f, 6.0f, 0.0f),    D3DXVECTOR3(-1.0f, 0.1f, 0.0f)  },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)   }
        }
    };
    m_Motion[WALK].KeyFrame.push_back(keyFrame);

    //走るモーション
    m_Motion[RUN].Key_Max = 2;
    m_Motion[RUN].Loop = true;
    keyFrame =
    {
        30,
        {
            { D3DXVECTOR3(0.0f, 7.0f, 0.0f),     D3DXVECTOR3(-1.0f, 0.1f, 0.0f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(1.0f, -0.1f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(1.5f, 1.0f, -1.3f) },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.5f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(-0.5f, 0.0f, 1.3f) },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(4.0f, 0.0f, 0.0f),     D3DXVECTOR3(-0.7f, -0.1f, 0.0f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-4.0f, 0.0f, 0.0f),    D3DXVECTOR3(2.8f, -0.1f, 0.0f) },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(-1.8f, 0.0f, 0.0f) }
        }
    };
    m_Motion[RUN].KeyFrame.push_back(keyFrame);
    keyFrame =
    {
        30,
        {
            { D3DXVECTOR3(0.0f, 7.0f, 0.0f),     D3DXVECTOR3(-1.0f, -0.1f, 0.0f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f),    D3DXVECTOR3(1.0f, 0.1f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f),    D3DXVECTOR3(-0.5f, 0.0f, -1.3f) },
            { D3DXVECTOR3(15.0f, 0.0f, 0.0f),    D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f),   D3DXVECTOR3(1.5f, -1.0f, 1.3f) },
            { D3DXVECTOR3(-15.0f, 0.0f, 0.0f),   D3DXVECTOR3(0.0f, -0.5f, 0.0f) },
            { D3DXVECTOR3(4.0f, 0.0f, 0.0f),     D3DXVECTOR3(2.8f, 0.1f, 0.0f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f),  D3DXVECTOR3(-1.8f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-4.0f, 0.0f, 0.0f),    D3DXVECTOR3(-0.7f, 0.1f, 0.0f) },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) }
        }
    };
    m_Motion[RUN].KeyFrame.push_back(keyFrame);

    //パンチモーション
    m_Motion[PUNTCH].Key_Max = 3;
    m_Motion[PUNTCH].Loop = false;
    keyFrame =
    {
        30,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f)    ,D3DXVECTOR3(0.0f, 0.7f, 0.0f)   },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f)    ,D3DXVECTOR3(-0.2f, -0.8f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f)    ,D3DXVECTOR3(0.3f, 0.0f, -0.9f)  },
            { D3DXVECTOR3(13.5f, 0.0f, 7.0f)    ,D3DXVECTOR3(0.0f, 2.0f, 0.0f)   },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f)   ,D3DXVECTOR3(0.0f, 0.0f, 1.0f)   },
            { D3DXVECTOR3(-13.5f, 0.0f, 7.0f)   ,D3DXVECTOR3(0.0f, -2.0f, 0.0f)  },
            { D3DXVECTOR3(4.0f, 2.0f, 3.0f)     ,D3DXVECTOR3(0.8f, -0.7f, 0.0f)  },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f)  ,D3DXVECTOR3(-0.8f, 0.0f, 0.0f)  },
            { D3DXVECTOR3(-9.0f, 6.0f, 0.0f)    ,D3DXVECTOR3(0.1f, -0.7f, 0.0f)  },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f) ,D3DXVECTOR3(-0.5f, 0.0f, 0.0f)  }
        }
    };
    m_Motion[PUNTCH].KeyFrame.push_back(keyFrame);
    keyFrame =
    {
        20,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f)    ,D3DXVECTOR3(-0.5f, 0.7f, -0.5f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f)    ,D3DXVECTOR3(0.3f, -0.8f, 0.0f)  },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f)    ,D3DXVECTOR3(0.3f, 0.0f, -0.9f)  },
            { D3DXVECTOR3(13.5f, 0.0f, 7.0f)    ,D3DXVECTOR3(0.0f, 2.0f, 0.0f)   },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f)   ,D3DXVECTOR3(0.0f, 0.0f, 1.0f)   },
            { D3DXVECTOR3(-13.5f, 0.0f, 7.0f)   ,D3DXVECTOR3(0.0f, -2.0f, 0.0f)  },
            { D3DXVECTOR3(4.0f, 2.0f, 3.0f)     ,D3DXVECTOR3(1.3f, -0.7f, 0.0f)  },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f)  ,D3DXVECTOR3(-0.8f, 0.0f, 0.0f)  },
            { D3DXVECTOR3(-9.0f, 6.0f, 0.0f)    ,D3DXVECTOR3(0.6f, -0.7f, 0.0f)  },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f) ,D3DXVECTOR3(-0.5f, 0.0f, 0.0f)  }
        }
    };
    m_Motion[PUNTCH].KeyFrame.push_back(keyFrame);
    keyFrame =
    {
        10,
        {
            { D3DXVECTOR3(0.0f, 10.0f, 0.0f)    ,D3DXVECTOR3(0.0f, -0.3f, 0.0f) },
            { D3DXVECTOR3(0.0f, 35.0f, 0.0f)    ,D3DXVECTOR3(0.0f, -0.1f, 0.0f) },
            { D3DXVECTOR3(9.0f, 30.0f, 0.0f)    ,D3DXVECTOR3(0.3f, 0.0f, -0.9f) },
            { D3DXVECTOR3(13.5f, 0.0f, 7.0f)    ,D3DXVECTOR3(0.0f, 2.0f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 30.0f, 0.0f)   ,D3DXVECTOR3(3.0f, 0.0f, 1.0f) },
            { D3DXVECTOR3(-13.5f, 0.0f, 7.0f)   ,D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
            { D3DXVECTOR3(4.0f, 2.0f, 3.0f)     ,D3DXVECTOR3(0.8f, 0.3f, 0.0f) },
            { D3DXVECTOR3(1.5f, -20.0f, -1.5f)  ,D3DXVECTOR3(-0.8f, 0.0f, 0.0f) },
            { D3DXVECTOR3(-9.0f, 6.0f, 0.0f)    ,D3DXVECTOR3(0.1f, 0.3f, 0.0f) },
            { D3DXVECTOR3(-1.5f, -20.0f, -1.5f) ,D3DXVECTOR3(-0.5f, 0.0f, 0.0f) }
        }
    };
    m_Motion[PUNTCH].KeyFrame.push_back(keyFrame);

    //待機ポーズ
    //m_Part[0].Position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);           //体
    //m_Part[1].Position = D3DXVECTOR3(0.0f, 35.0f, 0.0f);           //頭
    //m_Part[2].Position = D3DXVECTOR3(9.0f, 30.0f, 0.0f);           //左腕
    //m_Part[3].Position = D3DXVECTOR3(13.5f, 0.0f, 7.0f);           //左手
    //m_Part[4].Position = D3DXVECTOR3(-9.0f, 30.0f, 0.0f);          //右腕
    //m_Part[5].Position = D3DXVECTOR3(-13.5f, 0.0f, 7.0f);          //右手
    //m_Part[6].Position = D3DXVECTOR3(4.0f, 2.0f, 3.0f);            //左足
    //m_Part[7].Position = D3DXVECTOR3(1.5f, -20.0f, -1.5f);         //左足先
    //m_Part[8].Position = D3DXVECTOR3(-9.0f, 6.0f, 0.0f);           //右足
    //m_Part[9].Position = D3DXVECTOR3(-1.5f, -20.0f, -1.5f);        //右足先

    //m_Part[0].Rotation = D3DXVECTOR3(0.0f, -0.3f, 0.0f);         //体
    //m_Part[1].Rotation = D3DXVECTOR3(0.3f, -0.8f, 0.0f);         //頭
    //m_Part[2].Rotation = D3DXVECTOR3(0.3f, 0.0f, -0.9f);         //左腕
    //m_Part[3].Rotation = D3DXVECTOR3(0.0f, 2.0f, 0.0f);          //左手
    //m_Part[4].Rotation = D3DXVECTOR3(3.0f, 0.0f, 1.0f);          //右腕
    //m_Part[5].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //右手
    //m_Part[6].Rotation = D3DXVECTOR3(0.8f, 0.3f, 0.0f);         //左足
    //m_Part[7].Rotation = D3DXVECTOR3(-0.8f, 0.0f, 0.0f);         //左足先
    //m_Part[8].Rotation = D3DXVECTOR3(0.1f, 0.3f, 0.0f);        //右足
    //m_Part[9].Rotation = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);          //右足先

    //パンチポーズ
    //m_Part[0].Rotation = D3DXVECTOR3(-0.8f, 0.0f, -0.5f);      //体
    //m_Part[1].Rotation = D3DXVECTOR3(0.7f, 0.0f, 0.0f);        //頭
    //m_Part[2].Rotation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);        //左腕
    //m_Part[3].Rotation = D3DXVECTOR3(0.0f, 0.5f, 0.0f);        //左手
    //m_Part[4].Rotation = D3DXVECTOR3(0.0f, 0.0f, -1.0f);      //右腕
    //m_Part[5].Rotation = D3DXVECTOR3(0.0f, 0.0f, -0.2f);       //右手
    //m_Part[6].Rotation = D3DXVECTOR3(1.3f, 0.0f, 0.4f);        //左足
    //m_Part[7].Rotation = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);       //左足先
    //m_Part[8].Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //右足
    //m_Part[9].Rotation = D3DXVECTOR3(-0.1f, 0.0f, 0.0f);       //右足先

    //モーション情報の初期化
    for (int i = 0; i < STATE_MAX; i++)
    {
        m_Motion[i].Frame = 0.0f;
        m_Motion[i].Key = 0;
        m_Motion[i].Play = false;
    }
    m_Play = false;
    m_ItemNum = 0;
    m_MotionBlendFrame = 0;
    m_State = WAIT;
    m_BlendMotion = m_Motion[m_State];
    m_PassingCheckPoint = 0;

    //シーンモデルクラスの初期化処理
    CSceneModel::Init ( );

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CPlayer::Uninit(void)
{
    //シーンモデルクラスの終了処理
    CSceneModel::Uninit ( );
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CPlayer::Update(void)
{
    //シーンモデルクラスの更新処理
    CSceneModel::Update ( );

    //前回の座標と状態を更新
    m_OldPos = m_Pos;
    m_OldState = m_State;

    //各種初期化
    m_Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //カメラ演出（回転処理）
    if (m_Cout <= 200)
        m_Cout++;
    if (m_Cout > 50 && m_Cout <= 150)
    {
        CModeGame::GetCamera() ->AddLength(1.5f);
        CModeGame::GetCamera()->AddAngleY(D3DX_PI * 0.01f);
        CModeGame::GetCamera()->AddAngleX(D3DX_PI * 0.0015f);
    }

    //カウントダウン中フリーズ
    if (CModeGame::GetCountdawn()->GetCountDawn() == 0) m_Play = true;

    //入力/////////////////////////////////////////////////////////////
    m_Motion[WAIT].Play = true;
    m_Motion[WALK].Play = false;
    m_Motion[RUN].Play = false;
    if (m_Play == true)
    {
        //カメラ操作(JoyStick用)
        /*D3DXVECTOR3 work;
        work.x = (float)CManager::GetInputJoystick()->GetRightStickDisposition().y;
        work.y = (float)CManager::GetInputJoystick()->GetRightStickDisposition().x;
        work.z = 0.0f;
        D3DXVec3Normalize(&work, &work);
        if (work.x != 0 || work.y != 0)
        {
            CModeGame::GetCamera()->AddAngleY(D3DX_PI * work.y * 0.005f);
            CModeGame::GetCamera()->AddAngleX(D3DX_PI * work.x * 0.005f);
        }*/

        //カメラベクトル取得
        D3DXVECTOR3 cameraVecDirect = CModeGame::GetCamera()->GetVecDirect();
        cameraVecDirect.y = 0.0f;                   //カメラY軸成分なし
        D3DXVec3Normalize(&cameraVecDirect, &cameraVecDirect);
        D3DXVECTOR3 cameraVecRightDirect;           //カメラ右方向ベクトル
        D3DXVec3Cross(&cameraVecRightDirect, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &cameraVecDirect);
        D3DXVec3Normalize(&cameraVecRightDirect, &cameraVecRightDirect);

        //移動
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))  //前
        {
            m_Acceleration += cameraVecDirect * 0.8f;
            m_Motion[WAIT].Play = false;
        }
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))  //後
        {
            m_Acceleration -= cameraVecDirect * 0.8f;
            m_Motion[WAIT].Play = false;
        }
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))  //右
        {
            m_Acceleration += cameraVecRightDirect * 0.8f;
            m_Motion[WAIT].Play = false;
        }
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))  //左
        {
            m_Acceleration -= cameraVecRightDirect * 0.8f;
            m_Motion[WAIT].Play = false;
        }

        //移動(JoyStick用)
        /*D3DXVECTOR3 work2;
        work2.x = (float)CManager::GetInputJoystick()->GetLeftStickDisposition().x;
        work2.y = 0.0f;
        work2.z = (float)CManager::GetInputJoystick()->GetLeftStickDisposition().y;
        D3DXVec3Normalize(&work2, &work2);

        if (work2.z != 0)
        {
            m_Acceleration += cameraVecDirect  * -(work2.z * 0.5f * (((m_ItemNum+1)/10)+1));
        }
        if (work2.x != 0)
        {
            m_Acceleration += cameraVecRightDirect  * (work2.x * 0.5f * (((m_ItemNum + 1) / 10) + 1));
        }

        if (work2.x != 0 || work2.z != 0) m_Motion[WAIT].Play = false;*/

        //ジャンプ
        if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE) ||
             CManager::GetInputJoystick()->GetJoyTrigger( CInputJoystick::DIJ_BUTTON, DIJ_CROSS ))
        {
            //m_Acceleration.y += 200.0f;
        }

        //パンチ
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_P))  //P
        {
            m_Motion[PUNTCH].Play = true;
            m_Motion[PUNTCH].Frame = 0;
            m_Motion[PUNTCH].Key = 0;
        }

        //加速
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT) /*||
            CManager::GetInputJoystick()->GetJoyPress(CInputJoystick::DIJ_BUTTON, DIJ_CROSS)*/)
        {
            CManager::GetRenderTarget2D()->IsFeedbackBlur(true);
        }
        else
        {
            CManager::GetRenderTarget2D()->IsFeedbackBlur(false);
        }
    }

    //座標更新///////////////////////////////////////////////////////
    m_Acceleration.y = -0.98f;                  //重力
    m_Move += m_Acceleration;                   //移動量(速度)に加速度を足す

    if (m_Move.x >= 0.3f || m_Move.x <= -0.3f || m_Move.z >= 0.3f || m_Move.z <= -0.3f)
    {
        //方向
        D3DXMATRIX mtxRot;
        D3DXMatrixIdentity(&mtxRot);
        D3DXVec3Normalize(&m_Rot, &m_Move);
        m_Motion[WALK].Play = true;
        if (m_Move.x >= 20.0f || m_Move.x <= -20.0f || m_Move.z >= 20.0f || m_Move.z <= -20.0f)
        {
            m_Motion[RUN].KeyFrame[0].Frame = 30;
            m_Motion[RUN].KeyFrame[1].Frame = 30;
            m_Motion[RUN].Play = true;
        }
    }

    m_Pos += m_Move;                            //座標に移動量(速度)を足す
    m_Move *= 0.98f;                            //抵抗


    //プレイヤー状態の更新///////////////////////////////////////////
    if (m_Motion[WAIT].Play == true)        m_State = WAIT;   //待機
    else if (m_Motion[PUNTCH].Play == true) m_State = PUNTCH; //パンチ
    else if (m_Motion[RUN].Play == true)    m_State = RUN;    //走る
    else if (m_Motion[WALK].Play == true)   m_State = WALK;   //歩く
    else m_State = WAIT;                                      //待機

    //m_State = RUN;
    //プレイヤー状態毎のモーション再生///////////////////////////////

    //前回のモーションを保存
    if (m_State != m_OldState)
    {
        m_BlendMotion = m_Motion[m_OldState];
        m_MotionBlendFrame = 0;
    }

    //フレーム計算
    float rate = (float)m_BlendMotion.Frame / m_BlendMotion.KeyFrame[m_BlendMotion.Key].Frame;
    float blendRate = (float)m_Motion[m_State].Frame / m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Frame;
    float motionBlendRate = m_MotionBlendFrame / 30.0f;

    //モーション再生
    for (int part = 0; part < PART_MAX_NUM; part++)
    {
        D3DXVECTOR3 Pos;    //前回のモーション座標
        D3DXVECTOR3 Rot;    //前回のモーション回転

        //線形補間関数
        D3DXVec3Lerp(&Pos,                                                  //座標
                     &m_BlendMotion.KeyFrame[m_BlendMotion.Key].Key[part].Position,
                     &m_BlendMotion.KeyFrame[(m_BlendMotion.Key + 1) % m_BlendMotion.Key_Max].Key[part].Position,
                    rate);
        D3DXVec3Lerp(&Rot,                                                  //回転
                     &m_BlendMotion.KeyFrame[m_BlendMotion.Key].Key[part].Rotation,
                     &m_BlendMotion.KeyFrame[(m_BlendMotion.Key + 1) % m_BlendMotion.Key_Max].Key[part].Rotation,
                    rate);

        D3DXVECTOR3 BlendPos;
        D3DXVECTOR3 BlendRot;

        //線形補間関数
        D3DXVec3Lerp(&BlendPos,                                             //座標
                     &m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Key[part].Position,
                     &m_Motion[m_State].KeyFrame[(m_Motion[m_State].Key + 1) % m_Motion[m_State].Key_Max].Key[part].Position,
                     blendRate);
        D3DXVec3Lerp(&BlendRot,                                             //回転
                     &m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Key[part].Rotation,
                     &m_Motion[m_State].KeyFrame[(m_Motion[m_State].Key + 1) % m_Motion[m_State].Key_Max].Key[part].Rotation,
                     blendRate);

        //線形補間関数
        D3DXVec3Lerp(&m_Part[part].Position, &Pos, &BlendPos, motionBlendRate);
        D3DXVec3Lerp(&m_Part[part].Rotation, &Rot, &BlendRot, motionBlendRate);
    }

    //フレーム加算
    m_MotionBlendFrame++;   //モーションブレンド
    if (m_MotionBlendFrame >= MOTION_BLEND_FRAME) m_MotionBlendFrame = MOTION_BLEND_FRAME;
    m_Motion[m_State].Frame++;  //キーフレーム
    if (m_Motion[m_State].Frame >= m_Motion[m_State].KeyFrame[m_Motion[m_State].Key].Frame)
    {
        m_Motion[m_State].Frame = 0.0f;
        m_Motion[m_State].Key++;
        //キー最大になった時
        if (m_Motion[m_State].Key == m_Motion[m_State].Key_Max)
        { 
            //ループする場合
            if (m_Motion[m_State].Loop == true) m_Motion[m_State].Key = 0;

            //ループしない場合
            if (m_Motion[m_State].Loop == false)
            {
                //パンチ
                if (m_State == PUNTCH)
                {
                    m_Motion[PUNTCH].Play = false;
                    m_Motion[PUNTCH].Frame = 0;
                    m_Motion[PUNTCH].Key = 0;
                }
            }
        }
    }


    //トラック///////////////////////////////////////////////////////
    //地面の高さを取得
    float Pos_y = CModeGame::GetTrack()->GetHeight ( m_Pos );

    //地面に衝突した場合
    if ( m_Pos.y-37.0f <= Pos_y )
    {
        if (FLT_MAX == Pos_y)
        {
            //前回の座標を代入
            m_Pos = m_OldPos - (m_Rot * 20.0f);
            m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            m_ItemNum -= 5;
            if (m_ItemNum <= 0) m_ItemNum = 0;
        }
        else
        {
            m_Pos.y = CModeGame::GetTrack()->GetHeight(m_Pos) + 37.0f;
            m_Move.y = 0.0f;
        }

        m_Rot.x *= -1;
        m_Rot.z *= -1;

        //面の法線を取得
        D3DXVECTOR3 VecN = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXVECTOR3 Vx,Vz;
        D3DXVec3Cross ( &Vx, &VecN, &m_Rot );
        D3DXVec3Cross ( &Vz, &Vx, &VecN );
        D3DXMATRIX mtxRot;                      //回転行列

        mtxRot._11 = Vx.x;
        mtxRot._12 = Vx.y;
        mtxRot._13 = Vx.z;
        mtxRot._14 = 0;
        mtxRot._21 = VecN.x;
        mtxRot._22 = VecN.y;
        mtxRot._23 = VecN.z;
        mtxRot._24 = 0;
        mtxRot._31 = Vz.x;
        mtxRot._32 = Vz.y;
        mtxRot._33 = Vz.z;
        mtxRot._34 = 0;
        mtxRot._41 = 0;
        mtxRot._42 = 0;
        mtxRot._43 = 0;
        mtxRot._44 = 1;

        D3DXQUATERNION quaternion;                  //クォータニオン

        //回転行列からクォータニオンを作成する関数
        D3DXQuaternionRotationMatrix ( &quaternion, &mtxRot );

        //2つのクォータニオンの補間関数
        D3DXQuaternionSlerp ( &m_Quaternion, &m_Quaternion, &quaternion, 0.5f );

        m_Rot.x *= -1;
        m_Rot.z *= -1;
    }
    
    //メッシュボックスクラス
    CModeGame::GetMeshBox()->SetPos(m_Pos);


    //順位設定処理///////////////////////////////////////////////////

    //プレイヤーのチェックポイント設定
    for (int i = 0; i < CHECKPOINT_NUM; i++)
    {
        //チェックポイント（線分）
        D3DXVECTOR3 CheckPointStart = CModeGame::GetTrack()->GetCheckPointStart(i); //始点
        D3DXVECTOR3 CheckPointEnd = CModeGame::GetTrack()->GetCheckPointEnd(i);     //終点

        //チェックポイントからの距離を計算
        float Player1length = D3DXVec3Length(&(NearPosOnLine(m_Pos, CheckPointStart, CheckPointEnd) - m_Pos));
        float Player2length = D3DXVec3Length(&(NearPosOnLine(CModeGame::GetPlayer2()->GetPos(), CheckPointStart, CheckPointEnd) - CModeGame::GetPlayer2()->GetPos()));

        //チェックポイントを通過した時
        if (Player1length <= 50.0f )
        {
             m_PassingCheckPoint = i;
            if (m_PassingCheckPoint == CHECKPOINT_NUM - 1 && m_HalfGoal == true) m_Goal = true;  //ゴール
            if (m_PassingCheckPoint == CHECKPOINT_NUM / 2) m_HalfGoal = true;//半分ゴール
            else if (m_PassingCheckPoint == CHECKPOINT_NUM - 1 && m_HalfGoal == false) m_PassingCheckPoint = 0;
        }
        if (Player2length <= 50.0f )
        {
            CModeGame::GetPlayer2()->SetPassingCheckPoint(i);
            if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM - 1 && CModeGame::GetPlayer2()->GetHalfGoal() == true ) CModeGame::GetPlayer2()->SetGoal(true);      //ゴール
            if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM / 2) CModeGame::GetPlayer2()->SetHalfGoal(true);  //半分ゴール
            else if (CModeGame::GetPlayer2()->GetPassingCheckPoint() == CHECKPOINT_NUM - 1 && CModeGame::GetPlayer2()->GetHalfGoal() == false) CModeGame::GetPlayer2()->SetPassingCheckPoint(0);
        }
    }

    //チェックポイント番号が大きい方が上位
    if (m_PassingCheckPoint >= CModeGame::GetPlayer2()->GetPassingCheckPoint())
    { 
        m_Ranking = 0; 
        //チェックポイントが同じ場合
        if (m_PassingCheckPoint == CModeGame::GetPlayer2()->GetPassingCheckPoint())
        {
            //チェックポイント（線分）
            D3DXVECTOR3 CheckPointStart = CModeGame::GetTrack()->GetCheckPointStart(m_PassingCheckPoint+1); //始点
            D3DXVECTOR3 CheckPointEnd = CModeGame::GetTrack()->GetCheckPointEnd(m_PassingCheckPoint+1);     //終点

            //チェックポイントからの距離を計算
            float Player1length = D3DXVec3Length(&(NearPosOnLine(m_Pos, CheckPointStart, CheckPointEnd) - m_Pos));
            float Player2length = D3DXVec3Length(&(NearPosOnLine(CModeGame::GetPlayer2()->GetPos(), CheckPointStart, CheckPointEnd) - CModeGame::GetPlayer2()->GetPos()));

            //チェックポイントからの距離が近い方が上位
            if (Player1length <= Player2length) m_Ranking = 0;
            else m_Ranking = 1;
        }
    }
    else 
    { 
        m_Ranking = 1; 
    }

    //順位変動処理
    if (m_Ranking != CModeGame::GetRanking()->GetRanking()) CModeGame::GetRanking()->SetRanking(m_Ranking);


    //アイテム///////////////////////////////////////////////////////

    //アイテムとの衝突判定
    CScene *scene = CScene::GetList ( 3 );
    while ( scene != NULL )
    {
        if ( scene->GetType() == SCENE_TYPE_ITEM )
        {
            CItem *item = (CItem*)scene;
            D3DXVECTOR3 itemPos = item->GetPos();

            //衝突判定
            D3DXVECTOR3 direction = itemPos - m_Pos;
            float length = D3DXVec3Length( &direction );
            if ( length < 50.0f )
            {
                item->Release();
                m_ItemNum++;
                CManager::GetSound()->Play(CSound::SOUND_LAVEL_SE_ITEM);
            }
        }
        scene = scene->GetNext();
    }

    //アイテム取得個数カウントアップ
    CModeGame::GetGear()->SetItemNum(m_ItemNum);

    ////エフェクト処理
    //EMITTER Emitter;
    //Emitter.Pos = D3DXVECTOR3 ( 0.0f, 5.0f, 2300.0f + (float)(rand() % 1000));
    //Emitter.Speed = D3DXVECTOR3(0.0f, 0.7f, 0.0f);
    //Emitter.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    //Emitter.Size = D3DXVECTOR2(10.0f, 10.0f);
    //Emitter.Color = D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.7f);
    //Emitter.nLife = 60;
    //Emitter.nNum = 5;

    ////前回の座標と違う場合
    ///*if (m_OldPos != m_Pos)
    //{
    //    CEmitter *emitter = new CEmitter;
    //    emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());
    //}*/
    //CEmitter *emitter = new CEmitter;
    //emitter->EmitParticle(&Emitter, CModeGame::GetParticle()->Get());

    //パーティクルクラスの更新処理
    CModeGame::GetParticle()->Update();

    //カメラ注視点の設定
    CModeGame::GetCamera()->SetPosAt(m_Pos);

    // データ送信////////////////////////////////////////////////////
    DATA data;

    data.Type = DATA_TYPE_POSITION;         //座標
    data.Position.x = m_Pos.x;
    data.Position.y = m_Pos.y;
    data.Position.z = m_Pos.z;
    CSocket::SendData(data);

    data.Type = DATA_TYPE_ROTATION;         //回転
    data.Rotation.x = m_Rot.x;
    data.Rotation.y = m_Rot.y;
    data.Rotation.z = m_Rot.z;
    CSocket::SendData(data);

    data.Type = DATA_TYPE_MOTION;           //モーション
    data.Motion.WaitPlay = m_Motion[WAIT].Play;
    data.Motion.WalkPlay = m_Motion[WALK].Play;
    data.Motion.RunPlay = m_Motion[RUN].Play;
    data.Motion.PuntchPlay = m_Motion[PUNTCH].Play;
    CSocket::SendData(data);

    int GameEnd = CModeGame::GetGameEnd();

    //ゲーム勝利
    if (GameEnd == GAME_PLAY)
    {
        //ゴールした時
        if (m_Goal== true && m_HalfGoal == true && m_GoalEffect == false)
        {
            data.Type = DATA_TYPE_EVENT;
            data.Event.Type = DATA_EVENT_TYPE_GOAL;

            CSocket::SendData(data);

            CModeGame::SetGameEnd(GAME_GOAL);

            //ゴールしたタイム記録を残す
            CManager::SetPlayerTime(1, CModeGame::GetTime()->GetTime());

            data.Type = DATA_TYPE_TIME;

            //ゴールした時のタイムを相手に送信
            data.Time.Time = (int)CModeGame::GetTime()->GetTime();

            CSocket::SendData(data);

            CManager::GetSound()->Play(CSound::SOUND_LAVEL_SE_GOAL);
            m_GoalEffect = true;
        }
    }
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CPlayer::Draw(void)
{
    //パーティクルクラスの描画処理
    CModeGame::GetParticle()->Draw();

    //シーンモデルクラスの描画処理
    //CSceneModel::Draw ( );

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        D3DMATERIAL9 matDef;

        //現在デバイスに設定されているマテリアル情報を取得
        pDevice->GetMaterial(&matDef);

        D3DXMATRIX mtxWorld;                    //ワールド行列
        D3DXMATRIX mtxSclModel;                 //拡大行列
        D3DXMATRIX mtxRotModel;                 //回転行列
        D3DXMATRIX mtxPosModel;                 //平行移動行列

        //単位行列を作る関数
        D3DXMatrixIdentity(&mtxWorld);

        //拡大行列を作る関数
        D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);

        //D3DXVECTOR3 Axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        //D3DXQuaternionRotationAxis(&m_Quaternion, &Axis, D3DXToRadian(20));

        //回転行列を作る関数
        D3DXMatrixRotationQuaternion(&mtxRotModel, &m_Quaternion);

        //平行移動行列を作る関数
        D3DXMatrixTranslation(&mtxPosModel, m_Pos.x, m_Pos.y, m_Pos.z);

        //ワールドマトリクスに掛け算(拡大→回転→平行移動)
        mtxWorld *= mtxSclModel;
        mtxWorld *= mtxRotModel;
        mtxWorld *= mtxPosModel;

        for (int part = 0; part < PART_MAX_NUM; part++)
        {
            //単位行列を作る関数
            D3DXMatrixIdentity(&m_Part[part].Matrix);

            //拡大行列を作る関数
            D3DXMatrixScaling(&mtxSclModel, m_Scl.x, m_Scl.y, m_Scl.z);

            //回転行列を作る関数
            D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_Part[part].Rotation.y, m_Part[part].Rotation.x, m_Part[part].Rotation.z );

            //平行移動行列を作る関数
            D3DXMatrixTranslation(&mtxPosModel, m_Part[part].Position.x, m_Part[part].Position.y, m_Part[part].Position.z);

            //ワールドマトリクスに掛け算(拡大→回転→平行移動)
            m_Part[part].Matrix *= mtxSclModel;
            m_Part[part].Matrix *= mtxRotModel;
            m_Part[part].Matrix *= mtxPosModel;

            //親子構造構築
            if (m_Part[part].Parent == NULL)
            {
                D3DXMatrixMultiply(&m_Part[part].Matrix, &m_Part[part].Matrix, &mtxWorld);
            }
            else
            {
                D3DXMatrixMultiply(&m_Part[part].Matrix, &m_Part[part].Matrix, &m_Part[part].Parent->Matrix );
            }

            //デバイスにワールドマトリクスを設定
            pDevice->SetTransform(D3DTS_WORLD, &m_Part[part].Matrix);

            D3DXMATERIAL* pMat;
            pMat = (D3DXMATERIAL*)m_Part[part].pBuffMat->GetBufferPointer();

            //マテリアル情報の数分繰り返す
            for (int i = 0; i < (int)m_Part[part].nNumMat; i++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[i].MatD3D);

                //テクスチャの設定
                pDevice->SetTexture(0, m_Part[part].pTexture[i]);

                //メッシュの描画
                m_Part[part].pMesh->DrawSubset(i);
            }

            //マテリアルの設定
            pDevice->SetMaterial(&matDef);

            //単位行列を作る関数
            D3DXMatrixIdentity(&mtxWorld);

            //デバイスにワールドマトリクスを設定
            pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
        }
    }

#ifdef _DEBUG
    //変数宣言
    LPD3DXFONT Font = CRenderer::GetFont();
    char aStr[256];
    sprintf(&aStr[0], "[プレイヤー]\n<座標>   ( X:%f Y:%f Z:%f )\n", m_Pos.x, m_Pos.y, m_Pos.z);
    RECT rc3 = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
    Font->DrawText(NULL, &aStr[0], -1, &rc3, DT_LEFT, D3DCOLOR_RGBA(0, 255, 0, 255));
#endif //_DEBUG
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CPlayer *CPlayer::Create ( D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority )
{
    CPlayer *pPlayer;                           //自身クラスのポインタ

    //動的確保
    pPlayer = new CPlayer( nPriority );

    //2Pの場合
    if (CSocket::GetID() == 2) Pos = D3DXVECTOR3(0.0f, 0.0f, 3025.0f);

    //各種初期化
    pPlayer->m_Pos = Pos;
    pPlayer->m_Scl = Scl;

    //初期化処理
    pPlayer->Init ( );

    return pPlayer;
}

//*************************************************************************************************
// モーションの取得
//*************************************************************************************************
MOTION CPlayer::GetMotion(int i)
{
    return m_Motion[i];
}

//*************************************************************************************************
// 半分までゴールフラグ取得
//*************************************************************************************************
bool CPlayer::GetHalfGoal(void)
{
    return m_HalfGoal;
}

//*************************************************************************************************
// 通過したチェックポイントの取得
//*************************************************************************************************
int CPlayer::GetPassingCheckPoint(void)
{
    return m_PassingCheckPoint;
}

//*************************************************************************************************
// 最上座標の取得
//*************************************************************************************************
D3DXVECTOR3 CPlayer::GetPosUp(void)
{
    D3DXVECTOR3 PosUp = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );       //プレイヤー最上座標

    //  メッシュの頂点データ取得
    VERTEX *pVtx = NULL;

    //  パーツの最大数分繰り返す
    for (int i = 0; i < PART_MAX_NUM; i++)
    {
        //  各パーツの頂点数を取得
        int nNumVertex = m_Part[i].pMesh->GetNumVertices();

        //  メッシュから頂点情報を取得
        m_Part[i].pMesh->LockVertexBuffer(0, (void**)&pVtx);

        //  頂点数分のループ
        for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
        {
            if (PosUp.y <= pVtx->pos.y) PosUp = pVtx->pos;
            pVtx++;
        }

        //  頂点バッファのアンロック
        m_Part[i].pMesh->UnlockVertexBuffer();
    }

    return PosUp;
}

//*************************************************************************************************
// 最下座標の取得
//*************************************************************************************************
D3DXVECTOR3 CPlayer::GetPosDown(void)
{
    D3DXVECTOR3 PosDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     //プレイヤー最下座標

    //  メッシュの頂点データ取得
    VERTEX *pVtx = NULL;

    //  パーツの最大数分繰り返す
    for (int i = 0; i < PART_MAX_NUM; i++)
    {
        //  各パーツの頂点数を取得
        int nNumVertex = m_Part[i].pMesh->GetNumVertices();

        //  メッシュから頂点情報を取得
        m_Part[i].pMesh->LockVertexBuffer(0, (void**)&pVtx);

        //  頂点数分のループ
        for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
        {
            if (PosDown.y >= pVtx->pos.y) PosDown = pVtx->pos;
            pVtx++;
        }

        //  頂点バッファのアンロック
        m_Part[i].pMesh->UnlockVertexBuffer();
    }

    return PosDown;
}

//*************************************************************************************************
// 点Pと直線ABから線上最近点を求める
//*************************************************************************************************
D3DXVECTOR3 CPlayer::NearPosOnLine(D3DXVECTOR3 P, D3DXVECTOR3 A, D3DXVECTOR3 B)
{
    D3DXVECTOR3 AB, AP;//ベクトルAB AP

    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;
    AP.x = P.x - A.x;
    AP.y = P.y - A.y;
    AP.z = P.z - A.z;

    //ABの単位ベクトルを計算
    D3DXVECTOR3 nAB;
    D3DXVec3Normalize(&nAB, &AB);

    //Aから線上最近点までの距離（ABベクトルの後ろにあるときはマイナス値）
    float dist_AX = D3DXVec3Dot(&nAB, &AP);

    if (D3DXVec3Length(&AB) <= dist_AX)
    {
        return B;
    }
    else if (-D3DXVec3Length(&AB) >= dist_AX)
    {
        return A;
    }

    //線上最近点
    D3DXVECTOR3 ret;
    ret.x = A.x + (nAB.x * dist_AX);
    ret.y = A.y + (nAB.y * dist_AX);
    ret.z = A.z + (nAB.z * dist_AX);

    return ret;
}