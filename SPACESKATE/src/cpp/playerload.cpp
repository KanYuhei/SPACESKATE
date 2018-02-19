//*************************************************************************************************
// プレイヤー処理 (player.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// インクルードファイル
//*************************************************************************************************
#include "renderer.h"
#include "playerload.h"
#include "parts.h"
#include "motion.h"
#include <fstream>

//*************************************************************************************************
// マクロ定義 
//*************************************************************************************************

//*************************************************************************************************
// 構造体
//*************************************************************************************************

//*************************************************************************************************
// 名前空間
//*************************************************************************************************
using namespace std;

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
// 読み込み処理
//*************************************************************************************************
HRESULT CPlayerLoad::Load(int i)
{
    //Direct3Dデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // ファイルの読み込み
    string file;
    if (i == 1) file = "data/TEXT/motion.txt";
    else if (i == 2) file = "data/TEXT/motion2.txt";
    ifstream ifs(file);
    if (ifs.fail())
    {
        MessageBox(NULL, "プレイヤーモーションテキストの読み込みに失敗しました", "終了メッセージ", MB_OK);
        return E_FAIL;
    }

    //ファイルから文字列を読み込み
    string str;                             //作業用文字列
    string::size_type commentStart = 0;     //コメント開始位置
    vector<std::string> loadStr;            //読み込んだ文字列
    while (getline(ifs, str))
    {
        // '#'以降はコメントとして無視
        if ((commentStart = str.find('#')) != std::string::size_type(-1))
            str = str.substr(0, commentStart);

        // 行頭のスペース，タブを削除
        DeleteHeadSpace(str);

        // 空行は無視
        if (str.empty()) continue;

        loadStr.push_back(str);
    }
    ifs.close();

    //ファイル文字列解析
    vector<string> filname;                 //読み込むモデル名のリスト
    int ModelNum = 0;                       //モデル数
    vector<CParts> Parts;                   //パーツ情報
    int NumParts = 0;                       //パーツ数
    vector<int> Index;                      //インデックス
    vector<int> Parent;                     //親子
    vector<D3DXVECTOR3> position;           //座標
    vector<D3DXVECTOR3> rotation;           //回転
    vector<int> Loop;                       //ループするかどうか[0:しない / 1:する]
    vector<int> NumKey;                     //キー数
    vector<CKeyFrame> keyFrame;             //キーフレームクラス
    int MotionNum = 0;                      //モーションの数
    for (vector<string>::const_iterator it = loadStr.begin(); it != loadStr.end(); it++)
    {
        //NUM_MODELが見つかった場合
        if (it->find("NUM_MODEL") != -1) SetValue(it->c_str(), &ModelNum);

        //MODEL_FILENAMEが見つかった場合
        if (it->find("MODEL_FILENAME") != -1)
        {
            int start = it->find("data");
            int end = it->length();
            filname.push_back(it->substr(start, end - start));
        }

        //キャラクター情報///////////////////////////////////////////
        //NUM_PARTSが見つかった場合
        if (it->find("NUM_PARTS") != -1) SetValue(it->c_str(), &NumParts);

        //INDEXが見つかった場合
        if (it->find("INDEX") != -1)
        {
            int index;
            SetValue(it->c_str(), &index);
            Index.push_back(index);
        }

        //PARENTが見つかった場合
        if (it->find("PARENT") != -1)
        {
            int parent;
            SetValue(it->c_str(), &parent);
            Parent.push_back(parent);
        }

        //POSが見つかった場合
        if (it->find("POS") != -1)
        {
            D3DXVECTOR3 Pos;
            SetXYZ(it->c_str(), &Pos);
            position.push_back(Pos);
        }

        //ROTが見つかった場合
        if (it->find("ROT") != -1)
        {
            D3DXVECTOR3 Rot;
            SetXYZ(it->c_str(), &Rot);
            rotation.push_back(Rot);
        }

        //モーション情報/////////////////////////////////////////////
        //LOOPが見つかった場合
        if (it->find("LOOP") != -1)
        {
            int loop;
            SetValue(it->c_str(), &loop);
            Loop.push_back(loop);
            MotionNum++;
        }

        //NUM_KEYが見つかった場合
        if (it->find("NUM_KEY") != -1)
        {
            int num_key;
            SetValue(it->c_str(), &num_key);
            NumKey.push_back(num_key);
        }

        //KEYSETが見つかった場合
        if (it->find("KEYSET") != -1 && it->find("END_KEYSET") == -1)
        {
            CKeyFrame keyframe;
            SetKeyFrame(it, &keyframe, NumParts);
            keyFrame.push_back(keyframe);
        }
    }

    // パーツの作成
    for (int nCntPart = 0; nCntPart < NumParts; nCntPart++)
    {
        CParts *part = nullptr;
        if (Parent[nCntPart] == -1)     //(親)NULL
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], NULL);
        else                            //(子)
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], m_Part[Parent[nCntPart]]);
        m_Part.push_back(part);
    }

    // モーションの作成
    int num = 0;
    for (int nCntMotion = 0; nCntMotion < MotionNum; nCntMotion++)
    {
        vector<CKeyFrame> keyframe;
        for (int i = 0; i < NumKey[nCntMotion]; i++)
        {
            keyframe.push_back(keyFrame[num]);
            num++;
        }
        CMotion *motion = CMotion::Create(keyframe, NumKey[nCntMotion], Loop[nCntMotion]);
        m_Motion.push_back(motion);
    }

    return S_OK;
}

//*************************************************************************************************
// 解放処理
//*************************************************************************************************
void CPlayerLoad::Unload(void)
{
}

//*************************************************************************************************
// パーツ情報の取得
//*************************************************************************************************
std::vector<CParts*> CPlayerLoad::GetParts(void)
{
    return m_Part;
}

//*************************************************************************************************
// モーション情報の取得
//*************************************************************************************************
std::vector<CMotion*> CPlayerLoad::GetMotions(void)
{
    return m_Motion;
}

//*************************************************************************************************
// 先頭の空白(スペース，タブ)を削除
//*************************************************************************************************
void CPlayerLoad::DeleteHeadSpace(std::string &buf)
{
    size_t pos;
    while ((pos = buf.find_first_of(" 　\t")) == 0) {
        buf.erase(buf.begin());
        if (buf.empty()) break;
    }
}

//*************************************************************************************************
// 数値をにvalueセットする
//*************************************************************************************************
void CPlayerLoad::SetValue(std::string str, int *value)
{
    int start = str.find_first_of("-0123456789");
    int end = str.length();
    *value = atoi(str.substr(start, end - start).c_str());
}

//*************************************************************************************************
// XYZ座標をD3DXVECTOR3にセットする
//*************************************************************************************************
void CPlayerLoad::SetXYZ(std::string str, D3DXVECTOR3 *pos)
{
    int start = 0;
    int end = 0;
    float XYZ[3];
    for (int i = 0; i < 3; i++)
    {
        start = str.find_first_of("-0123456789", end);
        end = str.find_first_of(" ", start);
        XYZ[i] = atof(str.substr(start, end - start).c_str());
    }
    *pos = D3DXVECTOR3(XYZ[0], XYZ[1], XYZ[2]);
}

//*************************************************************************************************
// KEYをCKeyにセットする
//*************************************************************************************************
void CPlayerLoad::SetKey(std:: vector<std::string>::const_iterator it, CKey *key)
{
    //座標
    it++;
    D3DXVECTOR3 Pos;
    SetXYZ(it->c_str(), &Pos);
    key->SetPos(Pos);

    //回転
    it++;
    D3DXVECTOR3 Rot;
    SetXYZ(it->c_str(), &Rot);
    key->SetRot(Rot);
}

//*************************************************************************************************
// KEYFRAMEをCKeyFrameにセットする
//*************************************************************************************************
void CPlayerLoad::SetKeyFrame(std::vector<std::string>::const_iterator it, CKeyFrame *keyframe, int numPart)
{
    it++;

    //FRAME
    int frame;
    SetValue(it->c_str(), &frame);
    keyframe->SetFrame(frame);
    it++;

    //KEY
    for (int i = 0; i < numPart; i++)
    {
        CKey key;
        SetKey(it, &key);
        keyframe->SetKey(key);
        it += 4;            //次のKEY文字位置へ
    }
}