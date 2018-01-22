//*************************************************************************************************
// メッシュボックス処理 (meshBox.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene3D.h"
#include "meshBox.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00 "data/TEXTURE/GAME/skybox003.jpg"                                  //読み込むテクスチャの名前
#define TEXTURENAME01 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/frontImage.png"    //読み込むテクスチャの名前
#define TEXTURENAME02 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/leftImage.png"     //読み込むテクスチャの名前
#define TEXTURENAME03 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/backImage.png"     //読み込むテクスチャの名前
#define TEXTURENAME04 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/rightImage.png"    //読み込むテクスチャの名前
#define TEXTURENAME05 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/upImage.png"       //読み込むテクスチャの名前
#define TEXTURENAME06 "data/TEXTURE/GAME/skyBox/skybox_BlueWithPlanet/downImage.png"     //読み込むテクスチャの名前

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
// メッシュボックスのコンストラクタ
//*************************************************************************************************
CMeshBox::CMeshBox(int nPriority) :CScene3D(SCENE_TYPE_MESHBOX, nPriority)
{
}

//*************************************************************************************************
// メッシュボックスのデストラクタ
//*************************************************************************************************
CMeshBox::~CMeshBox()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshBox::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //頂点バッファの作成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_3D) * 24,                  //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                     //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                          //頂点フォーマット
            D3DPOOL_MANAGED,                        //メモリ管理方法
            &m_pVtxBuffer,                          //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }

        // テクスチャの生成
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME00, &m_pTexture[0] )
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME01, &m_pTexture[1] )
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME02, &m_pTexture[2])
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME03, &m_pTexture[3])
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME04, &m_pTexture[4])
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME05, &m_pTexture[5])
                || D3DXCreateTextureFromFile(pDevice, TEXTURENAME06, &m_pTexture[6])
           ))
        {
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
        }
    }

    //変数宣言
    D3DXVECTOR3 Pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, m_Pos.z);
    D3DXVECTOR2 Tex = D3DXVECTOR2(0.0f, 0.0f);

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshBox::Uninit(void)
{
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMeshBox::Update(void)
{
    //頂点バッファの設定開始/////////////////////////////////////////////////////////////
    VERTEX_3D* pVtx;                            //仮想アドレス頂点用ポインタ

                                                //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

    //一枚の時
    /*pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[0].color = m_Color;
    pVtx[0].tex = D3DXVECTOR2(0.0f, (1.0f / 3) * 2);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[1].color = m_Color;
    pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f / 3);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[2].color = m_Color;
    pVtx[2].tex = D3DXVECTOR2(0.25f, (1.0f / 3) * 2);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[3].color = m_Color;
    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f / 3);

    pVtx[4].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[4].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[4].color = m_Color;
    pVtx[4].tex = D3DXVECTOR2(0.25f, (1.0f / 3) * 2);
    pVtx[5].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[5].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[5].color = m_Color;
    pVtx[5].tex = D3DXVECTOR2(0.25f, 1.0f / 3);
    pVtx[6].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[6].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[6].color = m_Color;
    pVtx[6].tex = D3DXVECTOR2(0.5f, (1.0f / 3) * 2);
    pVtx[7].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[7].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[7].color = m_Color;
    pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f / 3 );

    pVtx[8].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[8].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[8].color = m_Color;
    pVtx[8].tex = D3DXVECTOR2(0.5f, (1.0f / 3) * 2);
    pVtx[9].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[9].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[9].color = m_Color;
    pVtx[9].tex = D3DXVECTOR2(0.5f, 1.0f / 3);
    pVtx[10].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[10].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[10].color = m_Color;
    pVtx[10].tex = D3DXVECTOR2(0.75f, (1.0f / 3) * 2);
    pVtx[11].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[11].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[11].color = m_Color;
    pVtx[11].tex = D3DXVECTOR2(0.75f, 1.0f / 3);

    pVtx[12].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[12].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[12].color = m_Color;
    pVtx[12].tex = D3DXVECTOR2(0.75f, (1.0f / 3) * 2);
    pVtx[13].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[13].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[13].color = m_Color;
    pVtx[13].tex = D3DXVECTOR2(0.75f, 1.0f / 3);
    pVtx[14].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[14].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[14].color = m_Color;
    pVtx[14].tex = D3DXVECTOR2(1.0f, (1.0f / 3) * 2);
    pVtx[15].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[15].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[15].color = m_Color;
    pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f / 3);

    pVtx[16].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[16].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[16].color = m_Color;
    pVtx[16].tex = D3DXVECTOR2(0.25f, 1.0f / 3);
    pVtx[17].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[17].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[17].color = m_Color;
    pVtx[17].tex = D3DXVECTOR2(0.25f, 0.0f);
    pVtx[18].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[18].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[18].color = m_Color;
    pVtx[18].tex = D3DXVECTOR2(0.5f, 1.0f / 3);
    pVtx[19].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[19].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[19].color = m_Color;
    pVtx[19].tex = D3DXVECTOR2(0.5f, 0.0f);

    pVtx[20].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[20].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[20].color = m_Color;
    pVtx[20].tex = D3DXVECTOR2(0.25f, 1.0f);
    pVtx[21].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[21].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[21].color = m_Color;
    pVtx[21].tex = D3DXVECTOR2(0.25f, (1.0f / 3) * 2);
    pVtx[22].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[22].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[22].color = m_Color;
    pVtx[22].tex = D3DXVECTOR2(0.5f, 1.0f);
    pVtx[23].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[23].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[23].color = m_Color;
    pVtx[23].tex = D3DXVECTOR2(0.5f, (1.0f / 3) * 2);*/


    //一枚ずつの時
    pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[0].color = m_Color;
    pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[1].color = m_Color;
    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[2].color = m_Color;
    pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[3].color = m_Color;
    pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[4].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[4].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[4].color = m_Color;
    pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[5].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[5].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[5].color = m_Color;
    pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[6].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[6].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[6].color = m_Color;
    pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[7].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[7].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[7].color = m_Color;
    pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[8].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[8].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[8].color = m_Color;
    pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[9].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[9].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[9].color = m_Color;
    pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[10].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[10].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[10].color = m_Color;
    pVtx[10].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[11].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[11].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[11].color = m_Color;
    pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[12].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[12].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[12].color = m_Color;
    pVtx[12].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[13].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[13].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[13].color = m_Color;
    pVtx[13].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[14].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[14].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[14].color = m_Color;
    pVtx[14].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[15].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[15].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[15].color = m_Color;
    pVtx[15].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[16].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[16].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[16].color = m_Color;
    pVtx[16].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[17].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[17].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[17].color = m_Color;
    pVtx[17].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[18].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[18].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[18].color = m_Color;
    pVtx[18].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[19].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y + m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[19].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[19].color = m_Color;
    pVtx[19].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[20].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[20].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[20].color = m_Color;
    pVtx[20].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[21].pos = D3DXVECTOR3(m_Pos.x - m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[21].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[21].color = m_Color;
    pVtx[21].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[22].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z - m_Size.y / 2);
    pVtx[22].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[22].color = m_Color;
    pVtx[22].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[23].pos = D3DXVECTOR3(m_Pos.x + m_Size.x / 2, m_Pos.y - m_Size.y / 2, m_Pos.z + m_Size.y / 2);
    pVtx[23].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[23].color = m_Color;
    pVtx[23].tex = D3DXVECTOR2(1.0f, 0.0f);

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshBox::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //頂点バッファを設定(ストリーム)
        pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_3D);

        for (int i = 0; i < 6; i++)
        {
            //テクスチャの設定
            pDevice->SetTexture(0, m_pTexture[i+1]);

            // ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //プリッミティブの種類
                i * 4,                                    //オフセット(頂点数)
                NUM_POLYGON);                             //プリミティブ数
        }
    }
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CMeshBox *CMeshBox::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, int nPriority)
{
    CMeshBox *pMeshBox;                     //メッシュボックスクラスのポインタ

    //動的確保
    pMeshBox = new CMeshBox(nPriority);

    //各種初期化
    pMeshBox->m_Pos = Pos;
    pMeshBox->m_Size = Size;
    pMeshBox->m_Color = Color;

    //初期化処理
    pMeshBox->Init();

    return pMeshBox;
}