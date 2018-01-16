//*************************************************************************************************
// メイン処理 (main.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// ヘッダー
//*************************************************************************************************
#include "main.h"
#include "manager.h"
#include "mode.h"
#include "modeGame.h"
#include "camera.h"
#include "renderer.h"

//*************************************************************************************************
// 定数定義
//*************************************************************************************************
#define CLASS_NAME      "サンプル"              //ウインドウクラスの名前
#define WINDOW_NAME     "ウィンドウ"            //ウインドウのタイトル名

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
#ifdef _DEBUG
int g_nCountFPS = 0;                            //FPSカウンタ
#endif //_DEBUG

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************
LRESULT CALLBACK WndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void ScreenShot(LPSTR szFileName, D3DXIMAGE_FILEFORMAT Format, RECT rect);

//*************************************************************************************************
// メイン関数
//*************************************************************************************************
int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    //変数宣言
    RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };                        //CreateWindowのRect
    DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);  //ウィンドウのスタイル
    AdjustWindowRect ( &cr, style, FALSE );                                 //外側のフレームを考慮しない
    int ww = cr.right - cr.left;                                            //CreateWindowの幅
    int wh = cr.bottom - cr.top;                                            //CreateWindowの高さ
    RECT dr;                                                                //デスクトップウィンドウのRect
    GetWindowRect( GetDesktopWindow(), &dr );                               //デスクトップウィンドウRect取得
    int dw = dr.right - dr.left;                                            //デスクトップ幅
    int dh = dr.bottom - dr.top;                                            //デスクトップ高さ
    int wx = ww > dw ? 0 : (dw/2) - (ww/2);                                 //デスクトップの中心にウィンドウ(X座標)
    int wy = wh > dh ? 0 : (dh/2) - (wh/2);                                 //デスクトップの中心にウィンドウ(Y座標)

    //引数未使用による警告をしない
    UNREFERENCED_PARAMETER ( hPrevInstance );
    UNREFERENCED_PARAMETER ( lpCmdLine );

    //ウィンドウクラスの設定
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof ( WNDCLASSEX );            //WNDCLASSEXのメモリサイズを指定（自身の構造体のサイズを入れる）
    wcex.style          = CS_CLASSDC;                       //表示するウィンドウのスタイルを設定
    wcex.lpfnWndProc    = WndProc;                          //ウィンドウプロージャのアドレス（関数名）を指定
    wcex.cbClsExtra     = 0;                                //通常は使用しないので"0"を指定
    wcex.cbWndExtra     = 0;                                //通常は使用しないので"0"を指定
    wcex.hInstance      = hInstance;                        //WinMainのパラメータのインスタンスハンドルを設定
    wcex.hIcon          = NULL;                             //使用するアイコンを指定(Windowsがもってる
    wcex.hCursor        = LoadCursor ( NULL, IDC_ARROW );   //マウスカーソルを指定
    wcex.hbrBackground  = ( HBRUSH )( COLOR_WINDOW + 1 );   //ウィンドウのクライアント領域の背景色を設定
    wcex.lpszMenuName   = NULL;                             //Windowにつけるメニューを設定
    wcex.lpszClassName  = CLASS_NAME;                       //ウィンドウクラスの名前
    wcex.hIconSm        = NULL;                             //小さいアイコンが設定された場合の情報を記述

    //ウィンドウクラスの登録
    RegisterClassEx ( &wcex );

    //フルスクリーン
    UINT nID = MessageBox ( NULL, "フルスクリーンにしますか？", "フルスクリーン", MB_YESNO | MB_DEFBUTTON2 );
    bool bWindow = true; //フルスクリーン判定
    if ( nID == IDYES )
    {
        bWindow = false;
        style = WS_POPUP;
    }
    else
    {
        bWindow = true;
    }

    HWND hWnd;
    MSG msg;

    //ウィンドウを作成
    hWnd = CreateWindowEx ( 0,                  //拡張ウィンドウスタイル
        CLASS_NAME,                             //ウインドウクラスの名前
        WINDOW_NAME,                            //ウインドウの名前
        style,                                  //【重要】ウインドウのスタイル
        wx,                                     //ウィンドウの左上Ｘ座標(CW__USEDefauT適当な場所に)
        wy,                                     //ウィンドウの左上Ｙ座標(CW__USEDefauT適当な場所に)
        ww,                                     //ウィンドウの幅（外側のフレームも合わせたもの）
        wh,                                     //ウィンドウの高さ（外側のフレームも合わせたもの）
        NULL,                                   //親ウィンドウのハンドル
        NULL,                                   //メニューハンドルまたは子ウィンドウID
        hInstance,                              //インスタンスハンドル（引数）
        NULL );                                 //ウィンドウ作成データ

    //ウィンドウの表示
    ShowWindow ( hWnd, nCmdShow );              //指定されたウィンドウを表示
    UpdateWindow ( hWnd );                      //ウィンドウの状態を直ちに反映

    timeBeginPeriod ( 1 );                      //分解能を設定

    //FPS関連初期化
    DWORD dwCurrentTime = 0;                    //現在の時刻
    DWORD dwFrameCount = 0;                     //フレームカウント
    DWORD dwExecLastTime = 0;                   //更新と描画をする前の時刻
    DWORD dwFPSLastTime = timeGetTime ( );      //システム時刻をミリ秒単位で取得

    //初期化処理
    if ( FAILED(CManager::Init ( hInstance, hWnd, bWindow ) ) )
    {
        MessageBox ( NULL, "初期化に失敗しました", "終了メッセージ", MB_OK );
    }
    else
    {
        //メッセージループ
        for ( ;; )
        {
            if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
            {
                //Windowの処理
                if ( msg.message == WM_QUIT )
                {
                    break;
                }
                else
                {
                    TranslateMessage ( &msg );  //仮想キーメッセージを文字メッセージへ変換
                    DispatchMessage ( &msg );   //ウィンドウプロシージャ
                }
            }
            else
            {
                dwCurrentTime = timeGetTime ( );
#ifdef _DEBUG
                if ( ( dwCurrentTime - dwFPSLastTime ) >= 500 ) //0.5秒毎に実行
                {
                    g_nCountFPS = ( dwFrameCount * 1000 ) / ( dwCurrentTime - dwFPSLastTime );
                    dwFPSLastTime = dwCurrentTime;
                    dwFrameCount = 0;                       //フレームカウンタをリセット
                }
#endif
                if ( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
                {
                    dwExecLastTime = dwCurrentTime; //処理した時間
                    //更新処理
                    CManager::Update ( );
                    //描画処理
                    CManager::Draw ( );
                    dwFrameCount++;
                }
            }
        }
    }

    //終了処理
    CManager::Uninit ( );

    return (int)msg.wParam;
}

//*************************************************************************************************
// ウィンドウプロシージャ
//*************************************************************************************************
LRESULT CALLBACK WndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
    //ウィンドウを破壊
    case WM_DESTROY:
        PostQuitMessage ( 0 );      //"WM_QUIT"メッセージを返す
        break;

    //描画
    case WM_PAINT:
        break;

    //画面サイズ
    case WM_SIZE:
        {
            //ウィンドウサイズ取得
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            //フレーム分の大きさを引く
            width -= GetSystemMetrics (SM_CXDLGFRAME ) * 2;
            height -= GetSystemMetrics ( SM_CXDLGFRAME ) * 2 + GetSystemMetrics ( SM_CYCAPTION ) + GetSystemMetrics ( SM_CYMENU );
            //カメラポインター取得
            CCamera *pCamera = CModeGame::GetCamera ( );
            if ( pCamera != NULL )              //取得できたら
            {
                //画面サイズの設定
                pCamera->SetScreenSize ( width, height );
            }
            InvalidateRect ( hWnd, NULL, FALSE );
        }
        break;

    //キー入力
    case WM_KEYDOWN:
        switch ( wParam )
        {
        case VK_ESCAPE:
            //UINT nID = MessageBox ( NULL, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2 );
            //if ( nID == IDYES )
            //{
                DestroyWindow ( hWnd );
            //}
            break;
        case VK_F5:
            RECT rect;
            GetClientRect(hWnd, &rect);
            char filename[256];
            static int num = 1;
            sprintf(filename, "Screenshot/screen_shot%d.jpg", num);
            ScreenShot(filename, D3DXIFF_JPG, rect);
            num++;
            break;
        }
        break;

    default:
        break;
    }

    return DefWindowProc ( hWnd, uMsg, wParam, lParam );        //既定の動きを返す
}

//*************************************************************************************************
// スクリーンショット保存用
//*************************************************************************************************
void ScreenShot(LPSTR szFileName, D3DXIMAGE_FILEFORMAT Format, RECT rect)
{
    // サーフェイス用変数宣言
    LPDIRECT3DSURFACE9 pSourceSurface = NULL;

    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSourceSurface);

    // 保存
    // 関数の仕様
    // 引数
    // その１ファイル名
    // その２画像拡張子フォーマット
    // その３保存元テクスチャーポインタ
    // その４パレット
    // その画像描画範囲ポインタ(NULLなら全画面)


    D3DXSaveSurfaceToFile(szFileName, Format, pSourceSurface, NULL, &rect);
}

#ifdef _DEBUG
//*************************************************************************************************
// FPSカウンタ取得
//*************************************************************************************************
int GetFPS ( void )
{
    return g_nCountFPS;
}
#endif //_DEBUG