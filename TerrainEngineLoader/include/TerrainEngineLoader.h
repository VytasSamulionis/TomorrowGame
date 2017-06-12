/** @file TerrainEngineLoader.h */

#pragma once

#include "../include/TerrainEngine.h"
#include "../include/Log.h"

/** Terrain engine loader. */
class TerrainEngineLoader {
public:
    /** Constructor.
    @param[in] _instance instance
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    TerrainEngineLoader (HINSTANCE _instance);

    /** Destructor. 
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL window API call failure */
    ~TerrainEngineLoader ();

    /** Creates terrain engine.
    @param[in] _device pointer to the RenderDevice
    @exception ErrorMessage

    - Possible error codes:
        - ERRC_OUT_OF_MEM not enough memory */
    void CreateTerrainEngine (RenderDevice* _device);

    /** Getter: the pointer to the terrain engine.
    @return the pointer to the terrain engine */
    TerrainEngine* GetTerrainEngine () const;
    
    /** Getter: DLL.
    @return DLL */
    HINSTANCE GetModule () const;
    
    /** Releases resources. */
    void Release ();
    
private:
    TerrainEngine* m_TerrainEngine; /**< Interface class. */
    HINSTANCE m_Instance;           /**< Instance. */
    HMODULE m_DLL;                  /**< DLL instance. */

    LogManager* m_Log;              /**< Log manager. */
};