/** @file FromAboveCamera.h */
#pragma once

#include "../include/Camera.h"

/** Isometric view camera. */
class FromAboveCamera: public Camera {
public:
    /** Constructor. */
    FromAboveCamera ();

    /** Updates the camera position. 
    @param[in] _delta time elapsed from the last call */
    virtual void Update (float _delta);

    /** Getter: the next position of the camera. 
    @return the next position */
    virtual VECTOR3 GetNextPosition ();

    /** Increase the rotation speed.
    @param[in] _rotation how much to increase rotation speed */
    void AddRotationSpeed (float _rotation);

    /** Setter: rotation speed.
    @param[in] _rotation camera rotation speed */
    void SetRotationSpeed (float _rotation);

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

    /** Increase view angle.
    @param[in] _angle how much to increase view angle. */
    void AddViewAngle (float _angle);

    /** Setter: view angle.
    @param[in] _angle view angle */
    void SetViewAngle (float _angle);

    /** Increase zoom speed.
    @param[in] _zoomSpeed how much to increase zoom speed */
    void AddZoomSpeed (float _zoomSpeed);

    /** Setter: zoom speed
    @param[in] _zoomSpeed zoom speed */
    void SetZoomSpeed (float _zoomSpeed);

    /** Setter: camera forward vector.
    @param[in] _forward camera forward vector */
    void SetForward (const VECTOR3& _forward);

protected:
    /** updates the camera axis. */
    void UpdateAxis ();

    VECTOR3 m_Forward;  /**< Camera forward vector. */
    float m_Speed;      /**< Camera speed. */
    float m_SlideSpeed; /**< Camera slide speed. */
    float m_ZoomSpeed;  /**< Camera zoom speed. */
};