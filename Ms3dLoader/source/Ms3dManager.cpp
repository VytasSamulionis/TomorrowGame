#include "../include/Ms3dManager.h"

using namespace ms3d;

Ms3dLoader::Ms3dLoader () {
    m_Log = NULL;
}

Ms3dLoader::Ms3dLoader (LogManager* _log) {
    m_Log = _log;
}

Ms3dLoader::Ms3dLoader (const char* _logFile) {
    try {
        m_Log = new LogManager (_logFile);
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (Ms3dLoader::Ms3dLoader (const char*))\n");
            }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

Ms3dLoader::~Ms3dLoader () {
    UnloadModels ();
    delete m_Log;
    m_Log = NULL;
}

UINT Ms3dLoader::LoadModel (const char* _modelFile) {
    UINT id = m_Models.size();
    try {
        Ms3dModel* model = new Ms3dModel (m_Log);
        model->Load (_modelFile);
        m_Models.push_back (model);
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (Ms3dLoader::Ms3dLoader (const char*))\n");
            }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }

    return id;
}

const char* Ms3dLoader::GetModelFile (UINT _id) const {
    if (_id >= m_Models.size()) {
        #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: ID is out of range. (Ms3dLoader::GetModelFile)\n");
            }
        #endif
        THROW_DETAILED_ERROR (ERRC_OUT_OF_RANGE, "Invalid ms3d model ID.");
    }
    return m_Models[_id]->GetFilename ();
}

Ms3dModel* Ms3dLoader::GetModel (UINT _id) const {
    if (_id >= m_Models.size()) {
        #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: ID is out of range. (Ms3dLoader::GetModel)\n");
            }
        #endif
        THROW_DETAILED_ERROR (ERRC_OUT_OF_RANGE, "Invalid ms3d model ID.");
    }
    return m_Models[_id];
}

void Ms3dLoader::UnloadModels () {
    for (UINT i = 0; i < m_Models.size(); i++) {
        delete m_Models[i];
    }
    m_Models.clear();
}