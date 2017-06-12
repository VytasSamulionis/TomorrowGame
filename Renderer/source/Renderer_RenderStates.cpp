#include "../include/Renderer.h"

void Renderer::SetCullingState (RENDERSTATETYPE _state) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (SetCullingState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush (); // flush everything
    switch (_state) {
        case RS_CULL_CW:
            if (FAILED (m_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_CW))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetCullingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_CULL_CCW:
            if (FAILED (m_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_CCW))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetCullingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_CULL_NONE:
            if (FAILED (m_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE))) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetCullingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown culling state. (SetCullingState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetDepthBufferState (RENDERSTATETYPE _state) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (SetDepthBufferState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush (); // flush everything
    switch (_state) {
        case RS_DEPTH_READWRITE:
            if (FAILED (m_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            if (FAILED (m_Device->SetRenderState (D3DRS_ZWRITEENABLE, TRUE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_DEPTH_READONLY:
            if (FAILED (m_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            if (FAILED (m_Device->SetRenderState (D3DRS_ZWRITEENABLE, FALSE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_DEPTH_NONE:
            if (FAILED (m_Device->SetRenderState (D3DRS_ZWRITEENABLE, FALSE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            if (FAILED (m_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_FALSE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDepthBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown depth buffer state (SetDepthBufferState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetDrawingState (RENDERSTATETYPE _state) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (SetDrawingState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush (); // flush everything
    switch (_state) {
        case RS_DRAW_POINTS:
            if (FAILED (m_Device->SetRenderState (D3DRS_FILLMODE, D3DFILL_POINT))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDrawingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_DRAW_WIRE:
            if (FAILED (m_Device->SetRenderState (D3DRS_FILLMODE, D3DFILL_WIREFRAME))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDrawingState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_DRAW_SOLID:
            if (FAILED (m_Device->SetRenderState (D3DRS_FILLMODE, D3DFILL_SOLID))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetDrawingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown drawing state. (SetDrawingState).\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetStencilBufferState (RENDERSTATETYPE _state) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (SetStencilBufferState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush (); // flush everything
    switch (_state) {
        case RS_STENCIL_DISABLE:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILENABLE, FALSE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_ENABLE:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILENABLE, TRUE))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_FUNC_ALWAYS:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILFUNC, D3DCMP_ALWAYS))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_FUNC_LESSEQUAL:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_FAIL_DECR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILFAIL, D3DSTENCILOP_DECR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_FAIL_INCR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILFAIL, D3DSTENCILOP_INCR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_FAIL_KEEP:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_ZFAIL_DECR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_ZFAIL_INCR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_ZFAIL_KEEP:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_PASS_DECR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILPASS, D3DSTENCILOP_DECR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_PASS_INCR:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILPASS, D3DSTENCILOP_INCR))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_PASS_KEEP:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILPASS, D3DSTENCILOP_KEEP))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown stencil buffer state. (SetStencilBufferState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetStencilBufferState (RENDERSTATETYPE _state, DWORD _value) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (SetStencilBufferState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush (); // flush everything
    switch (_state) {
        case RS_STENCIL_MASK:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILMASK, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_WRITEMASK:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILWRITEMASK, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_STENCIL_REF:
            if (FAILED (m_Device->SetRenderState (D3DRS_STENCILREF, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (SetStencilBufferState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown stencil buffer state. (SetStencilBufferState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetShadingState (RENDERSTATETYPE _state) {
    if (!m_Device) {
        #ifdef _DEBUG 
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (Renderer::SetShadingState)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush ();    // flush everything
    switch (_state) {
        case RS_SHADE_FLAT:
            if (FAILED (m_Device->SetRenderState (D3DRS_SHADEMODE, D3DSHADE_FLAT))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (Renderer::SetShadingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case RS_SHADE_GOURAUD:
            if (FAILED (m_Device->SetRenderState (D3DRS_SHADEMODE, D3DSHADE_GOURAUD))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetRenderState failed. (Renderer::SetShadingState)\n");
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown shading state. (Renderer::SetShadingState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetTextureStageState (UINT _stage, TEXTURESTAGESTATETYPE _type, DWORD _value) {
    m_vcm->Flush ();
    switch (_type) {
        case TSS_COLOROP:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_COLOROP, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_COLORARG1:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_COLORARG1, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_COLORARG2:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_COLORARG2, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_ALPHAOP:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_ALPHAOP, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_ALPHAARG1:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_ALPHAARG1, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_ALPHAARG2:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_ALPHAARG2, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVMAT00:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVMAT00, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVMAT01:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVMAT01, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVMAT10:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVMAT10, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVMAT11:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVMAT11, _value))) {
                #ifdef _DEBUG
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_TEXCOORDINDEX:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_TEXCOORDINDEX, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVLSCALE:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVLSCALE, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_BUMPENVLOFFSET:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_BUMPENVLOFFSET, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_TEXTURETRANSFORMFLAGS:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_TEXTURETRANSFORMFLAGS, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_COLORARG0:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_COLORARG0, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_ALPHAARG0:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_ALPHAARG0, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_RESULTARG:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_RESULTARG, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        case TSS_CONSTANT:
            if (FAILED (m_Device->SetTextureStageState (_stage, D3DTSS_CONSTANT, _value))) {
                #ifdef _DEBUG 
                if (m_Log) {
                    m_Log->Log ("Error: SetTextureStageState failed (%u, %u, %u) (Renderer::SetTextureStageState)\n", _stage, _type, _value);
                }
                #endif 
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTextureStageState() failure.");
            }
            break;
        default:
            #ifdef _DEBUG 
            if (m_Log) {
                m_Log->Log ("Error: Unknown TEXTURESTAGESTATETYPE value (Renderer::SetTextureStageState)\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::EnablePointSprites () {
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_POINTSPRITEENABLE, TRUE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::DisablePointSprites () {
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_POINTSPRITEENABLE, FALSE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::EnablePointsScale () {
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_POINTSCALEENABLE, TRUE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::DisablePointsScale () {
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_POINTSCALEENABLE, FALSE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::SetPointsSize (float _size) {
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_POINTSIZE, *((DWORD*)&_size)))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::SetPointSpriteState (POINTSPRITESTATETYPE _type, float _Value) {
    m_vcm->Flush();
    DWORD value = *((DWORD*)&_Value);
    switch (_type) {
        case PS_POINTSIZE_MIN:
            if (FAILED (m_Device->SetRenderState (D3DRS_POINTSIZE_MIN, value))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case PS_POINTSIZE_MAX:
            if (FAILED (m_Device->SetRenderState (D3DRS_POINTSIZE_MAX, value))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case PS_POINTSCALE_A:
            if (FAILED (m_Device->SetRenderState (D3DRS_POINTSCALE_A, value))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case PS_POINTSCALE_B:
            if (FAILED (m_Device->SetRenderState (D3DRS_POINTSCALE_B, value))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case PS_POINTSCALE_C:
            if (FAILED (m_Device->SetRenderState (D3DRS_POINTSCALE_C, value))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::SetAlphaBlendState (ALPHABLENDSTATETYPE _type, BLENDTYPE _blend) {
    m_vcm->Flush();
    switch (_type) {
        case AS_SRCBLEND:
            if (FAILED (m_Device->SetRenderState (D3DRS_SRCBLEND, _blend))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        case AS_DESTBLEND:
            if (FAILED (m_Device->SetRenderState (D3DRS_DESTBLEND, _blend))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
            }
            break;
        default:
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
}

void Renderer::EnableAlphaBlend () {
    m_vcm->Flush ();
    if (FAILED (m_Device->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::DisableAlphaBlend () {
    m_vcm->Flush ();
    if (FAILED (m_Device->SetRenderState (D3DRS_ALPHABLENDENABLE, FALSE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}