#include <format>

#include "libdsp/gui/imgui_window.h"
#include "libdsp/storage/buffer.h"
#include "libdsp/statistics/buffer_stats_helpers.h"
#include "libdsp/signal_processing/signal_processing.h"
#include "implot.h"

#include <iostream>
#include <numeric>

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

void Demo_SignalDecomposition(AppState& state)
{
    constexpr int NUM_POINTS = 25;
    static bool samplesInitialized = false;
    static dsp::StaticBuffer<double, NUM_POINTS> samples;
    if (!samplesInitialized)
    {
        dsp::statistics::PopulateNormalDistribution(samples);
        samplesInitialized = true;
    }
    static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
    static ImPlotSubplotFlags subplotFlags = ImPlotSubplotFlags_None;
    std::array<double, NUM_POINTS> xLabels{};
    std::iota(xLabels.begin(), xLabels.end(), 0);

    ImGui::BulletText("Modify signal samples by clicking and dragging them.");
    if (ImPlot::BeginPlot("Original Sample")) {
        ImPlot::SetupAxes("Sample Number","Voltage");
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        for (int i = 0; i < NUM_POINTS; ++i)
        {
            ImPlot::DragPoint(i, &xLabels[i], &samples._data[i], ImVec4(1, 1, 1, 1), 4, flags);
        }
        ImPlot::PlotLine("v(t)", xLabels.data(), &samples._data[0], NUM_POINTS);
        ImPlot::EndPlot();
    }

    ImGui::Spacing();
    ImGui::BulletText("The subplots below show the even/odd decomposition of the input signal");

    auto [evenDecompBuffer, oddDecompBuffer] = dsp::signals::decomposeEvenOdd(samples);
    if (ImPlot::BeginSubplots("Even/Odd Decomposition", 1, 2, {-1, 400}, subplotFlags))
    {
        if (ImPlot::BeginPlot("Even Decomposition"))
        {
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::SetupAxesLimits(xLabels[0], xLabels[NUM_POINTS - 1],
                std::min(evenDecompBuffer.min(), oddDecompBuffer.min()) - 0.25,
                std::max(evenDecompBuffer.max(), oddDecompBuffer.max()) + 0.25);
            ImPlot::PlotLine("v(t)", xLabels.data(), evenDecompBuffer._data.data(), NUM_POINTS);
        }
        ImPlot::EndPlot();
        if (ImPlot::BeginPlot("Odd Decomposition"))
        {
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::SetupAxesLimits(xLabels[0], xLabels[NUM_POINTS - 1],
            std::min(evenDecompBuffer.min(), oddDecompBuffer.min()) - 0.25,
            std::max(evenDecompBuffer.max(), oddDecompBuffer.max()) + 0.25);
            ImPlot::PlotLine("v(t)", xLabels.data(), oddDecompBuffer._data.data(), NUM_POINTS);
        }
        ImPlot::EndPlot();
    }
    ImPlot::EndSubplots();
}

bool appStep(AppState& state)
{
    auto& [show_demo_window, show_another_window, show_plot_window] = state;
    auto& io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

    if (show_plot_window)
    {
        ImPlot::ShowDemoWindow();
    }

    if (show_demo_window)
    {
        ImGui::ShowDemoWindow();
    }

    ImGui::Begin("Filter Even/Odd Decomposition Demo");

    if (ImGui::CollapsingHeader("Demo Description"))
    {
        ImGui::TextWrapped("This demonstration shows you what it's like to decompose an input signal into even/odd signals.");
    }
    if (ImGui::CollapsingHeader("Sample Signals"))
    {
        Demo_SignalDecomposition(state);
    }

    ImGui::End();

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
