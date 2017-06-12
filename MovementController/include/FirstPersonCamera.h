/** @file FirstPersonCamera.h */
#pragma once

#include "../include/Camera.h"

/** First person view camera. */
class FirstPersonCamera: public Camera {
public:
    /** Constructor. */
    FirstPersonCamera ();

    /** Destructor. */
    virtual ~FirstPersonCamera ();

    /** Updates the camera position. 
    @param[in] _delta time elapsed from the last call */ 
    virtual void Update (float _delta);

    /** Getter: the next position of the camera. 
    @return the next position */
    virtual VECTOR3 GetNextPosition ();

    /** Increase the pitch and yaw speed.
    @param[in] _pitch how much to increase pitch speed
    @param[in] _yaw how much to increase yaw speed */
    void AddRotationSpeed (float _pitch, float _yaw);

    /** Setter: pitch and yaw speed.
    @param[in] _pitch camera pitch speed
    @param[in] _yaw camera yaw speed */
    void SetRotationSpeed (float _pitch, float _yaw);

    /** Setter: pitch speed.
    @param[in] _pitch camera pitch speed */
    void SetPitchSpeed (float _pitch);

    /** Setter: camera yaw speed.
    @param[in] _yaw the yaw speed */
    void SetYawSpeed (float _yaw);

    /** Setter: lean speed.
    @param[in] _lean lean speed */
    void SetLeanSpeed (float _lean);

    /** Increase camera speed.
    @param[in] _speed how much to increase camera speed */
    void AddSpeed (float _speed);

    /** Setter: camera speed.
    @param[in] _speed camera speed */
    void SetSpeed (float _speed);

    /** Increase camera slide speed.
    @param[in] _slideSpeed how much to increase camera slide speed */
    void AddSlideSpeed (float _slideSpeed);

    /** Setter: camera slide speed.
    @param[in] _slideSpeed camera slide speed */
    void SetSlideSpeed (float _slideSpeed);

    /** Setter: camera forward vector.
    @param[in] _forward camera forward vector */
    void SetForward (const VECTOR3& _forward);

    /** Change the forward direction.
    @param[in] _x x axis rotation
    @param[in] _y y axis rotation
    @param[in] _z z axis rotation */
    void RotateForward (float _x, float _y, float _z);

protected:
    /** updates the camera axis. */
    void UpdateAxis ();

    VECTOR3 m_Forward;  /**< Camera forward vector. */
    float m_Speed;      /**< Camera speed. */
    float m_SlideSpeed; /**< Camera slide speed. */
};