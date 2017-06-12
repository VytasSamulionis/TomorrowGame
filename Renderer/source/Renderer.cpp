/*  Author: Vytas Samulionis
    Description: A class for an encapsulation of rendering.
*/
#include "../include/Renderer.h"

using namespace vs3d;

Renderer::Renderer (HINSTANCE _dll): m_DLL (_dll) {
    try {
        #ifdef _DEBUG
        m_Log = new LogManager ("log_renderer.txt", true);
        #else
        m_Log = NULL;
        #endif
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    m_IsRunning = false;
    m_IsRendering = false;
    m_d3d9 = NULL;
    m_Device = NULL;
    m_Skin = NULL;
    m_vcm = NULL;
    m_ClearColor = D3DCOLOR_COLORVALUE (1.0f, 1.0f, 1.0f, 1.0f);
    m_World.identity();
    ZeroMemory (&m_d3dpp, sizeof (D3DPRESENT_PARAMETERS));
    ZeroMemory (&m_ShadowMap, sizeof (m_ShadowMap));
    m_ShadowMap.EffectId = INVALID_ID;
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Renderer is up and running.\n");
    }
    #endif
}

Renderer::~Renderer () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Renderer is shutting down...\n");
    }
    #endif
    Release ();
}

//  inits full screen directx window
//  _hwnd - handle to window
//  _Width - width of screen
//  _Height - height of screen
void Renderer::InitFullScreen (HWND _hwnd, UINT _width, UINT _height) {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Starting full sreen mode initializing...\n");
    }
    #endif

    // shutdown if running
    if (m_IsRunning) {
        if (m_d3d9) {
            m_d3d9->Release ();
        }
        if (m_Device) {
            m_Device->Release ();
        }
    }

    m_d3d9 = Direct3DCreate9 (D3D_SDK_VERSION);
    if (!m_d3d9) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Direct3DCreate9 call failed.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Direct3DCreate9 failure.");
    }
    // Fill m_d3dpp structure
    m_d3dpp.Windowed = false;
    m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.BackBufferCount = 1;
    m_d3dpp.BackBufferHeight = _height;
    m_d3dpp.BackBufferWidth = _width;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    m_d3dpp.FullScreen_RefreshRateInHz = 60;
    m_d3dpp.hDeviceWindow = _hwnd;

    m_Width = _width;
    m_Height = _height;

    // check if device supports vertex processing
    D3DCAPS9 caps;
    m_d3d9->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    int vertexProc; // vertex processing
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
        vertexProc = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    } else {
        vertexProc = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    HRESULT hr = m_d3d9->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
        _hwnd, vertexProc, &m_d3dpp, &m_Device);
    if (FAILED(hr)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateDevice call failed.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateDevice failure.");
    }

    m_IsRunning = true;
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Full screen mode initialization finished successfully.\n");
    }
    #endif
    InitManagers ();
    
    DefaultInit ();
}

// windowed mode setup
// _hwnd - instance to window
void Renderer::InitWindowed (HWND _hwnd, UINT _width, UINT _height) {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Starting windowed mode initializing...\n");
    }
    #endif

    // shutdown if running
    if (m_IsRunning) {
        if (m_d3d9) {
            m_d3d9->Release ();
        }
        if (m_Device) {
            m_Device->Release ();
        }
    }

    m_d3d9 = Direct3DCreate9 (D3D_SDK_VERSION);
    if (!m_d3d9) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Direct3DCreate9 call failed.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Direct3DCreate9 failure.");
    }
    // Fill m_d3dpp structure
    m_d3dpp.Windowed = true;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    m_d3dpp.BackBufferWidth = _width;
    m_d3dpp.BackBufferHeight = _height;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    m_Width = _width;
    m_Height = _height;

    // check if device supports vertex processing
    D3DCAPS9 caps;
    m_d3d9->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    int vertexProc; // vertex processing
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
        vertexProc = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    } else {
        vertexProc = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    HRESULT hr = m_d3d9->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
        _hwnd, vertexProc, &m_d3dpp, &m_Device);
    if (FAILED(hr)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: CreateDevice call failed.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateDevice failure.");
    }

    m_IsRunning = true;
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Windowed mode initialization finished successfully.\n");
    }
    #endif
    InitManagers ();
    
    DefaultInit ();
}

// changes clear color
void Renderer::SetClearColor (float _red, float _green, float _blue) {    
    m_ClearColor = D3DCOLOR_COLORVALUE (_red, _green, _blue, 0);
}

// returns status
bool Renderer::IsRunning () const {
    return m_IsRunning;
}

// returns if rendering has began
bool Renderer::IsRendering () const {
    return m_IsRendering;
}

// clears window
// _Target - clear back buffer
// _Stencil - clear stencil
// _ZBuffer - clear z buffer
void Renderer::Clear (bool _target, bool _stencil, bool _zBuffer) {
    if (!m_IsRunning) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: trying to clear buffers when window is not initialized.\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    DWORD flags = 0;
    if (_target) {
        flags |= D3DCLEAR_TARGET;
    }
    if (_stencil) {
        flags |= D3DCLEAR_STENCIL;
    }
    if (_zBuffer) {
        flags |= D3DCLEAR_ZBUFFER;
    }
    if (FAILED (m_Device->Clear (0, NULL, flags, m_ClearColor, 1.0f, 0))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Clear() failure.");
    }
}

// starts the rendering
// _Target - clear back buffer
// _Stencil - clear stencil
// _ZBuffer - clear z buffer
void Renderer::BeginRendering (bool _target, bool _stencil, bool _zBuffer) {
    if (!m_IsRunning) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: attempt to begin rendering while window is not initialized.\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    if (m_IsRendering) {
        EndRendering ();
    }
    if (_target || _stencil || _zBuffer) {
        Clear (_target, _stencil, _zBuffer);
    }
    HRESULT hr = m_Device->BeginScene ();
    if (FAILED (hr)) {
        m_IsRendering = false;
        THROW_DETAILED_ERROR (ERRC_API_CALL, "BeginScene() failure.");
    } else {
        m_IsRendering = true;
    }
}

// ends rendering
void Renderer::EndRendering () {
    if (!m_IsRunning) {
        if (m_Log) {
            m_Log->Log ("Error: attempt to end rendering while window is not initialized.\n");
        }
        return;
    }
    m_vcm->Flush ();
    m_Device->EndScene ();
    HRESULT hr = m_Device->Present (NULL, NULL, NULL, NULL);
    if (hr == D3DERR_DEVICELOST) {
        THROW_DETAILED_ERROR (ERRC_NOT_READY, "Device has been lost.");
    }
    m_IsRendering = false;
}

// releases resources
void Renderer::Release () {
    if (m_d3d9) {
        m_d3d9->Release ();
        m_d3d9 = NULL;
    }
    if (m_Device) {
        m_Device->Release ();
        m_Device = NULL;
    }
    if (m_Log) {
        delete m_Log;
        m_Log = NULL;
    }
    if (m_vcm) {
        delete m_vcm;
        m_vcm = NULL;
    }
    if (m_Skin) {
        delete m_Skin;
        m_Skin = NULL;
    }
    m_DLL = NULL;
    m_IsRunning = false;
    m_IsRendering = false;
}

// returns skin manager interface
ISkinManager* Renderer::GetSkinManager () const {
    return m_Skin;
}

// returns vertex cache manager interface
IVertexCacheManager* Renderer::GetVCacheManager () const {
    return m_vcm;
}

MATRIX44 Renderer::GetProjectionMatrix () const {
    return m_Projection;
}

MATRIX44 Renderer::GetViewMatrix () const {
    return m_View;
}

MATRIX44 Renderer::GetWorldMatrix () const {
    return m_World;
}

void Renderer::SetWorldMatrix (MATRIX44 _Matrix) {
    m_vcm->Flush ();
    m_World = _Matrix;
    if (FAILED (m_Device->SetTransform (D3DTS_WORLD, (D3DXMATRIX*)&m_World))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTransform() failure.");
    }
}

MATRIX44 Renderer::GetViewportMatrix (float _ScreenWidth, float _ScreenHeight) const {
    MATRIX44 viewport;
    cml::matrix_viewport (viewport, 0.0f, _ScreenWidth, 0.0f, _ScreenHeight, cml::z_clip_zero);
    return viewport;
}

void Renderer::DefaultInit () {
    SetProjectionView (D3DX_PI * 0.5f, 1.0f, 1000.0f);
    LookAt (VECTOR3 (0.0f, 0.0f, -2.0f), VECTOR3 (0.0f, 0.0f, 0.0f), VECTOR3 (0.0f, 1.0f, 0.0f));
    SetDrawingState (RS_DRAW_SOLID);
    SetShadingState (RS_SHADE_GOURAUD);
    EnableLighting (false);
}

// initializes skin and vertex cache managers
void Renderer::InitManagers () {
    try {
        if (m_Skin) {
            delete m_Skin;
        }
        m_Skin = new SkinManager (m_Device);
        if (m_vcm) {
            delete m_vcm;
        }
        m_vcm = new VertexCacheManager (m_Device, m_Skin, m_Log);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

// sets projection view
void Renderer::SetProjectionView (float _fovy, float _znear, float _zfar) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (Renderer::SetProjectionView)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    D3DXMatrixPerspectiveFovLH ((D3DXMATRIX*)&m_Projection, _fovy, (float) m_Width / m_Height, _znear, _zfar);
    if (FAILED (m_Device->SetTransform (D3DTS_PROJECTION, (D3DXMATRIX*)&m_Projection))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTransform() failure.");
    }
}

// sets view
void Renderer::LookAt (const VECTOR3& _eye, const VECTOR3& _at, const VECTOR3& _up) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (Renderer::LookAt)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    //D3DXVECTOR3 eye, at, up;
    //memcpy (&eye, &_Eye, 3 * sizeof (float));
    //memcpy (&at, &_At, 3 * sizeof (float));
    //memcpy (&up, &_Up, 3 * sizeof (float));
    D3DXMatrixLookAtLH ((D3DXMATRIX*)&m_View, (D3DXVECTOR3*)&_eye, (D3DXVECTOR3*)&_at, (D3DXVECTOR3*)&_up);
    if (FAILED (m_Device->SetTransform (D3DTS_VIEW, (D3DXMATRIX*)&m_View))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTransform() failure.");
    }
}

void Renderer::TranslateWorldMatrix (float _x, float _y, float _z) {
    m_vcm->Flush ();    // flush everything
    //D3DXMatrixTranslation (((D3DXMATRIX*)&m_World), _x, _y, _z);
    cml::matrix_set_translation (m_World, VECTOR3 (_x, _y, _z));
    if (FAILED(m_Device->SetTransform (D3DTS_WORLD, (D3DXMATRIX*)&m_World))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTransform() failure.");
    }
}

void Renderer::RotateWorldMatrix (const VECTOR3& _rotation) {
    m_vcm->Flush();
    MATRIX44 oldWorld = m_World;
    cml::matrix_rotation_euler (m_World, _rotation[0], _rotation[1], _rotation[2], cml::euler_order_xyz);
    // save position
    m_World(3, 0) = oldWorld(3, 0);
    m_World(3, 1) = oldWorld(3, 1);
    m_World(3, 2) = oldWorld(3, 2);
    m_World(3, 3) = oldWorld(3, 3);
    if (FAILED (m_Device->SetTransform (D3DTS_WORLD, (D3DXMATRIX*)&m_World))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTransform() failure.");
    }
}

// creates render device interface
HRESULT CreateRenderDevice (HINSTANCE _Instance, RenderDevice** _Interface) {
    if (*_Interface) {
        delete *_Interface;
        *_Interface = NULL;
    }
    try {
        *_Interface = new Renderer (_Instance);
    } catch (std::bad_alloc) {
        return E_FAIL;
    }
    return S_OK;
}

// releases render device
void ReleaseRenderDevice (RenderDevice** _Interface) {
    if (*_Interface) {
        delete *_Interface;
        *_Interface = NULL;
    }
}