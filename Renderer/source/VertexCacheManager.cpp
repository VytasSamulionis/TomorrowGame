/*  Author: Vytas Samulionis
    Description: batches vertices for efficient rendering
*/
#include "../include/VertexCacheManager.h"

using namespace vs3d;

void VertexCacheManager::CreateVertexDeclarations () {
    D3DVERTEXELEMENT9 up[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 uu[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 uu2[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 ul[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 ul2[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 ulc[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 tp[] = {
        { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 tl[] = {
        { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
        { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END() };
    D3DVERTEXELEMENT9 tlc[] = {
        { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
        { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        D3DDECL_END() };
    if (FAILED (m_Device->CreateVertexDeclaration (up, &m_VertexDecl[0]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (uu, &m_VertexDecl[1]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (uu2, &m_VertexDecl[2]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (ul, &m_VertexDecl[3]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (ul2, &m_VertexDecl[4]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (ulc, &m_VertexDecl[5]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (tp, &m_VertexDecl[6]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (tl, &m_VertexDecl[7]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
    if (FAILED (m_Device->CreateVertexDeclaration (tlc, &m_VertexDecl[8]))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexDeclaration() failure.");
    }
}

VertexCacheManager::VertexCacheManager (LPDIRECT3DDEVICE9 _device, 
                                        SkinManager* _skin, LogManager* _log):
    m_Device (_device),
    m_Skin (_skin),
    m_Log (_log),
    m_ActiveSkin (INVALID_ID),
    m_ActiveEffect (NULL) {
    try {
        m_VertexFormatCaches = new RenderCache*[NUM_VERTEX_FORMATS];
        for (UINT i = 0; i < NUM_VERTEX_FORMATS; i++) {
            m_VertexFormatCaches[i] = new RenderCache(this);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    m_Font = NULL;
    strcpy (m_FontStyle, "Times New Roman");
    m_FontSize = 16;
    CreateVertexDeclarations ();
}

VertexCacheManager::~VertexCacheManager () {
    if (m_VertexFormatCaches) {
        for (UINT i = 0; i < NUM_VERTEX_FORMATS; i++) {
            delete m_VertexFormatCaches[i];
        }
    }
    for (UINT i = 0; i < m_Effects.size (); i++) {
        m_Effects[i].Effect->Release ();
    }
    delete[] m_VertexFormatCaches;
    ClearStaticVertexBuffers ();
    ClearStaticIndexBuffers ();
    delete m_Font;
}

UINT VertexCacheManager::CreateStaticVertexBuffer (void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft) {
    if (m_StaticVertexBuffers.size() >= INVALID_ID - 1) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    UINT vertexSize = GetVertexSize (_vft);
    vertexSize *= _numVertices;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    if (FAILED (m_Device->CreateVertexBuffer (vertexSize, D3DUSAGE_WRITEONLY, 0,
                                              D3DPOOL_DEFAULT, &vertexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateVertexBuffer failed. (VertexCacheManager::CreateStaticVertexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexBuffer() failure.");
    }
    void* data;
    if (FAILED (vertexBuffer->Lock (0, vertexSize, &data, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::CreateStaticVertexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
    }
    memcpy (data, _vertex, vertexSize);
    vertexBuffer->Unlock ();
    StaticVertexBuffer buffer;
    buffer.Buffer = vertexBuffer;
    buffer.NumVertices = _numVertices;
    try {
        m_StaticVertexBuffers.push_back (buffer);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }

    return m_StaticVertexBuffers.size() - 1;
}

void VertexCacheManager::AddToStaticVertexBuffer (UINT _vertexBufferId, void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft) {
    if (_vertexBufferId >= m_StaticVertexBuffers.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (m_StaticVertexBuffers[_vertexBufferId].NumVertices + _numVertices > MAX_VERTEX_NUM * 2) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    UINT size = GetVertexSize (_vft);
    size *= _numVertices;
    void* vertex;
    void* oldData;
    D3DVERTEXBUFFER_DESC description;
    m_StaticVertexBuffers[_vertexBufferId].Buffer->GetDesc (&description);
    if (FAILED (m_StaticVertexBuffers[_vertexBufferId].Buffer->Lock (0, description.Size, &oldData, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::AddToStaticVertexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
    }
    vertex = new BYTE[description.Size + size];
    memcpy (vertex, oldData, description.Size);
    memcpy ((BYTE*)vertex + description.Size, _vertex, size);
    m_StaticVertexBuffers[_vertexBufferId].Buffer->Unlock();
    m_StaticVertexBuffers[_vertexBufferId].Buffer->Release();

    LPDIRECT3DVERTEXBUFFER9 newVertexBuffer;
    if (FAILED (m_Device->CreateVertexBuffer (description.Size + size, D3DUSAGE_WRITEONLY, description.FVF,
                                              D3DPOOL_DEFAULT, &newVertexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateVertexBuffer failed. (VertexCacheManager::AddToStaticVertexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexBuffer() failure.");
    }
    void* data;
    if (FAILED (newVertexBuffer->Lock (0, description.Size + size, &data, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::AddToStaticVertexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
    }
    memcpy (data, vertex, description.Size + size);
    newVertexBuffer->Unlock ();

    m_StaticVertexBuffers[_vertexBufferId].Buffer = newVertexBuffer;
    m_StaticVertexBuffers[_vertexBufferId].NumVertices += _numVertices;
    delete[] vertex;
}

void VertexCacheManager::ClearStaticVertexBuffers () {
    for (UINT i = 0; i < m_StaticVertexBuffers.size(); i++) {
        m_StaticVertexBuffers[i].Buffer->Release ();
    }
    m_StaticVertexBuffers.clear ();
}

UINT VertexCacheManager::CreateStaticIndexBuffer (WORD* _index, UINT _numIndices) {
    if (m_StaticIndexBuffers.size() >= INVALID_ID - 1) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    LPDIRECT3DINDEXBUFFER9 indexBuffer;
    UINT indexSize = sizeof (WORD) * _numIndices;
    if (FAILED (m_Device->CreateIndexBuffer (indexSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
                                             D3DPOOL_DEFAULT, &indexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateIndexBuffer failed. (VertexCacheManager::CreateStaticIndexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateIndexBuffer() failure.");
    }
    void* data;
    if (FAILED (indexBuffer->Lock (0, indexSize, &data, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::CreateStaticIndexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
    }
    memcpy (data, (void*)_index, indexSize);
    indexBuffer->Unlock ();
    StaticIndexBuffer buffer;
    buffer.Buffer = indexBuffer;
    buffer.NumIndices = _numIndices;
    try {
        m_StaticIndexBuffers.push_back (buffer);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }

    return m_StaticIndexBuffers.size() - 1;
}

void VertexCacheManager::AddToStaticIndexBuffer (UINT _indexBufferId, WORD* _index, UINT _numIndices) {
    if (_indexBufferId >= m_StaticIndexBuffers.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (m_StaticIndexBuffers[_indexBufferId].NumIndices + _numIndices >= MAX_INDEX_NUM) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    UINT size = sizeof (WORD) * _numIndices;
    D3DINDEXBUFFER_DESC description;
    m_StaticIndexBuffers[_indexBufferId].Buffer->GetDesc(&description);
    void* index;
    void* oldData;
    if (FAILED (m_StaticIndexBuffers[_indexBufferId].Buffer->Lock (0, description.Size, &oldData, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::AddToStaticIndexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
    }
    try {
        index = new BYTE[description.Size + size];
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    memcpy (index, oldData, description.Size);
    memcpy ((BYTE*)index + description.Size, _index, size);
    m_StaticIndexBuffers[_indexBufferId].Buffer->Unlock();
    m_StaticIndexBuffers[_indexBufferId].Buffer->Release();

    LPDIRECT3DINDEXBUFFER9 newIndexBuffer;
    if (FAILED (m_Device->CreateIndexBuffer (description.Size + size, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
                                             D3DPOOL_DEFAULT, &newIndexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateIndexBuffer failed. (VertexCacheManager::AddToStaticIndexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateIndexBuffer() failure.");
    }
    void* data;
    if (FAILED (newIndexBuffer->Lock (0, description.Size + size, &data, 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Lock failed. (VertexCacheManager::AddToStaticIndexBuffer)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateIndexBuffer() failure.");
    }
    memcpy (data, index, description.Size + size);
    newIndexBuffer->Unlock ();

    m_StaticIndexBuffers[_indexBufferId].Buffer = newIndexBuffer;
    m_StaticIndexBuffers[_indexBufferId].NumIndices += _numIndices;
    delete[] index;
}

void VertexCacheManager::ClearStaticIndexBuffers () {
    for (UINT i = 0; i < m_StaticIndexBuffers.size(); i++) {
        m_StaticIndexBuffers[i].Buffer->Release();
    }
    m_StaticIndexBuffers.clear ();
}

UINT VertexCacheManager::CreateParticleBuffer (UINT _size) {
    Particle particle;
    if (FAILED (m_Device->CreateVertexBuffer (
                _size * sizeof (ULCVERTEX),
                D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
                0,
                D3DPOOL_DEFAULT,
                &particle.Buffer,
                NULL))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexBuffer() failure.");
    }
    particle.Size = _size;
    UINT id = m_ParticleBuffers.size();
    try {
        m_ParticleBuffers.push_back (particle);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    return id;
}

void VertexCacheManager::ClearParticleBuffers () {
    for (UINT i = 0; i < m_ParticleBuffers.size(); i++) {
        m_ParticleBuffers[i].Buffer->Release();
    }
    m_ParticleBuffers.clear();
}

void VertexCacheManager::Render (PRIMITIVETYPE _type,
                                 UINT _vertexBufferId, UINT _startVertex,
                                 UINT _indexBufferId, UINT _startIndex,
                                 UINT _numPrimitives,
                                 VERTEXFORMATTYPE _vft, UINT _skinId) {
    m_VertexFormatCaches[(UINT)_vft]->Insert(_skinId, _type, _vft);
    VertexCache* vertexCache = m_VertexFormatCaches[(UINT)_vft]->GetVertexCache(_skinId, _type);//m_VertexFormatsTree->GetVertexCache(_vft);

    StaticRendering staticRendering;
    staticRendering.BaseVertexIndex = _startVertex;
    staticRendering.StartIndex = _startIndex;
    staticRendering.VertexBufferId = _vertexBufferId;
    staticRendering.IndexBufferId = _indexBufferId;
    staticRendering.NumPrimitives = _numPrimitives;

    vertexCache->AddStaticRendering (staticRendering);
}

void VertexCacheManager::Render (PRIMITIVETYPE _type,
                                 void* _vertex, UINT _numVertices,
                                 UINT _indexBufferId, UINT _startIndex,
                                 UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId) {
    m_VertexFormatCaches[(UINT)_vft]->Insert(_skinId, _type, _vft);
    VertexCache* vertexCache = m_VertexFormatCaches[(UINT)_vft]->GetVertexCache(_skinId, _type);//m_VertexFormatsTree->GetVertexCache(_vft);

    StaticRendering staticRendering;
    staticRendering.BaseVertexIndex = vertexCache->GetNumStaticVertices();
    staticRendering.StartIndex = _startIndex;
    staticRendering.VertexBufferId = INVALID_ID;
    staticRendering.IndexBufferId = _indexBufferId;
    staticRendering.NumPrimitives = _numPrimitives;

    vertexCache->AddStaticVertices (_vertex, _numVertices);
    vertexCache->AddStaticRendering (staticRendering);
}

void VertexCacheManager::Render (PRIMITIVETYPE _type,
                                 UINT _vertexBufferId, UINT _startVertex,
                                 WORD* _index, UINT _numIndices,
                                 UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId) {
    m_VertexFormatCaches[(UINT)_vft]->Insert(_skinId, _type, _vft);
    VertexCache* vertexCache = m_VertexFormatCaches[(UINT)_vft]->GetVertexCache(_skinId, _type);//m_VertexFormatsTree->GetVertexCache(_vft);

    StaticRendering staticRendering;
    staticRendering.BaseVertexIndex = _startVertex;
    staticRendering.StartIndex = vertexCache->GetNumStaticIndices();
    if (_index) {
        vertexCache->AddStaticIndices (_index, _numIndices);
    }
    staticRendering.VertexBufferId = _vertexBufferId;
    staticRendering.IndexBufferId = INVALID_ID;
    staticRendering.NumPrimitives = _numPrimitives;

    vertexCache->AddStaticRendering (staticRendering);
}

void VertexCacheManager::Render (PRIMITIVETYPE _type,
                                 void* _vertex, UINT _numVertices, 
                                 WORD* _index, UINT _numIndices, 
                                 VERTEXFORMATTYPE _vft, UINT _skinId) {
    m_VertexFormatCaches[(UINT)_vft]->Insert(_skinId, _type, _vft);
    VertexCache* vertexCache = m_VertexFormatCaches[(UINT)_vft]->GetVertexCache(_skinId, _type);//m_VertexFormatsTree->GetVertexCache(_vft);

    vertexCache->Add (_vertex, _numVertices, _index, _numIndices);
    //m_VertexFormatsTree->GetAvlTreeRenderElements(_vft)->Insert(_SkinId, info);
}

void VertexCacheManager::RenderParticles (ULCVERTEX* _particle, UINT _numParticles,
                                          UINT _bufferId, UINT _skinId) {
    if (_bufferId >= m_ParticleBuffers.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    SetSkin (_skinId);
    m_Device->SetVertexDeclaration (VF_ULCVERTEX);
    LPDIRECT3DVERTEXBUFFER9 vb = m_ParticleBuffers[_bufferId].Buffer;
    UINT vertexSize = sizeof (ULCVERTEX);
    if (FAILED (m_Device->SetStreamSource (0, vb, 0, vertexSize))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetStreamSource() failure.");
    }
    void* data = NULL;
    while (_numParticles > 0) {
        if (FAILED (vb->Lock (0, 0, &data, D3DLOCK_DISCARD))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
        }
        UINT primitiveCount = 0;
        if (_numParticles < m_ParticleBuffers[_bufferId].Size) {
            memcpy (data, _particle, _numParticles * vertexSize);
            primitiveCount = _numParticles;
            _numParticles = 0;
        } else {
            memcpy (data, _particle, m_ParticleBuffers[_bufferId].Size * vertexSize);
            _numParticles -= m_ParticleBuffers[_bufferId].Size;
            _particle += m_ParticleBuffers[_bufferId].Size;
            primitiveCount = m_ParticleBuffers[_bufferId].Size;
        }
        vb->Unlock ();
        if (FAILED (m_Device->DrawPrimitive (D3DPT_POINTLIST, 0, primitiveCount))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "DrawPrimitive() failure.");
        }
    }
}

UINT VertexCacheManager::CreateEffect (void* _effectData, UINT _dataSize, 
                                       bool _isFromFile) {
    EffectData effectData;
    ZeroMemory (&effectData, sizeof (effectData));
    if (_isFromFile) {
        if (FAILED (D3DXCreateEffectFromFileA (m_Device, (char*)_effectData, 
                            NULL, NULL, 0, NULL, &effectData.Effect, NULL))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateEffectFromFile() failure.");
        }
    } else {
        HRESULT hr;
        if (FAILED (hr = D3DXCreateEffect (m_Device, _effectData, _dataSize, 
                            NULL, NULL, 0, NULL, &effectData.Effect, NULL))) {
            if (hr == D3DXERR_INVALIDDATA) {
                m_Log->Log ("invalid data\n");
            }
            if (hr == D3DERR_INVALIDCALL) {
                m_Log->Log ("invalid call\n");
            }
            if (hr == E_OUTOFMEMORY) {
                m_Log->Log ("no memory\n");
            }
            if (hr == E_NOTIMPL) {
                m_Log->Log ("not impl\n");
            }
            THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateEffect() failure.");
        }
    }
    try {
        m_Effects.push_back (effectData);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }

    return m_Effects.size() - 1;
}

void VertexCacheManager::EnableEffect (UINT _effectId, const char* _techniqueName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    Flush ();
    if (FAILED (m_Effects[_effectId].Effect->SetTechnique (_techniqueName))) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
    m_ActiveEffect = &m_Effects[_effectId];
}

void VertexCacheManager::SetEffectTextureParamName (UINT _effectId, UINT _stage, 
                                                    const char* _texParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (_stage >= 8) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].TextureHandle[_stage] = m_Effects[_effectId].Effect->GetParameterByName (NULL, _texParamName);
}
void VertexCacheManager::SetEffectTexture (UINT _effectId, const char* _texParamName, UINT _textureId) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    D3DXHANDLE texture = m_Effects[_effectId].Effect->GetParameterByName (NULL, _texParamName);
    if (FAILED (m_Effects[_effectId].Effect->SetTexture (texture, (LPDIRECT3DTEXTURE9)m_Skin->GetTexture(_textureId)))) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}


void VertexCacheManager::SetEffectMtrlDiffuseParamName (UINT _effectId, const char* _diffuseParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].MtrlDiffuseHandle = m_Effects[_effectId].Effect->GetParameterByName (NULL, _diffuseParamName);
}

void VertexCacheManager::SetEffectMtrlAmbientParamName (UINT _effectId, const char* _ambientParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].MtrlAmbientHandle = m_Effects[_effectId].Effect->GetParameterByName (NULL, _ambientParamName);
}

void VertexCacheManager::SetEffectMtrlSpecularParamName (UINT _effectId, const char* _specularParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].MtrlSpecularHandle = m_Effects[_effectId].Effect->GetParameterByName (NULL, _specularParamName);
}

void VertexCacheManager::SetEffectMtrlEmissiveParamName (UINT _effectId, const char* _emissiveParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].MtrlEmissiveHandle = m_Effects[_effectId].Effect->GetParameterByName (NULL, _emissiveParamName);
}

void VertexCacheManager::SetEffectMtrlPowerParamName (UINT _effectId, const char* _powerParamName) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Effects[_effectId].MtrlPowerHandle = m_Effects[_effectId].Effect->GetParameterByName (NULL, _powerParamName);
}

void VertexCacheManager::SetEffectParameter (UINT _effectId, const char* _parameterName, void* _value) {
    if (_effectId >= m_Effects.size ()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (FAILED (m_Effects[_effectId].Effect->SetValue (_parameterName, _value, D3DX_DEFAULT))) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void VertexCacheManager::DisableEffects () {
    Flush ();
    m_Device->SetVertexShader (NULL);
    m_ActiveEffect = NULL;
}

void VertexCacheManager::SetTextSize (int _size) {
    m_FontSize = _size;
    delete m_Font;
    m_Font = NULL;
}

void VertexCacheManager::SetTextStyle (const char* _style) {
    strcpy (m_FontStyle, _style);
    delete m_Font;
    m_Font = NULL;
}

TCHAR* GetWideStr (const char* _strC, TCHAR(& _str)[MAX_PATH]) {
    int size = strlen (_strC) +  1;
    if (MultiByteToWideChar(CP_ACP, 0, _strC, -1, _str, size) == 0) {
        return NULL;
    }
    
    return _str;
}

void VertexCacheManager::RenderText (const char* _text, DWORD _color, float _x, float _y) {
    TCHAR wideStr[MAX_PATH];
    if (!m_Font) {
        try {
            m_Font = new CD3DFont (GetWideStr(m_FontStyle, wideStr), m_FontSize, 0);
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
        if (FAILED (m_Font->InitDeviceObjects (m_Device))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Font's InitDeviceObjects() failure.");
        }
        if (FAILED (m_Font->RestoreDeviceObjects ())) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Font's RestoreDeviceObjects() failure.");
        }
    }
    
    if (FAILED (m_Font->DrawTextW(_x, _y, _color, GetWideStr(_text, wideStr), 0))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: DrawText failed. (VertexCacheManager::RenderText)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Font's DrawText() failure.");;
    }
}

void VertexCacheManager::SetSkin (UINT _skinId) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Device is not created. (VertexCacheManager::SetSkin)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    if (m_ActiveSkin == _skinId) {
        if (_skinId != INVALID_ID) {
            return;
        }
    }
    m_ActiveSkin = INVALID_ID;

    if (_skinId == INVALID_ID) {    // no texture or material specified
        for (UINT i = 0; i < 8; i++) {
            if (FAILED (m_Device->SetTexture (i, NULL))) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: SetTexture failed. (VertexCacheManager::SetSkin)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
            }
            if (m_ActiveEffect) {
                if (m_ActiveEffect->TextureHandle[i]) {
                    if (FAILED (m_ActiveEffect->Effect->SetTexture (m_ActiveEffect->TextureHandle[i], NULL))) {
                        #ifdef _DEBUG
                        if (m_Log) {
                            m_Log->Log ("Error: Effect's SetTexture() failure. (VertexCacheManager::SetSkin)\n");
                        }
                        #endif
                        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
                    }
                }
            }
        }
        // set default material
        D3DMATERIAL9 d3dmaterial;
        D3DCOLORVALUE diffuse, color;
        diffuse.r = diffuse.g = diffuse.b = 1.0f;
        diffuse.a = 0.0f;
        color.r = color.g = color.b = color.a = 0.0f;
        d3dmaterial.Diffuse = diffuse;
        d3dmaterial.Ambient = color;
        d3dmaterial.Specular = color;
        d3dmaterial.Emissive = color;
        d3dmaterial.Power = 0.0f;
        if (FAILED (m_Device->SetMaterial (&d3dmaterial))) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: SetMaterial failed. (VertexCacheManager::SetSkin)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "SetMaterial() failure.");
        }
        if (m_ActiveEffect) {
            HRESULT hrd = S_OK, hra = S_OK, hrs = S_OK, hre = S_OK, hrp = S_OK;
            if (m_ActiveEffect->MtrlDiffuseHandle) {
                hrd = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlDiffuseHandle, (D3DXVECTOR4*)&diffuse);
            }
            if (m_ActiveEffect->MtrlAmbientHandle) {
                hra = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlAmbientHandle, (D3DXVECTOR4*)&color);
            }
            if (m_ActiveEffect->MtrlSpecularHandle) {
                hrs = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlSpecularHandle, (D3DXVECTOR4*)&color);
            }
            if (m_ActiveEffect->MtrlEmissiveHandle) {
                hre = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlEmissiveHandle, (D3DXVECTOR4*)&color);
            }
            if (m_ActiveEffect->MtrlPowerHandle) {
                hrp = m_ActiveEffect->Effect->SetFloat (m_ActiveEffect->MtrlPowerHandle, 0.0f);
            }
            if (FAILED (hrd) || FAILED (hra) || FAILED (hrs) || FAILED (hre) || FAILED (hrp)) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: Failed to set effect's  material. (VertexCacheManager::SetSkin)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetVector() failure.");
            }
        }
        return;
    }

    SKIN skin = m_Skin->GetSkin (_skinId);
    for (UINT i = 0; i < skin.NumTextures; i++) {
        if (skin.TextureId[i] != INVALID_ID) {
            if (FAILED (m_Device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetSamplerState() failure.");
            }
            if (FAILED (m_Device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetSamplerState() failure.");
            }
            if (FAILED (m_Device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetSamplerState() failure.");
            }
            if (FAILED (m_Device->SetTexture(i, NULL))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
            }
            IDirect3DTexture9* texture = (IDirect3DTexture9*) m_Skin->GetTexture (skin.TextureId[i]);
            if (FAILED (m_Device->SetTexture (i, texture))) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: SetTexture failed. (VertexCacheManager::SetSkin)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
            }
            if (m_ActiveEffect) {
                if (m_ActiveEffect->TextureHandle[i]) {
                    if (FAILED (m_ActiveEffect->Effect->SetTexture (m_ActiveEffect->TextureHandle[i], texture))) {
                        #ifdef _DEBUG
                        if (m_Log) {
                            m_Log->Log ("Error: SetTexture failed. (VertexCacheManager::SetSkin)\n");
                        }
                        #endif
                        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
                    }
                }
            }
        }
    }
    if (skin.MaterialId != INVALID_ID) {
        D3DMATERIAL9 d3dmaterial;
        MATERIAL material = m_Skin->GetMaterial (skin.MaterialId);
        D3DCOLORVALUE color;
        memcpy (&color.r, &material.Ambient.r, sizeof (float) * 4);
        d3dmaterial.Ambient = color;
        memcpy (&color.r, &material.Diffuse.r, sizeof (float) * 4);
        d3dmaterial.Diffuse = color;
        memcpy (&color.r, &material.Emissive.r, sizeof (float) * 4);
        d3dmaterial.Emissive = color;
        memcpy (&color.r, &material.Specular.r, sizeof (float) * 4);
        d3dmaterial.Specular = color;
        d3dmaterial.Power = material.Power;
        if (m_Device->SetMaterial (&d3dmaterial)) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: SetMaterial failed. (VertexCacheManager::SetSkin)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "SetMaterial() failure.");;
        }
        if (m_ActiveEffect) {
            HRESULT hrd = S_OK, hra = S_OK, hrs = S_OK, hre = S_OK, hrp = S_OK;
            if (m_ActiveEffect->MtrlDiffuseHandle) {
                hrd = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlDiffuseHandle, (D3DXVECTOR4*)&d3dmaterial.Diffuse);
            }
            if (m_ActiveEffect->MtrlAmbientHandle) {
                hra = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlAmbientHandle, (D3DXVECTOR4*)&d3dmaterial.Ambient);
            }
            if (m_ActiveEffect->MtrlSpecularHandle) {
                hrs = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlSpecularHandle, (D3DXVECTOR4*)&d3dmaterial.Specular);
            }
            if (m_ActiveEffect->MtrlEmissiveHandle) {
                hre = m_ActiveEffect->Effect->SetVector (m_ActiveEffect->MtrlEmissiveHandle, (D3DXVECTOR4*)&d3dmaterial.Emissive);
            }
            if (m_ActiveEffect->MtrlPowerHandle) {
                hrp = m_ActiveEffect->Effect->SetFloat (m_ActiveEffect->MtrlPowerHandle, d3dmaterial.Power);
            }
            if (FAILED (hrd) || FAILED (hra) || FAILED (hrs) || FAILED (hre) || FAILED (hrp)) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: Failed to set effect's  material. (VertexCacheManager::SetSkin)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetVector() failure.");
            }
        }
    }

    m_ActiveSkin = _skinId;
}

UINT VertexCacheManager::GetActiveSkinId () const {
    return m_ActiveSkin;
}

void VertexCacheManager::Flush () {
    for (UINT i = 0; i < NUM_VERTEX_FORMATS; i++) {
        Flush (m_VertexFormatCaches[i]->GetRoot());
    }
    m_ActiveStaticVertexBuffer = INVALID_ID;
    m_ActiveCacheVertexBuffer = INVALID_ID;

    m_ActiveStaticIndexBuffer = INVALID_ID;
    m_ActiveCacheIndexBuffer = INVALID_ID;
}

void VertexCacheManager::Flush (RenderCacheNS::Node* _node) {
    if (_node) {
        Flush(_node->Left);
        Flush(_node->Right);

        if (GetActiveSkinId() != _node->Value.SkinId) {
            SetSkin(_node->Value.SkinId);
        }
        for (UINT i = 0; i < _node->Value.PrimitiveCaches.size(); i++) {
            PRIMITIVETYPE type = _node->Value.PrimitiveCaches[i]->Type;
            _node->Value.PrimitiveCaches[i]->Cache->Render();
        }
    }
}

UINT VertexCacheManager::GetVertexSize (VERTEXFORMATTYPE _vft) {
    UINT vertexSize;
    switch (_vft) {
        case VFT_UP:
            vertexSize = sizeof (UPVERTEX);
            break;
        case VFT_UU:
            vertexSize = sizeof (UUVERTEX);
            break;
        case VFT_UU2:
            vertexSize = sizeof (UUVERTEX2);
            break;
        case VFT_UL:
            vertexSize = sizeof (ULVERTEX);
            break;
        case VFT_UL2:
            vertexSize = sizeof (ULVERTEX2);
            break;
        case VFT_ULC:
            vertexSize = sizeof (ULCVERTEX);
            break;
        case VFT_TP:
            vertexSize = sizeof (TPVERTEX);
            break;
        case VFT_TL:
            vertexSize = sizeof (TLVERTEX);
            break;
        case VFT_TLC:
            vertexSize = sizeof (TLCVERTEX);
            break;
        default:
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Unknown vertex format. (VertexCacheManager::GetVertexSize)\n");
            }
            #endif
            THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    return vertexSize;
}

IDirect3DVertexDeclaration9* VertexCacheManager::GetVertexDeclaration (VERTEXFORMATTYPE _vft) {
    IDirect3DVertexDeclaration9* vertexDecl;
    switch (_vft) {
        case VFT_UP:
            vertexDecl = VF_UPVERTEX;
            break;
        case VFT_UU:
            vertexDecl = VF_UUVERTEX;
            break;
        case VFT_UU2:
            vertexDecl = VF_UUVERTEX2;
            break;
        case VFT_UL:
            vertexDecl = VF_ULVERTEX;
            break;
        case VFT_UL2:
            vertexDecl = VF_ULVERTEX2;
            break;
        case VFT_ULC:
            vertexDecl = VF_ULCVERTEX;
            break;
        case VFT_TP:
            vertexDecl = VF_TPVERTEX;
            break;
        case VFT_TL:
            vertexDecl = VF_TLVERTEX;
            break;
        case VFT_TLC:
            vertexDecl = VF_TLCVERTEX;
            break;
        default:
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Unknown vertex format. (VertexCacheManager::GetVertexDeclaration)\n");
            }
            #endif
            THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    return vertexDecl;
}

ID3DXEffect* VertexCacheManager::GetActiveEffect () {
    if (m_ActiveEffect) {
        return m_ActiveEffect->Effect;
    }
    return NULL;
}

ID3DXEffect* VertexCacheManager::GetEffect (UINT _effectId) {
    if (_effectId >= m_Effects.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Effects[_effectId].Effect;
}