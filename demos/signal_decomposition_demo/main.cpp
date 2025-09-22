#include "libdsp/gui/imgui_window.h"
#include "libdsp/gui/text_helpers.h"
#include "libdsp/storage/buffer.h"
#include "libdsp/statistics/buffer_stats_helpers.h"
#include "libdsp/signal_processing/signal_processing.h"
#include "implot.h"

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <format>

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
    TEXT_BULLET("-", "The subplots below show two versions of even/odd decomposition:");
    {
        ImGui::Indent();
        TEXT_BULLET("-", "Even/odd decomposition: 'True' decomposition, resulting in an even signal symmetrical about the "
                          "vertical axis at <Num Samples>/2, and an odd symmetrical about an origin of <Num Samples>/2. "
                          "(Imagine rotating the second half of the signal CCW about <Num Samples>/2 and you'll see what "
                          "I mean.) This would be the 'ideal' decomposition used for full Fourier transform calcs.");
        TEXT_BULLET("-", "Interlaced decomposition: A simpler even/odd signal decomposition where the even signal "
                          "is literally the even samples in the original signal and the odd signal is the odd samples. "
                          "It seems simple, but is extremely useful for the FFT calculations we all use today.");
        ImGui::Unindent();
    }

    constexpr int NUM_POINTS = 26;
    static bool samplesInitialized = false;
    static dsp::StaticBuffer<double, NUM_POINTS> samples;
    if (!samplesInitialized)
    {
        dsp::statistics::BatchSampleGaussian(samples, 0.0, 0.25);
        samplesInitialized = true;
    }
    static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
    static ImPlotSubplotFlags subplotFlags = ImPlotSubplotFlags_None;
    std::array<double, NUM_POINTS> xLabels{};
    std::iota(xLabels.begin(), xLabels.end(), 0);

    TEXT_BULLET("-", "Modify the original signal below by dragging points to see their effect on the even/odd decompositions.");
    if (ImPlot::BeginPlot("Original signal")) {
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

    auto [evenInterlacedBuffer, oddInterlacedBuffer] = dsp::signals::decomposeInterlaced(samples);
    if (ImPlot::BeginSubplots("Interlaced Decomposition", 1, 2, {-1, 400}, subplotFlags))
    {
        if (ImPlot::BeginPlot("Even Decomposition (Interlaced)"))
        {
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::SetupAxesLimits(xLabels[0], xLabels[NUM_POINTS - 1],
                std::min(evenInterlacedBuffer.min(), oddInterlacedBuffer.min()) - 0.25,
                std::max(evenInterlacedBuffer.max(), oddInterlacedBuffer.max()) + 0.25);
            ImPlot::PlotLine("v(t)", xLabels.data(), evenInterlacedBuffer._data.data(), NUM_POINTS);
        }
        ImPlot::EndPlot();
        if (ImPlot::BeginPlot("Odd Decomposition (Interlaced)"))
        {
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::SetupAxesLimits(xLabels[0], xLabels[NUM_POINTS - 1],
            std::min(evenInterlacedBuffer.min(), oddInterlacedBuffer.min()) - 0.25,
            std::max(evenInterlacedBuffer.max(), oddInterlacedBuffer.max()) + 0.25);
            ImPlot::PlotLine("v(t)", xLabels.data(), oddInterlacedBuffer._data.data(), NUM_POINTS);
        }
        ImPlot::EndPlot();
    }
    ImPlot::EndSubplots();
}

bool appStep(AppState& state)
{
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