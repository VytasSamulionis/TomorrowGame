#include "../include/InputSystem.h"

InputSystem::InputSystem () {
    for (UINT i = 0; i < 256; i++) {
        m_InputSignal[i] = false;
        m_LastSignal[i] = false;
    }
}

void InputSystem::Reset () {
    //memset (m_InputSignal, 0, sizeof(bool) * 256);
    memset (m_LastSignal, 0, sizeof(bool) * 256);
}

void InputSystem::Update (UINT _Message, UINT _WParam) {
    if (_Message == WM_KEYDOWN) {
            m_InputSignal[_WParam] = true;
            m_LastSignal[_WParam] = false;
            return;
    }
    if (_Message == WM_KEYUP) {
            m_InputSignal[_WParam] = false;
            m_LastSignal[_WParam] = true;
            return;
    }
    switch (_Message) {
        case WM_LBUTTONDOWN:
            m_InputSignal[VK_LBUTTON] = true;
            m_LastSignal[VK_LBUTTON] = false;
            break;
        case WM_RBUTTONDOWN:
            m_InputSignal[VK_RBUTTON] = true;
            m_LastSignal[VK_RBUTTON] = false;
            break;
        case WM_MBUTTONDOWN:
            m_InputSignal[VK_MBUTTON] = true;
            m_LastSignal[VK_MBUTTON] = false;
            break;
        case WM_LBUTTONUP:
            m_InputSignal[VK_LBUTTON] = false;
            m_LastSignal[VK_LBUTTON] = true;
            break;
        case WM_RBUTTONUP:
            m_InputSignal[VK_RBUTTON] = false;
            m_LastSignal[VK_RBUTTON] = true;
            break;
        case WM_MBUTTONUP:
            m_InputSignal[VK_MBUTTON] = false;
            m_LastSignal[VK_MBUTTON] = true;
            break;
    }
    if (GetAsyncKeyState (VK_LSHIFT) & 0x8000f) {
        m_InputSignal[VK_LSHIFT] = true;
        m_LastSignal[VK_LSHIFT] = false;
    } else if (m_InputSignal[VK_LSHIFT]) {
        m_InputSignal[VK_LSHIFT] = false;
        m_LastSignal[VK_LSHIFT] = true;
    }
    if (GetAsyncKeyState (VK_RSHIFT) & 0x8000f) {
        m_InputSignal[VK_RSHIFT] = true;
        m_LastSignal[VK_RSHIFT] = false;
    } else if (m_InputSignal[VK_RSHIFT]) {
        m_InputSignal[VK_RSHIFT] = false;
        m_LastSignal[VK_RSHIFT] = true;
    }
    if (GetAsyncKeyState (VK_RCONTROL) & 0x8000f) {
        m_InputSignal[VK_RCONTROL] = true;
        m_LastSignal[VK_RCONTROL] = false;
    } else if (m_InputSignal[VK_RCONTROL]) {
        m_InputSignal[VK_RCONTROL] = false;
        m_LastSignal[VK_RCONTROL] = true;
    }
    if (GetAsyncKeyState (VK_LCONTROL) & 0x8000f) {
        m_InputSignal[VK_LCONTROL] = true;
        m_LastSignal[VK_LCONTROL] = false;
    } else if (m_InputSignal[VK_LCONTROL]) {
        m_InputSignal[VK_LCONTROL] = false;
        m_LastSignal[VK_LCONTROL] = true;
    }
}

bool InputSystem::IsPressed (UINT _Key) const {
    return m_InputSignal[_Key];
}

bool InputSystem::WasPressed (UINT _Key) {
    bool result = m_LastSignal[_Key];
    m_LastSignal[_Key] = false;
    return result;
}