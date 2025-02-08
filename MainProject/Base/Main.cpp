//
// Main.cpp
//

#include "pch.h"
#include "GameBase.h"

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
    std::unique_ptr<GameBase> g_game;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!XMVerifyCPUSupport())
        return 1;

    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    if (FAILED(initialize))
        return 1;

#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    g_game = std::make_unique<GameBase>();

    // Register class and create window
    {
        // Register class
        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
        wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszClassName = L"DXTK_WindowClass";
        wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
        if (!RegisterClassExW(&wcex))
            return 1;

        // Create window
        int w, h;
        g_game->GetDefaultSize(w, h);

        RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

#if !DXTK_FULLSCREEN
        AdjustWindowRect(&rc, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, FALSE);

        const int  kScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
        const int  kScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        const long kWndWidth  = rc.right  - rc.left;
        const long kWndHeight = rc.bottom - rc.top;

        const int  kWndPosX = (kScreenWidth  - kWndWidth ) / 2;
        const int  kWndPosY = (kScreenHeight - kWndHeight) / 2;

        HWND hwnd = CreateWindowExW(0, L"DXTK_WindowClass", L"",
            WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
            kWndPosX, kWndPosY, kWndWidth, kWndHeight, nullptr, nullptr, hInstance,
            nullptr);
        // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"DirectX12_TestWindowClass", L"DirectX12 Test", WS_POPUP,
        // to default to fullscreen.

        if (!hwnd)
            return 1;

        ShowWindow(hwnd, nCmdShow);
        // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.
#else
        HWND hwnd = CreateWindowExW(
			WS_EX_TOPMOST, L"DXTK_WindowClass", L"",
			WS_POPUP | WS_SYSMENU,
			0, 0, 0, 0, nullptr, nullptr, hInstance,
			nullptr
		);
        if (!hwnd)
            return 1;

        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowWindow(hwnd, SW_SHOWMAXIMIZED);
#endif

        SetWindowLongPtr( hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()) );

        GetClientRect(hwnd, &rc);
        g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
    }

    // Main message loop
    MSG msg{};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_game->Tick();
        }
    }

    g_game.reset();

    return static_cast<int>(msg.wParam);
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool s_in_sizemove = false;
    static bool s_in_suspend  = false;
    static bool s_minimized   = false;
#if !DXTK_FULLSCREEN
    static bool s_fullscreen  = false;
    // TODO: Set s_fullscreen to true if defaulting to fullscreen.
#else
    static bool s_fullscreen  = true;
#endif

    auto game = reinterpret_cast<GameBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_PAINT:
        if (s_in_sizemove && game)
        {
            game->Tick();
        }
        else
        {
            PAINTSTRUCT ps;
            (void)BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            return 0;
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && game)
                    game->OnSuspending();
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && game)
                game->OnResuming();
            s_in_suspend = false;
        }
        return 0;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 180;
        }
        return 0;

    case WM_ACTIVATEAPP:
        if (game)
        {
            if (wParam)
            {
                game->OnActivated();
            }
            else
            {
                game->OnDeactivated();
            }
        }

#if DXTK_KEYBOARD
        Keyboard::ProcessMessage(message, wParam, lParam);
#endif
#if DXTK_MOUSE
        Mouse::ProcessMessage(message, wParam, lParam);
#endif

        return 0;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && game)
                game->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && game)
                    game->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

#if DXTK_MOUSE || DXTK_GAMEPAD
    case WM_INPUT:
#if DXTK_MOUSE
        Mouse::ProcessMessage(message, wParam, lParam);
#endif
#if DXTK_GAMEPAD
        wi::rawgamepad::ParseMessage((void*)lParam);
#endif
        return 0;
#endif

#if DXTK_MOUSE
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        Mouse::ProcessMessage(message, wParam, lParam);
        return 0;
#endif

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;
        }
        [[fallthrough]];
#ifdef DXTK_KEYBOARD
    case WM_KEYUP:
    case WM_SYSKEYUP:
        Keyboard::ProcessMessage(message, wParam, lParam);
        return 0;
#else
        return DefWindowProc(hWnd, wParam, lParam, )
#endif

    case WM_SYSKEYDOWN:
#if DXTK_KEYBOARD
        Keyboard::ProcessMessage(message, wParam, lParam);
#endif
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                constexpr DWORD WND_STYLE = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
                SetWindowLongPtr(hWnd, GWL_STYLE, WND_STYLE);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                int width  = static_cast<int>(screen::width);
                int height = static_cast<int>(screen::height);
                if (game)
                    game->GetDefaultSize(width, height);

                RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
                AdjustWindowRect(&rc, WND_STYLE, FALSE);

                const int  kScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
                const int  kScreenHeight = GetSystemMetrics(SM_CYSCREEN);

                const long kWndWidth  = rc.right  - rc.left;
                const long kWndHeight = rc.bottom - rc.top;

                const int  kWndPosX = (kScreenWidth  - kWndWidth ) / 2;
                const int  kWndPosY = (kScreenHeight - kWndHeight) / 2;

                ShowWindow(hWnd, SW_SHOWNORMAL);

                SetWindowPos(
                    hWnd, HWND_TOP, kWndPosX, kWndPosY, kWndWidth, kWndHeight,
                    SWP_NOZORDER | SWP_FRAMECHANGED
                );
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, 0);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }
        return 0;

    //case WM_MENUCHAR:
    //    // A menu is active and the user presses a key that does not correspond
    //    // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
    //    return MAKELRESULT(0, MNC_CLOSE);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Exit helper
void ExitGame() noexcept
{
    PostQuitMessage(0);
}
