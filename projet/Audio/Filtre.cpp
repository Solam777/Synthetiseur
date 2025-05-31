#include "Filtre.h"
#include <cmath>

// Constructor
Filtre::Filtre(double sampleRate) : _sampleRate(sampleRate) {
    updateParams();
}

// Set cutoff frequency and update coefficients
void Filtre::setCutoff(double cutoff) {
    _cutoff = cutoff;
    updateParams();
}

// Set resonance and update coefficients
void Filtre::setResonance(double resonance) {
    _resonance = resonance;
    updateParams();
}

// Update biquad coefficients based on current cutoff and resonance
void Filtre::updateParams() {
    double q = 0.5 / (1.0 - _resonance);  // Inverse of Q factor
    double omega = 2.0 * M_PI * _cutoff / _sampleRate;
    double alpha = sin(omega) / (2.0 * q);
    double cosw = cos(omega);
    double norm = 1.0 / (1.0 + alpha);

    a0 = (1.0 - cosw) * 0.5 * norm;
    a1 = (1.0 - cosw) * norm;
    a2 = (1.0 - cosw) * 0.5 * norm;
    b1 = -2.0 * cosw * norm;
    b2 = (1.0 - alpha) * norm;
}

// Process a sample through the filter for the given channel (0 = left, 1 = right)
float Filtre::process(float input, int channel) {
    float output = a0 * input
                 + a1 * x1[channel]
                 + a2 * x2[channel]
                 - b1 * y1[channel]
                 - b2 * y2[channel];

    // Update delay lines
    x2[channel] = x1[channel];
    x1[channel] = input;
    y2[channel] = y1[channel];
    y1[channel] = output;

    return output;
}
