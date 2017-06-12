#include "../include/Terrain.h"

void Terrain::NewHeightmap (UINT _size) {
    if (m_Heightmap) {
        UnloadData (m_Heightmap, m_HeightmapSize);
    }
    try {
        m_Heightmap = new UCHAR[_size * _size];
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    m_HeightmapSize = _size;
    memset (m_Heightmap, 128, sizeof (UCHAR) * _size * _size);
}

void Terrain::LoadHeightmap (const char* _heightmapFile) {
    strcpy (m_HeightmapFile, _heightmapFile);
    LoadData (_heightmapFile, m_Heightmap, m_HeightmapSize);
}

void Terrain::SaveHeightmap (const char* _heightmapFile) {
    strcpy (m_HeightmapFile, _heightmapFile);
    SaveData (_heightmapFile, m_Heightmap, m_HeightmapSize);
}

void Terrain::UnloadHeightmap () {
    m_HeightmapFile[0] = '\0';
    UnloadData (m_Heightmap, m_HeightmapSize);
    m_Scale[0] = m_Scale[1] = m_Scale[2] = 1.0f;
}

float Terrain::GetScaledHeight (UINT _x, UINT _z) const {
    if (_x >= m_HeightmapSize || _z >= m_HeightmapSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of range. (Terrain::GetScaledHeight)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Heightmap[_z * m_HeightmapSize + _x] * m_Scale[1];
}

UCHAR Terrain::GetHeight (UINT _x, UINT _z) const {
    if (_x >= m_HeightmapSize || _z >= m_HeightmapSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of range. (Terrain::GetHeight)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Heightmap[_z * m_HeightmapSize + _x];
}

void Terrain::SetHeight (UINT _x, UINT _z, UCHAR _height) {
    if (_x >= m_HeightmapSize || _z >= m_HeightmapSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of range. (Terrain::SetHeight)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Heightmap[_z * m_HeightmapSize + _x] = _height;
}

void Terrain::SetScale (float _ScaleX, float _ScaleY, float _ScaleZ) {
    if (_ScaleX <= 0.0f || _ScaleY <= 0.0f || _ScaleZ <= 0.0f) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
    m_Scale[0] = _ScaleX;
    m_Scale[1] = _ScaleY;
    m_Scale[2] = _ScaleZ;
}