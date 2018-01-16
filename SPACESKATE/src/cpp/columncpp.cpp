//*************************************************************************************************
// �~������ (column.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************
#include "manager.h"
#include "keyboard.h"
#include "mode.h"
#include "modeGame.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "track.h"
#include "player.h"
#include "column.h"

//*************************************************************************************************
// �}�N����` 
//*************************************************************************************************
#define MODELNAME        "data/MODEL/column.x"           //�ǂݍ��ރ��f���̖��O

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
// �~���̃R���X�g���N�^
//*************************************************************************************************
CColumn::CColumn(int nPriority) :CSceneModel(SCENE_TYPE_COLUMN, nPriority)
{
    m_pVtxBuff = NULL;
}

//*************************************************************************************************
// �~���̃f�X�g���N�^
//*************************************************************************************************
CColumn::~CColumn()
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CColumn::Init(void)
{
    //X�t�@�C���̓ǂݍ���
    CSceneModel::LoadX(MODELNAME);

    //�V�[�����f���N���X�̏���������
    CSceneModel::Init();

    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        // ���_�o�b�t�@�̐���
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_2D) * NUM_VERTEX,        //�쐬���������_�o�b�t�@�̃T�C�Y
            D3DUSAGE_WRITEONLY,                    //VertexBuffer�̎g�������w��
            FVF_VERTEX_2D,                         //���_�t�H�[�}�b�g
            D3DPOOL_MANAGED,                       //�������Ǘ��i�N���Ǘ����邩�j
            &m_pVtxBuff,                           //���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̃A�h���X
            NULL)))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CColumn::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    //�V�[�����f���N���X�̏I������
    CSceneModel::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CColumn::Update(void)
{
    //�V�[�����f���N���X�̍X�V����
    CSceneModel::Update();

    m_Pos = CModeGame::GetPlayer()->GetPos();

    // ���_����ݒ�
    VERTEX_2D* pVtx;                                //���z�A�h���X�p�|�C���^

    //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�(2D���W)
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //rhw�̐ݒ�(�K��1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //���_�J���[(0.0f�`1.0f�̐����l)
    pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
    pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

    //���_�o�b�t�@�̃��b�N������
    m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CColumn::Draw(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //�����_�[�X�e�[�g(�X�e���V��+1)
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);             //�X�e���V���o�b�t�@�@�L��
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);             //z�o�b�t�@�@����
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);             //�J���[�o�b�t�@�@����
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);      //
    pDevice->SetRenderState(D3DRS_STENCILREF, 1);                   //
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);  //�C���N�������g�Ōv�Z�@+1
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           //�\�ʂ�`��

    //�V�[�����f���N���X�̕`�揈��
    CSceneModel::Draw();

    //�����_�[�X�e�[�g(�X�e���V��-1)
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);  //�f�N�������g�Ōv�Z�@-1
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);            //���ʂ�`��

    //�V�[�����f���N���X�̕`�揈��(��)
    CSceneModel::Draw();

    //�����_�[�X�e�[�g
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           //�\�ʂ�`��
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);           //�J���[�o�b�t�@�@�L��
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);       //�X�e���V����1�݂̂�`��

                                                                    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
        pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

        // ���_�t�H�[�}�b�g�̐ݒ�
        pDevice->SetFVF(FVF_VERTEX_2D);

        // �|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //�v���b�~�e�B�u�̎��
            0,                                        //�I�t�Z�b�g(���_��)
            NUM_POLYGON);                             //�v���~�e�B�u��
    }

    //�����_�[�X�e�[�g(���ɖ߂�)
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              //z�o�b�t�@�@�L��
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);            //�X�e���V���o�b�t�@�@����
}

//*************************************************************************************************
// ���g�𐶐�
//*************************************************************************************************
CColumn *CColumn::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Scl, int nPriority)
{
    CColumn *pScenePlayer;                         //�V�[���QD�N���X�̃|�C���^

    //���I�m��
    pScenePlayer = new CColumn(nPriority);

    //�e�평����
    pScenePlayer->m_Pos = Pos;
    pScenePlayer->m_Scl = Scl;

    //����������
    pScenePlayer->Init();

    return pScenePlayer;
}

//*************************************************************************************************
// ��]�̎擾
//*************************************************************************************************
D3DXVECTOR3 CColumn::GetPos(void)
{
    return m_Pos;
}