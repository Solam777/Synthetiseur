#ifndef OSCILLATEURS_H
#define OSCILLATEURS_H

#include <vector>
#include "Delay.h"
#include "Filtre.h"
#include "Enveloppe.h"

// Available waveform types for the oscillator
enum class WaveForm {
    Sine,
    Square,
    Saw
};

// Synthesizer oscillator class with envelope, filter, and delay processing
class Oscillateurs {
public:
    // Constructor
    explicit Oscillateurs(double sampleRate);

    // Oscillator state
    void SetIsActive(bool isActive);
    [[nodiscard]] bool isActive() const;

    // Set waveform type
    void SetWaveForm(WaveForm wave_form);

    // Set main frequency and optional frequency offset
    void SetFrequency(double frequency);
    void SetFrequencyO0ffset(double frequencyOffset);
    double getFrequencyActual();

    // Audio rendering: fill stereo buffer with generated samples
    void fillBuffer(std::vector<float>& buffer, int frameCount, bool osc1Enable, bool osc2Enable);

    // Envelope control
    void noteOn();
    void noteOff();
    void setAttack(double time);
    void setRelease(double time);
    bool isNoteOn();

    // Filter parameters
    void setCutoff(double cutoff);
    void setResonance(double resonance);

    // Delay parameters
    void setDelayTime(float time);
    void setDelayMix(float mix);

private:
    double _sampleRate;
    double _phase;
    double _frequency;
    double _frequencyOffset;
    WaveForm _wave_form;
    bool _isActive;

    // Audio processing modules
    Enveloppe _enveloppe;
    Filtre _filtre;
    Delay _delay;

    // Internal waveform generation
    float generateSample();
    float generateSine();
    float generateSquare();
    float generateSaw();
};

#endif // OSCILLATEURS_H
