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
#include <optional>
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
    // TEXT_BULLET("-", "The subplots below show two versions of even/odd decomposition:");
    // {
    //     ImGui::Indent();
    //     TEXT_BULLET("-", "Even/odd decomposition: 'True' decomposition, resulting in an even signal symmetrical about the "
    //                       "vertical axis at <Num Samples>/2, and an odd symmetrical about an origin of <Num Samples>/2. "
    //                       "(Imagine rotating the second half of the signal CCW about <Num Samples>/2 and you'll see what "
    //                       "I mean.) This would be the 'ideal' decomposition used for full Fourier transform calcs.");
    //     TEXT_BULLET("-", "Interlaced decomposition: A simpler even/odd signal decomposition where the even signal "
    //                       "is literally the even samples in the original signal and the odd signal is the odd samples. "
    //                       "It seems simple, but is extremely useful for the FFT calculations we all use today.");
    //     ImGui::Unindent();
    // }
    constexpr int INPUT_SIGNAL_LENGTH = 12;
    constexpr int IMPULSE_RESPONSE_LENGTH = 4;
    static std::optional<dsp::StaticBuffer<double, INPUT_SIGNAL_LENGTH>> originalSignal;
    if (!originalSignal)
    {
        for (int i = 0; i < INPUT_SIGNAL_LENGTH; ++i)
        {
            if (i >= 4 && i < 8)
            {
                auto& signal = *originalSignal;
                signal[i] = 1;
            }
        }
    }

    static std::optional<dsp::StaticBuffer<double, IMPULSE_RESPONSE_LENGTH>> impulseResponse;
    if (!impulseResponse)
    {
        impulseResponse = {{
        { 0.0, 0.5, -.2, -0.1 }
        }};
    }

    static auto response = dsp::signals::convolve1D<double, INPUT_SIGNAL_LENGTH, IMPULSE_RESPONSE_LENGTH>(*originalSignal, *impulseResponse);

    static ImPlotDragToolFlags draggableFlags = ImPlotDragToolFlags_None;
    static ImPlotSubplotFlags subplotFlags = ImPlotSubplotFlags_None;
    std::array<double, INPUT_SIGNAL_LENGTH> inputSignalLabels{};
    std::iota(inputSignalLabels.begin(), inputSignalLabels.end(), 0);
    std::array<double, IMPULSE_RESPONSE_LENGTH> impulseResponseLabels{};
    std::iota(impulseResponseLabels.begin(), impulseResponseLabels.end(), 0);
    std::array<double, dsp::buffer_size(response)> outputLabels{};
    std::iota(outputLabels.begin(), outputLabels.end(), 0);

    TEXT_BULLET("-", "Below is the input signal you'll be testing with.");
    if (ImPlot::BeginSubplots("Signal and Impulse vs. Impulse Response", 1, 2, {-1, 400}, subplotFlags))
    {
        if (ImPlot::BeginPlot("Original Signal"))
        {
            ImPlot::SetupAxes("Sample Number","Voltage");
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine("v(t)", inputSignalLabels.data(), &(originalSignal->_data[0]), INPUT_SIGNAL_LENGTH);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Impulse Response"))
        {
            ImPlot::SetupAxes("Sample Number","Voltage");
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine("v(t)", impulseResponseLabels.data(), &(impulseResponse->_data[0]), IMPULSE_RESPONSE_LENGTH);
            ImPlot::EndPlot();
        }
    }
    ImPlot::EndSubplots();

    if (ImPlot::BeginPlot("Output Signal"))
    {
        ImPlot::SetupAxes("Sample Number","Voltage");
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine("v(t)", outputLabels.data(), &response._data[0], response.size());
        ImPlot::EndPlot();
    }

    ImGui::Spacing();
}

bool appStep(AppState& state)
{
    ImGui::Begin("Convolution Demo");

    if (ImGui::CollapsingHeader("Demo Description"))
    {
        ImGui::TextWrapped("This demo explains the process of convolution on a 1D and 2D signal");
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
    dsp::gui::ImguiWindow imguiWindow("Convolution Demo");
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