#include "../include/FromAboveCamera.h"

FromAboveCamera::FromAboveCamera () {
    m_Speed = m_SlideSpeed = 0.0f;
}

void FromAboveCamera::Update (float _Delta) {
    m_RotY += m_YawSpeed * _Delta;

    VECTOR3 slide;
    slide = m_Right * m_SlideSpeed * _Delta;
    m_Velocity = m_Forward * m_Speed * _Delta;
    VECTOR3 zoom;
    zoom = m_Up * m_ZoomSpeed * _Delta;
    m_Position += m_Velocity + slide + zoom;

    UpdateAxis ();
}

VECTOR3 FromAboveCamera::GetNextPosition () {
    VECTOR3 slide = m_Right * m_SlideSpeed;
    VECTOR3 velocity = m_Forward * m_Speed;
    VECTOR3 zoom = m_Up * m_ZoomSpeed;
    return m_Position + velocity + slide + zoom;
}

void FromAboveCamera::AddRotationSpeed (float _Rotation) {
    m_YawSpeed += _Rotation;
}

void FromAboveCamera::SetRotationSpeed (float _Rotation) {
    m_YawSpeed = _Rotation;
}

void FromAboveCamera::AddSpeed (float _Speed) {
    m_Speed += _Speed;
}

void FromAboveCamera::SetSpeed (float _Speed) {
    m_Speed = _Speed;
}

void FromAboveCamera::AddSlideSpeed (float _SlideSpeed) {
    m_SlideSpeed += _SlideSpeed;
}

void FromAboveCamera::SetSlideSpeed (float _SlideSpeed) {
    m_SlideSpeed = _SlideSpeed;
}

void FromAboveCamera::AddViewAngle (float _Angle) {
    m_RotX += _Angle;
}

void FromAboveCamera::SetViewAngle (float _Angle) {
    m_RotX = _Angle;
}

void FromAboveCamera::AddZoomSpeed (float _ZoomSpeed) {
    m_ZoomSpeed += -_ZoomSpeed;
}

void FromAboveCamera::SetZoomSpeed (float _ZoomSpeed) {
    m_ZoomSpeed = -_ZoomSpeed;
}

void FromAboveCamera::SetForward (const VECTOR3& _Forward) {
    m_Forward = _Forward;
}

void FromAboveCamera::UpdateAxis () {
    static float PIx2 = 6.283185f;

    // keep in range of 360 degrees
    if (m_RotY < -PIx2) {
        m_RotY += PIx2;
    } else if (m_RotY > PIx2) {
        m_RotY -= PIx2;
    }

    m_Right = VECTOR3 (1.0f, 0.0f, 0.0f);
    m_Up = VECTOR3 (0.0f, 1.0f, 0.0f);
    m_LookAt = VECTOR3 (0.0f, 0.0f, 1.0f);

    MATRIX44 matrix;

    // rotate around z axis
    /*m_LookAt.normalize ();
    matrix_rotation_axis_angle (matrix, m_LookAt, m_RotZ);
    m_Up = transform_vector (matrix, m_Up);
    m_Right = transform_vector (matrix, m_Right);*/

    // rotate around y axis
    m_Up.normalize ();
    matrix_rotation_axis_angle (matrix, m_Up, m_RotY);
    m_Right = transform_vector (matrix, m_Right);
    m_LookAt = transform_vector (matrix, m_LookAt);

    // rotate around x axis
    m_Right.normalize ();
    matrix_rotation_axis_angle (matrix, m_Right, m_RotX);
    //m_Up = transform_vector (matrix, m_Up);
    m_LookAt = transform_vector (matrix, m_LookAt);

    m_LookAt.normalize ();
    //m_Right = cross (m_Up, m_LookAt);
    m_Right.normalize ();
    //m_Up = cross (m_LookAt, m_Right);
    m_Up.normalize ();
    m_Forward = cross (m_Right, m_Up);
}