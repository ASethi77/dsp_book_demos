#include "libdsp/gui/imgui_window.h"
#include "implot.h"

#include <iostream>

namespace
{
    void onImguiContextCreated()
    {
        ImPlot::CreateContext();
    }

    void onImguiContextDestroyed()
    {
        ImPlot::DestroyContext();
    }
}

struct AppState
{
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_plot_window = true;
};

bool appStep(AppState& state)
{
    auto& [show_demo_window, show_another_window, show_plot_window] = state;
    auto& io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    if (show_plot_window)
        ImPlot::ShowDemoWindow(&show_plot_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))
            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    return true;
}

// Main code
int main(int, char**)
{
    AppState state;
    dsp::gui::ImguiWindow imguiWindow("Signal Decomposition Demo");
    dsp::gui::ImguiContextEventHandlers contextHandlers = {
        onImguiContextCreated,
        onImguiContextDestroyed,
        [&state]() -> bool
        {
            return appStep(state);
        }
    };
    imguiWindow.setImguiContextEventHandlers(contextHandlers);
    imguiWindow.initialize();
    imguiWindow.run();
    return 0;
}
