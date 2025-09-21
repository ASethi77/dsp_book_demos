#ifndef SIGNAL_PROCESSING_BOOK_SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_BOOK_SIGNAL_PROCESSING_H

#include "libdsp/storage/buffer.h"

namespace dsp::signals
{
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