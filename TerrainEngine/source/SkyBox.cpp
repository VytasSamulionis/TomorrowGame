#include "../include/SkyBox.h"

using namespace vs3d;

SkyBox::SkyBox (RenderDevice* _device):
    m_Device (_device) {
}

SkyBox::~SkyBox () {
}

void SkyBox::InitTopSide (float _size, UINT _skinId) {
    ULVERTEX topSide[4];
    float halfSize = _size / 2.0f;
    topSide[0].Color = 0xffffffff;
    topSide[0].X = -halfSize;
    topSide[0].Y = halfSize;
    topSide[0].Z = halfSize;
    topSide[0].Tu = 0.005f;
    topSide[0].Tv = 0.995f;

    topSide[1].Color = 0xffffffff;
    topSide[1].X = -halfSize;
    topSide[1].Y = halfSize;
    topSide[1].Z = -halfSize;
    topSide[1].Tu = 0.005f;
    topSide[1].Tv = 0.005f;

    topSide[2].Color = 0xffffffff;
    topSide[2].X = halfSize;
    topSide[2].Y = halfSize;
    topSide[2].Z = halfSize;
    topSide[2].Tu = 0.995f;
    topSide[2].Tv = 0.995f;

    topSide[3].Color = 0xffffffff;
    topSide[3].X = halfSize;
    topSide[3].Y = halfSize;
    topSide[3].Z = -halfSize;
    topSide[3].Tu = 0.995f;
    topSide[3].Tv = 0.005f;

    m_VertexBufferId = m_Device->GetVCacheManager()->CreateStaticVertexBuffer(topSide, 4, VFT_UL);
    m_SkinId[0] = _skinId;
}

void SkyBox::InitBottomSide (float _size, UINT _skinId) {
    ULVERTEX bottomSide[4];
    float halfSize = _size / 2.0f;

    bottomSide[0].Color = 0xffffffff;
    bottomSide[0].X = -halfSize;
    bottomSide[0].Y = -halfSize;
    bottomSide[0].Z = -halfSize;
    bottomSide[0].Tu = 0.005f;
    bottomSide[0].Tv = 0.995f;

    bottomSide[1].Color = 0xffffffff;
    bottomSide[1].X = -halfSize;
    bottomSide[1].Y = -halfSize;
    bottomSide[1].Z = halfSize;
    bottomSide[1].Tu = 0.005f;
    bottomSide[1].Tv = 0.005f;

    bottomSide[2].Color = 0xffffffff;
    bottomSide[2].X = halfSize;
    bottomSide[2].Y = -halfSize;
    bottomSide[2].Z = -halfSize;
    bottomSide[2].Tu = 0.995f;
    bottomSide[2].Tv = 0.995f;

    bottomSide[3].Color = 0xffffffff;
    bottomSide[3].X = halfSize;
    bottomSide[3].Y = -halfSize;
    bottomSide[3].Z = halfSize;
    bottomSide[3].Tu = 0.995f;
    bottomSide[3].Tv = 0.005f;

    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_VertexBufferId, bottomSide, 4, VFT_UL);
    m_SkinId[1] = _skinId;
}

void SkyBox::InitLeftSide (float _size, UINT _skinId) {
    ULVERTEX leftSide[4];
    float halfSize = _size / 2.0f;

    leftSide[0].Color = 0xffffffff;
    leftSide[0].X = -halfSize;
    leftSide[0].Y = halfSize;
    leftSide[0].Z = halfSize;
    leftSide[0].Tu = 0.995f;
    leftSide[0].Tv = 0.005f;

    leftSide[1].Color = 0xffffffff;
    leftSide[1].X = -halfSize;
    leftSide[1].Y = -halfSize;
    leftSide[1].Z = halfSize;
    leftSide[1].Tu = 0.995f;
    leftSide[1].Tv = 0.995f;

    leftSide[2].Color = 0xffffffff;
    leftSide[2].X = -halfSize;
    leftSide[2].Y = halfSize;
    leftSide[2].Z = -halfSize;
    leftSide[2].Tu = 0.005f;
    leftSide[2].Tv = 0.005f;

    leftSide[3].Color = 0xffffffff;
    leftSide[3].X = -halfSize;
    leftSide[3].Y = -halfSize;
    leftSide[3].Z = -halfSize;
    leftSide[3].Tu = 0.005f;
    leftSide[3].Tv = 0.995f;

    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_VertexBufferId, leftSide, 4, VFT_UL);
    m_SkinId[2] = _skinId;
}

void SkyBox::InitRightSide (float _size, UINT _skinId) {
    ULVERTEX rightSide[4];
    float halfSize = _size / 2.0f;
    rightSide[0].Color = 0xffffffff;
    rightSide[0].X = halfSize;
    rightSide[0].Y = -halfSize;
    rightSide[0].Z = halfSize;
    rightSide[0].Tu = 0.005f;
    rightSide[0].Tv = 0.995f;

    rightSide[1].Color = 0xffffffff;
    rightSide[1].X = halfSize;
    rightSide[1].Y = halfSize;
    rightSide[1].Z = halfSize;
    rightSide[1].Tu = 0.005f;
    rightSide[1].Tv = 0.005f;

    rightSide[2].Color = 0xffffffff;
    rightSide[2].X = halfSize;
    rightSide[2].Y = -halfSize;
    rightSide[2].Z = -halfSize;
    rightSide[2].Tu = 0.995f;
    rightSide[2].Tv = 0.995f;

    rightSide[3].Color = 0xffffffff;
    rightSide[3].X = halfSize;
    rightSide[3].Y = halfSize;
    rightSide[3].Z = -halfSize;
    rightSide[3].Tu = 0.995f;
    rightSide[3].Tv = 0.005f;

    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_VertexBufferId, rightSide, 4, VFT_UL);
    m_SkinId[3] = _skinId;
}

void SkyBox::InitFarSide (float _size, UINT _skinId) {
    ULVERTEX farSide[4];
    float halfSize = _size / 2.0f;

    farSide[0].Color = 0xffffffff;
    farSide[0].X = -halfSize;
    farSide[0].Y = +halfSize;
    farSide[0].Z = -halfSize;
    farSide[0].Tu = 0.995f;
    farSide[0].Tv = 0.005f;

    farSide[1].Color = 0xffffffff;
    farSide[1].X = -halfSize;
    farSide[1].Y = -halfSize;
    farSide[1].Z = -halfSize;
    farSide[1].Tu = 0.995f;
    farSide[1].Tv = 0.995f;

    farSide[2].Color = 0xffffffff;
    farSide[2].X = halfSize;
    farSide[2].Y = halfSize;
    farSide[2].Z = -halfSize;
    farSide[2].Tu = 0.005f;
    farSide[2].Tv = 0.005f;

    farSide[3].Color = 0xffffffff;
    farSide[3].X = halfSize;
    farSide[3].Y = -halfSize;
    farSide[3].Z = -halfSize;
    farSide[3].Tu = 0.005f;
    farSide[3].Tv = 0.995f;

    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_VertexBufferId, farSide, 4, VFT_UL);
    m_SkinId[4] = _skinId;
}

void SkyBox::InitNearSide (float _size, UINT _skinId) {
    ULVERTEX nearSide[4];
    float halfSize = _size / 2.0f;
    nearSide[0].Color = 0xffffffff;
    nearSide[0].X = -halfSize;
    nearSide[0].Y = -halfSize;
    nearSide[0].Z = halfSize;
    nearSide[0].Tu = 0.005f;
    nearSide[0].Tv = 0.995f;

    nearSide[1].Color = 0xffffffff;
    nearSide[1].X = -halfSize;
    nearSide[1].Y = halfSize;
    nearSide[1].Z = halfSize;
    nearSide[1].Tu = 0.005f;
    nearSide[1].Tv = 0.005f;

    nearSide[2].Color = 0xffffffff;
    nearSide[2].X = halfSize;
    nearSide[2].Y = -halfSize;
    nearSide[2].Z = halfSize;
    nearSide[2].Tu = 0.995f;
    nearSide[2].Tv = 0.995f;

    nearSide[3].Color = 0xffffffff;
    nearSide[3].X = halfSize;
    nearSide[3].Y = halfSize;
    nearSide[3].Z = halfSize;
    nearSide[3].Tu = 0.995f;
    nearSide[3].Tv = 0.005f;

    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_VertexBufferId, nearSide, 4, VFT_UL);
    m_SkinId[5] = _skinId;
}

void SkyBox::Init (float _x, float _y, float _z, float _size,
                   UINT _topSkinId, UINT _bottomSkinId,
                   UINT _leftSkinId, UINT _rightSkinId,
                   UINT _farSkinId, UINT _nearSkinId) {
    m_Center[0] = _x;
    m_Center[1] = _y;
    m_Center[2] = _z;
    InitTopSide (_size, _topSkinId);
    InitBottomSide (_size, _bottomSkinId);
    InitLeftSide (_size, _leftSkinId);
    InitRightSide (_size, _rightSkinId);
    InitFarSide (_size, _farSkinId);
    InitNearSide (_size, _nearSkinId);
}

void SkyBox::Render () {
    MATRIX44 world = m_Device->GetWorldMatrix();
    m_Device->TranslateWorldMatrix (m_Center[0], m_Center[1], m_Center[2]);
    for (UINT i = 0; i < 6; i++) {
        m_Device->GetVCacheManager()->Render(PT_TRIANGLESTRIP, m_VertexBufferId, i * 4, (WORD*)0, 0, 2, VFT_UL, m_SkinId[i]);
    }
    m_Device->SetWorldMatrix(world);
}