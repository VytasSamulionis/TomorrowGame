/*  Author: Vytas Samulionis
    Description: A class for loading a renderer dll.
*/
#include "../include/RendererLoader.h"

RendererLoader::RendererLoader (HINSTANCE _Instance): m_Instance (_Instance) {
    m_Device = NULL;
    m_DLL = NULL;
    try {
        #ifdef _DEBUG
        m_Log = new LogManager ("log_renderer_loader.txt", true);
        #else
        m_Log = NULL;
        #endif
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    #ifdef _DEBUG
    m_Log->Log ("RendererLoader is up and running.\n");
    #endif
}

RendererLoader::~RendererLoader () {
    #ifdef _DEBUG
    m_Log->Log ("RendererLoader is shutting down...\n");
    #endif
    if (m_Device) {
        delete m_Device;
        m_Device = NULL;
    }    
    if (m_DLL) {
        if (!FreeLibrary (m_DLL)) {
            #ifdef _DEBUG
            m_Log->Log ("Error: attempt to free Renderer.dll failed.\n");
            #endif
        }
    }
    if (m_Log) {
        delete m_Log;
        m_Log = NULL;
    }
}

// loads rendering dll and inits m_Device
void RendererLoader::CreateDevice () {
    #ifdef _DEBUG
    m_Log->Log ("Creating device...\n");
    #endif
    m_DLL = LoadLibrary ("Renderer.dll");
    if (!m_DLL) {
        #ifdef _DEBUG
        m_Log->Log ("Error: Cannot open Renderer.dll.\n");
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "LoadLibrary() failed.");
    }
    // gets function pointer
    CREATERENDERDEVICE CreateRenderDevice = 
        (CREATERENDERDEVICE) GetProcAddress (m_DLL, "CreateRenderDevice");
    if (!CreateRenderDevice) {
        #ifdef _DEBUG
        m_Log->Log ("Error: Cannot find function CreateRenderDevice () in the DLL.\n");
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failed.");
    }
    
    HRESULT hr = CreateRenderDevice (m_Instance, &m_Device);
    if (FAILED (hr)) {
        #ifdef _DEBUG
        m_Log->Log ("Error: Cannot create render device.\n");
        #endif
        THROW_DETAILED_ERROR (ERRC_OUT_OF_MEM, "CreateRenderDevice() failed.");
    }
    #ifdef _DEBUG
    m_Log->Log ("Device created.\n");
    #endif
}

// returns m_Device
RenderDevice* RendererLoader::GetDevice () const {
    return m_Device;
}

// returns handle to a rendering dll
HINSTANCE RendererLoader::GetModule () const {
    return m_DLL;
}


// releases m_Device
void RendererLoader::Release () {
    #ifdef _DEBUG
    m_Log->Log ("Releasing device...\n");
    #endif
    // gets function pointer
    RELEASERENDERDEVICE ReleaseRenderDevice = NULL;
    if (m_DLL) {
        ReleaseRenderDevice = 
            (RELEASERENDERDEVICE) GetProcAddress (m_DLL, "ReleaseRenderDevice");
    }
    if (!ReleaseRenderDevice) {
        #ifdef _DEBUG
        m_Log->Log ("Error: Cannot find function ReleaseRenderDevice in the DLL.\n");
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failed.");
    }
    if (m_Device) {
        ReleaseRenderDevice (&m_Device);
    }
    #ifdef _DEBUG
    m_Log->Log ("Device released.\n");
    #endif
}