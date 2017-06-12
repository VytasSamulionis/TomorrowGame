#include "../include/TerrainWater.h"

using namespace vs3d;

TerrainWater::TerrainWater (RenderDevice* _device, UINT _skinId):
    m_Device (_device),
    m_SkinId (_skinId),
    m_Transparency (180),
    m_U (0.0f),
    m_V (0.0f),
    m_Height (0.0f) {

    m_Color[0] = 255;
    m_Color[1] = 255;
    m_Color[2] = 255;
}

TerrainWater::~TerrainWater () {
}

void TerrainWater::Render (float _worldSize) {
    m_Device->EnableAlphaBlend ();
    m_Device->SetAlphaBlendState (AS_SRCBLEND, BLEND_SRCALPHA);
    m_Device->SetAlphaBlendState (AS_DESTBLEND, BLEND_INVSRCALPHA);
    m_Device->SetTextureStageState (0, TSS_ALPHAARG1, TA_DIFFUSE);
    m_Device->SetTextureStageState (0, TSS_ALPHAOP, TOP_SELECTARG1);
    for (UINT i = 0; i < 4; i++ ) {
        m_Water[i].Color = (((m_Transparency)&0xff)<<24)|(((m_Color[0])&0xff)<<16)|(((m_Color[1])&0xff)<<8)|((m_Color[2])&0xff);
    }
    float repeat = _worldSize / 64.0f;
    m_Water[0].X = 0.0f;
    m_Water[0].Y = m_Height;
    m_Water[0].Z = 0.0f;
    m_Water[0].Tu = m_U;
    m_Water[0].Tv = m_V;

    m_Water[1].X = 0.0f;
    m_Water[1].Y = m_Height;
    m_Water[1].Z = _worldSize;
    m_Water[1].Tu = m_U;
    m_Water[1].Tv = m_V + repeat;

    m_Water[2].X = _worldSize;
    m_Water[2].Y = m_Height;
    m_Water[2].Z = 0.0f;
    m_Water[2].Tu = m_U + repeat;
    m_Water[2].Tv = m_V;

    m_Water[3].X = _worldSize;
    m_Water[3].Y = m_Height;
    m_Water[3].Z = _worldSize;
    m_Water[3].Tu = m_U + repeat;
    m_Water[3].Tv = m_V + repeat;

    m_Device->GetVCacheManager()->Render(PT_TRIANGLESTRIP, m_Water, 4, (WORD*)0, 0, VFT_UL, m_SkinId);

    m_V += 0.01f;
    if (m_V > 30000.0f) {
        m_V = 0.0f;
    }
    m_Device->DisableAlphaBlend ();
}