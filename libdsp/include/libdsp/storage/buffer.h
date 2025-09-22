#ifndef SIGNAL_PROCESSING_BOOK_BUFFER_H
#define SIGNAL_PROCESSING_BOOK_BUFFER_H

#include <array>
#include <cstddef>
#include <functional>
#include <iosfwd>
#include <limits>
#include <optional>

namespace dsp
{
    /***
     * Summary statistics for the buffer classes. (min/max, etc.)
     */
    template<typename T>
    struct BufferStats
    {
        T minValue = std::numeric_limits<T>::max();
        T maxValue = std::numeric_limits<T>::min();
    };

    template<typename T, int N, bool WithStats>
    class StaticBuffer;

    /** Wraps operator[] to the backing std::array for
     * live statistics calculations.
     */
    template<typename T, bool WithStats = true>
    struct BufferEntry
    {
        std::reference_wrapper<T> _dataRef;
        std::optional<BufferStats<T>>& _statsRef;
        BufferEntry& operator=(const T& value)
        {
            _dataRef.get() = value;
            if constexpr (WithStats)
            {
                _statsRef->minValue = std::min(_statsRef->minValue, value);
                _statsRef->maxValue = std::max(_statsRef->maxValue, value);
            }
            return *this;
        }
        template<typename OtherT>
        T operator*(const BufferEntry<OtherT>& other)
        {
            return _dataRef.get() * other._dataRef.get();
        }
        friend std::ostream& operator<< (std::ostream& os, const BufferEntry& b)
        {
            os << b._dataRef.get();
            return os;
        }
    };

    /***
     * Static buffer. Wraps std::array<T, N> with a few extra
     * signal processing goodies such as running summary statistics.
     *
     * Template arg WithStats can be used to optionally disable
     * live summary statistic calculations if desired.
     */
    template<typename T, int N, bool WithStats = true>
    class StaticBuffer
    {
    public:
        std::array<T, N> _data;
        std::optional<BufferStats<T>> _stats;

        [[nodiscard]] constexpr size_t size() const { return N; }

        BufferEntry<T> operator[](unsigned long n)
        {
            {
                return {
                    std::ref(_data[n]),
                    _stats
                };
            }
        }

        T min()
        {
            if (!_stats)
            {
                for (int i = 0; i < N; ++i)
                {
                    _stats->minValue = std::min(_stats->minValue, _data[i]);
                    _stats->maxValue = std::max(_stats->maxValue, _data[i]);
                }
            }
            return _stats->minValue;
        }

        T max()
        {
            if (!_stats)
            {
                for (int i = 0; i < N; ++i)
                {
                    _stats->minValue = std::min(_stats->minValue, _data[i]);
                    _stats->maxValue = std::max(_stats->maxValue, _data[i]);
                }
            }
            return _stats->maxValue;
        }


    };

    template<typename T, int N, bool WithStats>
    consteval int buffer_size(const StaticBuffer<T, N, WithStats>&)
    {
        return N;
    }
}

#endif //SIGNAL_PROCESSING_BOOK_BUFFER_H