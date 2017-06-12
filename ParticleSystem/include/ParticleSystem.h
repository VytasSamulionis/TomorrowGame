/** @file ParticleSystem.h */

#pragma once

#include "../include/RenderDevice.h"
#include "../include/ErrorMessage.h"
#include <list>

#ifdef _DEBUG
    //#pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

/** The general information about a particle. */
struct ParticleAttribute {
    VECTOR3 Position;       /**< A particle's position. */
    VECTOR3 Velocity;       /**< A particle's velocity. */
    VECTOR3 Acceleration;   /**< A particle's acceleration. */
    float LifeTime;         /**< A particle's maximum existence time. */
    float Age;              /**< A particle's existence time. */
    DWORD Color;            /**< A particle's color. */
    DWORD ColorFade;        /**< A particle's final color. */
    bool IsAlive;           /**< A flag which indicates whether the particle is alive. */
};

/** The abstract base class for realization of the specific particles. */
class ParticleSystem {
public:
    /** Constructor. */
    ParticleSystem ();

    /** Setups the data required for the rendering.
    It should be called once. 
    @param[in] _device A pointer to a RenderDevice 
    @param[in] _textureFilename A texture filename.
    Send NULL if texture is not required. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    virtual void Init(RenderDevice* _device, const char* _textureFilename);

    /** Resets all the particles to their initial state. */
    virtual void Reset();

    /** Resets the specified particle to its initial state.
    @param[in] _particle The pointer to the particle. */
    virtual void ResetParticle(ParticleAttribute* _particle) = 0;

    /** Adds new particle to the system. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void AddParticle();

    /** Updates the particles. It should be called every frame.
    @param[in] _timeDelta time elapsed from the last call */
    virtual void Update(float _timeDelta) = 0;

    /** Renders the particles. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_OUT_OF_RANGE particle buffer ID is invalid
        - @c ERRC_API_CALL */
    virtual void Render();

    /** Checks if there are any particles.
    @return @c true no particles in the system. @c false otherwise. */
    bool IsEmpty();

    /** Checks if all the particles are dead.
    Particle is dead when its life time has expired. 
    @see ParticleAttribute::LifeTime
    @see ParticleAttribyte::IsAlive */
    bool IsDead();
protected:
    /** Setup which should be done before the rendering. 
    This method is called at the begining by ParticleSystem::Render() method. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL */
    virtual void PreRender();

    /** Setup which should be done after the rendering. 
    This method is called at the end by ParticlesSystem::Render() method.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL */
    virtual void PostRender();

    /** Removes all the dead particles. */
    virtual void RemoveDeadParticles ();

    /** A random float generator in specified range.
    If _low > _high, _low is returned.
    @param[in] _low the lower range
    @param[in] _high the higher range
    @return a random float between _low and _high */
    float GetRandomFloat (float _low, float _high);

    /** A random vector generator.
    @param[in] _min the minimum vector
    @param[in] _max the maximum vector
    @return a random vector */
    VECTOR3 GetRandomVector (const VECTOR3& _min, const VECTOR3& _max);

    RenderDevice* m_Device; /**< A pointer to the RenderDevice. */
    VECTOR3 m_Origin;       /**< Particles' initial position. */
    float m_EmitRate;       /**< Emition rate. */
    float m_Size;           /**< A particle's size. */
    UINT m_SkinId;          /**< Skin ID. */
    UINT m_ParticleBufferId;    /**< Buffer ID for the particles rendering. */
    std::list<ParticleAttribute> m_Particles;   /**< The list of particles. */
    UINT m_MaxParticles;    /**< Maximum number of the particles. */
};