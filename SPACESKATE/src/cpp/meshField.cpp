//*************************************************************************************************
// メッシュフィールド処理 (meshField.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "scene.h"
#include "scene3D.h"
#include "meshField.h"

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************
#define TEXTURENAME00 "data/TEXTURE/ground2.jpg"    //読み込むテクスチャの名前
#define DEGENERATE_NUM      ( 2 )                   //縮退ポリゴンによる頂点数増分(縦の分割数によって比例)

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
float g_FieldHeight[BLOACK_NUM_Z][BLOACK_NUM_X];//フィールドの高さ

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// メッシュフィールドのコンストラクタ
//*************************************************************************************************
CMeshField::CMeshField ( int nPriority ):CScene3D( SCENE_TYPE_MESHFIELD, nPriority )
{
}

//*************************************************************************************************
// メッシュフィールドのデストラクタ
//*************************************************************************************************
CMeshField::~CMeshField ()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshField::Init(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        //頂点バッファの作成
        if ( FAILED (pDevice->CreateVertexBuffer ( 
            sizeof ( VERTEX_3D ) * (BLOACK_NUM_Z+1) * (BLOACK_NUM_X+1),      //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                     //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                          //頂点フォーマット
            D3DPOOL_MANAGED,                        //メモリ管理方法
            &m_pVtxBuffer,                          //頂点バッファ管理インターフェースポインタのアドレス
            NULL ) ) )
        {
            return E_FAIL;
        }

        //インデックスバッファの作成
        if ( FAILED (pDevice->CreateIndexBuffer ( 
            sizeof ( WORD ) * BLOACK_NUM_Z * ( DEGENERATE_NUM * BLOACK_NUM_X + 4 ) - 2, //インデックスバッファのサイズ
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

    //変数宣言
    D3DXVECTOR3 Pos = D3DXVECTOR3 ( m_Pos.x, m_Pos.y, m_Pos.z );
    D3DXVECTOR3 Normal = D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR2 Tex = D3DXVECTOR2 ( 0.0f, 0.0f );
    D3DXVECTOR3 DirectionX, DirectionZ, NormalX, NormalZ;
    int z = 0;

    //頂点バッファの設定開始/////////////////////////////////////////////////////////////
    VERTEX_3D* pVtx;                            //仮想アドレス頂点用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock ( 0, 0, (void**)&pVtx, 0 );

    //頂点数分繰り返す
    for ( int i = 0; i < (BLOACK_NUM_Z+1) * (BLOACK_NUM_X+1); i++ )
    {
        Pos.x += m_Size.x;
        g_FieldHeight[z][i % (BLOACK_NUM_X + 1)] = 0.0f/*(float)(rand() % 200)*/;
        Pos.y = g_FieldHeight[z][i % (BLOACK_NUM_X+1)];
        Tex.x += 1.0f;
        if ( i % (BLOACK_NUM_X+1) == 0 )
        {
            Pos.z -= m_Size.z;
            Pos.x = m_Pos.x;
            Tex.x = 0.0f;
            Tex.y += 1.0f;
            z++;
        }

        //法線の計算
        DirectionX = D3DXVECTOR3 ( (Pos.x+m_Size.x) - (Pos.x-m_Size.x), 
                                   g_FieldHeight[z][i % (BLOACK_NUM_X+1)+1] - g_FieldHeight[z][i % (BLOACK_NUM_X+1)-1],
                                   0.0f );
        NormalX.x = -DirectionX.y;
        NormalX.y = DirectionX.x;
        NormalX.z = 0.0f;

        DirectionZ = D3DXVECTOR3 ( 0.0f, 
                                   g_FieldHeight[z][i % (BLOACK_NUM_X+1)+1] - g_FieldHeight[z][i % (BLOACK_NUM_X+1)-1],
                                   (Pos.z+m_Size.z) - (Pos.z-m_Size.z) );
        NormalZ.x = 0.0f;
        NormalZ.y = DirectionX.z;
        NormalZ.z = -DirectionX.y;

        Normal = NormalX + NormalZ;
        D3DXVec3Normalize ( &Normal, &Normal );

        //頂点座標の設定(3D座標)
        pVtx[i].pos = Pos;

        //ライトベクトルの設定
        pVtx[i].normal = Normal;

        //頂点カラー(0～255の整数値)
        pVtx[i].color = m_Color;

        //テクスチャ座標の設定
        pVtx[i].tex = Tex;
    }

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock ( );

    //インデックスバッファの設定開始/////////////////////////////////////////////////////
    WORD* pIdx;                                 //仮想アドレスインデックス用ポインタ

    //インデックスバッファをロックして、仮想アドレスを取得する
    m_pIdxBuffer->Lock ( 0, 0, (void**)&pIdx, 0 );

    //ブロックの数分繰り返す
    for ( int i = 0; i < BLOACK_NUM_Z * BLOACK_NUM_X; i++ )
    {
        int j;                                  //何行目か
        j = i / BLOACK_NUM_X;

        //バーテックスバッファの頂点情報をインデックスに設定
        pIdx[ 0 + i * 2 + j * 4 ] = j + i +( BLOACK_NUM_X + 1 );
        pIdx[ 1 + i * 2 + j * 4 ] = j + i;
        pIdx[ 2 + i * 2 + j * 4 ] = j + i + ( BLOACK_NUM_X + 1 ) + 1;
        pIdx[ 3 + i * 2 + j * 4 ] = j + i +1;

        //縮退ポリゴン
        if ( i % BLOACK_NUM_X == 0 && i != 0 )
        {
            pIdx[ -2 + i * 2 + j * 4 ] = pIdx[ -3 + i * 2 + j * 4 ];
            pIdx[ -1 + i * 2 + j * 4 ] = pIdx[ 0 + i * 2 + j * 4 ];
        }
    }

    //インデックスバッファのロックを解除
    m_pIdxBuffer->Unlock ( );

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshField::Uninit(void)
{
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMeshField::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshField::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice ( );

    //Direct3DデバイスのNULLチェック
    if ( pDevice != NULL )
    {
        //頂点バッファを設定(ストリーム)
        pDevice->SetStreamSource ( 0, m_pVtxBuffer, 0, sizeof ( VERTEX_3D ) );

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_3D);

        //デバイスにインデックスバッファの設定
        pDevice->SetIndices ( m_pIdxBuffer );

        //テクスチャの設定
        pDevice->SetTexture ( 0, m_pTexture );

        //プリミティブの描画
        pDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLESTRIP,    //プリッミティブの種類
            0,                                                  //オフセット(インデックス)
            0,                                                  //最小の頂点インデックス
            (BLOACK_NUM_Z+1) * (BLOACK_NUM_X+1),                //頂点数
            0,                                                  //スタートインデックス
            BLOACK_NUM_Z * ( DEGENERATE_NUM * BLOACK_NUM_X + 4 ) - 4 ); //プリミティブ数
    }
}

//*************************************************************************************************
// 自身を生成
//*************************************************************************************************
CMeshField *CMeshField::Create ( D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, int nPriority )
{
    CMeshField *pMeshField;                     //メッシュフィールドクラスのポインタ

    //動的確保
    pMeshField = new CMeshField(nPriority);

    //各種初期化
    pMeshField->m_Pos = Pos;
    pMeshField->m_Size = Size;
    pMeshField->m_Color = Color;

    //初期化処理
    pMeshField->Init ( );

    return pMeshField;
}

//*************************************************************************************************
// 地面の高さ取得
//*************************************************************************************************
float CMeshField::GetHeight ( D3DXVECTOR3 Pos )
{
    D3DXVECTOR3 P0, P1, P2;                                 //ポリゴンの各頂点
    D3DXVECTOR3 Vec01, Vec0P, Vec12, Vec1P, Vec20, Vec2P;   //ベクトル
    D3DXVECTOR3 Normal0, Normal1, Normal2;                  //各頂点の法線
    D3DXVECTOR3 VecN;                                       //面の法線

    VERTEX_3D* pVtx;                            //仮想アドレス頂点用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock ( 0, 0, (void**)&pVtx, 0 );

    //Posの座標から何処のインデックスの取得
    int nIndexX = (int)((Pos.x - m_Pos.x) / m_Size.x);
    int nIndexZ = (int)((-Pos.z + m_Pos.z) / m_Size.z);
    int nIndex = ( BLOACK_NUM_X + 1 ) * nIndexZ + nIndexX;

    //左下ポリゴン
    P0 = pVtx[ nIndex ].pos;
    P1 = pVtx[ nIndex - (BLOACK_NUM_X+1) ].pos;
    P2 = pVtx[ nIndex + 1 ].pos;
    Vec01 = P1 - P0;
    Vec0P = Pos - P0;
    D3DXVec3Cross ( &Normal0, &Vec01, &Vec0P );
    if ( Normal0.y >= 0.0f )
    {
        Vec12 = P2 - P1;
        Vec1P = Pos - P1;
        D3DXVec3Cross ( &Normal1, &Vec12, &Vec1P );
        if ( Normal1.y >= 0.0f )
        {
            Vec20 = P0 - P2;
            Vec2P = Pos - P2;
            D3DXVec3Cross ( &Normal2, &Vec20, &Vec2P );
            if ( Normal2.y >= 0.0f )
            {
                //高さ計算
                D3DXVec3Cross ( &VecN, &Vec01, &Vec20 );
                D3DXVec3Normalize ( &VecN, &VecN );
                Pos.y = P0.y - ((Pos.x - P0.x) * VecN.x + (Pos.z - P0.z) * VecN.z ) / VecN.y;

                //頂点バッファのロックを解除
                m_pVtxBuffer->Unlock ( );
                return Pos.y;
            }
        }
    }

    //右上ポリゴン
    P0 = pVtx[ nIndex - BLOACK_NUM_X ].pos;
    P1 = pVtx[ nIndex + 1 ].pos;
    P2 = pVtx[ nIndex - (BLOACK_NUM_X+1) ].pos;
    Vec01 = P1 - P0;
    Vec0P = Pos - P0;
    D3DXVec3Cross ( &Normal0, &Vec01, &Vec0P );
    if ( Normal0.y >= 0.0f )
    {
        Vec12 = P2 - P1;
        Vec1P = Pos - P1;
        D3DXVec3Cross ( &Normal1, &Vec12, &Vec1P );
        if ( Normal1.y >= 0.0f )
        {
            Vec20 = P0 - P2;
            Vec2P = Pos - P2;
            D3DXVec3Cross ( &Normal2, &Vec20, &Vec2P );
            if ( Normal2.y >= 0.0f )
            {
                //高さ計算
                D3DXVec3Cross ( &VecN, &Vec01, &Vec20 );
                D3DXVec3Normalize ( &VecN, &VecN );
                Pos.y = P0.y - ((Pos.x - P0.x) * VecN.x + (Pos.z - P0.z) * VecN.z ) / VecN.y;

                //頂点バッファのロックを解除
                m_pVtxBuffer->Unlock ( );
                return Pos.y;
            }
        }
    }

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock ( );

    return Pos.y;
}

//*************************************************************************************************
// 面の法線の取得
//*************************************************************************************************
D3DXVECTOR3 CMeshField::GetNormal ( D3DXVECTOR3 Pos )
{
    D3DXVECTOR3 P0, P1, P2;                                 //ポリゴンの各頂点
    D3DXVECTOR3 Vec01, Vec0P, Vec12, Vec1P, Vec20, Vec2P;   //ベクトル
    D3DXVECTOR3 Normal0, Normal1, Normal2;                  //各頂点の法線
    D3DXVECTOR3 VecN;                                       //面の法線

    VERTEX_3D* pVtx;                            //仮想アドレス頂点用ポインタ

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock ( 0, 0, (void**)&pVtx, 0 );

    //Posの座標から何処のインデックスの取得
    int nIndexX = (int)((Pos.x - m_Pos.x) / m_Size.x);
    int nIndexZ = (int)((-Pos.z + m_Pos.z) / m_Size.z);
    int nIndex = ( BLOACK_NUM_X + 1 ) * nIndexZ + nIndexX;

    //左下ポリゴン
    P0 = pVtx[ nIndex ].pos;
    P1 = pVtx[ nIndex - (BLOACK_NUM_X+1) ].pos;
    P2 = pVtx[ nIndex + 1 ].pos;
    Vec01 = P1 - P0;
    Vec0P = Pos - P0;
    D3DXVec3Cross ( &Normal0, &Vec01, &Vec0P );
    if ( Normal0.y >= 0.0f )
    {
        Vec12 = P2 - P1;
        Vec1P = Pos - P1;
        D3DXVec3Cross ( &Normal1, &Vec12, &Vec1P );
        if ( Normal1.y >= 0.0f )
        {
            Vec20 = P0 - P2;
            Vec2P = Pos - P2;
            D3DXVec3Cross ( &Normal2, &Vec20, &Vec2P );
            if ( Normal2.y >= 0.0f )
            {
                //高さ計算
                D3DXVec3Cross ( &VecN, &Vec20, &Vec01 );
                D3DXVec3Normalize ( &VecN, &VecN );

                //頂点バッファのロックを解除
                m_pVtxBuffer->Unlock ( );
                return VecN;
            }
        }
    }

    //右上ポリゴン
    P0 = pVtx[ nIndex - BLOACK_NUM_X ].pos;
    P1 = pVtx[ nIndex + 1 ].pos;
    P2 = pVtx[ nIndex - (BLOACK_NUM_X+1) ].pos;
    Vec01 = P1 - P0;
    Vec0P = Pos - P0;
    D3DXVec3Cross ( &Normal0, &Vec01, &Vec0P );
    if ( Normal0.y >= 0.0f )
    {
        Vec12 = P2 - P1;
        Vec1P = Pos - P1;
        D3DXVec3Cross ( &Normal1, &Vec12, &Vec1P );
        if ( Normal1.y >= 0.0f )
        {
            Vec20 = P0 - P2;
            Vec2P = Pos - P2;
            D3DXVec3Cross ( &Normal2, &Vec20, &Vec2P );
            if ( Normal2.y >= 0.0f )
            {
                //高さ計算
                D3DXVec3Cross ( &VecN, &Vec20, &Vec01 );
                D3DXVec3Normalize ( &VecN, &VecN );

                //頂点バッファのロックを解除
                m_pVtxBuffer->Unlock ( );
                return VecN;
            }
        }
    }

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock ( );

    return VecN;
}