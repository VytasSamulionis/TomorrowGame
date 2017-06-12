#include "../include/ParticleSystem.h"

ParticleSystem::ParticleSystem () {
    m_Device = NULL;
    m_Size = 1.0f;
    m_SkinId = INVALID_ID;
    m_ParticleBufferId = INVALID_ID;
    m_MaxParticles = 10000;
}

void ParticleSystem::Init (RenderDevice* _device, const char* _textureFilename) {
    m_Device = _device;
    if (_textureFilename) {
        m_SkinId = m_Device->GetSkinManager()->AddSkin (_textureFilename);
    } else {
        m_SkinId = INVALID_ID;
    }
    m_ParticleBufferId = m_Device->GetVCacheManager()->CreateParticleBuffer(m_MaxParticles);
}

void ParticleSystem::Reset () {
    std::list<ParticleAttribute>::iterator i;
    for (i = m_Particles.begin(); i != m_Particles.end(); i++) {
        ResetParticle (&(*i));
    }
}

void ParticleSystem::AddParticle () {
    ParticleAttribute particle;
    ResetParticle(&particle);
    try {
        m_Particles.push_back (particle);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void ParticleSystem::PreRender () {
    m_Device->EnableLighting(false);
    m_Device->EnablePointSprites();
    m_Device->EnablePointsScale();
    m_Device->SetPointsSize(m_Size);
    m_Device->SetPointSpriteState(PS_POINTSIZE_MIN, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_A, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_B, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_C, 1.0f);
    m_Device->SetTextureStageState(0, TSS_ALPHAARG1, TA_TEXTURE);
    m_Device->SetTextureStageState(0, TSS_ALPHAOP, TOP_SELECTARG1);
    m_Device->EnableAlphaBlend();
    m_Device->SetAlphaBlendState(AS_SRCBLEND, BLEND_SRCALPHA);
    m_Device->SetAlphaBlendState(AS_DESTBLEND, BLEND_INVSRCALPHA);
}

void ParticleSystem::Render () {
    vs3d::ULCVERTEX* vertex = NULL;
    try {
        PreRender();
        vertex = new vs3d::ULCVERTEX[m_Particles.size()];
        std::list<ParticleAttribute>::iterator i;
        UINT numVertices = 0;
        for (i = m_Particles.begin(); i != m_Particles.end(); i++) {
            if (i->IsAlive) {
                vertex[numVertices].X = i->Position[0];
                vertex[numVertices].Y = i->Position[1];
                vertex[numVertices].Z = i->Position[2];
                vertex[numVertices].Color = i->Color;
                numVertices++;
            }
        }
        m_Device->GetVCacheManager()->RenderParticles (vertex, numVertices, m_ParticleBufferId, m_SkinId);
        PostRender();
        delete[] vertex;
    } catch (ErrorMessage e) {
        delete[] vertex;
        throw e;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void ParticleSystem::PostRender () {
    m_Device->DisableAlphaBlend();
    m_Device->DisablePointSprites();
    m_Device->DisablePointsScale();
}

bool ParticleSystem::IsEmpty () {
    return m_Particles.size() ? false : true;
}

bool ParticleSystem::IsDead () {
    std::list<ParticleAttribute>::iterator i;
    for (i = m_Particles.begin(); i != m_Particles.end(); i++) {
        if (i->IsAlive) {
            return false;
        }
    }
    return true;
}

void ParticleSystem::RemoveDeadParticles () {
    std::list<ParticleAttribute>::iterator i;
    i = m_Particles.begin();
    while(i != m_Particles.end()) {
        if (i->IsAlive == false) {
        // erase returns the next iterator, so no need
        // to incrememnt to the next one ourselves.
            i = m_Particles.erase(i);
        } else {
            i++; // next in list
        }
    }
}

float ParticleSystem::GetRandomFloat (float _low, float _high) {
    if (_low >= _high) {
        return _low;
    }
    float f = (rand() % 10000) * 0.0001f;
    return (f * (_high - _low)) + _low;
}

VECTOR3 ParticleSystem::GetRandomVector (const VECTOR3& _minVector, const VECTOR3& _maxVector) {
    VECTOR3 out;
    out[0] = GetRandomFloat(_minVector[0], _maxVector[0]);
    out[1] = GetRandomFloat(_minVector[1], _maxVector[1]);
    out[2] = GetRandomFloat(_minVector[2], _maxVector[2]);
    return out;
}