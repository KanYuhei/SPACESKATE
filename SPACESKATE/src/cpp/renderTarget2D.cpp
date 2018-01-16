//*************************************************************************************************
// �����_�[�^�[�Q�b�g2D���� (renderTarget2D.cpp)
// Author : YUHEI KAN
//*************************************************************************************************

//*************************************************************************************************
// �C���N���[�h
//*************************************************************************************************
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "renderTarget2D.h"

//*************************************************************************************************
// �萔��`
//*************************************************************************************************

//*************************************************************************************************
// �񋓑�
//*************************************************************************************************

//*************************************************************************************************
// �\����
//*************************************************************************************************

//*************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************

//*************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************

//*************************************************************************************************
// �����_�[�^�[�Q�b�g2D�̃R���X�g���N�^
//*************************************************************************************************
CRenderTarget2D::CRenderTarget2D()
{
    m_RenderTargetTexture = NULL;
    m_RenderTargetTexture1 = NULL;
    m_RenderTargetTexture2 = NULL;
    m_RenderTargetSurface = NULL;
    m_RenderTargetSurface1 = NULL;
    m_RenderTargetSurface2 = NULL;
    m_BackBufferSurface = NULL;
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*************************************************************************************************
// �����_�[�^�[�Q�b�g2D�̃f�X�g���N�^
//*************************************************************************************************
CRenderTarget2D::~CRenderTarget2D()
{
}

//*************************************************************************************************
//����������
//*************************************************************************************************
HRESULT CRenderTarget2D::Init(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        // ���_�o�b�t�@�̐���
        if (FAILED(pDevice->CreateVertexBuffer(
            sizeof(VERTEX_2D) * NUM_VERTEX,     //�쐬���������_�o�b�t�@�̃T�C�Y
            D3DUSAGE_WRITEONLY,                 //VertexBuffer�̎g�������w��
            FVF_VERTEX_2D,                      //���_�t�H�[�}�b�g
            D3DPOOL_MANAGED,                    //�������Ǘ��i�N���Ǘ����邩�j
            &m_pVtxBuff,                        //���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̃A�h���X
            NULL)))
        {
            return E_FAIL;
        }
    }

    m_pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);
    m_size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    VERTEX_2D* pVtx;                            //���z�A�h���X�p�|�C���^

    //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�(2D���W)
    pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

    //rhw�̐ݒ�(�K��1.0f)
    pVtx[0].rhw =
    pVtx[1].rhw =
    pVtx[2].rhw =
    pVtx[3].rhw = 1.0f;

    //���_�J���[(0.0f�`1.0)
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //�e�N�X�`�����W�̐ݒ�
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    //���_�o�b�t�@�̃��b�N������
    m_pVtxBuff->Unlock();

    //�e�N�X�`������
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture);
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture1);
    D3DXCreateTexture(pDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture2);

    //�T�[�t�F�X�̎擾
    m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);
    m_RenderTargetTexture1->GetSurfaceLevel(0, &m_RenderTargetSurface1);
    m_RenderTargetTexture2->GetSurfaceLevel(0, &m_RenderTargetSurface2);

    //�����_�[�^�[�Q�b�g�擾
    pDevice->GetRenderTarget(0, &m_BackBufferSurface);

    m_FeedbackBlur = false;

    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CRenderTarget2D::Uninit(void)
{
    // �����_�[�^�[�Q�b�g�e�N�X�`��2�̔j��
    if (m_RenderTargetTexture2 != NULL)
    {
        m_RenderTargetTexture2->Release();
        m_RenderTargetTexture2 = NULL;
    }

    // �����_�[�^�[�Q�b�g�e�N�X�`��1�̔j��
    if (m_RenderTargetTexture1 != NULL)
    {
        m_RenderTargetTexture1->Release();
        m_RenderTargetTexture1 = NULL;
    }

    // �����_�[�^�[�Q�b�g�e�N�X�`���̔j��
    if (m_RenderTargetTexture != NULL)
    {
        m_RenderTargetTexture->Release();
        m_RenderTargetTexture = NULL;
    }

    // �����_�[�^�[�Q�b�g�T�[�t�F�X2�̔j��
    if (m_RenderTargetSurface2 != NULL)
    {
        m_RenderTargetSurface2->Release();
        m_RenderTargetSurface2 = NULL;
    }

    // �����_�[�^�[�Q�b�g�T�[�t�F�X1�̔j��
    if (m_RenderTargetSurface1 != NULL)
    {
        m_RenderTargetSurface1->Release();
        m_RenderTargetSurface1 = NULL;
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

    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CRenderTarget2D::Update(void)
{
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CRenderTarget2D::Draw(void)
{
    //Direct3D�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    //Direct3D�f�o�C�X��NULL�`�F�b�N
    if (pDevice != NULL)
    {
        //�t�B�[�h�o�b�N�u���[
        if (m_FeedbackBlur)
        {
            VERTEX_2D* pVtx;                            //���z�A�h���X�p�|�C���^

            //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
            m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

            //�S��ʂ�菭���傫��
            D3DXVECTOR3 size = D3DXVECTOR3(m_size.x * 1.01f, m_size.y * 1.01f, 0.0f);

            //���_���W�̐ݒ�(2D���W)
            pVtx[0].pos = D3DXVECTOR3(m_pos.x - size.x / 2, m_pos.y - size.y / 2, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(m_pos.x + size.x / 2, m_pos.y - size.y / 2, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(m_pos.x - size.x / 2, m_pos.y + size.y / 2, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(m_pos.x + size.x / 2, m_pos.y + size.y / 2, 0.0f);

            //���_�J���[(0.0f�`1.0)
            pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
            pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);

            //���_�o�b�t�@�̃��b�N������
            m_pVtxBuff->Unlock();

            //�����_�[�^�[�Q�b�g�̐ݒ�(�T�[�t�F�X2)
            pDevice->SetRenderTarget(0, m_RenderTargetSurface2);

            //�����_���N���X�̕`�揈���J�n
            CRenderer::DrawBegin();

            //���[�h�N���X�̕`�揈��
            CManager::GetMode()->Draw();

            //�V�[���N���X�̑S�Ă̕`�揈��
            CScene::DrawAll();

            // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // ���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_2D);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, m_RenderTargetTexture1);

            // �|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //�v���b�~�e�B�u�̎��
                0,                                        //�I�t�Z�b�g(���_��)
                NUM_POLYGON);                             //�v���~�e�B�u��

            //�`��̏I��
            pDevice->EndScene();


            //���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
            m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

            //���_���W�̐ݒ�(2D���W)
            pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

            //���_�J���[(0.0f�`1.0)
            pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            //���_�o�b�t�@�̃��b�N������
            m_pVtxBuff->Unlock();

            //�����_�[�^�[�Q�b�g�̐ݒ�(�o�b�N�o�b�t�@)
            pDevice->SetRenderTarget(0, m_BackBufferSurface);

            //�����_���N���X�̕`�揈���J�n
            CRenderer::DrawBegin();

            // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // ���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_2D);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, m_RenderTargetTexture2);

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); //�A���t�@�u�����h(OFF)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  //�A���t�@�e�X�g(OFF)

            // �|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //�v���b�~�e�B�u�̎��
                0,                                        //�I�t�Z�b�g(���_��)
                NUM_POLYGON);                             //�v���~�e�B�u��

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //�A���t�@�u�����h(ON)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);  //�A���t�@�e�X�g(ON)

            //�����_���N���X�̕`�揈���I��
            CRenderer::DrawEnd();

            //�e�N�X�`������ւ�
            LPDIRECT3DTEXTURE9 texture = m_RenderTargetTexture1;
            m_RenderTargetTexture1 = m_RenderTargetTexture2;
            m_RenderTargetTexture2 = texture;

            //�T�[�t�F�X����ւ�
            LPDIRECT3DSURFACE9 surface = m_RenderTargetSurface1;
            m_RenderTargetSurface1 = m_RenderTargetSurface2;
            m_RenderTargetSurface2 = surface;
        }
        else
        {
            //�����_�[�^�[�Q�b�g�̐ݒ�(�T�[�t�F�X)
            pDevice->SetRenderTarget(0, m_RenderTargetSurface);

            //�����_���N���X�̕`�揈���J�n
            CRenderer::DrawBegin();

            //���[�h�N���X�̕`�揈��
            CManager::GetMode()->Draw();

            //�V�[���N���X�̑S�Ă̕`�揈��
            CScene::DrawAll();

            //�`��̏I��
            pDevice->EndScene();

            //�����_�[�^�[�Q�b�g�̐ݒ�(�o�b�N�o�b�t�@)
            pDevice->SetRenderTarget(0, m_BackBufferSurface);

            //�����_���N���X�̕`�揈���J�n
            CRenderer::DrawBegin();

            // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

            // ���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_2D);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, m_RenderTargetTexture);

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); //�A���t�@�u�����h(OFF)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  //�A���t�@�e�X�g(OFF)

            // �|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,   //�v���b�~�e�B�u�̎��
                0,                                        //�I�t�Z�b�g(���_��)
                NUM_POLYGON);                             //�v���~�e�B�u��

            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //�A���t�@�u�����h(ON)
            pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);  //�A���t�@�e�X�g(ON)

            //�����_���N���X�̕`�揈���I��
            CRenderer::DrawEnd();
        }
    }
}

//*************************************************************************************************
// �t�B�[�h�o�b�N�u���[ON/OFF
//*************************************************************************************************
void CRenderTarget2D::IsFeedbackBlur(bool flag)
{
    m_FeedbackBlur = flag;
}

//*************************************************************************************************
// �t�B�[�h�o�b�N�u���[�t���O�̎擾
//*************************************************************************************************
bool CRenderTarget2D::GetFeedbackBlur(void)
{
    return m_FeedbackBlur;
}