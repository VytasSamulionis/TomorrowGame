#include "../include/FreeViewCamera.h"

FreeViewCamera::FreeViewCamera () {
    m_Thrust = 0.0f;
}

FreeViewCamera::~FreeViewCamera () {
}

void FreeViewCamera::Update (float _Delta) {
    // get rotations
    m_RotX += m_PitchSpeed * _Delta;
    m_RotY += m_YawSpeed * _Delta;
    m_RotZ += m_RollSpeed * _Delta;

    // calculate velocity vector
    m_Velocity = m_LookAt * m_Thrust;

    // update position
    m_Position += m_Velocity * _Delta;

    UpdateAxis ();
}

VECTOR3 FreeViewCamera::GetNextPosition () {
    VECTOR3 velocity = m_LookAt * m_Thrust;
    return m_Position + velocity;
}

void FreeViewCamera::AddRotationSpeed (float _x, float _y, float _z) {
    m_RollSpeed += _z;
    m_PitchSpeed += _x;
    m_YawSpeed += _y;
}

void FreeViewCamera::SetRotationSpeed (float _x, float _y, float _z) {
    m_RollSpeed = _z;
    m_PitchSpeed = _x;
    m_YawSpeed = _y;
}

void FreeViewCamera::SetPitchSpeed (float _Pitch) {
    m_PitchSpeed = _Pitch;
}

void FreeViewCamera::SetYawSpeed (float _Yaw) {
    m_YawSpeed = _Yaw;
}

void FreeViewCamera::SetRollSpeed (float _Roll) {
    m_RollSpeed = _Roll;
}

void FreeViewCamera::AddThrust (float _Thrust) {
    m_Thrust += _Thrust;
}

void FreeViewCamera::SetThrust (float _Thrust) {
    m_Thrust = _Thrust;
}