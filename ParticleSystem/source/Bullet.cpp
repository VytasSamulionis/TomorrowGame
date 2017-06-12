#include "../include/Bullet.h"

Bullet::Bullet (float _size, float _thickness):
    ParticleSystem () {
    m_Size = _size;
    m_Thickness = _thickness;
    m_MaxRange = 50.0f;
    m_MaxTime = 1.0f;
    m_Color = 0xffffffff;
}

void Bullet::ResetParticle (ParticleAttribute* _particle) {
    if (_particle) {
        _particle->Velocity = m_Direction * m_MaxRange / m_MaxTime;
        _particle->Color = m_Color;
        float thickness = m_Thickness / 2.0f;
        VECTOR3 minVector (m_Origin[0] - thickness, m_Origin[1] - thickness, m_Origin[2] - thickness);
        VECTOR3 maxVector (m_Origin[0] + thickness, m_Origin[1] + thickness, m_Origin[2] + thickness);
        _particle->Position = GetRandomVector(minVector, maxVector);
        _particle->Age = 0.0f;
        _particle->LifeTime = m_LifeTime;
    }
}

void Bullet::Update (float _timeDelta) {
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
}

void Bullet::PreRender() {
    m_Device->EnableLighting(false);
    m_Device->EnablePointSprites();
    m_Device->EnablePointsScale();
    m_Device->SetPointsSize(m_Size);
    m_Device->SetPointSpriteState(PS_POINTSIZE_MIN, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_A, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_B, 0.0f);
    m_Device->SetPointSpriteState(PS_POINTSCALE_C, 1.0f);
}

void Bullet::SetPosition (const VECTOR3& _origin, const VECTOR3& _destination) {
    m_Direction = (_destination - _origin).normalize();
    m_Origin = _origin;
    float x = _origin[0] - _destination[0];
    float y = _origin[1] - _destination[1];
    float z = _origin[2] - _destination[2];
    float length = sqrtf (x * x + y * y + z * z);
    m_LifeTime = length / m_MaxRange * m_MaxTime;
}

void Bullet::SetColor (DWORD _color) {
    m_Color = _color;
}

void Bullet::SetMaxRange (float _range) {
    m_MaxRange = _range;
}

void Bullet::SetMaxTime (float _time) {
    if (_time > 0.0f) {
        m_MaxTime = _time;
    }
}