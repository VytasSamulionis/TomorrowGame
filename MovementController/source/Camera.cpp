#include "../include/Camera.h"

using namespace cml;

Camera::Camera () {
    SetPosition (VECTOR3 (0.0f, 0.0f, 0.0f));
    SetLookingPoint (VECTOR3 (0.0f, 0.0f, 1.0f));
    SetUpVector (VECTOR3 (0.0f, 1.0f, 0.0f));
    SetVelocity (VECTOR3 (0.0f, 0.0f, 0.0f));
    SetRight (VECTOR3 (1.0f, 0.0f, 0.0f));
    m_RollSpeed = m_PitchSpeed = m_YawSpeed = 0.0f;
    m_RotX = m_RotY = m_RotZ = 0.0f;
    m_Quat = QUATERNION (0.0f, 0.0f, 0.0f, 1.0f);
}

Camera::~Camera () {
}

VECTOR3 Camera::GetPosition () const {
    return m_Position;
}

VECTOR3 Camera::GetLookingPoint () const {
    return m_LookAt;
}

VECTOR3 Camera::GetUpVector () const {
    return m_Up;
}

VECTOR3 Camera::GetRight () const {
    return m_Right;
}

VECTOR3 Camera::GetVelocity () const {
    return m_Velocity;
}

void Camera::SetPosition (VECTOR3 _Position) {
    m_Position = _Position;
}

void Camera::SetLookingPoint (VECTOR3 _LookAt) {
    m_LookAt = _LookAt;
}

void Camera::SetUpVector (VECTOR3 _Up) {
    m_Up = _Up;
}

void Camera::SetRight (VECTOR3 _Right) {
    m_Right = _Right;
}

void Camera::SetVelocity (VECTOR3 _Velocity) {
    m_Velocity = _Velocity;
}

void Camera::UpdateAxis () {
    static float PIx2 = 6.283185f;

    // keep in range of 360 degrees
    if (m_RotX < -PIx2) {
        m_RotX += PIx2;
    } else if (m_RotX > PIx2) {
        m_RotX -= PIx2;
    }
    /*if (m_RotX < -1.4f) {
        m_RotX = -1.4f;
    } else if (m_RotX > 1.4f) {
        m_RotX = 1.4f;
    }*/
    if (m_RotY < -PIx2) {
        m_RotY += PIx2;
    } else if (m_RotY > PIx2) {
        m_RotY -= PIx2;
    }
    if (m_RotZ < -PIx2) {
        m_RotZ += PIx2;
    } else if (m_RotZ > PIx2) {
        m_RotZ -= PIx2;
    }

    m_Right = VECTOR3 (1.0f, 0.0f, 0.0f);
    m_Up = VECTOR3 (0.0f, 1.0f, 0.0f);
    m_LookAt = VECTOR3 (0.0f, 0.0f, 1.0f);

    MATRIX44 matrix;

    // rotate around z axis
    m_LookAt.normalize ();
    matrix_rotation_axis_angle (matrix, m_LookAt, m_RotZ);
    m_Up = transform_vector (matrix, m_Up);
    m_Right = transform_vector (matrix, m_Right);

    // rotate around y axis
    m_Up.normalize ();
    matrix_rotation_axis_angle (matrix, m_Up, m_RotY);
    m_Right = transform_vector (matrix, m_Right);
    m_LookAt = transform_vector (matrix, m_LookAt);

    // rotate around x axis
    m_Right.normalize ();
    matrix_rotation_axis_angle (matrix, m_Right, m_RotX);
    m_Up = transform_vector (matrix, m_Up);
    m_LookAt = transform_vector (matrix, m_LookAt);

    m_LookAt.normalize ();
    m_Right = cross (m_Up, m_LookAt);
    m_Right.normalize ();
    m_Up = cross (m_LookAt, m_Right);
    m_Up.normalize ();
}