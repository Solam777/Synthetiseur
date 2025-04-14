//
// Created by ssolam on 07/04/2025.
//

#ifndef OSCILLATEURS_H
#define OSCILLATEURS_H
#include <vector>

#include "Enveloppe.h"

enum class WaveForm { // different form
    Sine,
    Square,
    Saw
};

class Oscillateurs {

private:
    double _phase = 0.0; // keep the sample at an important moment
    double _frequency = 440.0; // height of song (severe or acute)
    double _frequencyOffset = 0.0; // -5hz , 5hz add an offset to the base frequency
    double _sampleRate = 4410.0; // sample(échantillons) per seconde
    WaveForm _wave_form = WaveForm::Sine;
    bool _isActive = true; //oscillateur active or not
    float generateSample();
    float generateSine();
    float generateSquare();
    float generateSaw();
    Enveloppe _enveloppe;



public:
    Oscillateurs(double Samplerate);
    void SetIsActive(bool isActive);
    [[nodiscard]] bool isActive() const;

    void SetWaveForm(WaveForm wave_form);
    void SetFrequency(double frequency);
    void SetFrequencyO0ffset(double frequencyOffset);
    double getFrequencyActual();
    // buffer: table where were store some samples audio
    // frameCount: number de frames to generate
    void fillBuffer(std::vector<float>& buffer , int frame );
    void noteOn();
    void noteOf();
};



#endif //OSCILLATEURS_H
