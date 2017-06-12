/** @file FreeViewCamera.h */
#pragma once

#include "../include/Camera.h"

/** A camera which has six degrees of freedom. */
class FreeViewCamera: public Camera {
public:
    /** Consrtuctor. */
    FreeViewCamera ();

    /** Destructor. */
    virtual ~FreeViewCamera ();

    /** Updates the camera position. 
    @param[in] _delta time elapsed from the last call */
    virtual void Update (float _delta);

    /** Getter: the next position of the camera. 
    @return the next position */
    virtual VECTOR3 GetNextPosition ();

    /** Increase the speed of the camera pitch, yaw and roll.
    @param[in] _pitch how much to increase camera pitch speed
    @param[in] _yaw how much to increase camera yaw speed
    @param[in] _roll how much to increase camera roll speed */
    void AddRotationSpeed (float _pitch, float _yaw, float _roll);

    /** Setter: the speed of the camera pitch, yaw and roll.
    @param[in] _pitch the speed of the camera pitch
    @param[in] _yaw the speed of the camera yaw
    @param[in] _roll the speed of the camera roll */
    void SetRotationSpeed (float _pitch, float _yaw, float _roll);

    /** Setter: camera pitch speed.
    @param[in] _pitch the pitch speed */
    void SetPitchSpeed (float _pitch);

    /** Setter: camera yaw speed.
    @param[in] _yaw the yaw speed */
    void SetYawSpeed (float _yaw);

    /** Setter: camera roll speed.
    @param[in] _roll the roll speed */
    void SetRollSpeed (float _roll);

    /** Increase the camera thrust speed.
    @param[in] _thrust how much to increase the thrust speed */
    void AddThrust (float _thrust);

    /** Setter: camera thrust speed.
    @param[in] _thrust the thrust speed */
    void SetThrust (float _thrust);

protected:
    float m_Thrust; /**< The camera thrust speed. */
};