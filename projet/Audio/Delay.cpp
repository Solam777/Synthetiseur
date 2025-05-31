#include "Delay.h"
#include <algorithm>  // For std::min and std::clamp

// Constructor: initialize delay buffer size based on max delay
Delay::Delay(int sampleRate, float maxDelaySeconds)
    : _sampleRate(sampleRate)
{
    int maxSamples = static_cast<int>(maxDelaySeconds * sampleRate);
    _buffer.resize(maxSamples, 0.0f);
}

// Set the delay duration in seconds, clamped to buffer size
void Delay::setDelayTime(float seconds) {
    _delaySamples = std::min(static_cast<int>(seconds * _sampleRate), static_cast<int>(_buffer.size()));
}

// Set wet/dry mix: 0.0 = input only, 1.0 = delay only
void Delay::setMix(float mix) {
    _mix = std::clamp(mix, 0.0f, 1.0f);
}

// Process the input sample through the delay effect
float Delay::process(float inputSample) {
    // Compute read index using circular buffer logic
    const int readIndex = (_writeIndex - _delaySamples + _buffer.size()) % _buffer.size();

    float delayedSample = _buffer[readIndex];

    // Mix dry and wet signals
    float output = (1.0f - _mix) * inputSample + _mix * delayedSample;

    // Write current sample to delay buffer
    _buffer[_writeIndex] = inputSample;

    // Advance write index with wrap-around
    _writeIndex = (_writeIndex + 1) % _buffer.size();

    return output;
}
