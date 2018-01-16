//*************************************************************************************************
// レンダラ処理 (renderer.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "renderer.h"
#include "manager.h"

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

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
LPDIRECT3D9 CRenderer::m_D3D = NULL;                        //Direct3Dインターフェース
LPDIRECT3DDEVICE9 CRenderer::m_D3DDevice = NULL;            //Direct3Dデバイスへのポインタ
#ifdef _DEBUG
LPD3DXFONT CRenderer::m_Font = NULL;                        //フォントへのインターフェース
#endif //_DEBUG

//*************************************************************************************************
// レンダラのコンストラクタ
//*************************************************************************************************
CRenderer::CRenderer ()
{
}

//*************************************************************************************************
// レンダラのデストラクタ
//*************************************************************************************************
CRenderer::~CRenderer ()
{
}

//*************************************************************************************************
//初期化処理
//*************************************************************************************************
HRESULT CRenderer::Init ( HINSTANCE hInstance, HWND hWnd, BOOL bWindow )
{
   //変数宣言
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

    //Direct3Dインターフェースの取得
    m_D3D = Direct3DCreate9 ( D3D_SDK_VERSION );
    if ( m_D3D == NULL )
    {
        return E_FAIL;
    }

    if ( FAILED ( m_D3D->GetAdapterDisplayMode ( D3DADAPTER_DEFAULT, &d3ddm ) ) )
    {
        return E_FAIL;
    }

    //デバイスのプレゼントパラメータの設定
    ZeroMemory ( &d3dpp, sizeof ( d3dpp ) );
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferCount = 1;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;          //D24S8を設定しないと使えない
    d3dpp.Windowed = bWindow;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    //Direct3Dデバイスの作成
    if ( FAILED ( m_D3D->CreateDevice ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice ) ) )
    {
        if ( FAILED ( m_D3D->CreateDevice ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice ) ) )
        {
            if ( FAILED ( m_D3D->CreateDevice ( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice ) ) )
            {
                return E_FAIL;
            }
        }
    }

    //レンダーステート
    m_D3DDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CCW );           //カリング
    m_D3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );          //アルファブレンド
    m_D3DDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_D3DDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    //float StartPos = 1000.0f;
    //float EndPos = 1500.0f;
    //m_D3DDevice->SetRenderState ( D3DRS_FOGENABLE, TRUE );                  //フォグ
    //m_D3DDevice->SetRenderState ( D3DRS_FOGCOLOR, D3DXCOLOR ( 1.0f, 1.0f, 1.0f, 1.0f ) );   //フォグ カラー
    //m_D3DDevice->SetRenderState ( D3DRS_FOGVERTEXMODE, D3DFOG_NONE );       //頂点フォグ
    //m_D3DDevice->SetRenderState ( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );      //
    //m_D3DDevice->SetRenderState ( D3DRS_FOGSTART, *(DWORD*)(&StartPos) );
    //m_D3DDevice->SetRenderState ( D3DRS_FOGEND, *(DWORD*)(&EndPos) );

    //サンプラーステート
    m_D3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );// テクスチャＵ値の繰り返し設定
    m_D3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );// テクスチャＶ値の繰り返し設定
    m_D3DDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ); // テクスチャ拡大時の補間設定
    m_D3DDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ); // テクスチャ縮小時の補間設定
    //m_D3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTEXF_LINEAR );
    //m_D3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );

    //テクスチャステージステート
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_D3DDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

#ifdef _DEBUG
    //フォント作成
    D3DXCreateFont ( m_D3DDevice, 
        18,                                     //高さ
        0,                                      //平均文字幅
        0,                                      //太さ
        0,                                      //ミップマップレベル
        FALSE,                                  //斜体
        SHIFTJIS_CHARSET,                       //キャラクタ・セット(シフトJISを指定)
        OUT_DEFAULT_PRECIS,                     //出力制度
        DEFAULT_QUALITY,                        //品質
        DEFAULT_PITCH,                          //ピッチ
        "Terminal",                             //フォント名
        &m_Font );
#endif //_DEBUG

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CRenderer::Uninit ( void )
{
#ifdef _DEBUG
    //フォントへのインターフェースの解放
    if ( m_Font != NULL )
    {
        m_Font->Release();
        m_Font = NULL;
    }
#endif

    //Direct3Dデバイスの解放
    if ( m_D3DDevice != NULL )
    {
        m_D3DDevice->Release();
        m_D3DDevice = NULL;
    }

    //Direct3Dインターフェースの解放
    if ( m_D3D != NULL )
    {
        m_D3D->Release();
        m_D3D = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CRenderer::Updata ( void )
{
#ifdef _DEBUG
    //ワイヤーフレーム切り替え
    bool bWireFrame = CManager::GetWireFrame ( );
    if ( bWireFrame == true ) m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //ワイヤーフレーム
    else m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);     //通常(面を塗りつぶす)
#endif //_DEBUG
}

//*************************************************************************************************
// 描画処理開始
//*************************************************************************************************
void CRenderer::DrawBegin ( void )
{
    //画面（バックバッファ）のクリア
    m_D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA ( 255, 255, 255, 255 ), 1.0f, 0 );

    //描画の開始
    m_D3DDevice->BeginScene( );
}

//*************************************************************************************************
// 描画処理終了
//*************************************************************************************************
void CRenderer::DrawEnd ( void )
{
    //描画の終了
    m_D3DDevice->EndScene ( );

    //フロントバッファとバックバッファの入れ替え
    m_D3DDevice->Present ( NULL, NULL, NULL, NULL );
}

//*************************************************************************************************
// Direct3Dデバイスの取得
//*************************************************************************************************
LPDIRECT3DDEVICE9 CRenderer::GetDevice ( void )
{
    return m_D3DDevice;
}

#ifdef _DEBUG
//*************************************************************************************************
// フォントポインタを渡す関数
//*************************************************************************************************
LPD3DXFONT CRenderer::GetFont ( void )
{
    return m_Font;
}
#endif //_DEBUG
