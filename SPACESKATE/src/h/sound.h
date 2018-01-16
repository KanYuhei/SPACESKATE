//*************************************************************************************************
// サウンド処理 (sound.h)
// Author : YUHEI KAN
//*************************************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************

//*************************************************************************************************
// マクロ定義
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// クラス
//*************************************************************************************************
//サウンドクラス
class CSound
{
public:
    //サウンドラベル種類列挙体
    typedef enum
    {
        SOUND_LABEL_BGM_TITLE = 0,                      //タイトルBGM
        SOUND_LABEL_BGM_GAME,                           //ゲームBGM
        SOUND_LABEL_BGM_RESULT,                         //リザルトBGM
        SOUND_LABEL_SE_ENTER,                           //決定音SE
        SOUND_LABEL_SE_COUNTDAWN,                       //カウントダウン
        SOUND_LABEL_SE_START,                           //スタート
        SOUND_LABEL_SE_ITEM,                            //アイテム
        SOUND_LABEL_SE_GOAL,                            //ゴール
        SOUND_LABEL_MAX
    } SOUND_LABEL;

    CSound();                                           //コンストラクタ
    ~CSound();                                          //デストラクタ

    HRESULT Init(HWND hWnd);                            //初期化処理
    void Uninit(void);                                  //終了処理

    HRESULT Play(SOUND_LABEL label);                    //再生
    void Stop(SOUND_LABEL label);                       //停止
    void StopAll(void);                                 //全ての停止

private:
    //パラメータ構造体定義
    typedef struct
    {
        char *pFilename;                                // ファイル名
        int nCntLoop;                                   // ループカウント
    } SOUNDPARAM;

    HRESULT CheckChunk(
        HANDLE hFile, 
        DWORD format, 
        DWORD *pChunkSize, 
        DWORD *pChunkDataPosition);                     //チャンクのチェック
    
    HRESULT ReadChunkData(
        HANDLE hFile, 
        void *pBuffer, 
        DWORD dwBuffersize, 
        DWORD dwBufferoffset);                          //チャンクデータの読み込み

    IXAudio2 *m_pXAudio2;                                //XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;           //マスターボイス
    IXAudio2SourceVoice *m_pSourceVoice[SOUND_LABEL_MAX];//ソースボイス
    BYTE *m_pDataAudio[SOUND_LABEL_MAX];                 //オーディオデータ
    DWORD m_SizeAudio[SOUND_LABEL_MAX];                  //オーディオデータサイズ

    static SOUNDPARAM m_Param[SOUND_LABEL_MAX];         //各音素材のパラメータ
};

#endif
