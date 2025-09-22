#ifndef SIGNAL_PROCESSING_BOOK_SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_BOOK_SIGNAL_PROCESSING_H

#include "libdsp/storage/buffer.h"

namespace dsp::signals
{
    /**
     * Implements a 1D convolution against the input buffer using the output-side algorithm:
     *   y[i] = \sum_{j=0}{M - 1} h[j]x[i - j]
     * Given an input signal `x` of N samples and an impulse response `h` of M samples.
     * @tparam T The input signal datatype.
     * @tparam InputSignalLength The length of the input signal in sample counts (N)
     * @tparam ImpulseResponseLength The length of the impulse response in sample counts (M)
     * @param x The input signal buffer
     * @param h The impulse response buffer
     * @return The convolved output signal `y`
     */
    template<typename T, int InputSignalLength, int ImpulseResponseLength>
    StaticBuffer<T, ImpulseResponseLength + InputSignalLength - 1>
    convolve1D(StaticBuffer<T, InputSignalLength>& x, StaticBuffer<T, ImpulseResponseLength>& h)
    {
        constexpr int outputLength = InputSignalLength + ImpulseResponseLength - 1;
        StaticBuffer<T, outputLength> y;

        for (int i = 0; i < outputLength; ++i)
        {
            T response = 0;
            for (int j = 0; j < ImpulseResponseLength; ++j)
            {
                if (i - j >= 0 &&
                    i - j < InputSignalLength)
                {
                    response += h[j] * x[i - j];
                }
            }
            y[i] = response;
        }

        return y;
    }

    template<typename T, int N>
    std::pair<StaticBuffer<T, N>, StaticBuffer<T, N>>
    decomposeEvenOdd(const StaticBuffer<T, N>& buffer)
    {
        auto decomposition = std::make_pair(StaticBuffer<T, N>(), StaticBuffer<T, N>());
        for (int i = 1; i < N; ++i)
        {
            double evenDecomp = (buffer._data[i] + buffer._data[N - i]) / 2.f;
            double oddDecomp = (buffer._data[i] - buffer._data[N - i]) / 2.f;
            decomposition.first[i] = evenDecomp;
            decomposition.second[i] = oddDecomp;
        }
        return std::move(decomposition);
    }

    template<typename T, int N>
    std::pair<StaticBuffer<T, N>, StaticBuffer<T, N>>
    decomposeInterlaced(const StaticBuffer<T, N>& buffer)
    {
        auto decomposition = std::make_pair(StaticBuffer<T, N>(), StaticBuffer<T, N>());
        for (int i = 0; i < N; ++i)
        {
            double evenDecomp = i % 2 == 0 ? buffer._data[i] : 0.f;
            double oddDecomp = i % 2 == 1 ? buffer._data[i] : 0.f;
            decomposition.first[i] = evenDecomp;
            decomposition.second[i] = oddDecomp;
        }
        return std::move(decomposition);
    }
}

#endif //SIGNAL_PROCESSING_BOOK_SIGNAL_PROCESSING_H