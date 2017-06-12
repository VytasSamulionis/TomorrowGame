/** @file Camera.h */

#pragma once

#include <Windows.h>
#include <cml/cml.h>
#include "../include/Log.h"
#include "../include/Engine.h"

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

/** Abstract class for camera.
It is the base for the specific camera realizations.
*/
class Camera {
public:
    /** Constructor. */
    Camera ();

    /** Destructor. */
    virtual ~Camera ();

    /** Abstract method for updating the camera. */
    virtual void Update (float _delta) = 0;

    /** Abstract method which should return the next position of the camera. */
    virtual VECTOR3 GetNextPosition () = 0;

    /** Getter: position.
    @return camera position */
    VECTOR3 GetPosition () const;

    /** Getter: looking point.
    @return looking point */
    VECTOR3 GetLookingPoint () const;

    /** Getter: up vector.
    @return up vector */
    VECTOR3 GetUpVector () const;

    /** Getter: right vector.
    @return right vector */
    VECTOR3 GetRight () const;

    /** Getter: velocity.
    @return velocity */
    VECTOR3 GetVelocity () const;

    /** Setter: position.
    @param[in] _position camera position */
    void SetPosition (VECTOR3 _position);

    /** Setter: looking point.
    @param[in] _lookAt view point of the camera */
    void SetLookingPoint (VECTOR3 _lookAt);

    /** Setter: up vector
    @param[in] _up up vector */
    void SetUpVector (VECTOR3 _up);

    /** Setter: right vector
    @param[in] _right right vector */
    void SetRight (VECTOR3 _right);

    /** Setter: velocity
    @param[in] _velocity velocity */
    void SetVelocity (VECTOR3 _velocity);

protected:
    /** updates the camera axis. */
    virtual void UpdateAxis ();

    float m_RollSpeed;  /**< camera roll speed. */
    float m_PitchSpeed; /**< camera pitch speed. */
    float m_YawSpeed;   /**< camera yaw speed. */

    float m_RotX;   /**< x axis rotation. */
    float m_RotY;   /**< y axis rotation. */
    float m_RotZ;   /**< z axis rotation. */

    QUATERNION m_Quat;  /**< camera rotation. */
    VECTOR3 m_Position; /**< camera position. */
    VECTOR3 m_LookAt;   /**< camera view point. */
    VECTOR3 m_Up;       /**< camera up vector. */
    VECTOR3 m_Right;    /**< camera right vector. */
    VECTOR3 m_Velocity; /**< camera velocity. */
};