#ifndef SIGNAL_PROCESSING_BOOK_IMGUI_WINDOW_H
#define SIGNAL_PROCESSING_BOOK_IMGUI_WINDOW_H

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#ifdef _WIN32
#include <windows.h>        // SetProcessDPIAware()
#endif

#include <string>
#include <functional>

namespace dsp::gui
{
    struct ImguiContextEventHandlers
    {
        std::function<void(void)> on_imgui_context_created;
        std::function<void(void)> on_imgui_context_destroyed;
    };

    struct SDLInputEventHandlers
    {
        std::function<void(void)> on_quit;
        std::function<void(void)> on_window_close;
        // std::function<void(void)> on_window_minimized;
    };

    class ImguiWindow
    {
    public:
        ImguiWindow(const std::string& windowTitle);
        ImguiWindow(const ImguiWindow&) = delete;
        ImguiWindow(ImguiWindow&&) = delete;
        ~ImguiWindow();

        bool initialize();
        void setWindowTitle(const std::string& windowTitle);
        void setImguiContextEventHandlers(const ImguiContextEventHandlers& handlers);
        void setSDLInputEventHandlers(const SDLInputEventHandlers& handlers);
        void processInputEvents();

        SDL_Window* getWindow();
        SDL_GLContext getGLContext();

    private:
        SDL_Window* _window = nullptr;
        SDL_GLContext _context = nullptr;
        ImguiContextEventHandlers _imguiContextEventHandlers;
        SDLInputEventHandlers _sdlInputEventHandlers;
        std::string _windowTitle;
        float _dpiScale = 1.5f;
    };
}

#endif //SIGNAL_PROCESSING_BOOK_IMGUI_WINDOW_H