//*************************************************************************************************
// ソケット処理 (socket.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルード
//*************************************************************************************************
#include "main.h"
#include "manager.h"
#include "sound.h"
#include "mode.h"
#include "modeTitle.h" 
#include "modeGame.h"
#include "renderer.h"
#include "socket.h"
#include <stdarg.h>
#include "scene.h"
#include "sceneModel.h"
#include "item.h"
#include "player.h"
#include "player2.h"

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
SOCKET CSocket::m_sock = INVALID_SOCKET;        //Socketデータ構造体
SOCKET CSocket::m_Sendsock = INVALID_SOCKET;    //Socketデータ構造体
int CSocket::m_ID = -1;                         //ID
SOCKADDR_IN CSocket::m_ServerAddress;           //sockaddr_inデータ構造体
USER_INFO CSocket::m_UserInfo[MAX_PLAYER];      //ユーザー情報
bool CSocket::m_InitID = false;                 //ID初期化フラグ

//*************************************************************************************************
// ソケットのコンストラクタ
//*************************************************************************************************
CSocket::CSocket ()
{
}

//*************************************************************************************************
// ソケットのデストラクタ
//*************************************************************************************************
CSocket::~CSocket ()
{
}

//*************************************************************************************************
// ソケット初期化処理
//*************************************************************************************************
HRESULT CSocket::Init ( void )
{
    // IDをランダムに設定（本来ならばユーザID等を設定すべき）
    srand(timeGetTime());
    m_ID = 1;

    //ユーザー情報の初期化
    for (int i = 0; i < MAX_PLAYER; i++)
    {
        m_UserInfo[i].m_ID = -1;
        memset(&m_UserInfo[i].fromaddr, 0, sizeof(m_UserInfo[i].fromaddr));
    }

    //Winsockの初期化
    WSAData wsaData;
    if ( WSAStartup(MAKEWORD(2,0), &wsaData) != 0 )
    {
        MessageBox ( NULL, "Winsockの初期化に失敗しました。", "終了メッセージ", MB_OK );
        return E_FAIL;
    }

    //ソケットの作成
    m_Sendsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //送信先アドレス
    m_ServerAddress.sin_port = htons((unsigned short)CLIENT_TO_SERVER_PORT);
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = inet_addr(IP);

    //ソケットの作成
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( m_sock == INVALID_SOCKET )
    {
        MessageBox ( NULL, "ソケットの作成に失敗しました。\n", "終了メッセージ", MB_OK );
        return E_FAIL;
    }

    int ret;

    //マルチキャスト受信許可
    int value = 1;
    ret = setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

    //受信アドレス
    sockaddr_in addressServer;
    addressServer.sin_port = htons(SERVER_TO_CLIENT_PORT);
    addressServer.sin_family = AF_INET;
    addressServer.sin_addr.s_addr = INADDR_ANY;

    //バインド
    ret = bind(m_sock, (sockaddr*)&addressServer, sizeof(addressServer));

    //マルチキャストグループに参加
    ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP); //マルチキャストアドレス
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

    // ブロードキャスト
    //BOOL yes = 1;
    //setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));

    //受信スレッド開始
    _beginthreadex(NULL, 0, ReceiveThread, NULL, 0, NULL);

    return S_OK;
}

//*************************************************************************************************
// ソケット終了処理
//*************************************************************************************************
void CSocket::Uninit ( void )
{
    //ソケットの解放
    closesocket(m_sock);

    //Winsock終了
    WSACleanup();
}

//*************************************************************************************************
// 受信スレッド
//*************************************************************************************************
unsigned __stdcall CSocket::ReceiveThread( LPVOID Param )
{
    int ret;
    DATA data;

    while (1)
    {
        ret = recv(m_sock, (char*)&data, sizeof(data), 0);

        if (ret != SOCKET_ERROR)
        {
            //データタイプ解析
            switch (data.Type)
            {
                //ID作成
                case DATA_TYPE_ID:
                {
                    if (m_InitID == false)
                    {
                        m_ID = data.ID;
                        m_InitID = true;
                    }
                    break;
                }

                //イベント関連
                case DATA_TYPE_EVENT:
                {
                    if (data.ID != m_ID)
                    {
                        //準備完了の場合
                        if (data.Event.Type == DATA_EVENT_TYPE_START)
                        {
                            //タイトルBGMを停止する
                            CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);

                            //ゲーム進行フラグ
                            CModeTitle::SetGame();
                        }

                        //相手が勝利の場合
                        if (data.Event.Type == DATA_EVENT_TYPE_GOAL)
                        {
                            CModeGame::SetPlayer2GameEnd(GAME_GOAL);
                        }
                    }
                    break;
                }

                //座標
                case DATA_TYPE_POSITION:
                {
                    if (data.ID != m_ID)
                    {
                        CPlayer2 *player2 = CModeGame::GetPlayer2( );

                        if (player2)
                        {
                            D3DXVECTOR3 pos;

                            pos.x = data.Position.x;
                            pos.y = data.Position.y;
                            pos.z = data.Position.z;

                            player2->SetPos(pos);
                        }
                    }
                    break;
                }

                //回転角度
                case DATA_TYPE_ROTATION:
                {
                    if (data.ID != m_ID)
                    {
                        CPlayer2 *player2 = CModeGame::GetPlayer2();

                        if (player2)
                        {
                            D3DXVECTOR3 rot;

                            rot.x = data.Rotation.x;
                            rot.y = data.Rotation.y;
                            rot.z = data.Rotation.z;

                            player2->SetRot(rot);
                        }
                    }
                    break;
                }

                //モーション
                case DATA_TYPE_MOTION:
                {
                    if (data.ID != m_ID)
                    {
                        CPlayer2 *player2 = CModeGame::GetPlayer2();

                        if (player2)
                        {
                            player2->GetMotion(CPlayer2::WAIT)->SetPlay(data.Motion.WaitPlay);
                            player2->GetMotion(CPlayer2::WALK)->SetPlay(data.Motion.WalkPlay);
                            player2->GetMotion(CPlayer2::RUN)->SetPlay(data.Motion.RunPlay);
                            player2->GetMotion(CPlayer2::PUNTCH)->SetPlay(data.Motion.PuntchPlay);
                        }
                    }
                    break;
                }

                //タイム
                case DATA_TYPE_TIME:
                {
                    if (data.ID != m_ID)
                    {
                        //相手プレイヤーのゴールした時のタイムを取得・設定
                        CManager::SetPlayerTime(2, data.Time.Time);
                    }
                    break;
                }
            }
        }
    }

    return 0;
}

//*************************************************************************************************
// データ送信
//*************************************************************************************************
void CSocket::SendData(DATA Data)
{
    //ID設定
    Data.ID = m_ID;

    int ret;

    //サーバにデータ送信
    ret = sendto(m_Sendsock, (char*)&Data, sizeof(Data), 0, (sockaddr*)&m_ServerAddress, sizeof(m_ServerAddress));

    if (ret == SOCKET_ERROR) m_Sendsock = INVALID_SOCKET;
}

//*************************************************************************************************
// IDの取得
//*************************************************************************************************
int CSocket::GetID(void)
{
    return m_ID;
}