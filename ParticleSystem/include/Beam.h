/** @file ShootingBeam.h */
#pragma once

#include "../include/ParticleSystem.h"

/** Group of particles flying in one direction imitating the shot. */
class Beam : public ParticleSystem {
public:
    /** Constructor.
    @param[in] _size the size of a particle
    @param[in] _thickness the thickness of the shot
    @param[in] _respawnTime respawn time of the particles
    @param[in] _numRespawned number of the respawned particles
    @param[in] _beamTime beam shot time until the pause
    @param[in] _pauseTime pause time until the beam shot */
    Beam (float _size, float _thickness, float _respawnTime, UINT _numRespawned, float _beamTime, float _pauseTime);

    /** Resets the particle. 
    @see ParticleSystem::ResetParticle() */
    virtual void ResetParticle(ParticleAttribute* _Particle);

    /** Updates the particles. @see ParticleSystem::Update() */
    virtual void Update(float _timeDelta);
    
    /** Setter: initial position and direction.
    @param[in] _origin initial particles' position
    @param[in] _destination the position where particles will fly */
    virtual void SetPosition (const VECTOR3& _origin, const VECTOR3& _destination);

    /** Kills all the particles. */
    void Destroy ();

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
    float m_RespawnTime;    /**< Respawn time of the particles. */
    float m_TimeLeft;       /**< Time left until respawn */
    float m_PauseTime;      /**< Pause time of beam. */
    float m_PauseTimeLeft;  /**< How much time left of pause. */
    float m_BeamTime;       /**< Beam shot time until pause start. */
    float m_BeamTimeLeft;   /**< How much time left of beam shot. */
    UINT m_NumRespawned;    /**< Number of the respawned particles. */
    DWORD m_Color;      /**< A particle's color. */
    VECTOR3 m_Direction;    /**< Shooting direction. */
};