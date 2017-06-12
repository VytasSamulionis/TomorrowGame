/** @file Renderer.h */

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../include/Log.h"
#include "../include/RenderDevice.h"
#include "../include/VertexCacheManager.h"

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

#pragma comment (lib, "legacy_stdio_definitions.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

struct ShadowMap {
    ID3DXRenderToSurface* RenderToSurface;
    IDirect3DTexture9* Texture;
    IDirect3DSurface9* Surface;
    IDirect3DSurface9* DepthStencil;
    IDirect3DSurface9* OldDepthStencil;
    UINT Size;
    UINT EffectId;
};

/** Renderer. */
class Renderer: public RenderDevice {
public:
    /** Constructor.
    @param[in] _dll handle to the renderer DLL 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    Renderer (HINSTANCE _dll);

    /** Destructor. */
    ~Renderer ();

    /** Initializes graphics API for full screen window.
    @param[in] _hwnd handle to the window
    @param[in] _width width of the window
    @param[in] _height height of the window 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_MEM not enough memory */
    void InitFullScreen (HWND _hwnd, UINT _width, UINT _height);
    
    /** Initializes graphics API for windowed screen window.
    @param[in] _hwnd handle to window
    @param[in] _width width of window
    @param[in] _height height of window
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void InitWindowed (HWND _hwnd, UINT _width, UINT _height);
    
    /** Changes clear color (background).
    Default clear color is white.
    @param[in] _red red color value in range of [0.0; -1.0]
    @param[in] _green green color value in range of [0.0; -1.0]
    @param[in] _blue blue color value in range of [0.0; -1.0] */
    void SetClearColor (float _red, float _green, float _blue);
    
    /** Checks if device is running and ready.
    @return @c true device is ready. @c false otherwise. */
    bool IsRunning () const;
    
    /** Checks if rendering is in progress.
    @return @c true rendering is in progress. @c false otherwise */
    bool IsRendering () const;
    
    /** Clears buffers.
    @param[in] _target target buffer should be cleared
    @param[in] _stencil stencil buffer should be cleared
    @param[in] _zBuffer zbuffer should be cleared 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL Clear() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    void Clear (bool _target, bool _stencil, bool _zBuffer);
    
    /** Clears buffers and starts rendering.
    @param[in] _target target buffer should be cleared
    @param[in] _stencil stencil buffer should be cleared
    @param[in] _zBuffer zbuffer should be cleared
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL
        - @c ERRC_NO_DEVICE device is not ready */
    void BeginRendering (bool _target, bool _stencil, bool _zBuffer);
    
    /** Ends rendering. */
    void EndRendering ();
    
    /** Releases resources. */
    void Release ();
    
    /** Default initialization for the rendering.
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    void DefaultInit ();

    // views

    /** Changes projection view.
    @param[in] _fovy field of view in the y direction, in radians
    @param[in] _znear z-value of the near view-plane
    @param[in] _zfar z-value of the far view-plane
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    void SetProjectionView (float _fovy, float _znear, float _zfar);
    
    /** Changes camera view.
    @param[in] _eye position of the camera
    @param[in] _at where to look
    @param[in] _up vector pointing upwards
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    void LookAt (const VECTOR3& _eye, const VECTOR3& _at, const VECTOR3& _up);
    
    // transformations

    /** Translates world matrix.
    @param[in] _x translation in x coordinate
    @param[in] _y translation in y coordinate
    @param[in] _z translation in z coordinate 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure */
    void TranslateWorldMatrix (float _x, float _y, float _z);
    
    /** Rotates world matrix.
    @param[in] _rotation euler angles 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure */
    void RotateWorldMatrix (const VECTOR3& _rotation);

    // rendering states

    /** Changes cull mode.
    Valid RENDERSTATETYPE values:
    - RS_CULL_CW
    - RS_CULL_CCW
    - RS_CULL_NONE
    @param[in] _state culling mode 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetCullingState (RENDERSTATETYPE _state);
    
    /** Changes depth buffer state.
    Valid RENDERSTATETYPE values:
    - RS_DEPTH_READWRITE
    - RS_DEPTH_READONLY
    - RS_DEPTH_NONE 
    @param[in] _state depth buffer state
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetDepthBufferState (RENDERSTATETYPE _state);

    /** Changes rendering mode.
    Valid RENDERSTATETYPE values:
    - RS_DRAW_POINTS
    - RS_DRAW_WIRE
    - RS_DRAW_SOLID
    @param[in] _state render mode
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetDrawingState (RENDERSTATETYPE _state);
    
    /** Changes stencil buffer mode.
    Valid RENDERSTATETYPE values:
    - RS_STENCIL_DISABLE
    - RS_STENCIL_ENABLE
    - RS_STENCIL_FUNC_ALWAYS
    - RS_STENCIL_FUNC_LESSEQUAL
    - RS_STENCIL_FAIL_DECR
    - RS_STENCIL_FAIL_INC
    - RS_STENCIL_FAIL_KEEP
    - RS_STENCIL_ZFAIL_DECR
    - RS_STENCIL_ZFAIL_INCR
    - RS_STENCIL_ZFAIL_KEEP
    - RS_STENCIL_PASS_DECR
    - RS_STENCIL_PASS_INCR
    - RS_STENCIL_PASS_KEEP
    @param[in] _state stencil buffer mode
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetStencilBufferState (RENDERSTATETYPE _state);
    
    /** Changes stencil buffer mode.
    Valid RENDERSTATETYPE values:
    - RS_STENCIL_MASK
    - RS_STENCIL_WRITEMASK
    - RS_STENCIL_REF
    @param[in] _state stencil buffer mode
    @param[in] _value value
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetStencilBufferState (RENDERSTATETYPE _state, DWORD _value);
    
    /** Changes shading mode.
    Valid RENDERSTATETYPE values:
    - RS_SHADE_FLAT
    - RS_SHADE_GOURAUD
    @param[in] _state shading mode 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetShadingState (RENDERSTATETYPE _state);
    
    /** Changes texture stage state.
    @param[in] _stage stage of a texture
    @param[in] _type what state to change
    @param[in] _value TEXTUREOP or TA value, depends on state
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTextureStageState() failure
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetTextureStageState (UINT _stage, TEXTURESTAGESTATETYPE _type, DWORD _value);
    
    /** Enables point sprites. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void EnablePointSprites ();

    /** Disables point sprites.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void DisablePointSprites ();

    /** Enables point scale. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void EnablePointsScale ();

    /** Disables point scale.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void DisablePointsScale ();

    /** Setter: point size.
    @param[in] _size size of the point 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void SetPointsSize (float _size);

    /** Changes point sprite state.
    @param[in] _type what state to change
    @param[in] _value value
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure 
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetPointSpriteState (POINTSPRITESTATETYPE _type, float _value);

    /** Changes alpha blend state.
    @param[in] _type what state to change
    @param[in] _blend blend type 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure 
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    void SetAlphaBlendState (ALPHABLENDSTATETYPE _type, BLENDTYPE _blend);

    /** Enables alpha blend.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void EnableAlphaBlend ();

    /** Disables alpha blend. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void DisableAlphaBlend ();

    // lighting

    /** Setter: ambient light.
    @param[in] _color ambient light's ARGB format color
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void SetAmbientLight (DWORD _color);

    /** Sets light.
    @param[in] _index light index [0-8]
    @param[in] _light light information 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetLight() failure 
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid _light parameter */
    void SetLight (DWORD _index, const vs3d::LIGHT& _light);
    
    /** Enable light.
    @param[in] _index which light
    @param[in] _enable enable light
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL LightEnable() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    void EnableLight (DWORD _index, bool _enable);
    
    /** Enable lighting.
    @param[in] _enable enable lighting 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_API_CALL SetRenderState() failure */
    void EnableLighting (bool _enable);

    void CreateShadowMap (UINT _size, const MATRIX44& _lightWorldView, float _farClip);

    void BeginRenderingToShadowMap ();

    void EndRenderingToShadowMap ();

    void SetShadowMap (UINT _effectId, const char* _shadowMapParamName);
    
    /** Enables fog.
    @param[in] _start the distance where the fog will start
    @param[in] _end the distance where the fog will end
    @param[in] _density fog density
    @param[in] _mode fog mode
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void EnableFog (float _start, float _end, float _density, FOGMODE _mode);

    /** Disables fog. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    void DisableFog ();

    // managers

    /** Returns pointer to SkinManager interface.
    @return pointer to SkinManager interface */
    ISkinManager* GetSkinManager () const;
    
    /** Returns pointer to VertexCacheManager interface.
    @return pointer to VertexCacheManager interface */
    IVertexCacheManager* GetVCacheManager () const;
    
    /** Getter: projection matrix.
    @return projection matrix */
    MATRIX44 GetProjectionMatrix () const;
    
    /** Getter: view matrix.
    @return view matrix */
    MATRIX44 GetViewMatrix () const;
    
    /** Getter: world matrix.
    @return world matrix */
    MATRIX44 GetWorldMatrix () const;
    
    /** Setter: world matrix.
    @param[in] _matrix world matrix
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetTransform() failure */
    void SetWorldMatrix (MATRIX44 _matrix);
    
    /** Getter: viewport matrix.
    @return viewport matrix */
    MATRIX44 GetViewportMatrix (float _screenWidth, float _screenHeight) const;

private:
    /** Initializes skin and vertex cache managers.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void InitManagers ();

    IDirect3D9* m_d3d9;             /**< DirectX. */
    IDirect3DDevice9* m_Device;     /**< Rendering device. */
    D3DPRESENT_PARAMETERS m_d3dpp;  /**< DirectX parameters. */
    HINSTANCE m_DLL;                /**< DLL. */

    D3DCOLOR m_ClearColor;      /**< Back buffer clear color. */
    UINT m_Width;               /**< Width of the window. */
    UINT m_Height;              /**< Height of the window. */
    MATRIX44 m_World;           /**< World matrix. */
    MATRIX44 m_Projection;      /**< Projection matrix. */
    MATRIX44 m_View;            /**< View matrix. */

    ShadowMap m_ShadowMap;      /**< ShadowMap information. */

    bool m_IsRunning;       /**< Renderer is ready and running. */
    bool m_IsRendering;     /**< Renderer is rendering. */

    VertexCacheManager* m_vcm;  /**< Vertex cache manager. */

    SkinManager* m_Skin;    /**< Skin manager. */
    LogManager* m_Log;      /**< Log manager. */
};

// exported functions
extern "C" __declspec (dllexport) HRESULT CreateRenderDevice (HINSTANCE _Instance, RenderDevice** _Interface);
extern "C" __declspec (dllexport) void ReleaseRenderDevice (RenderDevice** _Interface);