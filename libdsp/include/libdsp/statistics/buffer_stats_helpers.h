#ifndef SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H
#define SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H

#include "libdsp/storage/buffer.h"
#include "libdsp/statistics/sample.h"

#include <random>

namespace dsp::statistics
{
    template<int N>
    void PopulateNormalDistribution(StaticBuffer<double, N>& storage, double mean, double sdev)
    {
        std::random_device rd{};
        std::mt19937 gen{rd()};

        // Values near the mean are the most likely. Standard deviation
        // affects the dispersion of generated values from the mean.
        std::normal_distribution d{mean, sdev};

        auto sample = d(gen);
        for (int i = 0; i < N; ++i)
        {
            storage[i] = d(gen);
        }
    }
}

#endif //SIGNAL_PROCESSING_BOOK_BUFFER_STATS_HELPERS_H