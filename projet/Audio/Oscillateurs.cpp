#include "Oscillateurs.h"
#include <cmath>
#include <vector>

// Constructor
Oscillateurs::Oscillateurs(double sampleRate)
    : _sampleRate(sampleRate),
      _phase(0.0),
      _frequency(440.0),
      _frequencyOffset(0.0),
      _isActive(true),
      _wave_form(WaveForm::Sine),
      _enveloppe(sampleRate),
      _filtre(sampleRate),
      _delay(sampleRate, 2.0f) // Max delay: 2 seconds
{
    _delay.setDelayTime(0.5f);
    _delay.setMix(0.4f);
}

// Note ON: trigger envelope attack
void Oscillateurs::noteOn() {
    _enveloppe.noteOn();
}

// Note OFF: trigger envelope release
void Oscillateurs::noteOff() {
    _enveloppe.noteOff();
}

// Generate sample based on waveform
float Oscillateurs::generateSample() {
    switch (_wave_form) {
        case WaveForm::Sine: return generateSine();
        case WaveForm::Square: return generateSquare();
        case WaveForm::Saw: return generateSaw();
        default: return 0.0f;
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
    return _frequency + _frequencyOffset;
}

// Render stereo audio samples into buffer
void Oscillateurs::fillBuffer(std::vector<float>& buffer, int frameCount, bool osc1Enable, bool osc2Enable) {
    buffer.resize(frameCount * 2, 0.0f);

    if (!_isActive && !osc2Enable)
        return;

    double phaseStep = 2.0 * M_PI * getFrequencyActual() / _sampleRate;

    for (int i = 0; i < frameCount; ++i) {
        float sample = 0.0f;

        if (osc1Enable)
            sample += generateSample();
        if (osc2Enable)
            sample += generateSaw();

        // Prevent clipping if both OSCs are enabled
        if (osc1Enable && osc2Enable)
            sample *= 0.5f;

        // Apply envelope
        float env = _enveloppe.process();
        sample *= env;

        // Filter processing
        sample = _filtre.process(sample, 0); // left
        float sampleR = _filtre.process(sample, 1); // right

        // Delay processing
        sample = _delay.process(sample);
        sampleR = _delay.process(sampleR);

        // Turn off oscillator when envelope has ended
        if (env <= 0.0 && !_enveloppe.isNoteOn() && !osc2Enable)
            _isActive = false;

        buffer[i * 2]     = sample;  // Left
        buffer[i * 2 + 1] = sampleR; // Right

        _phase += phaseStep;
        while (_phase >= 2.0 * M_PI)
            _phase -= 2.0 * M_PI;
    }
}

// Envelope control
void Oscillateurs::setAttack(double time) {
    _enveloppe.setAttack(time);
}

void Oscillateurs::setRelease(double time) {
    _enveloppe.setRelease(time);
}

bool Oscillateurs::isNoteOn() {
    return _enveloppe.isNoteOn();
}

// Filter control
void Oscillateurs::setCutoff(double c) {
    _filtre.setCutoff(c);
}

void Oscillateurs::setResonance(double r) {
    _filtre.setResonance(r);
}

// Delay control
void Oscillateurs::setDelayTime(float time) {
    _delay.setDelayTime(time);
}

void Oscillateurs::setDelayMix(float mix) {
    _delay.setMix(mix);
}
