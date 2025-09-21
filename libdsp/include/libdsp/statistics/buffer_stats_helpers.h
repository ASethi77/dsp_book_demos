#ifndef SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H
#define SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H

#include "libdsp/storage/buffer.h"
#include "libdsp/statistics/sample.h"

namespace dsp::statistics
{
    template<int N>
    void PopulateNormalDistribution(StaticBuffer<double, N>& storage)
    {
        for (int i = 0; i < N; ++i)
        {
            storage[i] = RandomGauss();
        }
    }
}

#endif //SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H