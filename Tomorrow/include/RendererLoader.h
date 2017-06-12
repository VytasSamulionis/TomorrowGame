/** @file RendererLoader.h */
#pragma once

#include "../include/RenderDevice.h"
#include "../include/Log.h"

/** Loads the renderer and returns its interface. */
class RendererLoader {
public:
    /** Constructor.
    @param[in] _instance WinMain instance
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    RendererLoader (HINSTANCE _instance);

    /** Destructor. */
    ~RendererLoader ();

    /** Creates RenderDevice interface.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL the call to the windows API function failed */
    void CreateDevice ();

    /** Returns RenderDevice interface.
    @return RenderDevice interface */
    RenderDevice* GetDevice () const;

    /** Returns DLL handle.
    @return DLL handle */
    HINSTANCE GetModule () const;
    
    /** Releases resources.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL the call to the windows API function failed */
    void Release ();

private:
    RenderDevice* m_Device;     /**< Interface class. */
    HINSTANCE m_Instance;       /**< WinMain instance. */
    HMODULE m_DLL;              /**< DLL instance. */

    LogManager* m_Log;          /**< Log manager. */
};