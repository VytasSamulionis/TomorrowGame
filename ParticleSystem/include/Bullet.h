/** @file ShootingBeam.h */
#pragma once

#include "../include/ParticleSystem.h"

/** Group of particles flying in one direction imitating the shot. */
class Bullet : public ParticleSystem {
public:
    /** Constructor.
    @param[in] _size the size of a particle
    @param[in] _thickness the thickness of the shot */
    Bullet (float _size, float _thickness);

    /** Resets the particle. 
    @see ParticleSystem::ResetParticle() */
    virtual void ResetParticle(ParticleAttribute* _Particle);

    /** Updates the particles. @see ParticleSystem::Update() */
    virtual void Update(float _timeDelta);
    
    /** Setter: initial position and direction.
    @param[in] _origin initial particles' position
    @param[in] _destination the position where particles will fly */
    virtual void SetPosition (const VECTOR3& _origin, const VECTOR3& _destination);

    /** Setter: maximum range of the shot.
    @param[in] _range the maximum range */
    void SetMaxRange (float _range);

    /** Setter: maximum time of the shot if target is in the maximum range. 
    @param[in] _time the maximum time */
    void SetMaxTime (float _time);

    /** Setter: the color of a particle.
    @param[in] _color ARGB format color. */
    void SetColor (DWORD _color);

    /** Getter: a particle's life time.
    @return a particle's life time */
    float GetLifeTime () const {
        return m_LifeTime;
    }

    /** Getter: maximum time of the shot.
    @return maximum time of the shot */
    float GetMaxTime () const {
        return m_MaxTime;
    }
protected:
    /** It overrides the parent class.
    @see ParticleSystem::PreRender() */
    virtual void PreRender();

    float m_Thickness;  /**< The thickness of the shot. */
    float m_LifeTime;   /**< A particle's life time. */
    float m_MaxRange;   /**< The maximum shot range. */
    float m_MaxTime;    /**< The maximum time of the shot. */
    DWORD m_Color;      /**< A particle's color. */
    VECTOR3 m_Direction;    /**< Shooting direction. */
};