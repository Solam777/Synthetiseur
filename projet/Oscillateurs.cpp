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
    _enveloppe(sampleRate),
    _filtre(sampleRate),
    _delay(sampleRate, 2.0f)
{
    _delay.setDelayTime(0.5f); // demi-seconde
    _delay.setMix(0.4f);       // 40% delay

}

void Oscillateurs::noteOff() {
    _enveloppe.noteOff();
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

void Oscillateurs::fillBuffer(std::vector<float>& buffer, int frame,bool osc1Enable ,bool osc2Enable) {
    buffer.resize(frame * 2, 0.0f); //resize the buffer

    //si au moins OSC2 est actif, même si _isActive est false
    if (!_isActive && !osc2Enable) {
        return;
    }

    double phaseStep = 2.0 * M_PI * getFrequencyActual() / _sampleRate;

    for (int i = 0 ; i < frame ; i++) {
        float sample = 0.0f;

        if (osc1Enable) {
            sample += generateSample();
        }

        if (osc2Enable) {
           sample += generateSaw();
        }

        // Mix : diviser si les deux sont actifs
        if (osc1Enable && osc2Enable) {
            sample *= 0.5f; // éviter saturation
        }

        float env = _enveloppe.process();
        sample *= env;
        sample = _filtre.process(sample, 0);  // canal gauche
        float sampleR = _filtre.process(sample, 1);  // canal droit

        sample = _delay.process(sample);     // canal gauche avec delay
        sampleR = _delay.process(sampleR);   // canal droit avec delay


        // évite de désactiver tout si OSC2 est toujours active
        if (env <= 0.0  && !_enveloppe.isNoteOn() && !osc2Enable) {
            _isActive = false;
        }
        buffer.at(i*2) = sample; // left osc1
        buffer.at(i*2 +1 ) = sampleR; //right osc1

        _phase += phaseStep;


        // between 0 and 2pi
        while (_phase >= 2.0 * M_PI) {
            _phase -= 2.0 * M_PI;
        }


    }

}

void Oscillateurs::setAttack(double time) {
    _enveloppe.setAttack(time);
}

void Oscillateurs::setRelease(double time) {
    _enveloppe.setRelease(time);
}

void Oscillateurs::setCutoff(double c) {
    _filtre.setCutoff(c);
}
void Oscillateurs::setResonance(double r) {
    _filtre.setResonance(r);
}

void Oscillateurs::setDelayTime(float time) {
    _delay.setDelayTime(time);
}

void Oscillateurs::setDelayMix(float mix) {
    _delay.setMix(mix);

}

bool Oscillateurs::isNoteOn() {
    return _enveloppe.isNoteOn(); // ou un bool local

}





