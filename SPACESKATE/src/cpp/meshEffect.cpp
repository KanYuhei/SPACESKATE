//*************************************************************************************************
// ���b�V���G�t�F�N�g���� (meshEffect.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************
#include "mode.h"
#include "modeGame.h"
#include "scene.h"
#include "scene3D.h"
#include "meshEffect.h"
#include "sceneModel.h"
#include "player.h"

//*************************************************************************************************
// �}�N����` 
//*************************************************************************************************
#define TEXTURENAME00 "data/TEXTURE/ground2.jpg"    //�ǂݍ��ރe�N�X�`���̖��O

//*************************************************************************************************
// �\����
//*************************************************************************************************

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
// ���b�V���G�t�F�N�g�̃R���X�g���N�^
//*************************************************************************************************
CMeshEffect::CMeshEffect(SCENE_TYPE type, int nPriolity) :CScene3D(SCENE_TYPE_MESHEFFECT, nPriolity)
{
}

//*************************************************************************************************
// ���b�V���G�t�F�N�g�̃f�X�g���N�^
//*************************************************************************************************
CMeshEffect::~CMeshEffect()
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CMeshEffect::Init(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        //���_�o�b�t�@�̍쐬
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_3D) * MESHEFFECT_POS_NUM, //���_�o�b�t�@�̃T�C�Y
            D3DUSAGE_WRITEONLY,                     //VertexBuffer�̎g�������w��
            FVF_VERTEX_3D,                          //���_�t�H�[�}�b�g
            D3DPOOL_MANAGED,                        //�������Ǘ����@
            &m_pVtxBuffer,                          //���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̃A�h���X
            NULL)))
        {
            return E_FAIL;
        }

        //�C���f�b�N�X�o�b�t�@�̍쐬
        if (FAILED(pDevice->CreateIndexBuffer(
            sizeof(WORD) * MESHEFFECT_POS_NUM,      //�C���f�b�N�X�o�b�t�@�̃T�C�Y
            D3DUSAGE_WRITEONLY,                     //�g�p�t���O
            D3DFMT_INDEX16,                         //�C���f�b�N�X�f�[�^�t�H�[�}�b�g
            D3DPOOL_MANAGED,                        //�������̊Ǘ����@
            &m_pIdxBuffer,                          //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�|�C���^�̃A�h���X
            NULL)))
        {
            return E_FAIL;
        }

        // �e�N�X�`���̐���
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURENAME00, &m_pTexture)))
        {
            MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
        }
    }

    m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f, 1.0f);
    m_Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMeshEffect::Uninit(void)
{
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CMeshEffect::Update(void)
{
    //���_�̔����J��Ԃ�
    for (int i = 0; i < MESHEFFECT_POS_NUM/2; i++)
    {
        m_Pos[i * 2] = m_Pos[i * 2 + 2];
        m_Pos[i * 2 + 1] = m_Pos[i * 2 + 3];
    }

    //�v���C���[�̏���W�Ɖ����W�̎擾
    m_Pos[MESHEFFECT_POS_NUM - 2] = CModeGame::GetPlayer()->GetPosUp();
    m_Pos[MESHEFFECT_POS_NUM - 1] = CModeGame::GetPlayer()->GetPosDown();

    //���_����ݒ�
    VERTEX_3D* pVtx;                            //���z�A�h���X�p�|�C���^

    //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
    m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

    for (int i = 0; i < MESHEFFECT_POS_NUM/2; i++)
    {
        //���_���W�̐ݒ�(3D���W)
        pVtx[i * 2].pos = m_Pos[i * 2];
        pVtx[i * 2 + 1].pos = m_Pos[i * 2 + 1];

        //���C�g�x�N�g���̐ݒ�
        pVtx[i * 2].normal = m_Normal;
        pVtx[i * 2 + 1].normal = m_Normal;

        //���_�J���[(0.0f�`1.0f�̐����l)
        pVtx[i * 2].color = m_Color;
        pVtx[i * 2 + 1].color = m_Color;

        //�e�N�X�`�����W�̐ݒ�
        pVtx[i * 2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
        pVtx[i * 2 + 1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    }

    //���_�o�b�t�@�̃��b�N������
    m_pVtxBuffer->Unlock();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMeshEffect::Draw(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
        pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

        // ���_�t�H�[�}�b�g�̐ݒ�
        pDevice->SetFVF(FVF_VERTEX_3D);

        //�f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̐ݒ�
        pDevice->SetIndices(m_pIdxBuffer);

        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, m_pTexture);

        //�v���~�e�B�u�̕`��
        pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,    //�v���b�~�e�B�u�̎��
            0,                                                //�I�t�Z�b�g(�C���f�b�N�X)
            0,                                                //�ŏ��̒��_�C���f�b�N�X
            MESHEFFECT_POS_NUM,                               //���_��
            0,                                                //�X�^�[�g�C���f�b�N�X
            MESHEFFECT_POS_NUM);                              //�v���~�e�B�u��
    }
}

//***************************************************************************
// ���g�𐶐�
//***************************************************************************
CMeshEffect *CMeshEffect::Create(void)
{
    CMeshEffect *pMeshEffect;                         //���b�V���G�t�F�N�g�N���X�̃|�C���^

    //���I�m��
    pMeshEffect = new CMeshEffect(SCENE_TYPE_MESHEFFECT);

    //����������
    pMeshEffect->Init();

    return pMeshEffect;
}