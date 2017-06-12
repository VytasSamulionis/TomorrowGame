#include "../include/Terrain.h"

using namespace vs3d;

Terrain::Terrain (LogManager* _log, RenderDevice* _device) {
    m_Scale[0] = m_Scale[1] = m_Scale[2] = 1.0f;
    m_HeightmapSize = 0;
    m_Heightmap = NULL;
    m_Log = _log;
    m_Device = _device;
    m_NumTiles = 0;
    m_Lightmap = NULL;
    m_LightmapSize = 0;
    m_LightMode = TLM_HEIGHT_BASED;
    m_LightColor[0] = 255;
    m_LightColor[1] = 255;
    m_LightColor[2] = 255;
    m_HeightmapFile[0] = '\0';
    m_LightmapFile[0] = '\0';
    m_Patch = NULL;
    //m_IsBruteForceEnabled = false;
    m_IsBruteForceEnabled = true;   /* Brute force is always enabled because 
                                    geomipmaping  is not fully implemented */
}

Terrain::Terrain (RenderDevice* _device) {
    m_Scale[0] = m_Scale[1] = m_Scale[2] = 1.0f;
    m_HeightmapSize = 0;
    m_Heightmap = NULL;
    m_Log = NULL;
    m_Device = _device;
    m_NumTiles = 0;
    m_Lightmap = NULL;
    m_LightmapSize = 0;
    m_LightMode = TLM_HEIGHT_BASED;
    m_LightColor[0] = 255;
    m_LightColor[1] = 255;
    m_LightColor[2] = 255;
    m_RepeatDetailMap = 4;
    m_HeightmapFile[0] = '\0';
    m_LightmapFile[0] = '\0';
    m_Patch = NULL;
    //m_IsBruteForceEnabled = false;
    m_IsBruteForceEnabled = true;   /* Brute force is always enabled because 
                                    geomipmaping  is not fully implemented */
}

Terrain::~Terrain () {
    UnloadHeightmap ();
    UnloadLightmap ();
    Shutdown ();
}

void Terrain::EnableLog (LogManager* _log) {
    m_Log = _log;
}

void Terrain::LoadData (const char* _filename, UCHAR*& _data, UINT& _size) {
    FILE* dataFile = fopen (_filename, "rb");
    if (!dataFile) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: cannot open heightmap %s (Terrain::LoadData)\n", _filename);
        }
        #endif
        THROW_ERROR (ERRC_FILE_NOT_FOUND);
    }
    // find out the file size
    if (fseek (dataFile, 0, SEEK_END)) {   // non zero value = error
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: cannot seek to the end of the file %s (Terrain::LoadData)\n", _filename);
        }
        #endif
        fclose (dataFile);
        THROW_ERROR (ERRC_BAD_FILE);
    }
    DWORD fileSize = ftell (dataFile);
    // check if file is valid
    float size = sqrt ((float) fileSize);
    if (size - ceil (size) != 0.0f) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("File size is invalid. (Terrain::LoadData)\n");
        }
        #endif
        fclose (dataFile);
        THROW_ERROR (ERRC_BAD_FILE);
    }
    // go back to file begining
    rewind (dataFile);
    // read data
    if (_data) {
        UnloadData (_data, _size);
    }
    try {
        _data = new UCHAR[fileSize];
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    fread (_data, 1, fileSize, dataFile);
    if (ferror (dataFile) || feof (dataFile)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: cannot read data from file %s (Terrain::LoadData)\n", _filename);
        }
        #endif
        delete[] _data;
        _data = NULL;
        fclose (dataFile);
        THROW_ERROR (ERRC_BAD_FILE);
    }
    fclose (dataFile);
    _size = (UINT) size;
}

void Terrain::UnloadData (UCHAR*& _data, UINT& _size) {
    if (_data) {
        delete[] _data;
        _data = NULL;
    }
    _size = 0;   
}

void Terrain::SaveData (const char* _filename, UCHAR* _data, UINT _size) {
    if (!_data) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: trying to save empty data. (Terrain::SaveData)\n");
        }
        #endif
        return;
    }
    FILE* file = fopen (_filename, "wb");
    if (!file) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: cannot open file for writing %. (Terrain::SaveData)\n", _filename);
        }
        #endif
        THROW_ERROR (ERRC_FILE_NOT_FOUND);
    }
    fwrite (_data, 1, _size * _size, file);
    if (ferror (file)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: cannot write to file %s (Terrain::SaveData)\n", _filename);
        }
        #endif
        fclose (file);
        THROW_ERROR (ERRC_BAD_FILE);
    }
    fclose (file);
}

DWORD Terrain::GetColor (UINT _x, UINT _z) {
    DWORD color;
    UCHAR r, g, b;
    switch (m_LightMode) {
        case TLM_HEIGHT_BASED:
            r = (UCHAR) (m_LightColor[0] * GetHeight (_x, _z) / 255);
            g = (UCHAR) (m_LightColor[1] * GetHeight (_x, _z) / 255);
            b = (UCHAR) (m_LightColor[2] * GetHeight (_x, _z) / 255);
            break;
        case TLM_SLOPE_LIGHT:
            {
                float shade;
                if ((int)_x >= m_LightDirectionX && (int)_z >= m_LightDirectionZ) {
                    UINT x = _x - m_LightDirectionX;
                    UINT z = _z - m_LightDirectionZ;
                    float HeightDiff = (float)GetHeight (x, z) - GetHeight (_x, _z);
                    shade = 1.0f - (HeightDiff) / m_LightSoftness;
                } else {
                    shade = 1.0f;
                }
                if (shade < m_MinBrightness) {
                    shade = m_MinBrightness;
                }
                if (shade > m_MaxBrightness) {
                    shade = m_MaxBrightness;
                }
                SetBrightness (_x, _z, (UCHAR) (255 * shade));
            }
            // continue to lightmap processing:
        case TLM_LIGHTMAP:
            UCHAR shade = GetBrightness (_x, _z);
            r = (UCHAR) (m_LightColor[0] * shade / 255);
            g = (UCHAR) (m_LightColor[1] * shade / 255);
            b = (UCHAR) (m_LightColor[2] * shade / 255);
            break;
    }
    color = (((255)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff);
    return color;
}

void Terrain::UpdateData () {
    for (UINT i = 0; i < m_PatchesPerSide; i++) {
        for (UINT j = 0; j < m_PatchesPerSide; j++) {
            UINT index = i * m_PatchesPerSide + j;
            for (UINT k = 0; k < m_PatchSize; k++) {
                for (UINT l = 0; l < m_PatchSize; l++) {
                    UINT x = j * (m_PatchSize - 1) + l;
                    UINT z = i * (m_PatchSize - 1) + k;
                    DWORD color = GetColor (x, z);
                    float scaledHeight = GetScaledHeight (x, z);
                    UINT vertIndex = k * m_PatchSize + l;
                    switch (m_VertexFormat) {
                        case VFT_UL:
                            {
                                ULVERTEX* vertex = (ULVERTEX*)m_Patch[index].VertexData;
                                vertex[vertIndex].Color = color;
                                vertex[vertIndex].Y = scaledHeight;
                            }
                            break;
                        case VFT_UL2:
                            {
                                ULVERTEX2* vertex = (ULVERTEX2*)m_Patch[index].VertexData;
                                vertex[vertIndex].Color = color;
                                vertex[vertIndex].Y = scaledHeight;
                            }
                            break;
                        default:
                            #ifdef _DEBUG
                            if (m_Log) {
                                m_Log->Log ("Error: Unknown vertex format. (Terrain::InitPatchData)\n");
                            }
                            #endif
                            THROW_ERROR (ERRC_UNKNOWN_VF);
                    }
                }
            }
        }
    }
}

void Terrain::InitPatchData (UINT _patchX, UINT _patchZ) {
    UINT index = _patchZ * m_PatchesPerSide + _patchX;
    try {
        switch (m_VertexFormat) {
            case VFT_UL:
                m_Patch[index].VertexData = new ULVERTEX[m_PatchSize * m_PatchSize];
                break;
            case VFT_UL2:
                m_Patch[index].VertexData = new ULVERTEX2[m_PatchSize * m_PatchSize];
                break;
            default:
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: Unknown vertex format. (Terrain::InitPatchData)\n");
                }
                #endif
                THROW_ERROR (ERRC_UNKNOWN_VF);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    /*m_Patch[index].IndexData = new WORD[m_PatchSize * m_PatchSize * 4];
    m_Patch[index].NumIndices = 0;*/
    for (UINT i = 0; i < m_PatchSize; i++) {
        for (UINT j = 0; j < m_PatchSize; j++) {
            UINT x = _patchX * (m_PatchSize - 1) + j;
            UINT z = _patchZ * (m_PatchSize - 1) + i;
            DWORD color = GetColor (x, z);
            float scaledHeight = GetScaledHeight (x, z);
            float texU = (float)x / m_HeightmapSize;
            float texV = (float)z / m_HeightmapSize;
            UINT vertIndex = i * m_PatchSize + j;
            switch (m_VertexFormat) {
                case VFT_UL:
                    {
                        ULVERTEX* vertex = (ULVERTEX*)m_Patch[index].VertexData;
                        vertex[vertIndex].Color = color;
                        vertex[vertIndex].X = x * m_Scale[0];
                        vertex[vertIndex].Y = scaledHeight;
                        vertex[vertIndex].Z = z * m_Scale[2];
                        //m_Log->Log ("%u %f %f\n", vertIndex, vertex[vertIndex].x, vertex[vertIndex].z);
                        vertex[vertIndex].Tu = texU;
                        vertex[vertIndex].Tv = texV;
                    }
                    break;
                case VFT_UL2:
                    {
                        ULVERTEX2* vertex = (ULVERTEX2*)m_Patch[index].VertexData;
                        vertex[vertIndex].Color = color;
                        vertex[vertIndex].X = x * m_Scale[0];
                        vertex[vertIndex].Y = scaledHeight;
                        vertex[vertIndex].Z = z * m_Scale[2];
                        //m_Log->Log ("%u %f %f\n", vertIndex, vertex[vertIndex].x, vertex[vertIndex].z);
                        vertex[vertIndex].Tu1 = texU;
                        vertex[vertIndex].Tv1 = texV;
                        vertex[vertIndex].Tu2 = texU * m_RepeatDetailMap;
                        vertex[vertIndex].Tv2 = texV * m_RepeatDetailMap;
                    }
                    break;
                default:
                    #ifdef _DEBUG
                    if (m_Log) {
                        m_Log->Log ("Error: Unknown vertex format. (Terrain::InitPatchData)\n");
                    }
                    #endif
                    THROW_ERROR (ERRC_UNKNOWN_VF);
            }
        }
    }
    float x = (float)_patchX * (m_PatchSize - 1);
    float z = (float)_patchZ * (m_PatchSize - 1);
    float size = (float)m_PatchSize - 1;
    m_Patch[index].Bounds = GeneratePatchPartBounds (x, z, size);
    GeneratePatchParts (m_Patch[index].Bounds, x, z, size);
}

void Terrain::AddIndicesNoLeft (WORD _center, UINT _size) {
    UINT d = _size;

    try {
        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize);
        m_Indices.push_back (_center - d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d);
        m_Indices.push_back (_center - d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize + d);
        m_Indices.push_back (_center + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize);
        m_Indices.push_back (_center + d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize - d);

        m_IndexBufferOffsets[m_IndexBufferOffsets.size()-1].Number += 21;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::AddIndicesNoRight (WORD _center, UINT _size) {
    UINT d = _size;

    try {
        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize);
        m_Indices.push_back (_center - d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize);
        m_Indices.push_back (_center + d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d);
        m_Indices.push_back (_center + d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize - d);
        m_Indices.push_back (_center - d);

        m_IndexBufferOffsets[m_IndexBufferOffsets.size()-1].Number += 21;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::AddIndicesNoTop (WORD _center, UINT _size) {
    UINT d = _size;

    try {
        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize);
        m_Indices.push_back (_center - d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d);
        m_Indices.push_back (_center - d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize + d);
        m_Indices.push_back (_center + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d);
        m_Indices.push_back (_center + d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize - d);
        m_Indices.push_back (_center - d);

        m_IndexBufferOffsets[m_IndexBufferOffsets.size()-1].Number += 21;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::AddIndicesNoBottom (WORD _center, UINT _size) {
    UINT d = _size;

    try {
        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d);
        m_Indices.push_back (_center - d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize + d);
        m_Indices.push_back (_center + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize);
        m_Indices.push_back (_center + d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d);
        m_Indices.push_back (_center + d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize - d);
        m_Indices.push_back (_center - d);

        m_IndexBufferOffsets[m_IndexBufferOffsets.size()-1].Number += 21;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::AddIndices (WORD _center, UINT _size) {
    UINT d = _size;

    try {
        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize);
        m_Indices.push_back (_center - d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize + d);
        m_Indices.push_back (_center - d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d);
        m_Indices.push_back (_center - d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize + d);
        m_Indices.push_back (_center + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize);
        m_Indices.push_back (_center + d * m_PatchSize + d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center + d * m_PatchSize - d);
        m_Indices.push_back (_center + d * m_PatchSize);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d);
        m_Indices.push_back (_center + d * m_PatchSize - d);

        m_Indices.push_back (_center);
        m_Indices.push_back (_center - d * m_PatchSize - d);
        m_Indices.push_back (_center - d);

        m_IndexBufferOffsets[m_IndexBufferOffsets.size()-1].Number += 24;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::GenerateIndices () {
    m_Indices.clear ();
    IndicesInfo info;
    info.Offset = m_Indices.size();
    info.Number = 0;
    try {
        m_IndexBufferOffsets.push_back (info);
        for (UINT i = 1; i < (UCHAR)(m_PatchSize - 1); i += 2) { // run through fan columns
            for (UINT j = 1; j < (UCHAR)(m_PatchSize - 1); j += 2) { // run through fan rows
                WORD center = i * m_PatchSize + j;  // get the center of the fan
                AddIndices (center, 1);
            }
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    /*for (UINT i = 0; i < m_IndexBufferOffsets.size(); i++) {
        m_Log->Log ("%u %u\n", m_IndexBufferOffsets[i].Offset, m_IndexBufferOffsets[i].Number);
    }
    m_Log->Log(" %u\n", m_Indices.size());
    for (UINT i = 0; i < m_Indices.size(); i++) {
        m_Log->Log("%u %f %f %f\n", m_Indices[i], ((ULVERTEX2*)m_Patch[0].VertexData)[m_Indices[i]].x, ((ULVERTEX2*)m_Patch[0].VertexData)[m_Indices[i]].y, ((ULVERTEX2*)m_Patch[0].VertexData)[m_Indices[i]].z);
    }*/
}

PatchBounds Terrain::GeneratePatchPartBounds (float _x, float _z, float _Size) {
    PatchBounds partBB;
    partBB.Min[0] = _x * m_Scale[0];
    partBB.Min[1] = 0.0f;
    partBB.Min[2] = _z * m_Scale[2];

    partBB.Max[0] = (_x + _Size) * m_Scale[0];
    partBB.Max[1] = 255.0f * m_Scale[1];
    partBB.Max[2] = (_z + _Size) * m_Scale[2];

    return partBB;
}

void Terrain::GeneratePatchParts (PatchBounds& _Part, float _x, float _z, float _Size) {
    //float min[3];
    //float max[3];
    //min[0] = (float)_x * (m_PatchSize - 1);
    //min[2] = (float)_z * (m_PatchSize - 1);
    //min[1] = /*GetScaledHeight ((UINT)minX, (UINT)minZ)*/0.0f * m_Scale[1];
    //min[0] *= m_Scale[0];
    //min[2] *= m_Scale[2];
    //
    //max[0] = (float)(_x) * (m_PatchSize - 1) + (m_PatchSize - 1);
    //max[2] = (float)(_z) * (m_PatchSize - 1) + (m_PatchSize - 1);
    //max[1] = /*GetScaledHeight ((UINT)minX, (UINT)minZ)*/255.0f * m_Scale[1];
    //max[0] *= m_Scale[0];
    //max[2] *= m_Scale[2];
    try {
        if (_Size > 2) {
            _Size = _Size / 2;
            _Part.Children = new PatchBounds[4];
            _Part.Children[0] = GeneratePatchPartBounds(_x, _z, _Size);
            GeneratePatchParts (_Part.Children[0], _x, _z, _Size);
            _Part.Children[1] = GeneratePatchPartBounds(_x + _Size, _z, _Size);
            GeneratePatchParts (_Part.Children[1], _x + _Size, _z, _Size);
            _Part.Children[2] = GeneratePatchPartBounds(_x, _z + _Size, _Size);
            GeneratePatchParts (_Part.Children[2], _x, _z + _Size, _Size);
            _Part.Children[3] = GeneratePatchPartBounds(_x + _Size, _z + _Size, _Size);
            GeneratePatchParts (_Part.Children[3], _x + _Size, _z + _Size, _Size);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::Init (UCHAR _patchSize, VERTEXFORMATTYPE _vft, UINT _skinId) {
    Shutdown ();    // delete previous data if it exists
    if (m_HeightmapSize == 0) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: There are no heightmap loaded. (Terrain::Init)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_NOT_READY, "Heightmap is not loaded.");
    }
    switch (_vft) {
        case VFT_UL:
            m_VertexSize = sizeof (ULVERTEX);
            break;
        case VFT_UL2:
            m_VertexSize = sizeof (ULVERTEX2);
            break;
        default:
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Unknown vertex format. (Terrain::Init)\n");
            }
            #endif
            THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    m_PatchesPerSide = (UCHAR) (m_HeightmapSize / _patchSize);
    try {
        m_Patch = new Patch[m_PatchesPerSide * m_PatchesPerSide];
        m_PatchSize = _patchSize;
        UCHAR divisor = m_PatchSize - 1;
        m_MaxLod = 0;
        while (divisor >= 2) {
            divisor >>= 1;
            m_MaxLod++;
        }
        m_VertexFormat = _vft;
        m_SkinId = _skinId;

        for (UINT i = 0; i < m_PatchesPerSide; i++) {
            for (UINT j = 0; j < m_PatchesPerSide; j++) {
                m_Patch[i*m_PatchesPerSide+j].Lod = m_MaxLod;
                InitPatchData (j, i);
            }
        }

        GenerateIndices ();
        WORD* indices = new WORD[m_Indices.size()];
        for (UINT k = 0; k < m_Indices.size(); k++) {
            indices[k] = m_Indices[k];
        }
        m_IndexBufferId = m_Device->GetVCacheManager()->CreateStaticIndexBuffer (indices, m_Indices.size());
        delete[] indices;
    } catch (std::bad_alloc) {
        Shutdown ();
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void Terrain::ReleasePatchBounds (PatchBounds _bounds) {
    if (_bounds.Children != NULL) {
        ReleasePatchBounds (_bounds.Children[0]);
        ReleasePatchBounds (_bounds.Children[1]);
        ReleasePatchBounds (_bounds.Children[2]);
        ReleasePatchBounds (_bounds.Children[3]);
    }
    delete[] _bounds.Children;
}

void Terrain::Shutdown () {
    if (m_Patch) {
        for (UINT i = 0; i < m_PatchesPerSide; i++) {
            for (UINT j = 0; j < m_PatchesPerSide; j++) {
                delete[] m_Patch[i*m_PatchesPerSide + j].VertexData;
                ReleasePatchBounds (m_Patch[i*m_PatchesPerSide + j].Bounds);
            }
        }
        delete[] m_Patch;
        m_Patch = NULL;
    }
    m_PatchSize = m_PatchesPerSide = m_MaxLod = 0;
}

bool Terrain::IsPatchInFrustum (float _x, float _y, float _z) {
    float sizeX = (m_PatchSize - 1) * m_Scale[0] / 2;
    float sizeY = 128.0f * m_Scale[1];
    float sizeZ = (m_PatchSize - 1) * m_Scale[2] / 2;
    // draw bounds
    /*ULCVERTEX* bound = new ULCVERTEX[8];
    WORD* index = new WORD[48];
    bound[0].Color = 0xff00ffff;
    bound[0].x = x - sizeX;
    bound[0].y = y - sizeY;
    bound[0].z = z - sizeZ;
    
    bound[1].Color = 0xff00ffff;
    bound[1].x = x + sizeX;
    bound[1].y = y - sizeY;
    bound[1].z = z - sizeZ;

    bound[2].Color = 0xff00ffff;
    bound[2].x = x + sizeX;
    bound[2].y = y - sizeY;
    bound[2].z = z + sizeZ;

    bound[3].Color = 0xff00ffff;
    bound[3].x = x - sizeX;
    bound[3].y = y - sizeY;
    bound[3].z = z + sizeZ;

    bound[4].Color = 0xff00ffff;
    bound[4].x = x - sizeX;
    bound[4].y = y + sizeY;
    bound[4].z = z - sizeZ;

    bound[5].Color = 0xff00ffff;
    bound[5].x = x + sizeX;
    bound[5].y = y + sizeY;
    bound[5].z = z - sizeZ;

    bound[6].Color = 0xff00ffff;
    bound[6].x = x + sizeX;
    bound[6].y = y + sizeY;
    bound[6].z = z + sizeZ;

    bound[7].Color = 0xff00ffff;
    bound[7].x = x - sizeX;
    bound[7].y = y + sizeY;
    bound[7].z = z + sizeZ;

    index[0] = 0;
    index[1] = 1;
    index[2] = 1;
    index[3] = 2;
    index[4] = 2;
    index[5] = 3;
    index[6] = 3;
    index[7] = 0;
    index[8] = 4;
    index[9] = 5;
    index[10] = 5;
    index[11] = 6;
    index[12] = 6;
    index[13] = 7;
    index[14] = 7;
    index[15] = 4;
    index[16] = 2;
    index[17] = 3;
    index[18] = 3;
    index[19] = 7;
    index[20] = 7;
    index[21] = 6;
    index[22] = 6;
    index[23] = 2;
    index[24] = 1;
    index[25] = 0;
    index[26] = 0;
    index[27] = 4;
    index[28] = 4;
    index[29] = 5;
    index[30] = 5;
    index[31] = 1;
    index[32] = 0;
    index[33] = 3;
    index[34] = 3;
    index[35] = 7;
    index[36] = 7;
    index[37] = 4;
    index[38] = 4;
    index[39] = 0;
    index[40] = 1;
    index[41] = 2;
    index[42] = 2;
    index[43] = 6;
    index[44] = 6;
    index[45] = 5;
    index[46] = 5;
    index[47] = 1;

    m_Device->GetVCacheManager()->Render(PT_LINELIST, 8, bound, 48, index, VFT_ULC, INVALID_ID);
    delete[] bound;
    delete[] index;*/

    for(UINT i = 0; i < 6; i++) {
        if(m_Frustum[i][0] * (_x - sizeX) + m_Frustum[i][1] * (_y - sizeY) + m_Frustum[i][2] * (_z - sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x + sizeX) + m_Frustum[i][1] * (_y - sizeY) + m_Frustum[i][2] * (_z - sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x - sizeX) + m_Frustum[i][1] * (_y + sizeY) + m_Frustum[i][2] * (_z - sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x + sizeX) + m_Frustum[i][1] * (_y + sizeY) + m_Frustum[i][2] * (_z - sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x - sizeX) + m_Frustum[i][1] * (_y - sizeY) + m_Frustum[i][2] * (_z + sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x + sizeX) + m_Frustum[i][1] * (_y - sizeY) + m_Frustum[i][2] * (_z + sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x - sizeX) + m_Frustum[i][1] * (_y + sizeY) + m_Frustum[i][2] * (_z + sizeZ) + m_Frustum[i][3] > 0)
            continue;
        if(m_Frustum[i][0] * (_x + sizeX) + m_Frustum[i][1] * (_y + sizeY) + m_Frustum[i][2] * (_z + sizeZ) + m_Frustum[i][3] > 0)
            continue;
        return false;
    }
    return true;
}

void Terrain::Update (VECTOR3 _cameraPos, MATRIX44 _view, MATRIX44 _proj) {
    if (m_HeightmapSize == 0) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Heightmap is not loaded. (Terrain::Update)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_NOT_READY, "Heightmap is not loaded.");
    }
    if (!m_Patch) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Terrain is not initialized.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_NOT_READY, "Terrain is not initialized.");
    }

    extract_frustum_planes (_view, _proj, m_Frustum, cml::z_clip_zero);
    /*m_Log->Log ("%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n\n",
        m_Frustum[0][0], m_Frustum[0][1], m_Frustum[0][2], m_Frustum[0][3],
        m_Frustum[1][0], m_Frustum[1][1], m_Frustum[1][2], m_Frustum[1][3],
        m_Frustum[2][0], m_Frustum[2][1], m_Frustum[2][2], m_Frustum[2][3],
        m_Frustum[3][0], m_Frustum[3][1], m_Frustum[3][2], m_Frustum[3][3],
        m_Frustum[4][0], m_Frustum[4][1], m_Frustum[4][2], m_Frustum[4][3],
        m_Frustum[5][0], m_Frustum[5][1], m_Frustum[5][2], m_Frustum[5][3]);*/
    for (UINT i = 0; i < m_PatchesPerSide; i++) {
        for (UINT j = 0; j < m_PatchesPerSide; j++) {
            /* selection bounds */

            //float minX, minY, minZ;
            //float maxX, maxY, maxZ;
            //minX = (float)j * (m_PatchSize - 1);
            //minZ = (float)i * (m_PatchSize - 1);
            //minY = /*GetScaledHeight ((UINT)minX, (UINT)minZ)*/0.0f * m_Scale[1];
            //minX *= m_Scale[0];
            //minZ *= m_Scale[2];
    
            //maxX = (float)(j + 1) * (m_PatchSize - 1);
            //maxZ = (float)(i + 1) * (m_PatchSize - 1);
            //maxY = /*GetScaledHeight ((UINT)maxX, (UINT)maxZ)*/255.0f * m_Scale[1];
            //maxX *= m_Scale[0];
            //maxZ *= m_Scale[2];

            //ULCVERTEX* bound = new ULCVERTEX[8];
            //WORD* index = new WORD[48];
            //bound[0].Color = 0xffff0000;
            //bound[0].x = minX;
            //bound[0].y = minY;
            //bound[0].z = minZ;
    
            //bound[1].Color = 0xffff0000;
            //bound[1].x = maxX;
            //bound[1].y = minY;
            //bound[1].z = minZ;

            //bound[2].Color = 0xffff0000;
            //bound[2].x = maxX;
            //bound[2].y = minY;
            //bound[2].z = maxZ;

            //bound[3].Color = 0xffff0000;
            //bound[3].x = minX;
            //bound[3].y = minY;
            //bound[3].z = maxZ;

            //bound[4].Color = 0xffff0000;
            //bound[4].x = minX;
            //bound[4].y = maxY;
            //bound[4].z = minZ;

            //bound[5].Color = 0xffff0000;
            //bound[5].x = maxX;
            //bound[5].y = maxY;
            //bound[5].z = minZ;

            //bound[6].Color = 0xffff0000;
            //bound[6].x = maxX;
            //bound[6].y = maxY;
            //bound[6].z = maxZ;

            //bound[7].Color = 0xffff0000;
            //bound[7].x = minX;
            //bound[7].y = maxY;
            //bound[7].z = maxZ;

            //index[0] = 0;
            //index[1] = 1;
            //index[2] = 1;
            //index[3] = 2;
            //index[4] = 2;
            //index[5] = 3;
            //index[6] = 3;
            //index[7] = 0;
            //index[8] = 4;
            //index[9] = 5;
            //index[10] = 5;
            //index[11] = 6;
            //index[12] = 6;
            //index[13] = 7;
            //index[14] = 7;
            //index[15] = 4;
            //index[16] = 2;
            //index[17] = 3;
            //index[18] = 3;
            //index[19] = 7;
            //index[20] = 7;
            //index[21] = 6;
            //index[22] = 6;
            //index[23] = 2;
            //index[24] = 1;
            //index[25] = 0;
            //index[26] = 0;
            //index[27] = 4;
            //index[28] = 4;
            //index[29] = 5;
            //index[30] = 5;
            //index[31] = 1;
            //index[32] = 0;
            //index[33] = 3;
            //index[34] = 3;
            //index[35] = 7;
            //index[36] = 7;
            //index[37] = 4;
            //index[38] = 4;
            //index[39] = 0;
            //index[40] = 1;
            //index[41] = 2;
            //index[42] = 2;
            //index[43] = 6;
            //index[44] = 6;
            //index[45] = 5;
            //index[46] = 5;
            //index[47] = 1;

            //m_Device->GetVCacheManager()->Render(PT_LINELIST, 8, bound, 48, index, VFT_ULC, INVALID_ID);
            //delete[] bound;
            //delete[] index;

            /* end =========== */

            UINT patchNum = i * m_PatchesPerSide + j;
            // get the center of the patch
            float x = (j * (m_PatchSize - 1)) + ((m_PatchSize - 1) / 2.0f);
            float z = (i * (m_PatchSize - 1)) + ((m_PatchSize - 1) / 2.0f);
            float y = GetScaledHeight ((UINT)x, (UINT)z);
            x *= m_Scale[0];
            z *= m_Scale[2];
            m_Patch[patchNum].IsCulled = !IsPatchInFrustum (x, y, z);
            if (!m_Patch[patchNum].IsCulled) {
                if (m_IsBruteForceEnabled) {
                    m_Patch[patchNum].Lod = 0;
                } else {
                    float xsqr = (x - _cameraPos.data()[0]) * (x - _cameraPos.data()[0]);
                    float ysqr = (y - _cameraPos.data()[1]) * (y - _cameraPos.data()[1]);
                    float zsqr = (z - _cameraPos.data()[2]) * (z - _cameraPos.data()[2]);
                    m_Patch[patchNum].Distance = sqrtf (xsqr + ysqr + zsqr);
                    if (m_Patch[patchNum].Distance < 100) {
                        m_Patch[patchNum].Lod = 0;
                    } else if (m_Patch[patchNum].Distance < 200) {
                        m_Patch[patchNum].Lod = 1;
                    } else if (m_Patch[patchNum].Distance < 500) {
                        m_Patch[patchNum].Lod = 2;
                    } else {
                        m_Patch[patchNum].Lod = 3;
                    }
                }
            }
        }
    }
    /*m_Patch[0].Lod = 1;
    m_Patch[1].Lod = 1;
    m_Patch[2].Lod = 1;*/
}

void Terrain::Render () {
    for (UINT i = 0; i < m_PatchesPerSide; i++) {
        for (UINT j = 0; j < m_PatchesPerSide; j++) {
            if (!m_Patch[i*m_PatchesPerSide+j].IsCulled) {
                UINT index = i * m_PatchesPerSide + j;
                int indexVariant = 0;
                UINT offsetIndex = m_Patch[index].Lod * NUM_INDEX_VARIANTS + indexVariant;
                m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST,
                    (void*)(m_Patch[index].VertexData),
                    (UINT)(m_PatchSize * m_PatchSize),
                    m_IndexBufferId, m_IndexBufferOffsets[offsetIndex].Offset,
                    (UINT)(m_IndexBufferOffsets[offsetIndex].Number / 3),
                    m_VertexFormat, m_SkinId);
            }
        }
    }
}

bool Terrain::IsRayIntersectingPatch (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                                        PatchBounds _bounds, std::vector<PatchBounds>& _intersectedParts) {
    float t0, t1, tmp;
    float tNear = -999999.9f;
    float tFar  =  999999.9f;
    float epsilon = 0.00001f;

    // first pair of planes
    for (UINT i = 0; i < 3; i++) {
        if (fabs(_rayDirection[i]) < epsilon) {
            if (_rayOrigin[i] < _bounds.Min[i] || _rayOrigin[i] > _bounds.Max[i]) {
                return false;
            }
        } else {
            t0 = (_bounds.Min[i] - _rayOrigin[i]) / _rayDirection[i];
            t1 = (_bounds.Max[i] - _rayOrigin[i]) / _rayDirection[i];
            if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
            if (t0 > tNear) tNear = t0;
            if (t1 < tFar)  tFar = t1;
            if (tNear > tFar) return false;
            if (tFar < 0) return false;
        }
    }
    if (_bounds.Children == NULL) {
        try {
            _intersectedParts.push_back (_bounds);
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    } else {
        IsRayIntersectingPatch(_rayOrigin, _rayDirection, _bounds.Children[0], _intersectedParts);
        IsRayIntersectingPatch(_rayOrigin, _rayDirection, _bounds.Children[1], _intersectedParts);
        IsRayIntersectingPatch(_rayOrigin, _rayDirection, _bounds.Children[2], _intersectedParts);
        IsRayIntersectingPatch(_rayOrigin, _rayDirection, _bounds.Children[3], _intersectedParts);
    }
    return true;
}


bool Terrain::IsPatchPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, UINT _x, UINT _z,
                               std::vector<PatchBounds>& _intersectedParts) {
    _rayOrigin = transform_point (m_Device->GetWorldMatrix(), _rayOrigin);
    _rayDirection = transform_vector (m_Device->GetWorldMatrix(), _rayDirection);
    UINT index = _x * m_PatchesPerSide + _z;

    return IsRayIntersectingPatch (_rayOrigin, _rayDirection, m_Patch[index].Bounds, _intersectedParts);
    //float min[3];
    //float max[3];
    //min[0] = (float)_x * (m_PatchSize - 1);
    //min[2] = (float)_z * (m_PatchSize - 1);
    //min[1] = /*GetScaledHeight ((UINT)minX, (UINT)minZ)*/0.0f * m_Scale[1];
    //min[0] *= m_Scale[0];
    //min[2] *= m_Scale[2];
    //
    //max[0] = (float)(_x) * (m_PatchSize - 1) + (m_PatchSize - 1);
    //max[2] = (float)(_z) * (m_PatchSize - 1) + (m_PatchSize - 1);
    //max[1] = /*GetScaledHeight ((UINT)minX, (UINT)minZ)*/255.0f * m_Scale[1];
    //max[0] *= m_Scale[0];
    //max[2] *= m_Scale[2];

    //float t0, t1, tmp;
    //float tNear = -999999.9f;
    //float tFar  =  999999.9f;
    //float epsilon = 0.00001f;

    //// first pair of planes
    //if (fabs(_RayDirection[0]) < epsilon) {
    //    if ((_RayOrigin[0] < min[0]) || (_RayOrigin[0] > max[0]) ) {
    //        return false;
    //    }
    //} else {
    //    t0 = (min[0] - _RayOrigin[0]) / _RayDirection[0];
    //    t1 = (max[0] - _RayOrigin[0]) / _RayDirection[0];
    //    if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
    //    if (t0 > tNear) tNear = t0;
    //    if (t1 < tFar)  tFar = t1;
    //    if (tNear > tFar) return false;
    //    if (tFar < 0) return false;
    //}

    //// second pair of planes
    //if (fabs(_RayDirection[1]) < epsilon) {
    //    if (_RayOrigin[1] < min[1] || _RayOrigin[1] > max[1] ) {
    //        return false;
    //    }
    //} else {
    //    t0 = (min[1] - _RayOrigin[1]) / _RayDirection[1];
    //    t1 = (max[1] - _RayOrigin[1]) / _RayDirection[1];
    //    if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
    //    if (t0 > tNear) tNear = t0;
    //    if (t1 < tFar)  tFar = t1;
    //    if (tNear > tFar) return false;
    //    if (tFar < 0) return false;
    //}

    //// third pair of planes
    //if (fabs(_RayDirection[2]) < epsilon) {
    //    if (_RayOrigin[2] < min[2] || _RayOrigin[2] > max[2] ) {
    //        return false;
    //    }
    //} else {
    //    t0 = (min[2] - _RayOrigin[2]) / _RayDirection[2];
    //    t1 = (max[2] - _RayOrigin[2]) / _RayDirection[2];
    //    if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
    //    if (t0 > tNear) tNear = t0;
    //    if (t1 < tFar)  tFar = t1;
    //    if (tNear > tFar) return false;
    //    if (tFar < 0) return false;
    //}

    //return true;
}

bool TriangleRayIntersection (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, VECTOR3 _v0, VECTOR3 _v1, VECTOR3 _v2, VECTOR3& _point) {
    // find plane of the triangle
    VECTOR3 n = cross (_v1 - _v0, _v2 - _v0);   // plane normal
    float d = dot(-n, _v0);
    float t = -(dot(_rayOrigin, n) + d) / dot(_rayDirection, n);  // get t value
    _point = _rayOrigin + t * _rayDirection; // compute the intersection point
    
    // is intersection point in the triangle?    	
    // Compute vectors        
    VECTOR3 v0 = _v2 - _v0;
    VECTOR3 v1 = _v1 - _v0;
    VECTOR3 v2 = _point - _v0;

    // Compute dot products
    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u > 0) && (v > 0) && (u + v < 1);
}

float GetDistance (VECTOR3 _p1, VECTOR3 _p2) {
    float x = (_p1[0] - _p2[0]) * (_p1[0] - _p2[0]);
    float y = (_p1[1] - _p2[1]) * (_p1[1] - _p2[1]);
    float z = (_p1[2] - _p2[2]) * (_p1[2] - _p2[2]);
    return sqrt(x + y + z);
}

UINT GetClosestVertex (VECTOR3 _point, VECTOR3 _v0, VECTOR3 _v1, VECTOR3 _v2, UINT _i) {
    float dist1 = GetDistance (_point, _v0);
    float dist2 = GetDistance (_point, _v1);
    float dist3 = GetDistance (_point, _v2);
    if (dist1 < dist2) {
        if (dist1 < dist3) {
            return _i;
        } else {
            return _i+2;
        }
    } else {
        if (dist2 < dist3) {
            return _i+1;
        } else {
            return _i+2;
        }
    }
}

bool Terrain::IsVertexPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                                PatchBounds _bounds, VECTOR3 _viewPoint,
                                POINT& _vertex, float& _distance) {    // _Index = index of the vertex
    UINT patchX = (UINT)(_bounds.Min[0] / m_Scale[0]);
    patchX = (UINT)(patchX / (m_PatchSize - 1));
    UINT patchZ = (UINT)(_bounds.Min[2] / m_Scale[2]);
    patchZ = (UINT)(patchZ / (m_PatchSize - 1));
    UINT index = patchZ * m_PatchesPerSide + patchX;
    UINT x = ((UINT)(_bounds.Min[0] / m_Scale[0]) % (m_PatchSize - 1)) / 2;
    UINT z = ((UINT)(_bounds.Min[2] / m_Scale[2]) % (m_PatchSize - 1)) / 2 ;
    UINT indicesIndex = (z * (m_PatchSize - 1) / 2 + x) * 24;
    bool isPicked = false;
    _distance = 9999.0f;
    UINT vertIndex;
    /*_RayOrigin = cml::transform_point (m_Device->GetWorldMatrix(), _RayOrigin);
    _RayDirection = cml::transform_vector (m_Device->GetWorldMatrix(), _RayDirection);*/
    switch (m_VertexFormat) {
        case VFT_UL:
            {
                VECTOR3 v1, v2, v3;
                VECTOR3 point;
                ULVERTEX* data = (ULVERTEX*)m_Patch[index].VertexData;
                for (UINT i = indicesIndex; i < indicesIndex + 24; i += 3) {
                    //m_Log->Log ("%u %u %u %u\n", indicesIndex, m_Indices[i], m_Indices[i + 1], m_Indices[i + 2]);
                    UINT ind = m_Indices[i];
                    v1 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    ind = m_Indices[i + 1];
                    v2 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    ind = m_Indices[i + 2];
                    v3 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    if (TriangleRayIntersection (_rayOrigin, _rayDirection, v1, v2, v3, point)) {
                        float dist = GetDistance (_viewPoint, point);
                        if (dist < _distance) {
                            isPicked = true;
                            _distance = dist;
                            vertIndex = GetClosestVertex (point, v1, v2, v3, i);
                        }
                    }
                }
            }
            break;
        case VFT_UL2:
            {
                VECTOR3 v1, v2, v3;
                VECTOR3 point;
                ULVERTEX2* data = (ULVERTEX2*)m_Patch[index].VertexData;
                for (UINT i = indicesIndex; i < indicesIndex + 24; i += 3) {
                    //m_Log->Log ("%u %u %u %u\n", indicesIndex, m_Indices[i], m_Indices[i + 1], m_Indices[i + 2]);
                    UINT ind = m_Indices[i];
                    v1 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    ind = m_Indices[i + 1];
                    v2 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    ind = m_Indices[i + 2];
                    v3 = VECTOR3 (data[ind].X, data[ind].Y, data[ind].Z);
                    if (TriangleRayIntersection (_rayOrigin, _rayDirection, v1, v2, v3, point)) {
                        float dist = GetDistance (_viewPoint, point);
                        if (dist < _distance) {
                            isPicked = true;
                            _distance = dist;
                            vertIndex = GetClosestVertex (point, v1, v2, v3, i);
                        }
                    }
                }
            }
            break;
        default:
            THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    if (isPicked) {
        switch (m_VertexFormat) {
            case VFT_UL:
                _vertex.x = (UINT) (((ULVERTEX*)m_Patch[index].VertexData)[m_Indices[vertIndex]].X / m_Scale[0]);
                _vertex.y = (UINT) (((ULVERTEX*)m_Patch[index].VertexData)[m_Indices[vertIndex]].Z / m_Scale[2]);
                break;
            case VFT_UL2:
                _vertex.x = (UINT) (((ULVERTEX2*)m_Patch[index].VertexData)[m_Indices[vertIndex]].X / m_Scale[0]);
                _vertex.y = (UINT) (((ULVERTEX2*)m_Patch[index].VertexData)[m_Indices[vertIndex]].Z / m_Scale[2]);
                break;
            
            default:
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log("Invalid vertex format. (Terrain::IsVertexPicked)\n");
                }
                #endif
                THROW_ERROR (ERRC_UNKNOWN_VF);
        }
    }
    return isPicked;
}