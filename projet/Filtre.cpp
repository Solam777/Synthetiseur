//
// Created by ssolam on 28/05/2025.
//

#include "Filtre.h"
#include <cmath>

Filtre::Filtre(double sampleRate) : _sampleRate(sampleRate) {
    updateParams();
}

void Filtre::setCutoff(double cutoff) {
    _cutoff = cutoff;
    updateParams();
}

void Filtre::setResonance(double resonance) {
    _resonance = resonance;
    updateParams();
}

void Filtre::updateParams() {
    double q = 0.5 / (1.0 - _resonance);
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

float Filtre::process(float input, int channel) {
    float output = a0 * input + a1 * x1[channel] + a2 * x2[channel]
                   - b1 * y1[channel] - b2 * y2[channel];

    // Mise à jour de l’état
    x2[channel] = x1[channel];
    x1[channel] = input;
    y2[channel] = y1[channel];
    y1[channel] = output;

    return output;
}




