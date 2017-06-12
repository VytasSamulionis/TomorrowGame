/** @file Window.h */
#pragma once

#include <Windows.h>

/** Creates a window. */
class Window {
public:
    /** Constructor. */
    Window ();

    /** Destructor. */
    ~Window ();

    /** Initializes window. 
    @param[in] _instance WinMain HINSTANCE parameter
    @param[in] _width width of the window
    @param[in] _height height of the window
    @param[in] _caption window's title 
    @return S_OK if succeeded. E_FAIL otherwise. */
    HRESULT Init (HINSTANCE _instance, int _width, int _height, LPSTR _caption);
    
    /** Setter: active window.
    @param[in] _window the pointer to the active window. */
    static void SetActiveWindow (Window* _window);

    /** Takes care of the window messages.
    @param[in] _hwnd window handle
    @param[in] _msg window message
    @param[in] _wParam window parameter
    @param[in] _lParam window parameter */
    LRESULT CALLBACK WndProc (HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

    /** It is required to be able to use WndProc. 
    @param[in] _hwnd window handle
    @param[in] _msg window message
    @param[in] _wParam window parameter
    @param[in] _lParam window parameter */
    static LRESULT CALLBACK WndProcWrapper (HWND _hwnd, UINT _msg, 
                                            WPARAM _wParam, LPARAM _lParam);

    /** Getter: window's handle. 
    @return window's handle */
    HWND GetHwnd () const;

    /** Setter: WNDWPROC function.
    By passing NULL, the default WndProc function is enabled.
    @param[in] _wndProc the pointer to the function */
    void SetMsgLoopFunc (WNDPROC _wndProc);
private:
    HWND m_hwnd;                /**< window handle. */
    HINSTANCE m_Instance;       /**< instance. */
    bool m_IsCustomMsgFunc;     /**< is custom WNDPROC function set? */
    WNDPROC m_CustomMsgFunc;    /**< pointer to custom WNDPROC function. */
};
