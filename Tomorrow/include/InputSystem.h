/** @file InputSystem.h
Tracks the pushed keys.
*/
#pragma once

#include <Windows.h>

/** Tracks the pushed keys. */
class InputSystem {
public:
    /** Constructor. */
    InputSystem ();

    /** Updates the status of keys.
    It should be called for every message sent to window.
    @param[in] _message a window message
    @param[in] _wParam a wParam sent to window */
    void Update (UINT _message, UINT _wParam);

    /** Resets all the keys.
    It should be called once per frame. */
    void Reset ();

    /** Is the key pressed?
    @param[in] _key the key which should be checked
    @return true the key is pressed
    @return false the key is not pressed */
    bool IsPressed (UINT _key) const;

    /** Was the key pressed?
    After the call to this method, 
    a flag of the pressed key is cleared.
    @param[in] _key the key which should be checked
    @return true the key was pressed
    @return false the key was not pressed */
    bool WasPressed (UINT _key);

protected:
    bool m_InputSignal[256];    /**< The flags of the keys which are pressed. */
    bool m_LastSignal[256];     /**< The flags of the keys which were pressed. */
};