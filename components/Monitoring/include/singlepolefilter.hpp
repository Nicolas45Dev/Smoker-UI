#ifndef SINGLE_POLE_FILTER_HPP
#define SINGLE_POLE_FILTER_HPP

#include <cmath>

namespace IIR::SinglePoleFilter {

class Filter {
public:
    Filter(float alpha) : alpha(alpha), prev_output(0.0f) {}

    float update(float input) {
        float output = alpha * input + (1.0f - alpha) * prev_output;
        prev_output = output;
        return output;
    }

    static float computeAlpha(const float cutoff_freq, const float sample_rate) {
        float y = 1 - cos(2 * M_PI * cutoff_freq / sample_rate);
        return -y + sqrt(y * y + 2 * y);
    }

private:
    float alpha;
    float prev_output;
};

};

#endif // SINGLE_POLE_FILTER_HPP