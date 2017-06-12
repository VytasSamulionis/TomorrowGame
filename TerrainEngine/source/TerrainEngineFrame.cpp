#include "../include/TerrainEngineFrame.h"

TerrainEngineFrame::TerrainEngineFrame (RenderDevice* _device): m_Device (_device) {
    try {
        m_Terrain = new Terrain (_device);
        m_TerrainWater = new TerrainWater (_device, INVALID_ID);
        m_SkyBox = new SkyBox (_device);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

TerrainEngineFrame::~TerrainEngineFrame () {
    delete m_Terrain;
    m_Terrain = NULL;
    delete m_TerrainWater;
    m_TerrainWater = NULL;
    delete m_SkyBox;
    m_SkyBox = NULL;
}

ITerrain* TerrainEngineFrame::GetTerrain () const {
    return (ITerrain*)m_Terrain;
}

ISkyBox* TerrainEngineFrame::GetSkyBox () const {
    return m_SkyBox;
}

ISkyBox* TerrainEngineFrame::CreateSkyBox () const {
    SkyBox* skybox = NULL;
    try {
        skybox = new SkyBox (m_Device);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    return skybox;
}

ITerrainWater* TerrainEngineFrame::GetTerrainWater () const {
    return m_TerrainWater;
}

HRESULT CreateTerrainEngine (RenderDevice* _device, TerrainEngine** _interface) {
    if (*_interface) {
        delete *_interface;
        *_interface = NULL;
    }
    try {
        *_interface = new TerrainEngineFrame (_device);
    } catch (std::bad_alloc) {
        return E_FAIL;
    }
    return S_OK;
}

void ReleaseTerrainEngine (TerrainEngine** _interface) {
    if (*_interface) {
        delete *_interface;
        *_interface = NULL;
    }
}