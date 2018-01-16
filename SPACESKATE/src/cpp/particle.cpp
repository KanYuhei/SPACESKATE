//*************************************************************************************************
// パーティクル処理 (particle.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "particle.h"
#include "mode.h"
#include "modeGame.h"
#include "camera.h"

//*************************************************************************************************
// 定数定義
//*************************************************************************************************
#define TEXTURENAME00 "data/TEXTURE/shadow000.jpg"    //読み込むテクスチャの名前

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
// パーティクルのコンストラクタ
//*************************************************************************************************
CParticle::CParticle()
{
}

//*************************************************************************************************
// パーティクルのデストラクタ
//*************************************************************************************************
CParticle::~CParticle()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CParticle::Init(void)
{
    int nNumVertex;                             //作成したい頂点数

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        //作成したい頂点数の設定
        nNumVertex = NUM_VERTEX * PARTICLE_MAX;

        //頂点バッファの作成
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_3D) * nNumVertex,     //頂点バッファのサイズ
            D3DUSAGE_WRITEONLY,                 //VertexBufferの使い方を指定
            FVF_VERTEX_3D,                      //頂点フォーマット
            D3DPOOL_MANAGED,                    //メモリ管理方法
            &m_pVtxBuffer,                      //頂点バッファ管理インターフェースポインタのアドレス
            NULL)))
        {
            return E_FAIL;
        }

        // テクスチャの生成
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME00, &m_pTexture)))
        {
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
        }
    }

    //各種初期化処理
    for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
    {
        m_Particle[nCntParticle].Size = D3DXVECTOR2(1.0f, 1.0f);
        m_Particle[nCntParticle].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
        m_Particle[nCntParticle].bUse = false;
    }

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CParticle::Uninit(void)
{
    //テクスチャインターフェースの解放
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }

    //頂点バッファ管理インターフェースの解放
    if (m_pVtxBuffer != NULL)
    {
        m_pVtxBuffer->Release();
        m_pVtxBuffer = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CParticle::Update(void)
{
    VERTEX_3D* pVtx;                           //仮想アドレス用ポインタ

    //カメラの座標
    const D3DXVECTOR3 cameraPos = CModeGame::GetCamera()->GetPos();

    //頂点バッファをロックして、仮想アドレスを取得する
    m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
    {
        if (!m_Particle[nCntParticle].bUse)
        {
            continue;
        }

        //寿命の更新
        m_Particle[nCntParticle].nLife--;
        if (m_Particle[nCntParticle].nLife <= 0)
        {
            m_Particle[nCntParticle].bUse = false;
        }
        else
        {
            //加速度の加算
            m_Particle[nCntParticle].Speed.x += m_Particle[nCntParticle].Acceleration.x;
            m_Particle[nCntParticle].Speed.y += m_Particle[nCntParticle].Acceleration.y;
            m_Particle[nCntParticle].Speed.z += m_Particle[nCntParticle].Acceleration.z;

            //位置の更新
            m_Particle[nCntParticle].Pos += m_Particle[nCntParticle].Speed;
        }

        //距離を取得する
        m_Particle[nCntParticle].fRange = RangeV(cameraPos, m_Particle[nCntParticle].Pos);

        //Zソート
        qsort(m_Particle, PARTICLE_MAX, sizeof(PARTICLE), Comp);

        //頂点の設定
        if (m_Particle[nCntParticle].bUse == true)
        {
            SetVertexParticle(pVtx, nCntParticle);
        }
    }

    //頂点バッファのロックを解除
    m_pVtxBuffer->Unlock();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CParticle::Draw(void)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3DデバイスのNULLチェック
    if (pDevice != NULL)
    {
        D3DXMATRIX mtxViewInverse;                  //逆行列
        D3DXMATRIX mtxWorld;                        //ワールド行列
        D3DXMATRIX mtxSclModel;                     //拡大行列
        D3DXMATRIX mtxRotModel;                     //回転行列
        D3DXMATRIX mtxPosModel;                     //平行移動行列

        //頂点バッファを設定(ストリーム)
        pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_3D);

        //加算合成
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);      //アルファブレンド
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //テクスチャα
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);      //背景α

        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);              //ライト全体を無効にする
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);        //αテスト有効

        //テクスチャの設定
        pDevice->SetTexture(0, m_pTexture);

        //パーティクルの描画
        for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
        {
            if (!m_Particle[nCntParticle].bUse)
            {
                continue;
            }

            //単位行列を作る関数
            D3DXMatrixIdentity(&mtxWorld);

            //拡大行列を作る関数
            D3DXMatrixScaling(&mtxSclModel, m_Particle[nCntParticle].scl.x, m_Particle[nCntParticle].scl.y, m_Particle[nCntParticle].scl.z);

            //回転行列を作る関数
            D3DXMatrixRotationYawPitchRoll(&mtxRotModel, D3DXToRadian(0), 0.0f, 0.0f);

            //平行移動行列を作る関数
            D3DXMatrixTranslation(&mtxPosModel, m_Particle[nCntParticle].Pos.x, m_Particle[nCntParticle].Pos.y, m_Particle[nCntParticle].Pos.z);

            //ワールドマトリクスに掛け算(拡大→回転→平行移動)
            mtxWorld *= mtxSclModel;
            mtxWorld *= mtxRotModel;
            mtxWorld *= mtxPosModel;

            //逆行列を作る
            D3DXMatrixInverse(&mtxViewInverse, NULL, CModeGame::GetCamera()->GetmtxView());

            //平行移動成分をカット
            mtxViewInverse._41 = 0.0f;
            mtxViewInverse._42 = 0.0f;
            mtxViewInverse._43 = 0.0f;

            //行列の乗算
            D3DXMatrixMultiply(&mtxWorld, &mtxViewInverse, &mtxWorld);

            //デバイスにワールド変換行列を設定
            pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

            if (m_Particle[nCntParticle].bUse == true)
            {
                //プリミティブの描画
                pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,     //プリッミティブの種類
                    nCntParticle * 4,                           //オフセット(頂点数)
                    NUM_POLYGON);                               //プリミティブ数
            }
        }

        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);          //αテスト無効
        pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                  //ライト全体を有効にする
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //背景α
    }
}

//*************************************************************************************************
// 自身の生成
//*************************************************************************************************
CParticle *CParticle::Create(void)
{
    CParticle *pParticle;                             //パーティクルクラスのポインタ

    //動的確保
    pParticle = new CParticle();

    //初期化処理
    pParticle->Init();

    return pParticle;
}

//*************************************************************************************************
// パーティクルの設定
//*************************************************************************************************
void CParticle::Set(D3DXVECTOR3 Pos, D3DXVECTOR3 Speed, D3DXCOLOR Color, int nLife)
{
    for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
    {
        if (m_Particle[nCntParticle].bUse == false)
        {
            m_Particle[nCntParticle].Pos = Pos;
            m_Particle[nCntParticle].Speed = Speed;
            m_Particle[nCntParticle].Color = Color;
            m_Particle[nCntParticle].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            m_Particle[nCntParticle].nLife = nLife;
            m_Particle[nCntParticle].bUse = true;
            break;
        }
    }
}

//*************************************************************************************************
// パーティクルの取得
//*************************************************************************************************
PARTICLE *CParticle::Get(void)
{
    return &m_Particle[0];
}

//*************************************************************************************************
// ベクトル間の距離
//*************************************************************************************************
float CParticle::RangeV(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
    return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z);
}

//*************************************************************************************************
// 頂点の設定
//*************************************************************************************************
void CParticle::SetVertexParticle( VERTEX_3D* pVtx, int nNum )
{
    //頂点座標の設定(2D座標)
    pVtx[0 + nNum * 4].pos = D3DXVECTOR3(-(m_Particle[nNum].Size.x / 2), -(m_Particle[nNum].Size.y / 2), 0.0f);
    pVtx[1 + nNum * 4].pos = D3DXVECTOR3(-(m_Particle[nNum].Size.x / 2), +(m_Particle[nNum].Size.y / 2), 0.0f);
    pVtx[2 + nNum * 4].pos = D3DXVECTOR3(+(m_Particle[nNum].Size.x / 2), -(m_Particle[nNum].Size.y / 2), 0.0f);
    pVtx[3 + nNum * 4].pos = D3DXVECTOR3(+(m_Particle[nNum].Size.x / 2), +(m_Particle[nNum].Size.y / 2), 0.0f);

    //ライトベクトルの設定
    pVtx[0 + nNum * 4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    pVtx[1 + nNum * 4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    pVtx[2 + nNum * 4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    pVtx[3 + nNum * 4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

    //頂点カラー(0～255の整数値)
    pVtx[0 + nNum * 4].color = D3DXCOLOR(m_Particle[nNum].Color.r, m_Particle[nNum].Color.g, m_Particle[nNum].Color.b, m_Particle[nNum].Color.a);
    pVtx[1 + nNum * 4].color = D3DXCOLOR(m_Particle[nNum].Color.r, m_Particle[nNum].Color.g, m_Particle[nNum].Color.b, m_Particle[nNum].Color.a);
    pVtx[2 + nNum * 4].color = D3DXCOLOR(m_Particle[nNum].Color.r, m_Particle[nNum].Color.g, m_Particle[nNum].Color.b, m_Particle[nNum].Color.a);
    pVtx[3 + nNum * 4].color = D3DXCOLOR(m_Particle[nNum].Color.r, m_Particle[nNum].Color.g, m_Particle[nNum].Color.b, m_Particle[nNum].Color.a);

    //テクスチャ座標の設定
    pVtx[0 + nNum * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1 + nNum * 4].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2 + nNum * 4].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3 + nNum * 4].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//*************************************************************************************************
// カメラから遠い順にソートする
//*************************************************************************************************
int Comp(const void * a, const void * b)
{
    PARTICLE * c = (PARTICLE *)a;
    PARTICLE * d = (PARTICLE *)b;

    //qsortはint型しか扱えない？
    return (unsigned int)(d->fRange * 10) - (unsigned int)(c->fRange * 10);
}