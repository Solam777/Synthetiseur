//
// Created by ssolam on 07/04/2025.
//

#include "Oscillateurs.h"

#include <cmath>
#include <vector>

Oscillateurs::Oscillateurs(double sampleRate) :
    _sampleRate(sampleRate),
    _phase(0.0),
    _frequency(440.0),
    _frequencyOffset(0.0),
    _isActive(true),
    _wave_form(WaveForm::Sine),
    _enveloppe(sampleRate)
{

}

void Oscillateurs::noteOf() {
    _enveloppe.noteOf();
}

void Oscillateurs::noteOn() {
    _enveloppe.noteOn();
}

float Oscillateurs::generateSample() {
    switch (_wave_form) {
        case WaveForm::Sine:
           return generateSine();
        case WaveForm::Square:
           return generateSquare();
        case WaveForm::Saw:
            return generateSaw();
        default:
            return 0.0f;
    }
}

float Oscillateurs::generateSine() {
    return 0.5f * std::cos(_phase);
}

float Oscillateurs::generateSquare() {
    return (std::cos(_phase) >= 0.0) ? 0.5f : -0.5f;
}

float Oscillateurs::generateSaw() {
    double normalizedPhase = _phase / (2.0 * M_PI);
    return static_cast<float>(normalizedPhase - 0.5);
}

void Oscillateurs::SetIsActive(bool isActive) {
    _isActive = isActive;
}

bool Oscillateurs::isActive() const {
    return _isActive;
}

void Oscillateurs::SetWaveForm(WaveForm wave_form) {
    _wave_form = wave_form;
}

void Oscillateurs::SetFrequency(double frequency) {
    _frequency = frequency;
}

void Oscillateurs::SetFrequencyO0ffset(double frequencyOffset) {
    _frequencyOffset = frequencyOffset;

}

double Oscillateurs::getFrequencyActual() {
    return _frequency+_frequencyOffset;
}

void Oscillateurs::fillBuffer(std::vector<float>& buffer, int frame) {
    buffer.resize(frame * 2, 0.0f); //resize the buffer
    if (!_isActive) {
        return;
    }

    double phaseStep = 2.0 * M_PI * getFrequencyActual() / _sampleRate;

    for (int i = 0 ; i < frame ; i++) {
        float sample = generateSample();

        //put enveloppe
        double enveloppeValue = _enveloppe.processus();
        sample *=  generateSample();
        //if enveleppe = 0 et note = false desactive oscillateur
        if (enveloppeValue <= 0.0  && _enveloppe.isNoteOn()) {
            _isActive = false;
        }
        buffer.at(i*2)=sample; // left osc1
        buffer.at(i*2 +1 ) = sample; //right osc1
        _phase += phaseStep;
        // between 0 and 2pi
        while (_phase >= 2.0 * M_PI) {
            _phase -= 2.0 * M_PI;
        }
    }
}

