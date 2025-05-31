#ifndef DELAY_H
#define DELAY_H

#include <vector>

// Delay effect class.
// This class implements a basic delay line with configurable delay time and mix.
class Delay {
public:
    // Constructor
    // sampleRate: audio sample rate (e.g., 44100)
    // maxDelaySeconds: maximum delay allowed (defines internal buffer size)
    Delay(int sampleRate, float maxDelaySeconds);

    // Set the delay time in seconds (clamped to maxDelaySeconds)
    void setDelayTime(float seconds);

    // Set the mix level: 0.0 = dry only, 1.0 = delayed only
    void setMix(float mix);

    // Process a single audio sample and return the delayed result
    float process(float inputSample);

private:
    std::vector<float> _buffer;   // Circular delay buffer
    int _writeIndex = 0;          // Write position in the buffer
    int _sampleRate;              // Sampling rate (Hz)

    int _delaySamples = 0;        // Delay in samples
    float _mix = 0.5f;            // Wet/dry mix ratio (0 = dry, 1 = wet)
};

#endif // DELAY_H
