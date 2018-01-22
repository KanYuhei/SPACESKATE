//*************************************************************************************************
// メッシュウォール処理 (meshWall.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene3D.h"
#include "meshWall.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00 "data/TEXTURE/GAME/wall.jpg"   //読み込むテクスチャの名前
#define WALL_NUM            ( 4 )               //ウォールの枚数
//#define TB_WIDTH_BLOAK_NUM  ( 10 )              //ブロックの横の分割数(奥と手前)
//#define TB_HEIGHT_BLOAK_NUM ( 3 )               //ブロックの縦の分割数(奥と手前)
//#define LR_WIDTH_BLOAK_NUM  ( 10 )              //ブロックの横の分割数(左と右)
//#define LR_HEIGHT_BLOAK_NUM ( 3 )               //ブロックの縦の分割数(左と右)
#define DEGENERATE_NUM      ( 2 )               //縮退ポリゴンによる頂点数増分(縦の分割数によって比例)

//*************************************************************************************************
// 構造体
//*************************************************************************************************
//ウォール情報
typedef struct
{
    D3DXVECTOR3 pos;                            //座標
    D3DXVECTOR3 rot;                            //回転
    D3DXVECTOR3 normal;                         //法線
    int nNumBlockX;                             //ブロックの縦の分割数
    int nNumBlockY;                             //ブロックの横の分割数
    float fBlockSizeX;                          //ブロックの縦のサイズ
    float fBlockSizeY;                          //ブロックの横のサイズ
}MESH_WALL;

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
MESH_WALL g_MeshWall[WALL_NUM];                         //ウォール情報

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// メッシュウォールのコンストラクタ
//*************************************************************************************************
CMeshWall::CMeshWall ( int nPriolity ):CScene3D( SCENE_TYPE_MESHWALL, nPriolity )
{
}

//*************************************************************************************************
// メッシュウォールのデストラクタ
//*************************************************************************************************
CMeshWall::~CMeshWall ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshWall::Init(void)
{
    //変数宣言
    D3DXVECTOR2 Tex = D3DXVECTOR2 ( 0.0f, 0.0f );
    int nNumVertex = 0;                         //作成したい頂点数
    int nNumIndex = 0;                          //作成したいインデックス数
    int nNowVertex = 0;                         //現在の頂点数
    int nNumFaceBlock;                          //1面分のブロック数
    int nNowIndex = 0;                          //現在のインデックス数

    //ウォールの初期化
    for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
    {
        g_MeshWall[nNumWall].rot = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
        if ( nNumWall < 2 )             //(奥と手前)
        {
            g_MeshWall[nNumWall].pos = m_FrontPos;
            g_MeshWall[nNumWall].normal = D3DXVECTOR3 ( 0.0f, 0.0f, -1.0f );
            g_MeshWall[nNumWall].nNumBlockX = m_FrontBlockNum_X;
            g_MeshWall[nNumWall].nNumBlockY = m_FrontBlockNum_Y;
            g_MeshWall[nNumWall].fBlockSizeX = m_FrontSize.x;
            g_MeshWall[nNumWall].fBlockSizeY = m_FrontSize.y;
            if ( nNumWall == 1 )        //(手前)
            {
                g_MeshWall[nNumWall].pos.x *= -1;
                g_MeshWall[nNumWall].pos.z *= -1;
                g_MeshWall[nNumWall].normal.z *= -1;
                g_MeshWall[nNumWall].fBlockSizeX *= -1;
            }
        }
        else                            //(左と右)
        {
            g_MeshWall[nNumWall].pos = m_LeftPos;
            g_MeshWall[nNumWall].normal = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
            g_MeshWall[nNumWall].nNumBlockX = m_LeftBlockNum_X;
            g_MeshWall[nNumWall].nNumBlockY = m_LeftBlockNum_Y;
            g_MeshWall[nNumWall].fBlockSizeX = m_LeftSize.x;
            g_MeshWall[nNumWall].fBlockSizeY = m_LeftSize.y;
            if ( nNumWall == 3 )        //(右)
            {
                g_MeshWall[nNumWall].pos.x *= -1;
                g_MeshWall[nNumWall].pos.z *= -1;
                g_MeshWall[nNumWall].normal.x *= -1;
                g_MeshWall[nNumWall].fBlockSizeX *= -1;
            }
        }
    }

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        //作成したい頂点数
        for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
        {
            nNumVertex += (g_MeshWall[nNumWall].nNumBlockX+1) * (g_MeshWall[nNumWall].nNumBlockY+1);
        }

        //頂点バッファの作成
        if ( FAILED (pDevice->CreateVertexBuffer ( 
            sizeof ( VERTEX_3D ) * nNumVertex,      //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                     //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                          //頂点フォーマット
            D3DPOOL_MANAGED,                        //メモリ管理方法
            &m_pVtxBuffer,                          //頂点バッファ管理インターフェースポインタのアドレス
            NULL ) ) )
        {
            return E_FAIL;
        }

        //作成したいインデックス数
        for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
        {
            nNumIndex += g_MeshWall[nNumWall].nNumBlockY * ( DEGENERATE_NUM * g_MeshWall[nNumWall].nNumBlockX + 4 );
        }
        nNumIndex -= 2;

        //インデックスバッファの作成
        if ( FAILED (pDevice->CreateIndexBuffer ( 
        sizeof ( WORD ) * nNumIndex,            //インデックスバッファのサイズ
        D3DUSAGE_WRITEONLY,                     //使用フラグ
        D3DFMT_INDEX16,                         //インデックスデータフォーマット
        D3DPOOL_MANAGED,                        //メモリの管理方法
        &m_pIdxBuffer,                          //インデックスバッファインターフェースポインタのアドレス
        NULL ) ) )
        {
            return E_FAIL;
        }

        // テクスチャの生成
        if ( FAILED ( D3DXCreateTextureFromFile ( pDevice, TEXTURENAME00, &m_pTexture ) ) )
        {
            MessageBox ( NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK );
        }
    }

    VERTEX_3D* pVtx;                            //仮想アドレス頂点用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock ( 0, 0, (void**)&pVtx, 0 );

    //ウォールの数分繰り返す
    for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
    {
        //頂点数の設定
        nNumVertex = (g_MeshWall[nNumWall].nNumBlockX+1) * (g_MeshWall[nNumWall].nNumBlockY+1);
        Tex.x = 0.0f;
        Tex.y = 0.0f;

        //頂点数分繰り返す
        for ( int i = 0; i < nNumVertex; i++, nNowVertex++ )
        {
            if ( nNumWall < 2 )             //(奥と手前)
            {
                if ( i % (g_MeshWall[nNumWall].nNumBlockX+1) == 0 && i != 0 )
                {
                    g_MeshWall[nNumWall].pos.y -= g_MeshWall[nNumWall].fBlockSizeY;
                    g_MeshWall[nNumWall].pos.x = m_FrontPos.x;
                    if ( nNumWall == 1 )        //(手前)
                    {
                        g_MeshWall[nNumWall].pos.x *= -1;
                    }
                    Tex.x = 0.0f;
                    Tex.y += 1.0f;
                }
            }
            else
            {
                if ( i % (g_MeshWall[nNumWall].nNumBlockX+1) == 0 && i != 0 )
                {
                    g_MeshWall[nNumWall].pos.y -= g_MeshWall[nNumWall].fBlockSizeY;
                    g_MeshWall[nNumWall].pos.z = m_LeftPos.z;
                    if ( nNumWall == 3 )        //(右)
                    {
                        g_MeshWall[nNumWall].pos.z *= -1;
                    }
                    Tex.x = 0.0f;
                    Tex.y += 1.0f;
                }
            }

            //頂点情報設定開始--------------------------------------------------------------

            //頂点座標の設定(3D座標)
            pVtx[nNowVertex].pos = g_MeshWall[nNumWall].pos;

            //ライトベクトルの設定
            pVtx[nNowVertex].normal = D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );

            //頂点カラー(0～255の整数値)
            pVtx[nNowVertex].color = m_FrontColor;

            //テクスチャ座標の設定
            pVtx[nNowVertex].tex = Tex;

            //頂点情報設定終了--------------------------------------------------------------

            //座標更新
            if ( nNumWall < 2 )             //(奥と手前)
            {
                g_MeshWall[nNumWall].pos.x += g_MeshWall[nNumWall].fBlockSizeX;
                Tex.x += 1.0f;
            }
            else
            {
                g_MeshWall[nNumWall].pos.z += g_MeshWall[nNumWall].fBlockSizeX;
                Tex.x += 1.0f;
            }
        }
    }

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock ( );

    WORD* pIdx;                                 //仮想アドレスインデックス用ポインタ

    //インデックスバッファをロックして、仮想アドレスを取得する
    m_pIdxBuffer->Lock ( 0, 0, (void**)&pIdx, 0 );

    //ウォールの数分繰り返す
    for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
    {
        //1面分のブロック数の設定
        nNumFaceBlock = g_MeshWall[nNumWall].nNumBlockX * g_MeshWall[nNumWall].nNumBlockY;

        //1面分のブロック数分繰り返す
        for ( int i = 0; i < nNumFaceBlock; i++, nNowIndex++ )
        {
            //インデックス情報設定開始-----------------------------------------------------------

            //変数宣言
            int j;                                      //何行目か
            int j2;                                     //２枚目以降の壁用何行目か
            int NowWallVertex=0;                        //現在の壁頂点
            int NowWallBloack = 0;                      //現在の壁のブロック数目

            //現在のウォールの数分繰り返す
            for ( int j = 0; j < nNumWall; j++ )
            {
                NowWallVertex += (g_MeshWall[j].nNumBlockX+1) * (g_MeshWall[j].nNumBlockY+1);
            }

            j = nNowIndex / g_MeshWall[nNumWall].nNumBlockX;

            //バーテックスバッファの頂点情報をインデックスに設定
            pIdx[ 0 + nNowIndex * 2 + j * 4 ] = j+nNowIndex+(g_MeshWall[nNumWall].nNumBlockX+1);
            pIdx[ 1 + nNowIndex * 2 + j * 4 ] = j+nNowIndex;
            pIdx[ 2 + nNowIndex * 2 + j * 4 ] = j+nNowIndex+(g_MeshWall[nNumWall].nNumBlockX+1)+1;
            pIdx[ 3 + nNowIndex * 2 + j * 4 ] = j+nNowIndex+1;

            NowWallBloack = nNowIndex;
            //現在のウォールの数分繰り返す
            for ( int j = 0; j < nNumWall; j++ )
            {
                NowWallBloack -= g_MeshWall[j].nNumBlockX * g_MeshWall[j].nNumBlockY;
            }

            j2 = NowWallBloack / g_MeshWall[nNumWall].nNumBlockX;

            //２枚目の壁から
            if ( nNumWall >= 1 )
            {
                //バーテックスバッファの頂点情報をインデックスに設定
                pIdx[ 0 + nNowIndex * 2 + j * 4 ] = (NowWallVertex + (g_MeshWall[nNumWall].nNumBlockX+1)) + NowWallBloack+j2;
                pIdx[ 1 + nNowIndex * 2 + j * 4 ] = NowWallVertex + NowWallBloack+j2;
                pIdx[ 2 + nNowIndex * 2 + j * 4 ] = (NowWallVertex + (g_MeshWall[nNumWall].nNumBlockX+1)) + 1 + NowWallBloack+j2;
                pIdx[ 3 + nNowIndex * 2 + j * 4 ] = NowWallVertex+1 + NowWallBloack+j2;
            }

            //縮退ポリゴン
            if ( nNowIndex % g_MeshWall[nNumWall].nNumBlockX == 0 && nNowIndex != 0 )
            {
                pIdx[ -2 + nNowIndex * 2 + j * 4 ] = pIdx[ -3 + nNowIndex * 2 + j * 4 ];
                pIdx[ -1 + nNowIndex * 2 + j * 4 ] = pIdx[ 0 + nNowIndex * 2 + j * 4 ];
            }

            //インデックス情報設定終了-----------------------------------------------------------
        }
    }

    //インデックスバッファのロックを解除
    m_pIdxBuffer->Unlock ( );

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshWall::Uninit(void)
{
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMeshWall::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshWall::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        //変数宣言
        int nNumVertex = 0;                         //頂点数
        int nPrimitiveCount = 0;                    //プリミティブ数

        //頂点バッファを設定(ストリーム)
        pDevice->SetStreamSource ( 0, m_pVtxBuffer, 0, sizeof ( VERTEX_3D ) );

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_3D);

        //デバイスにインデックスバッファの設定
        pDevice->SetIndices ( m_pIdxBuffer );

        //テクスチャの設定
        pDevice->SetTexture ( 0, m_pTexture );

        //ウォールの数分繰り返す
        for ( int nNumWall = 0; nNumWall < WALL_NUM; nNumWall++ )
        {
            nNumVertex += (g_MeshWall[nNumWall].nNumBlockX+1) * (g_MeshWall[nNumWall].nNumBlockY+1);
            nPrimitiveCount += g_MeshWall[nNumWall].nNumBlockY * ( DEGENERATE_NUM * g_MeshWall[nNumWall].nNumBlockX + 4 );
        }
        nPrimitiveCount -= 4;

        //プリミティブの描画
        pDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLESTRIP,    //プリッミティブの種類
            0,                                                  //オフセット(インデックス)
            0,                                                  //最小の頂点インデックス
            nNumVertex,                                         //頂点数
            0,                                                  //スタートインデックス
            nPrimitiveCount );                                  //プリミティブ数
    }
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CMeshWall *CMeshWall::Create ( D3DXVECTOR3 FrontPos, D3DXVECTOR3 FrontSize, D3DXCOLOR FrontColor, int nFrontBlockNum_X, int nFrontBlockNum_Y, 
                               D3DXVECTOR3 LeftPos, D3DXVECTOR3 LeftSize, D3DXCOLOR LeftColor, int nLeftBlockNum_X, int nLeftBlockNum_Y, int nPriority )
{
    CMeshWall *pMeshWall;                     //シーン２Dクラスのポインタ

    //動的確保
    pMeshWall = new CMeshWall( nPriority );

    //各種初期化
    pMeshWall->m_FrontPos = FrontPos;
    pMeshWall->m_FrontSize = FrontSize;
    pMeshWall->m_FrontColor = FrontColor;
    pMeshWall->m_FrontBlockNum_X = nFrontBlockNum_X;
    pMeshWall->m_FrontBlockNum_Y = nFrontBlockNum_Y;
    pMeshWall->m_LeftPos = LeftPos;
    pMeshWall->m_LeftSize = LeftSize;
    pMeshWall->m_LeftColor = LeftColor;
    pMeshWall->m_LeftBlockNum_X = nLeftBlockNum_X;
    pMeshWall->m_LeftBlockNum_Y = nLeftBlockNum_Y;

    //初期化処理
    pMeshWall->Init ( );

    return pMeshWall;
}