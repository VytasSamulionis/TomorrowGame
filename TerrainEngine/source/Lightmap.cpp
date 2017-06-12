#include "../include/Terrain.h"

void Terrain::LoadLightmap (const char* _filename) {
    strcpy (m_LightmapFile, _filename);
    m_LightMode = TLM_HEIGHT_BASED;
    LoadData (_filename, m_Lightmap, m_LightmapSize);
    if (m_LightmapSize != m_HeightmapSize) {
        THROW_ERROR (ERRC_BAD_FILE);
    }
    m_LightMode = TLM_LIGHTMAP;
}

void Terrain::SaveLightmap (const char* _filename) {
    strcpy (m_LightmapFile, _filename);
    SaveData (_filename, m_Lightmap, m_LightmapSize);
}

void Terrain::UnloadLightmap () {
    m_LightmapFile[0] = '\0';
    UnloadData (m_Lightmap, m_LightmapSize);
    m_LightMode = TLM_HEIGHT_BASED;
}

UCHAR Terrain::GetBrightness (UINT _x, UINT _z) const {
    if (_x >= m_LightmapSize || _z >= m_LightmapSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of range. (Terrain::GetBrightness)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Lightmap[_z * m_LightmapSize + _x];
}

void Terrain::SetBrightness (UINT _x, UINT _z, UCHAR _value) {
    if (_x >= m_LightmapSize || _z >= m_LightmapSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of range. (Terrain::GetBrightness)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Lightmap[_z * m_LightmapSize + _x] = _value;
}

void Terrain::SetLightColor (UCHAR _red, UCHAR _green, UCHAR _blue) {
    m_LightColor[0] = _red;
    m_LightColor[1] = _green;
    m_LightColor[2] = _blue;
}