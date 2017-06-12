#include "../include/Terrain.h"

void Terrain::LoadTextureTile (UINT _level, const char* _filename) {
    if (_level >= MAX_TILE_NUM) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (m_Tile[_level].IsLoaded ()) {
        UnloadTextureTile (_level);
    }
    m_Tile[_level].Load (_filename);
    if (m_Tile[_level].IsError ()) {
        THROW_ERROR (ERRC_BAD_FILE);
    }
    m_NumTiles++;
    m_IsUpdated = false;
}

void Terrain::UnloadTextureTile (UINT _level) {
    if (_level >= MAX_TILE_NUM) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Tile[_level].Unload ();
    m_NumTiles--;
    m_IsUpdated = false;
}

void Terrain::UnloadAllTextureTiles () {
    for (UINT i = 0; i < MAX_TILE_NUM; i++) {
        m_Tile[i].Unload ();
    }
    m_NumTiles = 0;
    m_IsUpdated = false;
}

void Terrain::GenerateTextureMap (const char* _filename, UINT _size) {
    UCHAR red, green, blue;
    UINT texX, texZ;
    float totalRed, totalGreen, totalBlue;
    float blend[4];
    float mapRatio;
    if (!m_IsUpdated) {
        UpdateRegions ();
    }
    TgaImage texture;
    texture.CreateImage (_size, _size, 24);
    
    mapRatio = (float) m_HeightmapSize / _size;

    for (UINT i = 0; i < _size; i++) {
        for (UINT j = 0; j < _size; j++) {
            totalRed = totalGreen = totalBlue = 0.0f;
            for (UINT k = 0; k < MAX_TILE_NUM; k++) {
                if (!m_Tile[k].IsLoaded ()) {
                    continue;
                }
                texX = j;
                texZ = i;
                GetTexCoords (m_Tile[k], texX, texZ);
                m_Tile[k].GetPixelColor (texX, texZ, red, green, blue);
                blend[k] = GetTexPresence (InterpolateHeight (j, i, mapRatio), k);
                
                totalRed += red * blend[k];
                totalGreen += green * blend[k];
                totalBlue += blue * blend[k];

            }
            texture.SetPixelColor (j, i, (UCHAR)totalRed, (UCHAR)totalGreen, (UCHAR)totalBlue);
        }
    }
    texture.SaveImage (_filename);
}


float Terrain::GetTexPresence (UCHAR _height, UINT _level) {
    if (_level >= MAX_TILE_NUM) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    for (UINT i = 0; i < MAX_TILE_NUM; i++) {
        if (m_Tile[i].IsLoaded ()) {
            if (i == _level) {
                if (_height < m_Region[i].Optimal) {
                    return 1.0f;
                }
            }
            break;
        }
    }
    if (_height < m_Region[_level].Lowest) {
        return 0.0f;
    }
    if (_height > m_Region[_level].Highest) {
        return 0.0f;
    }
    if (_height < m_Region[_level].Optimal) {
        float temp1 = (float)_height - m_Region[_level].Lowest;
        float temp2 = (float)m_Region[_level].Optimal - m_Region[_level].Lowest;
        return temp1 / temp2;
    }
    if (_height > m_Region[_level].Optimal) {
        float temp1 = (float)m_Region[_level].Highest - m_Region[_level].Optimal;
        return (temp1 - (_height - m_Region[_level].Optimal)) / temp1;
    }
    return 1.0f;    // height is equal to optimal
}

void Terrain::GetTexCoords (const TgaImage& _texture, UINT& _x, UINT& _y) {
    UINT width = _texture.GetWidth ();
    UINT height = _texture.GetHeight ();
    int repeatX = -1;
    int repeatY = -1;
    int i = 0;
    while (repeatX == -1) {
        i++;
        if (_x < width * i) {
            repeatX = i - 1;
        }
    }
    i = 0;
    while (repeatY == -1) {
        i++;
        if (_y < height * i) {
            repeatY = i - 1;
        }
    }
    _x = _x - width * repeatX;
    _y = _y - height * repeatY;
}

UCHAR Terrain::InterpolateHeight (UINT _x, UINT _z, float _ratio) {
    UCHAR low, highX, highZ;
    float scaledX = _x * _ratio;
    float scaledZ = _z * _ratio;
    float interpolation;

    low = (UCHAR) GetHeight ((UINT)scaledX, (UINT) scaledZ);

    if ((UINT)scaledX + 1 >= m_HeightmapSize) {
        return low;
    } else {
        highX = (UCHAR) GetHeight ((UINT) scaledX + 1, (UINT) scaledZ);
    }

    interpolation = scaledX - (UINT)scaledX;
    float x = (highX - low) * interpolation + low;

    if ((UINT)scaledZ + 1 >= m_HeightmapSize) {
        return low;
    } else {
        highZ = (UCHAR) GetHeight ((UINT) scaledX, (UINT)scaledZ + 1);
    }

    interpolation = scaledZ - (UINT) scaledZ;
    float z = (highZ - low) * interpolation + low;

    return (UCHAR) ((x + z) / 2);
}

void Terrain::UpdateRegions () {
    UCHAR lastHeight = 0;
    for (UINT i = 0; i < MAX_TILE_NUM; i++) {
        if (m_Tile[i].IsLoaded ()) {
            if (lastHeight == 0) {
                m_Region[i].Lowest = lastHeight;
            } else {
                m_Region[i].Lowest = lastHeight + 1;
            }
            lastHeight += 255 / m_NumTiles;
            m_Region[i].Optimal = lastHeight;
            m_Region[i].Highest = (lastHeight - m_Region[i].Lowest) + lastHeight;
        }
    }
    m_IsUpdated = true;
}