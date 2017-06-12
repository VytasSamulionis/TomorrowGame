#include "../include/FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera () {
    m_Speed = m_SlideSpeed = 0.0f;
}

FirstPersonCamera::~FirstPersonCamera () {
}

void FirstPersonCamera::Update (float _Delta) {
    m_RotX += m_PitchSpeed * _Delta;
    m_RotY += m_YawSpeed * _Delta;
    m_RotZ += m_RollSpeed * _Delta;

    VECTOR3 slide;
    slide = m_Right * m_SlideSpeed * _Delta;
    m_Velocity = m_Forward * m_Speed * _Delta;
    m_Position += m_Velocity + slide;

    UpdateAxis ();
}

VECTOR3 FirstPersonCamera::GetNextPosition () {
    VECTOR3 slide = m_Right * m_SlideSpeed;
    VECTOR3 velocity = m_Forward * m_Speed;
    return m_Position + velocity + slide;
}

void FirstPersonCamera::AddRotationSpeed (float _Pitch, float _Yaw) {
    m_PitchSpeed += _Pitch;
    m_YawSpeed += _Yaw;
}

void FirstPersonCamera::SetRotationSpeed (float _Pitch, float _Yaw) {
    m_PitchSpeed = _Pitch;
    m_YawSpeed = _Yaw;
}

void FirstPersonCamera::SetPitchSpeed (float _Pitch) {
    m_PitchSpeed = _Pitch;
}

void FirstPersonCamera::SetYawSpeed (float _Yaw) {
    m_YawSpeed = _Yaw;
}

void FirstPersonCamera::SetLeanSpeed (float _Lean) {
    m_RollSpeed = _Lean;
}

void FirstPersonCamera::AddSpeed (float _Speed) {
    m_Speed += _Speed;
}

void FirstPersonCamera::SetSpeed (float _Speed) {
    m_Speed = _Speed;
}

void FirstPersonCamera::AddSlideSpeed (float _SlideSpeed) {
    m_SlideSpeed += _SlideSpeed;
}

void FirstPersonCamera::SetSlideSpeed (float _SlideSpeed) {
    m_SlideSpeed = _SlideSpeed;
}

void FirstPersonCamera::SetForward (const VECTOR3& _Forward) {
    m_Forward = _Forward;
}

void FirstPersonCamera::RotateForward (float _x, float _y, float _z) {
    MATRIX44 matrix;
    matrix_rotation_euler (matrix, _x, _y, _z, cml::euler_order_xyz);
    m_Forward = transform_vector (matrix, m_Forward);
}

void FirstPersonCamera::UpdateAxis () {
    static float PIx2 = 6.283185f;

    // keep in range of 80 degrees
    if (m_RotX < -1.4f) {
        m_RotX = -1.4f;
    } else if (m_RotX > 1.4f) {
        m_RotX = 1.4f;
    }
    // keep in range of 360 degrees
    if (m_RotY < -PIx2) {
        m_RotY += PIx2;
    } else if (m_RotY > PIx2) {
        m_RotY -= PIx2;
    }
    // keep in range of 30 degrees
    if (m_RotZ < -0.523599f) {
        m_RotZ = -0.523599f;
    } else if (m_RotZ > 0.523599f) {
        m_RotZ = 0.523599f;
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