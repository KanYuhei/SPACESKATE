//*************************************************************************************************
// サウンド処理 (sound.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "main.h"
#include "sound.h"

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
// 各音素材のパラメータ
CSound::SOUNDPARAM CSound::m_Param[SOUND_LABEL_MAX] =
{
    { "data/SOUND/BGM/title.wav", -1 },                 //タイトルBGM
    { "data/SOUND/BGM/game.wav", -1 },                  //ゲームBGM
    { "data/SOUND/BGM/result.wav", -1 },                //リザルトBGM
    { "data/SOUND/SE/enter.wav", 0 },                   //決定音SE
    { "data/SOUND/SE/coutdawn.wav", 0 },                //カウントダウン
    { "data/SOUND/SE/start.wav", 0 },                   //スタート
    { "data/SOUND/SE/item.wav", 0 },                    //アイテム
    { "data/SOUND/SE/goal.wav", 0 }                     //ゴール
};

//*************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************

//*************************************************************************************************
// サウンドクラスのコンストラクタ
//*************************************************************************************************
CSound::CSound()
{
    m_pXAudio2 = NULL;
    m_pMasteringVoice = NULL;
    for (int nCntLabel = 0; nCntLabel < CSound::SOUND_LABEL_MAX; nCntLabel++)
    {
        m_pSourceVoice[nCntLabel] = NULL;
        m_pDataAudio[nCntLabel] = NULL;
        m_SizeAudio[nCntLabel] = NULL;
    }
}

//*************************************************************************************************
//  サウンドクラスのデストラクタ
//*************************************************************************************************
CSound::~CSound()
{

}

//*************************************************************************************************
// サウンド初期化処理
//*************************************************************************************************
HRESULT CSound::Init(HWND hWnd)
{
    HRESULT hr;

    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    hr = XAudio2Create(&m_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // マスターボイスの生成
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

        if (m_pXAudio2)
        {
            // XAudio2オブジェクトの開放
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // サウンドデータの初期化
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // バッファのクリア
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(m_Param[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &m_SizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        m_pDataAudio[nCntSound] = (BYTE*)malloc(m_SizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_pDataAudio[nCntSound], m_SizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // ソースボイスの生成
        hr = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_SizeAudio[nCntSound];
        buffer.pAudioData = m_pDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = m_Param[nCntSound].nCntLoop;

        // オーディオバッファの登録
        m_pSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
    }

    return S_OK;
}

//*************************************************************************************************
// サウンド終了処理
//*************************************************************************************************
void CSound::Uninit(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if (m_pSourceVoice[nCntSound])
        {
            // 一時停止
            m_pSourceVoice[nCntSound]->Stop(0);

            // ソースボイスの破棄
            m_pSourceVoice[nCntSound]->DestroyVoice();
            m_pSourceVoice[nCntSound] = NULL;

            // オーディオデータの開放
            free(m_pDataAudio[nCntSound]);
            m_pDataAudio[nCntSound] = NULL;
        }
    }

    // マスターボイスの破棄
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if (m_pXAudio2)
    {
        // XAudio2オブジェクトの解放
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COMライブラリの終了処理
    CoUninitialize();
}

//*************************************************************************************************
// セグメント再生(再生中なら停止)
//*************************************************************************************************
HRESULT CSound::Play(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // バッファの値設定
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_SizeAudio[label];
    buffer.pAudioData = m_pDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = m_Param[label].nCntLoop;

    // 状態取得
    m_pSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_pSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_pSourceVoice[label]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    m_pSourceVoice[label]->SubmitSourceBuffer(&buffer);

    // 再生
    m_pSourceVoice[label]->Start(0);

    return S_OK;
}


//*************************************************************************************************
// セグメント停止(ラベル指定)
//*************************************************************************************************
void CSound::Stop(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    m_pSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_pSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_pSourceVoice[label]->FlushSourceBuffers();
    }
}

//*************************************************************************************************
// セグメント停止(全て)
//*************************************************************************************************
void CSound::StopAll(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if (m_pSourceVoice[nCntSound])
        {
            // 一時停止
            m_pSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//*************************************************************************************************
// チャンクのチェック
//*************************************************************************************************
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
    HRESULT hr = S_OK;
    DWORD dwRead;
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwBytesRead = 0;
    DWORD dwOffset = 0;

    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを先頭に移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクデータの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// ファイルタイプの読み込み
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// ファイルポインタをチャンクデータ分移動
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        dwOffset += sizeof(DWORD) * 2;
        if (dwChunkType == format)
        {
            *pChunkSize = dwChunkDataSize;
            *pChunkDataPosition = dwOffset;

            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        if (dwBytesRead >= dwRIFFDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

//*************************************************************************************************
// チャンクデータの読み込み
//*************************************************************************************************
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// データの読み込み
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

