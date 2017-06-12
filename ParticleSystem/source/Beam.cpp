#include "../include/Beam.h"

Beam::Beam (float _size, float _thickness, float _respawnTime, UINT _numRespawned, float _beamTime, float _pauseTime):
    ParticleSystem () {
    m_Size = _size;
    m_Thickness = _thickness;
    m_MaxRange = 50.0f;
    m_MaxTime = 1.0f;
    m_Color = 0xffffffff;
    m_RespawnTime = _respawnTime;
    m_NumRespawned = _numRespawned;
    m_TimeLeft = _respawnTime;
    m_BeamTime = _beamTime;
    m_BeamTimeLeft = _beamTime;
    m_PauseTime = _pauseTime;
    m_PauseTimeLeft = _pauseTime;
}

void Beam::ResetParticle (ParticleAttribute* _particle) {
    if (_particle) {
        _particle->Velocity = m_Direction * m_MaxRange / m_MaxTime;
        _particle->Color = m_Color;
        float thickness = m_Thickness / 2.0f;
        VECTOR3 minVector (m_Origin[0]/* - thickness*/, m_Origin[1]/* - thickness*/, m_Origin[2]/* - thickness*/);
        //VECTOR3 maxVector (m_Origin[0]/* + thickness*/, m_Origin[1]/* + thickness*/, m_Origin[2]/* + thickness*/);
        _particle->Position = minVector + GetRandomFloat (0.0f, m_Thickness) * m_Direction;//GetRandomVector(minVector, maxVector + m_Thickness * m_Direction);
        _particle->Age = 0.0f;
        _particle->LifeTime = m_LifeTime;
    }
}

void Beam::Update (float _timeDelta) {
    std::list<ParticleAttribute>::iterator i;
    i = m_Particles.begin();
    while (i != m_Particles.end()) {
        i->Position += i->Velocity * _timeDelta;
        i->Age += _timeDelta;
        if (i->Age > i->LifeTime) {
            i = m_Particles.erase (i);
        } else {
            i++;
        }
    }
    if (m_BeamTimeLeft > 0.0f) {
        m_TimeLeft -= _timeDelta;
        if (m_TimeLeft <= 0.0f) {
            m_TimeLeft = m_RespawnTime;
            for (UINT j = 0; j < m_NumRespawned; j++) {
                AddParticle ();
            }
        }
        m_BeamTimeLeft -= _timeDelta;
        if (m_BeamTimeLeft <= 0.0f) {
            m_PauseTimeLeft = m_PauseTime;
        }
    } else {
        m_PauseTimeLeft -= _timeDelta;
        if (m_PauseTimeLeft <= 0.0f) {
            m_BeamTimeLeft = m_BeamTime;
        }
    }
}

void Beam::PreRender() {
    m_Device->EnableLighting(false);
    m_Device->EnablePointSprites();
    m_Device->EnablePointsScale();
    m_Device->SetPointsSize(m_Size);
    m_Device->SetPointSpriteState(PS_POINTSIZE_MIN, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_A, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_B, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_C, 1.0f);
}

void Beam::SetPosition (const VECTOR3& _origin, const VECTOR3& _destination) {
    m_Direction = (_destination - _origin).normalize();
    m_Origin = _origin;
    float x = _origin[0] - _destination[0];
    float y = _origin[1] - _destination[1];
    float z = _origin[2] - _destination[2];
    float length = sqrtf (x * x + y * y + z * z);
    m_LifeTime = length / m_MaxRange * m_MaxTime;
}

void Beam::Destroy () {
    m_Particles.clear ();
}

void Beam::SetColor (DWORD _color) {
    m_Color = _color;
}

void Beam::SetMaxRange (float _range) {
    m_MaxRange = _range;
}

void Beam::SetMaxTime (float _time) {
    if (_time > 0.0f) {
        m_MaxTime = _time;
    }
}