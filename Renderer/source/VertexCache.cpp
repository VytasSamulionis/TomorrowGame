/*  Author: Vytas Samulionis
    Description: batches vertices and indices
*/

#include "../include/VertexCache.h"

using namespace vs3d;

VertexCache::VertexCache (VERTEXFORMATTYPE _vft,
                          PRIMITIVETYPE _type,
                          VertexCacheManager* _vcm,
                          UINT _skinId,
                          UINT _maxVertices,
                          UINT _maxIndices) {
    m_MaxVertices = _maxVertices;
    m_MaxIndices = _maxIndices;
    m_VertexFormat = _vft;
    m_SkinId = _skinId;
    m_Type = _type;
    m_IsEmpty = true;
    m_NumVertices = 0;
    m_NumIndices = 0;
    m_vb = NULL;
    m_ib = NULL;
    m_NumStaticVertices = 0;
    m_NumStaticIndices = 0;
    m_StaticVertexBuffer = NULL;
    m_StaticIndexBuffer = NULL;
    m_vcm = _vcm;

    m_Id = 100000 * (UINT)m_VertexFormat + 100 * (UINT)m_SkinId + (UINT)m_Type;

    m_VertexDecl = m_vcm->GetVertexDeclaration (m_VertexFormat);
    switch (m_VertexFormat) {
        case VFT_UP:
            m_VertexSize = sizeof (UPVERTEX);
            break;
        case VFT_UU:
            m_VertexSize = sizeof (UUVERTEX);
            break;
        case VFT_UU2:
            m_VertexSize = sizeof (UUVERTEX2);
            break;
        case VFT_UL:
            m_VertexSize = sizeof (ULVERTEX);
            break;
        case VFT_UL2:
            m_VertexSize = sizeof (ULVERTEX2);
            break;
        case VFT_ULC:
            m_VertexSize = sizeof (ULCVERTEX);
            break;
        case VFT_TP:
            m_VertexSize = sizeof (TPVERTEX);
            break;
        case VFT_TL:
            m_VertexSize = sizeof (TLVERTEX);
            break;
        case VFT_TLC:
            m_VertexSize = sizeof (TLCVERTEX);
            break;
        default:
            m_VertexSize = 0;
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Unknown vertex format. (VertexCache)\n");
            }
            #endif
            THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    
    int vertexSize = m_MaxVertices * m_VertexSize;
    int indexSize = m_MaxIndices * sizeof (WORD);
    
    if (FAILED (m_vcm->GetDirect3DDevice()->CreateVertexBuffer (vertexSize, D3DUSAGE_DYNAMIC, 0,
                                                                D3DPOOL_DEFAULT, &m_vb, NULL))) {
        #ifdef _DEBUG
        if (m_vcm->GetLog()) {
            m_vcm->GetLog()->Log ("Error: CreateVertexBuffer failed. (VertexCache)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexBuffer() failure.");
    }
    if (FAILED (m_vcm->GetDirect3DDevice()->CreateVertexBuffer (vertexSize, D3DUSAGE_DYNAMIC, 0,
                                                D3DPOOL_DEFAULT, &m_StaticVertexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_vcm->GetLog()) {
            m_vcm->GetLog()->Log ("Error: CreateVertexBuffer failed. (VertexCache)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateVertexBuffer() failure.");
    }
    if (FAILED (m_vcm->GetDirect3DDevice()->CreateIndexBuffer (indexSize, D3DUSAGE_DYNAMIC, D3DFMT_INDEX16,
                                                               D3DPOOL_DEFAULT, &m_ib, NULL))) {
        #ifdef _DEBUG
        if (m_vcm->GetLog()) {
            m_vcm->GetLog()->Log ("Error: CreateIndexBuffer failed. (VertexCache)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateIndexBuffer() failure.");
    }
    if (FAILED (m_vcm->GetDirect3DDevice()->CreateIndexBuffer (indexSize, D3DUSAGE_DYNAMIC, D3DFMT_INDEX16,
                                                D3DPOOL_DEFAULT, &m_StaticIndexBuffer, NULL))) {
        #ifdef _DEBUG
        if (m_vcm->GetLog()) {
            m_vcm->GetLog()->Log ("Error: CreateIndexBuffer failed. (VertexCache)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateIndexBuffer() failure.");
    }
    
    m_IsAddingWithoutIndices = true;
    m_IsFlushed = false;
}

VertexCache::~VertexCache () {
    if (m_vb) {
        m_vb->Release ();
        m_vb = NULL;
    }
    if (m_ib) {
        m_ib->Release ();
        m_ib = NULL;
    }
    if (m_StaticVertexBuffer) {
        m_StaticVertexBuffer->Release();
        m_StaticVertexBuffer = NULL;
    }
    if (m_StaticIndexBuffer) {
        m_StaticIndexBuffer->Release();
        m_StaticIndexBuffer = NULL;
    }
}

void VertexCache::Add (void* _vertex, UINT _numVertices,
                       WORD* _index, UINT _numIndices) {
    m_IsAddingWithoutIndices = true;
    if (_index) {
        m_IsAddingWithoutIndices = false;
    }
    AddVertices (_vertex, _numVertices);
    if (_index) {
        WORD* index = NULL;
        try {
            index = new WORD[_numIndices];
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
        memcpy (index, _index, _numIndices * sizeof (WORD));
        for (UINT i = 0; i < _numIndices; i++) {
            index[i] += m_NumVertices - _numVertices;
        }
        AddIndices (index, _numIndices);
        delete[] index;
    }
    m_IsEmpty = false;
}

void VertexCache::AddVertices (void* _vertex, UINT _numVertices) {
    /*if (m_VertexFormat == FVF_UL2) {
        for (UINT i = 0; i < _numVertices; i++) {
            m_vcm->GetLog()->Log("%f %f %f\n", ((ULVERTEX2*)_Vertex)[i].x, ((ULVERTEX2*)_Vertex)[i].y, ((ULVERTEX2*)_Vertex)[i].z);
        }
    }*/
    bool m_IsFlushed = false;
    if (m_NumVertices + _numVertices >= m_MaxVertices || m_NumIndices + _numVertices >= m_MaxIndices) {
        if (m_vcm->GetActiveSkinId() != m_SkinId) {
            m_vcm->SetSkin(m_SkinId);
        }
        Render ();
        m_IsFlushed = true;
    }
    UINT offset = m_NumVertices * m_VertexSize;
    UINT size = _numVertices * m_VertexSize;
    void* data;
  
    if (m_IsFlushed || offset == 0) {
        if (FAILED (m_vb->Lock (0, 0, &data, D3DLOCK_DISCARD))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::AddVertexBuffer)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
        }
    } else {
        if (FAILED (m_vb->Lock (offset, size, &data, D3DLOCK_NOOVERWRITE))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::AddVertexBuffer)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
        }
    }
    memcpy (data, _vertex, size);
    m_vb->Unlock ();
    m_NumVertices += _numVertices;

    if (m_IsAddingWithoutIndices) {
        AddIndices (_numVertices);
    }
    m_IsAddingWithoutIndices = true;
}

void VertexCache::AddIndices (WORD* _index, UINT _numIndices) {
    UINT offset = m_NumIndices * sizeof (WORD);
    UINT size = _numIndices * sizeof (WORD);
    void* data;
 
    if (m_IsFlushed || offset == 0) {
        if (FAILED (m_ib->Lock (0, 0, &data, D3DLOCK_DISCARD))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::Add)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
        }
    } else {
        if (FAILED (m_ib->Lock (offset, size, &data, D3DLOCK_NOOVERWRITE))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::Add)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
        }
    }
    memcpy (data, (void*)_index, size);
    m_ib->Unlock ();
    m_NumIndices += _numIndices;
}

void VertexCache::AddIndices (UINT _numIndices) {
    WORD* index = NULL;
    try {
        index = new WORD[_numIndices];
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    for (UINT i = 0; i < _numIndices; i++) {
        index[i] = m_NumVertices - _numIndices + i;
    }
    AddIndices (index, _numIndices);
    delete[] index;
}

void VertexCache::AddStaticVertices (void* _vertex, UINT _numVertices) {
    bool m_IsFlushed = false;
    if (m_NumStaticVertices + _numVertices >= m_MaxVertices || m_NumStaticIndices + _numVertices >= m_MaxIndices) {
        if (m_vcm->GetActiveSkinId() != m_SkinId) {
            m_vcm->SetSkin(m_SkinId);
        }
        Render ();
        m_IsFlushed = true;
    }
    UINT offset = m_NumStaticVertices * m_VertexSize;
    UINT size = _numVertices * m_VertexSize;
    void* data;
  
    if (m_IsFlushed || offset == 0) {
        if (FAILED (m_StaticVertexBuffer->Lock (0, 0, &data, D3DLOCK_DISCARD))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::AddVertexBuffer)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
        }
    } else {
        if (FAILED (m_StaticVertexBuffer->Lock (offset, size, &data, D3DLOCK_NOOVERWRITE))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::AddVertexBuffer)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Vertex buffer Lock() failure.");
        }
    }
    memcpy (data, _vertex, size);
    m_StaticVertexBuffer->Unlock ();
    m_NumStaticVertices += _numVertices;
}

void VertexCache::AddStaticIndices (WORD* _index, UINT _numIndices) {
    UINT offset = m_NumStaticIndices * sizeof (WORD);
    UINT size = _numIndices * sizeof (WORD);
    void* data;
 
    if (m_IsFlushed || offset == 0) {
        if (FAILED (m_StaticIndexBuffer->Lock (0, 0, &data, D3DLOCK_DISCARD))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::AddStaticIndices)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
        }
    } else {
        if (FAILED (m_StaticIndexBuffer->Lock (offset, size, &data, D3DLOCK_NOOVERWRITE))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Lock failed. (VertexCache::Add)\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "Index buffer Lock() failure.");
        }
    }
    memcpy (data, (void*)_index, size);
    m_StaticIndexBuffer->Unlock ();
    m_NumStaticIndices += _numIndices;
}

bool VertexCache::JoinStaticRendering (const StaticRendering& _staticRendering) {
    for (UINT i = 0; i < m_StaticRendering.size(); i++) {
        if (m_StaticRendering[i].VertexBufferId == _staticRendering.VertexBufferId &&
            m_StaticRendering[i].IndexBufferId == _staticRendering.IndexBufferId) {
                UINT primVertices = 0;
                switch (m_Type) {
                    case PT_POINT:
                        primVertices = 1;
                        break;
                    case PT_LINELIST:
                    case PT_LINESTRIP:
                        primVertices = 2;
                        break;
                    case PT_TRIANGLELIST:
                    case PT_TRIANGLESTRIP:
                        primVertices = 3;
                        break;
                }
                if (m_StaticRendering[i].IndexBufferId != INVALID_ID) {
                    //m_vcm->GetLog()->Log("%u %u %u\n", primVertices, m_StaticRendering[i].StartIndex + m_StaticRendering[i].NumPrimitives * primVertices, _staticRendering.StartIndex);
                    if (m_StaticRendering[i].StartIndex + m_StaticRendering[i].NumPrimitives * primVertices == _staticRendering.StartIndex) {
                        m_StaticRendering[i].NumPrimitives += _staticRendering.NumPrimitives;
                        return true;
                    }
                } else if ((m_StaticRendering[i].BaseVertexIndex + m_StaticRendering[i].NumPrimitives * primVertices) == _staticRendering.BaseVertexIndex) {
                    //m_vcm->GetLog()->Log("%u %u\n", m_StaticRendering[i].BaseVertexIndex + m_StaticRendering[i].NumPrimitives * primVertices, _staticRendering.BaseVertexIndex);
                    if (m_StaticRendering[i].StartIndex + m_StaticRendering[i].NumPrimitives * primVertices == _staticRendering.StartIndex) {
                        m_StaticRendering[i].NumPrimitives += _staticRendering.NumPrimitives;
                        return true;
                    }
                }
        }
    }
    return false;
}

void VertexCache::AddStaticRendering (const StaticRendering& _staticRendering) {
    // TODO: find out if it can be attached to existing static rendering
    try {
        if (!JoinStaticRendering(_staticRendering)) {
            m_StaticRendering.push_back (_staticRendering);
            m_IsEmpty = false;
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void VertexCache::SetVertexBuffer (UINT _id, bool _isForStatic) {
    //if (_isForStatic) {
    //    if (_Id == INVALID_ID) {
    //        //m_vcm->GetLog()->Log("Static vertex buffer\n");
    //        if (FAILED (m_vcm->GetDirect3DDevice()->SetStreamSource (0, m_StaticVertexBuffer, 0, m_VertexSize))) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: SetStreamSource failed. (VertexCache::SetVertexBuffer)\n");
    //                }
    //            #endif
    //            //m_vcm->SetActiveVertexBufferId (false, INVALID_ID);
    //            return VS3D_API_CALL_FAILED;
    //        }
    //        m_vcm->SetActiveVertexBufferId (false, INVALID_ID);
    //        return VS3D_OK;
    //    } else {
    //        LPDIRECT3DVERTEXBUFFER9 vertexBuffer = m_vcm->GetVertexBufferData(_Id);
    //        if (!vertexBuffer) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: GetVertexBufferData returned NULL (VertexCache::SetVertexBuffer)\n");
    //                }
    //            #endif
    //            return VS3D_FAIL;
    //        }
    //        //m_vcm->SetActiveVertexBufferId (true, _id);
    //        if (FAILED (m_vcm->GetDirect3DDevice()->SetStreamSource (0, vertexBuffer, 0, m_VertexSize))) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: SetStreamSource failed. (VertexCache::SetVertexBuffer)\n");
    //                }
    //            #endif
    //            //m_vcm->SetActiveVertexBufferId (true, INVALID_ID);
    //            return VS3D_API_CALL_FAILED;
    //        }
    //        m_vcm->SetActiveVertexBufferId (true, INVALID_ID);
    //        return VS3D_OK;
    //    }
    //}
    if (_id == INVALID_ID) {
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer = m_vb;
        if (_isForStatic) {
            vertexBuffer = m_StaticVertexBuffer;
        }
        if (m_vcm->GetActiveVertexBufferId (false, _isForStatic) != m_Id) {
            m_vcm->SetActiveVertexBufferId (false, m_Id, _isForStatic);
            if (FAILED (m_vcm->GetDirect3DDevice()->SetStreamSource (0, vertexBuffer, 0, m_VertexSize))) {
                #ifdef _DEBUG
                if (m_vcm->GetLog()) {
                    m_vcm->GetLog()->Log ("Error: SetStreamSource failed. (VertexCache::SetVertexBuffer)\n");
                }
                #endif
                m_vcm->SetActiveVertexBufferId (false, INVALID_ID);
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetStreamSource() failure.");
            }
        }
    } else {
        if (m_vcm->GetActiveVertexBufferId (true) != _id) {
            LPDIRECT3DVERTEXBUFFER9 vertexBuffer = m_vcm->GetVertexBufferData(_id);
            m_vcm->SetActiveVertexBufferId (true, _id);
            if (FAILED (m_vcm->GetDirect3DDevice()->SetStreamSource (0, vertexBuffer, 0, m_VertexSize))) {
                #ifdef _DEBUG
                if (m_vcm->GetLog()) {
                    m_vcm->GetLog()->Log ("Error: SetStreamSource failed. (VertexCache::SetVertexBuffer)\n");
                }
                #endif
                m_vcm->SetActiveVertexBufferId (true, INVALID_ID);
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetStreamSource() failure.");
            }
        }
    }
}

void VertexCache::SetIndexBuffer (UINT _id, bool _isForStatic) {
    //if (_IsStatic) {
    //    if (_Id == INVALID_ID) {
    //        if (FAILED (m_vcm->GetDirect3DDevice()->SetIndices (m_StaticIndexBuffer))) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: SetIndices failed. (VertexCache::Flush)\n");
    //                }
    //            #endif
    //            m_vcm->SetActiveIndexBufferId (false, INVALID_ID);
    //            return VS3D_API_CALL_FAILED;
    //        }
    //        return VS3D_OK;
    //    } else {
    //        LPDIRECT3DINDEXBUFFER9 indexBuffer = m_vcm->GetIndexBufferData (_Id);
    //        if (!indexBuffer) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: GetIndexBufferData returned NULL (VertexCache::Flush)\n");
    //                
    //                }
    //            #endif
    //            return VS3D_FAIL;
    //        }
    //        //m_vcm->SetActiveIndexBufferId (true, _id);
    //        //m_vcm->GetLog()->Log("Static index buffer\n");
    //        if (FAILED (m_vcm->GetDirect3DDevice()->SetIndices (indexBuffer))) {
    //            #ifdef _DEBUG
    //                if (m_vcm->GetLog()) {
    //                    m_vcm->GetLog()->Log ("Error: SetIndices failed. (VertexCache::Flush)\n");
    //                }
    //            #endif
    //            m_vcm->SetActiveIndexBufferId (true, INVALID_ID);
    //            return VS3D_API_CALL_FAILED;
    //        }
    //        m_vcm->SetActiveIndexBufferId (true, INVALID_ID);
    //        return VS3D_OK;
    //    }
    //}
    if (_id == INVALID_ID) {
        LPDIRECT3DINDEXBUFFER9 indexBuffer = m_ib;
        if (_isForStatic) {
            indexBuffer = m_StaticIndexBuffer;
        }
        /*if (m_vcm->GetActiveIndexBufferId(false, _isForStatic) != m_Id) {
            m_vcm->SetActiveIndexBufferId (false, m_Id, _isForStatic);*/
        if (FAILED (m_vcm->GetDirect3DDevice()->SetIndices (indexBuffer))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: SetIndices failed. (VertexCache::Flush)\n");
            }
            #endif
            m_vcm->SetActiveIndexBufferId (false, INVALID_ID);
            THROW_DETAILED_ERROR (ERRC_API_CALL, "SetIndices() failure.");
        }
        //}
    } else {
        //if (m_vcm->GetActiveIndexBufferId(true) != _id) {
        LPDIRECT3DINDEXBUFFER9 indexBuffer = m_vcm->GetIndexBufferData (_id);
        m_vcm->SetActiveIndexBufferId (true, _id);
        if (FAILED (m_vcm->GetDirect3DDevice()->SetIndices (indexBuffer))) {
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: SetIndices failed. (VertexCache::Flush)\n");
            }
            #endif
            m_vcm->SetActiveIndexBufferId (true, INVALID_ID);
            THROW_DETAILED_ERROR (ERRC_API_CALL, "SetIndices() failure.");
        }
        //}
    }
}

D3DPRIMITIVETYPE VertexCache::GetDirect3DType (PRIMITIVETYPE _type) {
    D3DPRIMITIVETYPE d3dpt;
    switch (_type) {
        case PT_POINT:
            d3dpt = D3DPT_POINTLIST;
            break;
        case PT_LINELIST:
            d3dpt = D3DPT_LINELIST;
            break;
        case PT_LINESTRIP:
            d3dpt = D3DPT_LINESTRIP;
            break;
        case PT_TRIANGLELIST:
            d3dpt = D3DPT_TRIANGLELIST;
            break;
        case PT_TRIANGLESTRIP:
            d3dpt = D3DPT_TRIANGLESTRIP;
            break;
        default:
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Unknown primitive type. (VertexCache::GetDirect3DType)\n");
            }
            #endif
            d3dpt = D3DPT_POINTLIST;
    }
    return d3dpt;
}

void VertexCache::Render (UINT _startVertex, UINT _startIndex, 
                          UINT _numPrimitives, bool _isIndexed, bool _isForStatic) {
    if (!m_IsEmpty) {
        ID3DXEffect* effect = m_vcm->GetActiveEffect ();
        UINT numPasses = 1;
        if (effect) {
            effect->Begin (&numPasses, 0);
        }
        for (UINT i = 0; i < numPasses; i++) {  /* run through passes */
            if (effect) {
                effect->BeginPass (i);
            }
            if (_isIndexed) {
                UINT numVertices = m_NumVertices;
                if (_isForStatic) {
                    numVertices = m_NumStaticVertices;
                }
                //m_vcm->GetLog()->Log("%u %u %u %u %u %u\n", numVertices, _startVertex, _startIndex, _numPrimitives, _isIndexed, _isForStatic);
                if (FAILED (m_vcm->GetDirect3DDevice()->DrawIndexedPrimitive (
                            GetDirect3DType (m_Type), 
                            _startVertex,
                            0, 
                            numVertices,
                            _startIndex,
                            _numPrimitives))) {
                    #ifdef _DEBUG
                    if (m_vcm->GetLog()) {
                        m_vcm->GetLog()->Log ("Error: DrawIndexedPrimitive failed. (VertexCache::Render)\n");
                    }
                    #endif
                    THROW_DETAILED_ERROR (ERRC_API_CALL, "DrawIndexedPrimitive() failure.");
                }
            } else {
                if (FAILED (m_vcm->GetDirect3DDevice()->DrawPrimitive (
                            GetDirect3DType (m_Type), 
                            _startVertex, 
                            _numPrimitives))) {
                    #ifdef _DEBUG
                    if (m_vcm->GetLog()) {
                        m_vcm->GetLog()->Log ("Error: DrawPrimitive failed. (VertexCache::Render)\n");
                    }
                    #endif
                    THROW_DETAILED_ERROR (ERRC_API_CALL, "DrawPrimitive() failure.");
                }
            }
            if (effect) {
                effect->EndPass ();
            }
        }
        if (effect) {
            effect->End ();
        }
    }
}

void VertexCache::Render () {
    if (!m_vcm->GetDirect3DDevice()) {
        #ifdef _DEBUG
        if (m_vcm->GetLog()) {
            m_vcm->GetLog()->Log ("Error: Device is not created. (VertexCache::Render)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    if (!m_IsEmpty) {
        try {
            SetVertexBuffer (INVALID_ID);
            SetIndexBuffer (INVALID_ID);
            m_vcm->GetDirect3DDevice()->SetVertexDeclaration (m_VertexDecl);
            Render (0, 0, GetPrimitiveCount (m_Type, m_NumIndices), true);
            for (UINT i = 0; i < m_StaticRendering.size(); i++) {
                SetVertexBuffer (m_StaticRendering[i].VertexBufferId, true);
                SetIndexBuffer (m_StaticRendering[i].IndexBufferId, true);
                bool isIndexed = m_StaticRendering[i].IndexBufferId != INVALID_ID;
                Render (m_StaticRendering[i].BaseVertexIndex, 
                        m_StaticRendering[i].StartIndex,
                        m_StaticRendering[i].NumPrimitives,
                        isIndexed, true);
            }
            m_NumIndices = 0;
            m_NumVertices = 0;
            m_StaticRendering.clear ();
            m_NumStaticVertices = m_NumStaticIndices = 0;
            m_IsEmpty = true;
        }  catch (ErrorMessage) {
            m_NumIndices = 0;
            m_NumVertices = 0;
            m_StaticRendering.clear ();
            m_NumStaticVertices = m_NumStaticIndices = 0;
            m_IsEmpty = true;
            throw;
        }
    }
}

UINT VertexCache::GetPrimitiveCount (PRIMITIVETYPE _type, UINT _number) {
    switch (_type) {
        case PT_POINT:
            return _number;
        case PT_LINELIST:
            return UINT (_number / 2);
        case PT_LINESTRIP:
            return _number - 1;
        case PT_TRIANGLELIST:
            return UINT (_number / 3);
        case PT_TRIANGLESTRIP:
            return _number - 2;
        default:
            #ifdef _DEBUG
            if (m_vcm->GetLog()) {
                m_vcm->GetLog()->Log ("Error: Unknown primitive type. (VertexCache::GetPrimitiveCount)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}
