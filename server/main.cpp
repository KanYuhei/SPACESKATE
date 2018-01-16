//*************************************************************************************************
// メイン処理 (main.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// ヘッダー
//*************************************************************************************************
#include <stdio.h>
#include <WinSock2.h>
#include "data.h"
#include <ws2tcpip.h>

//*************************************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*************************************************************************************************
#pragma comment ( lib, "ws2_32.lib" )

//*************************************************************************************************
// マクロ定義
//*************************************************************************************************
#define CLIENT_TO_SERVER_PORT 20000             //クライアントからサーバーへ
#define SERVER_TO_CLIENT_PORT 20001             //サーバーからクライアントへ
#define IP           "192.168.0.7"              //IPアドレス
#define IP2          "172.29.17.111"
#define MULTICAST_IP "225.0.0.2"                //マルチキャストIPアドレス
//#define MULTICAST                             //マルチキャストON/OFF
//#define BROADCAST                             //ブロードキャストON/OFF
#define PLAYER_NUM 2                            //プレイヤーの数

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// グローバル変数
//*************************************************************************************************
int g_ID[PLAYER_NUM];                           //ユーザーID格納

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************
void Server ( void );
SOCKET createServerSocket ( unsigned short port );

//*************************************************************************************************
// メイン関数
//*************************************************************************************************
int main ( void )
{
    //サーバー
    Server ( );
}

//*************************************************************************************************
// サーバー
//*************************************************************************************************
void Server ( void )
{
    //Winsockの初期化
    WSAData wsaData;                            //Winscokデータ構造体
    if ( WSAStartup(MAKEWORD(2,0), &wsaData) != 0 )
    {
        printf ( "Winsockの初期化に失敗しました。\n" );
        return;
    }

    // ホスト名・IPアドレス取得
    char host[256];
    char ip[16];
    PHOSTENT phostent;
    IN_ADDR in;

    gethostname(host, 256);
    phostent = gethostbyname(host);

    memcpy(&in, phostent->h_addr, 4);
    sprintf(ip, inet_ntoa(in));
    printf("ホスト名:%s\nIPアドレス:%s\n", host, ip);

    //ソケットの作成
    SOCKET sock = createServerSocket (CLIENT_TO_SERVER_PORT);

    printf ( "UDP通信開始！！\n" );

    //送信先アドレス
    sockaddr_in addressSend;
    addressSend.sin_port = htons(SERVER_TO_CLIENT_PORT);
    addressSend.sin_family = AF_INET;
    addressSend.sin_addr.s_addr = inet_addr(MULTICAST_IP);//マルチキャストアドレス

    SOCKET sendsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* マルチキャスト用追加 */
    struct sockaddr_in addrLocal;
    struct ip_mreq stMreq;
    u_long lTTL = 2;

    /* マルチキャスト用追加 */
    addrLocal.sin_family = AF_INET;
    addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    addrLocal.sin_port = 0;
    bind(sendsock, (struct sockaddr *) &addrLocal, sizeof(addrLocal));
    stMreq.imr_multiaddr.s_addr = addressSend.sin_addr.s_addr;
    stMreq.imr_interface.s_addr = INADDR_ANY;
    setsockopt(sendsock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq)); // メンバシップ
    setsockopt(sendsock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&lTTL, sizeof(lTTL)); // TTL

    // データ受信
    DATA data;
    g_ID[0] = -1;       //初期ID初期化

    while ( 1 )
    {
        //受信
        recv(sock, (char*)&data, sizeof(data), 0);

        // 新規ユーザー
        /*if (data.Type == DATA_TYPE_ID)
        {
            if (PLAYER_NUM <= data.ID) break;
            if (g_ID[data.ID - 1] == data.ID)
                data.ID++;

            g_ID[data.ID - 1] = data.ID;
        }*/

        printf("データ受信 プレイヤーID:%d\n", data.ID);

        int a = sizeof(data);
        //送信
        sendto(sendsock, (char*)&data, sizeof(data), 0, (sockaddr*)&addressSend, sizeof(addressSend));

        //時刻の表示
        SYSTEMTIME st;                          //SYSTEMTIMEデータ構造体
        GetLocalTime ( &st );                   //時間の取得
        printf( "時間:%d時%d分%d秒%dミリ \n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
    }

    //ソケットの解放
    closesocket(sock);

    //Winsock終了
    WSACleanup();

    rewind ( stdin );
    getchar ( );
    return;
}

//*************************************************************************************************
// サーバーソケットを作成する(UDP/IP通信)
//*************************************************************************************************
SOCKET createServerSocket ( unsigned short port )
{
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( sock == INVALID_SOCKET )
    {
        printf ( "ソケットの作成に失敗しました。\n" );
        printf ( "error : %d\n", WSAGetLastError( ) );
        rewind ( stdin );
        getchar ( );
    }

    struct sockaddr_in addr;                    //sockaddr_inデータ構造体

    addr.sin_family = AF_INET;                  //AF_INETで固定
    addr.sin_port = htons(port);                //ポート番号
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //IPアドレス

    //ソケットに名前をつける
    bind( sock, (sockaddr *)&addr, sizeof(addr) );

#ifdef MULTICAST

    //マルチキャスト受信許可
    int value = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

    //受信アドレス
    sockaddr_in addressServer;
    addressServer.sin_port = htons(port);
    addressServer.sin_family = AF_INET;
    addressServer.sin_addr.s_addr = INADDR_ANY;

    //バインド
    bind(sock, (sockaddr*)&addressServer, sizeof(addressServer));

    //マルチキャストグループに参加
    ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP); //マルチキャストアドレス
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

#endif //MULTICAST

#ifdef BROADCAST

    //ブロードキャスト
    BOOL yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));

#endif //BROADCAST
    return sock;
}