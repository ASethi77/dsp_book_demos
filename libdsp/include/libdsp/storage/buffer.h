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
    template<typename T>
    struct BufferStats
    {
        T minValue = std::numeric_limits<T>::max();
        T maxValue = std::numeric_limits<T>::min();
    };

    template<typename T, int N, bool WithStats = true>
    class StaticBuffer
    {
    public:
        /***
         * Wraps operator[] accesses to underlying buffer with
         * rolling statistics (sample mean, min, etc.);
         * Disabled at compile time via template arg WithStats
         */
        struct BufferEntry
        {
            std::reference_wrapper<T> _dataRef;
            BufferStats<T>& _statsRef;
            BufferEntry& operator=(const T& value)
            {
                _dataRef.get() = value;
                if constexpr (WithStats)
                {
                    _statsRef.minValue = std::min(_statsRef.minValue, value);
                    _statsRef.maxValue = std::max(_statsRef.maxValue, value);
                }
                return *this;
            }
            friend std::ostream& operator<< (std::ostream& os, const BufferEntry& b)
            {
                os << b._dataRef.get();
                return os;
            }
        };

        std::array<T, N> _data;
        BufferStats<T> _stats;
        BufferEntry operator[](unsigned long n)
        {
            return {
                std::ref(_data[n]),
                _stats
            };
        }

        [[nodiscard]] consteval size_t size() const { return N; }

        T min() const
        {
            return _stats.minValue;
        }

        T max() const
        {
            return _stats.maxValue;
        }
    };
}

#endif //SIGNAL_PROCESSING_BOOK_BUFFER_H