/** @file TerrainEngineFrame.h */
#pragma once

#include "../../TerrainEngineLoader/include/TerrainEngine.h"
#include "../include/Terrain.h"
#include "../include/TerrainWater.h"
#include "../include/SkyBox.h"

/** TerrainEngine interface implementation. */
class TerrainEngineFrame: public TerrainEngine {
public:
    /** Constructor.
    @param[in] _device pointer to the RenderDevice
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    TerrainEngineFrame (RenderDevice* _device);

    /** Destructor. */
    ~TerrainEngineFrame ();

    /** Getter: terrain.
    @return terrain interface */
    ITerrain* GetTerrain () const;

    /** Getter: skybox.
    @return skybox interface. */
    ISkyBox* GetSkyBox () const;

    /** Getter: new skybox.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory

    @return new skybox interface.
    @warning The returned object has to be deleted manually. */
    ISkyBox* CreateSkyBox () const;

    /** Getter: terrain water.
    @return terrain water interface. */
    ITerrainWater* GetTerrainWater () const;
private:
    RenderDevice* m_Device;         /**< Pointer to the RenderDevice. */
    Terrain* m_Terrain;             /**< Terrain. */
    TerrainWater* m_TerrainWater;   /**< Terrain water. */
    SkyBox* m_SkyBox;               /**< Skybox. */
};

extern "C" __declspec (dllexport) HRESULT CreateTerrainEngine (RenderDevice* _device, TerrainEngine** _interface);
extern "C" __declspec (dllexport) void ReleaseTerrainEngine (TerrainEngine** _interface);