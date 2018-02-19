//*************************************************************************************************
// �v���C���[���� (player.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************
#include "renderer.h"
#include "playerload.h"
#include "parts.h"
#include "motion.h"
#include <fstream>

//*************************************************************************************************
// �}�N����` 
//*************************************************************************************************

//*************************************************************************************************
// �\����
//*************************************************************************************************

//*************************************************************************************************
// ���O���
//*************************************************************************************************
using namespace std;

//*************************************************************************************************
// �N���X
//*************************************************************************************************

//*************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************

//*************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************

//*************************************************************************************************
// �ǂݍ��ݏ���
//*************************************************************************************************
HRESULT CPlayerLoad::Load(int i)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // �t�@�C���̓ǂݍ���
    string file;
    if (i == 1) file = "data/TEXT/motion.txt";
    else if (i == 2) file = "data/TEXT/motion2.txt";
    ifstream ifs(file);
    if (ifs.fail())
    {
        MessageBox(NULL, "�v���C���[���[�V�����e�L�X�g�̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
        return E_FAIL;
    }

    //�t�@�C�����當�����ǂݍ���
    string str;                             //��Ɨp������
    string::size_type commentStart = 0;     //�R�����g�J�n�ʒu
    vector<std::string> loadStr;            //�ǂݍ��񂾕�����
    while (getline(ifs, str))
    {
        // '#'�ȍ~�̓R�����g�Ƃ��Ė���
        if ((commentStart = str.find('#')) != std::string::size_type(-1))
            str = str.substr(0, commentStart);

        // �s���̃X�y�[�X�C�^�u���폜
        DeleteHeadSpace(str);

        // ��s�͖���
        if (str.empty()) continue;

        loadStr.push_back(str);
    }
    ifs.close();

    //�t�@�C����������
    vector<string> filname;                 //�ǂݍ��ރ��f�����̃��X�g
    int ModelNum = 0;                       //���f����
    vector<CParts> Parts;                   //�p�[�c���
    int NumParts = 0;                       //�p�[�c��
    vector<int> Index;                      //�C���f�b�N�X
    vector<int> Parent;                     //�e�q
    vector<D3DXVECTOR3> position;           //���W
    vector<D3DXVECTOR3> rotation;           //��]
    vector<int> Loop;                       //���[�v���邩�ǂ���[0:���Ȃ� / 1:����]
    vector<int> NumKey;                     //�L�[��
    vector<CKeyFrame> keyFrame;             //�L�[�t���[���N���X
    int MotionNum = 0;                      //���[�V�����̐�
    for (vector<string>::const_iterator it = loadStr.begin(); it != loadStr.end(); it++)
    {
        //NUM_MODEL�����������ꍇ
        if (it->find("NUM_MODEL") != -1) SetValue(it->c_str(), &ModelNum);

        //MODEL_FILENAME�����������ꍇ
        if (it->find("MODEL_FILENAME") != -1)
        {
            int start = it->find("data");
            int end = it->length();
            filname.push_back(it->substr(start, end - start));
        }

        //�L�����N�^�[���///////////////////////////////////////////
        //NUM_PARTS�����������ꍇ
        if (it->find("NUM_PARTS") != -1) SetValue(it->c_str(), &NumParts);

        //INDEX�����������ꍇ
        if (it->find("INDEX") != -1)
        {
            int index;
            SetValue(it->c_str(), &index);
            Index.push_back(index);
        }

        //PARENT�����������ꍇ
        if (it->find("PARENT") != -1)
        {
            int parent;
            SetValue(it->c_str(), &parent);
            Parent.push_back(parent);
        }

        //POS�����������ꍇ
        if (it->find("POS") != -1)
        {
            D3DXVECTOR3 Pos;
            SetXYZ(it->c_str(), &Pos);
            position.push_back(Pos);
        }

        //ROT�����������ꍇ
        if (it->find("ROT") != -1)
        {
            D3DXVECTOR3 Rot;
            SetXYZ(it->c_str(), &Rot);
            rotation.push_back(Rot);
        }

        //���[�V�������/////////////////////////////////////////////
        //LOOP�����������ꍇ
        if (it->find("LOOP") != -1)
        {
            int loop;
            SetValue(it->c_str(), &loop);
            Loop.push_back(loop);
            MotionNum++;
        }

        //NUM_KEY�����������ꍇ
        if (it->find("NUM_KEY") != -1)
        {
            int num_key;
            SetValue(it->c_str(), &num_key);
            NumKey.push_back(num_key);
        }

        //KEYSET�����������ꍇ
        if (it->find("KEYSET") != -1 && it->find("END_KEYSET") == -1)
        {
            CKeyFrame keyframe;
            SetKeyFrame(it, &keyframe, NumParts);
            keyFrame.push_back(keyframe);
        }
    }

    // �p�[�c�̍쐬
    for (int nCntPart = 0; nCntPart < NumParts; nCntPart++)
    {
        CParts *part = nullptr;
        if (Parent[nCntPart] == -1)     //(�e)NULL
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], NULL);
        else                            //(�q)
            part = CParts::Create(filname[nCntPart].c_str(), position[nCntPart], rotation[nCntPart], m_Part[Parent[nCntPart]]);
        m_Part.push_back(part);
    }

    // ���[�V�����̍쐬
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
// �������
//*************************************************************************************************
void CPlayerLoad::Unload(void)
{
}

//*************************************************************************************************
// �p�[�c���̎擾
//*************************************************************************************************
std::vector<CParts*> CPlayerLoad::GetParts(void)
{
    return m_Part;
}

//*************************************************************************************************
// ���[�V�������̎擾
//*************************************************************************************************
std::vector<CMotion*> CPlayerLoad::GetMotions(void)
{
    return m_Motion;
}

//*************************************************************************************************
// �擪�̋�(�X�y�[�X�C�^�u)���폜
//*************************************************************************************************
void CPlayerLoad::DeleteHeadSpace(std::string &buf)
{
    size_t pos;
    while ((pos = buf.find_first_of(" �@\t")) == 0) {
        buf.erase(buf.begin());
        if (buf.empty()) break;
    }
}

//*************************************************************************************************
// ���l����value�Z�b�g����
//*************************************************************************************************
void CPlayerLoad::SetValue(std::string str, int *value)
{
    int start = str.find_first_of("-0123456789");
    int end = str.length();
    *value = atoi(str.substr(start, end - start).c_str());
}

//*************************************************************************************************
// XYZ���W��D3DXVECTOR3�ɃZ�b�g����
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
// KEY��CKey�ɃZ�b�g����
//*************************************************************************************************
void CPlayerLoad::SetKey(std:: vector<std::string>::const_iterator it, CKey *key)
{
    //���W
    it++;
    D3DXVECTOR3 Pos;
    SetXYZ(it->c_str(), &Pos);
    key->SetPos(Pos);

    //��]
    it++;
    D3DXVECTOR3 Rot;
    SetXYZ(it->c_str(), &Rot);
    key->SetRot(Rot);
}

//*************************************************************************************************
// KEYFRAME��CKeyFrame�ɃZ�b�g����
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
        it += 4;            //����KEY�����ʒu��
    }
}