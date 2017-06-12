#include "../include/Terrain.h"

void Terrain::SetSlopeLighting (int _lightDirX, int _lightDirZ,
                                UINT _minBrightness, UINT _maxBrightness,
                                float _softness) {
    m_LightDirectionX = _lightDirX;
    m_LightDirectionZ = _lightDirZ;
    m_MinBrightness = (float)_minBrightness / 255;
    m_MaxBrightness = (float)_maxBrightness / 255;
    m_LightSoftness = _softness;

    // check lightmap data
    if (m_LightmapSize != m_HeightmapSize) {
        delete[] m_Lightmap;
        m_Lightmap = NULL;
        m_LightmapSize = 0;
    }
    if (!m_Lightmap) {
        try {
            m_Lightmap = new UCHAR[m_HeightmapSize * m_HeightmapSize];
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
        m_LightmapSize = m_HeightmapSize;
    }
    m_LightMode = TLM_SLOPE_LIGHT;
}