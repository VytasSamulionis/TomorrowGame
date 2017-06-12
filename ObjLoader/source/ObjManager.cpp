#include "..\include\ObjManager.h"

ObjManager::ObjManager (RenderDevice* _device):
    m_Device (_device) {
        
    try {
        for (UINT i = 0; i < 6; i++) {
            std::vector<Buffer> renderingBuffers;
            Buffer buffer;
            buffer.BufferId = INVALID_ID;
            buffer.Num = 0;
            renderingBuffers.push_back (buffer);
            m_RenderingBuffers.push_back (renderingBuffers);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    m_IndexBuffer.BufferId = INVALID_ID;
    m_IndexBuffer.Num = 0;
}

ObjManager::~ObjManager () {
    UnloadAll ();
    /*#ifdef _DEBUG
        _CrtDumpMemoryLeaks();
    #endif*/
}

UINT ObjManager::Load (const char* _filename) {
    UINT numModels = m_Models.size();
    ObjModel* model = NULL;
    try {
        model = new ObjModel (m_Device, this);
        m_Models.push_back(model);
        m_Models[numModels]->Load (_filename);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    } catch (ErrorMessage e) {
        delete model;
        m_Models.pop_back ();
        throw e;
    }
    return numModels;
}

UINT ObjManager::Load (const char* _filename, UINT _skinId) {
    UINT modelId = Load (_filename);
    m_Models[modelId]->SetSkinId (_skinId);
    return modelId;
}

UINT ObjManager::AddModel (ObjModel* _model) {
    if (m_Models.size() == INVALID_ID - 1) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    try {
        m_Models.push_back (_model);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    return m_Models.size() - 1;
}

UINT ObjManager::AddModelToRenderingBuffer (void* _modelData, UINT _numVertices, VERTEXFORMATTYPE _vft, UINT& _startVertex) {
    UINT i = (UINT)_vft;
    if (i >= m_RenderingBuffers.size()) {
        THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    for (UINT j = 0; j < m_RenderingBuffers[i].size(); j++) {
        if (m_RenderingBuffers[i][j].BufferId == INVALID_ID) {
            m_RenderingBuffers[i][j].BufferId = m_Device->GetVCacheManager()->CreateStaticVertexBuffer(_modelData, _numVertices, _vft);
            _startVertex = m_RenderingBuffers[i][j].Num;
            m_RenderingBuffers[i][j].Num = _numVertices;
            return m_RenderingBuffers[i][j].BufferId;
        } else {
            try {
                m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_RenderingBuffers[i][j].BufferId, _modelData, _numVertices, _vft);
                _startVertex = m_RenderingBuffers[i][j].Num;
                m_RenderingBuffers[i][j].Num += _numVertices;
                return m_RenderingBuffers[i][j].BufferId;
            } catch (ErrorMessage e) {
                if (e.GetErrorCode () != ERRC_OUT_OF_MEM) { /* Static vertex buffer was not full. */
                    throw;
                }
            }
        }
    }
    Buffer newBuffer;
    newBuffer.BufferId = m_Device->GetVCacheManager()->CreateStaticVertexBuffer(_modelData, _numVertices, _vft);
    if (newBuffer.BufferId != INVALID_ID) {
        _startVertex = 0;
        newBuffer.Num = _numVertices;
        try {
            m_RenderingBuffers[i].push_back (newBuffer);
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    return newBuffer.BufferId;
}

UINT ObjManager::GetBufferNumVertices (VERTEXFORMATTYPE _vft, UINT _bufferId) {
    UINT i = (UINT)_vft;
    if (i >= m_RenderingBuffers.size()) {
        THROW_ERROR (ERRC_UNKNOWN_VF);
    }
    for (UINT j = 0; j < m_RenderingBuffers[i].size(); j++) {
        if (m_RenderingBuffers[i][j].BufferId == _bufferId) {
            return m_RenderingBuffers[i][j].Num;
        }
    }
    return 0;
}

UINT ObjManager::AddToIndexBuffer (WORD* _IndexData, UINT _NumIndices) {
    if (m_IndexBuffer.BufferId == INVALID_ID) {
        m_IndexBuffer.BufferId = m_Device->GetVCacheManager()->CreateStaticIndexBuffer(_IndexData, _NumIndices);
        m_IndexBuffer.Num = _NumIndices;
    } else {
        m_Device->GetVCacheManager()->AddToStaticIndexBuffer(m_IndexBuffer.BufferId, _IndexData, _NumIndices);
        m_IndexBuffer.Num += _NumIndices;
    }
    return m_IndexBuffer.BufferId;
}

UINT ObjManager::GetIndexBufferNumIndices () {
    return m_IndexBuffer.Num;
}

ObjModel* ObjManager::GetModel (UINT _id) {
    if (_id >= m_Models.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Models[_id];
}

void ObjManager::UnloadAll () {
    for (UINT i = 0; i < m_Models.size(); i++) {
        delete m_Models[i];
    }
    m_Models.clear ();
}
