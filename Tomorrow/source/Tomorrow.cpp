#include "../include/Game.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

Game* g_Game;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE _instance, HINSTANCE _previous, LPSTR _cmdLine, int _show) {
    Window* win = NULL;
    g_Game = NULL;
    try {
        win = new Window ();
        if (FAILED(win->Init (_instance, WINDOW_WIDTH, WINDOW_HEIGHT, "Tomorrow"))) {
            MessageBox (NULL, "Error", "Error", 0);
            delete win;
            return 0;
        }
        g_Game = new Game (win->GetHwnd(), _instance);
        win->SetMsgLoopFunc ((WNDPROC)WndProc);
        
        MSG msg;
        ZeroMemory (&msg, sizeof (MSG));
        while (true) {
            if (msg.message == WM_QUIT) {
                break;
            }
            if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            } else {
                g_Game->ProcessInGameInput ();
                if (g_Game->IsPaused ()) {
                    g_Game->RenderMainScreen ();
                } else {
                    if (!g_Game->IsLevelLoaded()) {
                        g_Game->StartNew ();
                        if (g_Game->IsContinuing()) {
                            g_Game->Load ();
                        }
                    } else {
                        g_Game->Update ();
                    }
                }
                g_Game->ResetInput ();
            }
        }
        if (g_Game->IsLevelLoaded()) {
            g_Game->Save ();
        }
    } catch (std::bad_alloc) {
        ShowWindow (win->GetHwnd(), SW_HIDE);
        MessageBox (win->GetHwnd(), "Not enough memory.", "Error", MB_ICONERROR);
    } catch (ErrorMessage e) {
        if (e.GetErrorCode() == ERRC_NOT_READY) {   /* Possible device lost */
            g_Game->Save ();
        }
        ShowWindow (win->GetHwnd(), SW_HIDE);
        MessageBox (win->GetHwnd(), e.GetErrorMessage(), "Error", MB_ICONERROR);
    } catch (...) {
        ShowWindow (win->GetHwnd(), SW_HIDE);
        MessageBox (win->GetHwnd(), "Unknown error.", "Error", MB_ICONERROR);
    }
    delete g_Game;
    delete win;

    return 0;
}

LRESULT CALLBACK WndProc (HWND _wnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
    g_Game->UpdateInput (_message, _wParam);
    switch (_message) {
        case WM_MOUSEWHEEL:
            if ((short) HIWORD(_wParam) < 0) {
                g_Game->ZoomCamera (-500.0f);
            } else {
                g_Game->ZoomCamera (500.0f);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (_wnd, _message, _wParam, _lParam);
    }
    return 0;
}