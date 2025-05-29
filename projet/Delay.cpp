//
// Created by ssolam on 28/05/2025.
//

#include "Delay.h"
#include <algorithm>

Delay::Delay(int sampleRate, float maxDelaySeconds)
    : _sampleRate(sampleRate)
{
    int maxSamples = static_cast<int>(maxDelaySeconds * sampleRate);
    _buffer.resize(maxSamples, 0.0f);
}

void Delay::setDelayTime(float seconds) {
    _delaySamples = std::min(static_cast<int>(seconds * _sampleRate), (int)_buffer.size());
}

void Delay::setMix(float mix) {
    _mix = std::clamp(mix, 0.0f, 1.0f);
}

float Delay::process(float inputSample) {
    const int readIndex = (_writeIndex - _delaySamples + _buffer.size()) % _buffer.size();

    float delayedSample = _buffer[readIndex];
    float output = (1.0f - _mix) * inputSample + _mix * delayedSample;

    // On ecrit le nouveau sample dans le buffer circulaire
    _buffer[_writeIndex] = inputSample;
    _writeIndex = (_writeIndex + 1) % _buffer.size();

    return output;
}

