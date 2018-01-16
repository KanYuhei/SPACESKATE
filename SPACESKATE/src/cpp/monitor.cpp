//*************************************************************************************************
// ���j�^�[���� (monitor.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************
#include "manager.h"
#include "mode.h"
#include "scene.h"
#include "scene3D.h"
#include "monitor.h"

//*************************************************************************************************
// �}�N����` 
//*************************************************************************************************

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
// ���j�^�[�̃R���X�g���N�^
//*************************************************************************************************
CMonitor::CMonitor(int nPriority) :CScene3D(SCENE_TYPE_MONITOR, nPriority)
{
    m_RenderTargetTexture = NULL;
    m_RenderTargetSurface = NULL;
    m_BackBufferSurface = NULL;
}

//*************************************************************************************************
// ���j�^�[�̃f�X�g���N�^
//*************************************************************************************************
CMonitor::~CMonitor()
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CMonitor::Init(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        //���_�o�b�t�@�̍쐬
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_3D) * NUM_VERTEX,         //���_�o�b�t�@�̃T�C�Y
            D3DUSAGE_WRITEONLY,                     //VertexBuffer�̎g�������w��
            FVF_VERTEX_3D,                          //���_�t�H�[�}�b�g
            D3DPOOL_MANAGED,                        //�������Ǘ����@
            &m_pVtxBuffer,                          //���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̃A�h���X
            NULL)))
        {
            return E_FAIL;
        }

        //�e�N�X�`������
        D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture);

        //�T�[�t�F�X�̎擾
        m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);

        //�����_�[�^�[�Q�b�g�擾
        pDevice->GetRenderTarget(0, &m_BackBufferSurface);
    }

    m_Tex = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f, 1.0f);
    m_Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMonitor::Uninit(void)
{
    // �����_�[�^�[�Q�b�g�e�N�X�`���̔j��
    if (m_RenderTargetTexture != NULL)
    {
        m_RenderTargetTexture->Release();
        m_RenderTargetTexture = NULL;
    }

    // �����_�[�^�[�Q�b�g�T�[�t�F�X�̔j��
    if (m_RenderTargetSurface != NULL)
    {
        m_RenderTargetSurface->Release();
        m_RenderTargetSurface = NULL;
    }

    // �o�b�N�o�b�t�@�T�[�t�F�X�̔j��
    if (m_BackBufferSurface != NULL)
    {
        m_BackBufferSurface->Release();
        m_BackBufferSurface = NULL;
    }
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CMonitor::Update(void)
{
    //���_����ݒ�
    VERTEX_3D* pVtx;                            //���z�A�h���X�p�|�C���^

    //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
    m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�(3D���W)
    pVtx[0].pos = D3DXVECTOR3(m_Pos.x,            m_Pos.y + m_Size.y, m_Pos.z);
    pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, m_Pos.z);
    pVtx[2].pos = D3DXVECTOR3(m_Pos.x,            m_Pos.y,            m_Pos.z);
    pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y,            m_Pos.z);

    //���C�g�x�N�g���̐ݒ�
    pVtx[0].normal = m_Normal;
    pVtx[1].normal = m_Normal;
    pVtx[2].normal = m_Normal;
    pVtx[3].normal = m_Normal;

    //���_�J���[(0.0f�`1.0f�̐����l)
    pVtx[0].color = m_Color;
    pVtx[1].color = m_Color;
    pVtx[2].color = m_Color;
    pVtx[3].color = m_Color;

    //�e�N�X�`�����W�̐ݒ�
    pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
    pVtx[1].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y);
    pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + m_TexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_Tex.x + m_TexSize.x, m_Tex.y + m_TexSize.y);

    //���_�o�b�t�@�̃��b�N������
    m_pVtxBuffer->Unlock();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMonitor::Draw(void)
{
    ////Direct3D�f�o�C�X�̎擾
    //LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    ////Direct3D�f�o�C�X��NULL�`�F�b�N
    //if (pDevice != NULL && m_RenderTargetTexture != NULL)
    //{
    //    //�����_�[�^�[�Q�b�g�̐ݒ�(�T�[�t�F�X)
    //    pDevice->SetRenderTarget(0, m_RenderTargetSurface);

    //    //�����_���N���X�̕`�揈���J�n
    //    CRenderer::DrawBegin();

    //    //���[�h�N���X�̕`�揈��
    //    CManager::GetMode()->Draw();

    //    //�w�肵���D��x��S�ĕ`��
    //    CScene::DrawPriolityAll(3);

    //    //�`��̏I��
    //    pDevice->EndScene();

    //    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    //    pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

    //    // ���_�t�H�[�}�b�g�̐ݒ�
    //    pDevice->SetFVF(FVF_VERTEX_3D);

    //    // �e�N�X�`���̐ݒ�
    //    pDevice->SetTexture(0, m_RenderTargetTexture);

    //    // �|���S���̕`��
    //    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //�v���b�~�e�B�u�̎��
    //        0,                                        //�I�t�Z�b�g(���_��)
    //        NUM_POLYGON);                             //�v���~�e�B�u��
    //}
}

//*************************************************************************************************
// ���g�𐶐�
//*************************************************************************************************
CMonitor *CMonitor::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size, D3DXCOLOR Color, int nPriority)
{
    CMonitor *pMeshField;                     //���j�^�[�N���X�̃|�C���^

    //���I�m��
    pMeshField = new CMonitor(nPriority);

    //�e�평����
    pMeshField->m_Pos = Pos;
    pMeshField->m_Size = Size;
    pMeshField->m_Color = Color;

    //����������
    pMeshField->Init();

    return pMeshField;
}