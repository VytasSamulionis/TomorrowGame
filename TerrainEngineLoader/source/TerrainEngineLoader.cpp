/*  Author: Vytas Samulionis
    Description: A class for loading a renderer dll.
*/
#include "../include/TerrainEngineLoader.h"

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
#endif

TerrainEngineLoader::TerrainEngineLoader (HINSTANCE _instance): m_Instance (_instance) {
    m_TerrainEngine = NULL;
    m_DLL = NULL;
    try {
        #ifdef _DEBUG
        m_Log = new LogManager ("log_terrain_engine_loader.txt", true);
        #else
        m_Log = NULL;
        #endif
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("TerrainEngineLoader is up and running.\n");
    }
    #endif
}

TerrainEngineLoader::~TerrainEngineLoader () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("TerrainEngineLoader is shutting down...\n");
    }
    #endif
    if (m_TerrainEngine) {
        Release ();
    }
    if (m_DLL) {
        if (!FreeLibrary (m_DLL)) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: attempt to free TerrainEngine.dll failed.\n");
            }
            #endif
            THROW_DETAILED_ERROR (ERRC_API_CALL, "FreeLibrary() failure.");
        }
    }
    #ifdef _DEBUG
    if (m_Log) {
        delete m_Log;
        m_Log = NULL;
    }
    #endif
}

// loads rendering dll and inits m_Device
void TerrainEngineLoader::CreateTerrainEngine (RenderDevice* _device) {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Creating device...\n");
    }
    #endif
    m_DLL = LoadLibrary ("TerrainEngine.dll");
    if (!m_DLL) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot open TerrainEngine.dll.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "LoadLibrary() failure.");
    }
    // gets function pointer
    CREATETERRAINENGINE CreateEngine = 
        (CREATETERRAINENGINE) GetProcAddress (m_DLL, "CreateTerrainEngine");
    if (!CreateEngine) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot find function CreateTerrainEngine () in the DLL.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failure.");
    }
    
    HRESULT hr = CreateEngine (_device, &m_TerrainEngine);
    if (FAILED (hr)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot create Terrain Engine.\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Terrain Engine created.\n");
    }
    #endif
}

// returns m_Device
TerrainEngine* TerrainEngineLoader::GetTerrainEngine () const {
    return m_TerrainEngine;
}

// returns handle to a rendering dll
HINSTANCE TerrainEngineLoader::GetModule () const {
    return m_DLL;
}


// releases m_Device
void TerrainEngineLoader::Release () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Releasing Terrain Engine...\n");
    }
    #endif
    // gets function pointer
    RELEASETERRAINENGINE ReleaseTerrainEngine = NULL;
    if (m_DLL) {
        ReleaseTerrainEngine = 
            (RELEASETERRAINENGINE) GetProcAddress (m_DLL, "ReleaseTerrainEngine");
    }
    if (!ReleaseTerrainEngine) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot find function ReleaseTerrainEngine in the DLL.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failure.");
    }
    if (m_TerrainEngine) {
        ReleaseTerrainEngine (&m_TerrainEngine);
    }
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Terrain Engine released.\n");
    }
    #endif
}