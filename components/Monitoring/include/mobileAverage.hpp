#ifndef MOBILE_AVERAGE_HPP
#define MOBILE_AVERAGE_HPP

#include <array>
#include <numeric>

namespace FIR::MobileAverage {

template <typename T, std::size_t N>
class MobileAverage {
public:
    MobileAverage() : values{}, index(0), count(0) {}

    void addValue(T value) {
        values[index] = value;
        index = (index + 1) % N;
        if (count < N) {
            count++;
        }
    }

    float getAverage() const {
        if (count == 0) {
            return 0.0f;
        }
        return std::accumulate(values.begin(), values.begin() + count, 0.0f) / count;
    }

    bool isFull() const {
        return count == N;
    }

    bool isEmpty() const {
        return count == 0;
    }

    void reset() {
        values.fill(T{});
        index = 0;
        count = 0;
    }

private:
    std::array<T, N> values;
    std::size_t index;
    std::size_t count;
};

} // namespace FIR::MobileAverage

#endif // MOBILE_AVERAGE_HPP