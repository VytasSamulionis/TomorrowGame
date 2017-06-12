#include "../include/Renderer.h"

using namespace vs3d;

void Renderer::SetAmbientLight (DWORD _color) {
    m_vcm->Flush();
    HRESULT hr = m_Device->SetRenderState (D3DRS_AMBIENT, _color);
    if (FAILED (hr)) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::SetLight (DWORD _index, const LIGHT& _light) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: unable to set a light (device is not created).\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    D3DLIGHT9 light;
    int sizeVector = sizeof (float) * 3;
    int sizeColor = sizeof (float) * 4;
    memcpy (&(light.Diffuse), &(_light.Diffuse), sizeColor);
    memcpy (&(light.Specular), &(_light.Specular), sizeColor);
    memcpy (&(light.Ambient), &(_light.Ambient), sizeColor);
    switch (_light.Type) {
        case LIGHT_POINT:
            light.Type = D3DLIGHT_POINT;
            memcpy (&(light.Position), &(_light.Position), sizeVector);
            light.Range = _light.Range;
            light.Attenuation0 = _light.Attenuation0;
            light.Attenuation1 = _light.Attenuation1;
            light.Attenuation2 = _light.Attenuation2;
            break;
        case LIGHT_SPOT:
            light.Type = D3DLIGHT_SPOT;
            memcpy (&(light.Direction), &(_light.Direction), sizeVector);
            memcpy (&(light.Position), &(_light.Position), sizeVector);
            light.Range = _light.Range;
            light.Falloff = _light.Falloff;
            light.Attenuation0 = _light.Attenuation0;
            light.Attenuation1 = _light.Attenuation1;
            light.Attenuation2 = _light.Attenuation2;
            light.Phi = _light.Phi;
            light.Theta = _light.Theta;
            break;
        case LIGHT_DIRECTIONAL:
            light.Type = D3DLIGHT_DIRECTIONAL;
            memcpy (&(light.Direction), &(_light.Direction), sizeVector);
            break;
        default:
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Unknown light type.\n");
            }
            #endif
            THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
    
    if (FAILED (m_Device->SetLight (_index, &light))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: SetLight failed. (Renderer::SetLight)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetLight() failure.");
    }
}

void Renderer::EnableLight (DWORD _index, bool _enable) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: unable to enable a light (device is not created).\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush();
    if (FAILED (m_Device->LightEnable (_index, _enable))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: LightEnable failed. (Renderer::EnableLight).\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "LightEnable() failure.");
    }
}

void Renderer::EnableLighting (bool _enable) {
    if (!m_Device) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: device is not created. (Renderer::EnableLighting)\n");
        }
        #endif
        THROW_ERROR (ERRC_NO_DEVICE);
    }
    m_vcm->Flush();
    if (FAILED (m_Device->SetRenderState (D3DRS_LIGHTING, _enable))) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: SetRenderState failed. (Renderer::EnableLighting)\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::EnableFog (float _start, float _end, float _density, FOGMODE _mode) {
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGSTART, *((DWORD*)(&_start))))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGEND, *((DWORD*)(&_end))))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGDENSITY, *((DWORD*)(&_density))))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGVERTEXMODE, _mode))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGCOLOR, 0xffffffff))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
    if (FAILED(m_Device->SetRenderState (D3DRS_FOGENABLE, TRUE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::DisableFog () {
    if (FAILED (m_Device->SetRenderState (D3DRS_FOGENABLE, FALSE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetRenderState() failure.");
    }
}

void Renderer::CreateShadowMap (UINT _size, const MATRIX44& _lightWorldView, float _farClip) {
    if (m_ShadowMap.Size != _size) {
        /* Create shadow map texture. */
        if (m_ShadowMap.Texture) {
            m_ShadowMap.Texture->Release ();
            m_ShadowMap.Texture = NULL;
        }
        if (FAILED (D3DXCreateTexture (m_Device, _size, _size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ShadowMap.Texture))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateTexture() failure.");
        }
        if (FAILED (m_Device->CreateDepthStencilSurface (_size, _size, m_d3dpp.AutoDepthStencilFormat, D3DMULTISAMPLE_NONE, 0, TRUE, &m_ShadowMap.DepthStencil, NULL))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateDepthStencilSurface() failure.");
        }
        if (FAILED (m_ShadowMap.Texture->GetSurfaceLevel (0, &m_ShadowMap.Surface))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "GetSurfaceLevel() failure.");
        }
        if (FAILED (D3DXCreateRenderToSurface (m_Device, _size, _size, D3DFMT_R32F, TRUE, m_d3dpp.AutoDepthStencilFormat, &m_ShadowMap.RenderToSurface))) {
            THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateRenderToSurface() failure.");
        }
        m_ShadowMap.Size = _size;
    }
    if (m_ShadowMap.EffectId == INVALID_ID) {
        char effectData[] = //"struct VSOutput { float4 Position: POSITION; }; struct PSInput { float4 Position: TEXCOORD0; }; struct PSOutput { float4 Color : COLOR0; }; float4x4 g_LightWorldViewProjection; float g_FarClip; VSOutput ShadowMapVertexShader (float4 inPos : POSITION) { VSOutput output = (VSOutput)0; output.Position = mul(inPos, g_LightWorldViewProjection); return output;  } PSOutput ShadowMapPixelShader(PSInput input) { PSOutput output = (PSOutput)0; output.Color = input.Position.z / input.Position.w; return output; } technique ShadowMap { pass Pass0 { VertexShader = compile vs_2_0 ShadowMapVertexShader(); PixelShader = compile ps_2_0 ShadowMapPixelShader(); } }";
            "struct VSInput {                                                   \
                float4 Position : POSITION;                                     \
                float3 Normal : NORMAL;                                         \
                float2 TexCoord : TEXCOORD0;                                    \
            };                                                                  \
            struct VSOutput {                                                   \
                float4 Position : POSITION;                                     \
                float Depth : TEXCOORD0;                                        \
            };                                                                  \
                                                                                \
            struct PSInput {                                                    \
                float Depth : TEXCOORD0;                                        \
            };                                                                  \
                                                                                \
            struct PSOutput {                                                   \
                float4 Color : COLOR0;                                          \
            };                                                                  \
                                                                                \
            float4x4 g_LightWorldViewProjection;                                \
            float g_FarClip;                                                    \
                                                                                \
            VSOutput ShadowMapVertexShader (VSInput input) {                    \
                VSOutput output = (VSOutput)0;                                  \
                output.Position = mul(input.Position, g_LightWorldViewProjection);       \
                output.Depth = output.Position.z / g_FarClip;                   \
                return output;                                                  \
            }                                                                   \
                                                                                \
            PSOutput ShadowMapPixelShader(PSInput input) {                      \
                PSOutput output = (PSOutput)0;                                                \
                output.Color = float4 (input.Depth, 0.0, 0.0, 1.0);                                     \
                                                     \
                return output;                                                  \
            }                                                                   \
                                                                                \
            technique ShadowMap {                                               \
                pass Pass0 {                                                    \
                    VertexShader = compile vs_2_0 ShadowMapVertexShader();      \
                    PixelShader = compile ps_2_0 ShadowMapPixelShader();        \
                }                                                               \
            }";
        m_ShadowMap.EffectId = m_vcm->CreateEffect (effectData, sizeof (effectData), false);
    }
    m_vcm->SetEffectParameter (m_ShadowMap.EffectId, "g_LightWorldViewProjection", (void*)_lightWorldView.data());
    m_vcm->SetEffectParameter (m_ShadowMap.EffectId, "g_FarClip", &_farClip);
}

void Renderer::BeginRenderingToShadowMap () {
    if (m_ShadowMap.EffectId == INVALID_ID) {
        THROW_ERROR (ERRC_NOT_READY);
    }
    m_vcm->EnableEffect (m_ShadowMap.EffectId, "ShadowMap");
    ID3DXEffect* effect = m_vcm->GetActiveEffect ();
    D3DVIEWPORT9 viewport;
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Width = m_ShadowMap.Size;
    viewport.Height = m_ShadowMap.Size;
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
    if (FAILED (m_Device->GetDepthStencilSurface (&m_ShadowMap.OldDepthStencil))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetDepthStencilSurface() failure.");
    }
    if (FAILED (m_Device->SetDepthStencilSurface (m_ShadowMap.DepthStencil))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetDepthStencilSurface() failure.");
    }
    m_Device->Clear (0, NULL, D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
    if (FAILED (m_ShadowMap.RenderToSurface->BeginScene (m_ShadowMap.Surface, &viewport))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "BeginScene() failure.");
    }
    /*UINT numPasses;
    if (FAILED (effect->Begin (&numPasses, 0))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Begin() failure.");
    }
    if (FAILED (effect->BeginPass (0))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "BeginPass() failure.");
    }*/
}

void Renderer::EndRenderingToShadowMap () {
    /*ID3DXEffect* effect = m_vcm->GetActiveEffect ();
    effect->EndPass ();
    effect->End ();*/
    m_vcm->Flush ();
    if (FAILED (m_ShadowMap.RenderToSurface->EndScene (D3DX_FILTER_NONE))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "EndScene() failure.");
    }
    /*if (FAILED (D3DXSaveTextureToFileA ("texture.jpg", D3DXIFF_JPG, m_ShadowMap.Texture, NULL))) {
        THROW_ERROR (ERRC_API_CALL);
    }*/
    if (FAILED (m_Device->SetDepthStencilSurface (m_ShadowMap.OldDepthStencil))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetDepthStencilSurface() failure.");
    }
    m_vcm->SetSkin (INVALID_ID);
}

void Renderer::SetShadowMap (UINT _effectId, const char* _shadowMapParamName) {
    ID3DXEffect* effect = m_vcm->GetEffect (_effectId);
    D3DXHANDLE shadowMap = effect->GetParameterByName (NULL, _shadowMapParamName);
    if (FAILED (effect->SetTexture (shadowMap, m_ShadowMap.Texture))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "SetTexture() failure.");
    }
}