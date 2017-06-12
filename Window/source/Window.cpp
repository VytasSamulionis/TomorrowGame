#include "../include/Window.h"

static Window* activeWindow = NULL;

Window::Window()
: m_hwnd(0)
, m_Instance(0)
, m_IsCustomMsgFunc(false)
, m_CustomMsgFunc(NULL)
{
	activeWindow = this;
}

Window::~Window()
{
	if (activeWindow == this)
	{
		activeWindow = NULL;
	}
}

HRESULT Window::Init(HINSTANCE _instance, int _width, int _height, LPSTR _caption)
{
	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = (WNDPROC)Window::WndProcWrapper;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = _instance;
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = _caption;
	if (!RegisterClass(&wndClass))
	{
		return E_FAIL;
	}

	m_hwnd = CreateWindow(_caption, _caption, WS_OVERLAPPEDWINDOW, 0, 0, _width, _height, 0, 0, _instance, 0);
	if (!m_hwnd)
	{
		return E_FAIL;
	}
	
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
	return S_OK;
}

void Window::SetActiveWindow(Window* _window)
{
	activeWindow = _window;
}

LRESULT CALLBACK Window::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
	}
	return 0;
}

LRESULT CALLBACK Window::WndProcWrapper(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (activeWindow->m_IsCustomMsgFunc)
	{
		return activeWindow->m_CustomMsgFunc(_hwnd, _msg, _wParam, _lParam);
	}
	else
	{
		return activeWindow->WndProc(_hwnd, _msg, _wParam, _lParam);
	}
}


HWND Window::GetHwnd() const
{
	return m_hwnd;
}

void Window::SetMsgLoopFunc(WNDPROC _wndProc)
{
	m_CustomMsgFunc = _wndProc;
	m_IsCustomMsgFunc = m_CustomMsgFunc != NULL;
}
