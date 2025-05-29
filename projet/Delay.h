//
// Created by ssolam on 28/05/2025.
//

#ifndef DELAY_H
#define DELAY_H
#include <vector>


class Delay {
public:
    Delay(int sampleRate, float maxDelaySeconds);

    void setDelayTime(float seconds);
    void setMix(float mix); // 0 = dry, 1 = wet

    float process(float inputSample);

private:
    std::vector<float> _buffer;
    int _writeIndex = 0;
    int _sampleRate;

    int _delaySamples = 0;
    float _mix = 0.5f; // 50% wet, 50% dry
};



#endif //DELAY_H
