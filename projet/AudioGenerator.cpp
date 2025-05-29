#include <iostream>
#include "AudioGenerator.h"
#include "Oscillateurs.h"
#include <cmath>
#include <vector>

constexpr int FRAMES_PER_BUFFER {1024};
constexpr int SAMPLE_RATE {44100};

void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();
    if(errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): "
                  << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    // Configurer l'oscillateur
    oscillateur.SetWaveForm(WaveForm::Sine);
    oscillateur.SetFrequency(440.0);  // La 440Hz
    oscillateur.SetIsActive(true);

    // Ouvrir le flux audio avec this en userData
    PaError errorStream = Pa_OpenDefaultStream(&this->stream,
                                     0,
                                     2,
                                     paFloat32,
                                     SAMPLE_RATE,
                                     FRAMES_PER_BUFFER,
                                     audioCallback,
                                     this);  // Passer this comme userData

    if(errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_OpenDefaultStream(): "
                  << Pa_GetErrorText(errorStream) << std::endl;
        return;
    }

    errorStream = Pa_StartStream(this->stream);
    if(errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_StartStream(): "
                  << Pa_GetErrorText(errorStream) << std::endl;
    }
}

void AudioGenerator::cleanup() const {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}

AudioGenerator::AudioGenerator() : oscillateur(44100.0) {
    // Initialisation si nécessaire
}

int AudioGenerator::audioCallback(const void *inputBuffer,
                                  void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData) {
    // Cast userData en AudioGenerator*
    AudioGenerator* audio = static_cast<AudioGenerator*>(userData);
    float *out = static_cast<float*>(outputBuffer);

    // Créer un buffer temporaire pour les échantillons
    std::vector<float> buffer;

    // Remplir le buffer avec les données de l'oscillateur
    audio->oscillateur.fillBuffer(buffer, framesPerBuffer,audio->isOsc1Enabled(),
                              audio->isOsc2Enabled());

    // Copier les données du buffer dans outputBuffer
    for (unsigned int i = 0; i < framesPerBuffer * 2; i++) {
        out[i] = buffer[i];
    }

    return paContinue;
}

void AudioGenerator::noteOn(int noteIndex) {
    if (noteIndex == _lastNoteIndex && oscillateur.isActive()) {
        // Même note déjà active → ne redémarre pas
        return;
    }

    _lastNoteIndex = noteIndex;

    double freq = 220.0 * std::pow(2.0, noteIndex / 12.0); // conversion note -> freq
    oscillateur.SetFrequency(freq);
    oscillateur.SetIsActive(true);
    oscillateur.noteOn();  // démarre l'enveloppe seulement si nouvelle note
}


void AudioGenerator::noteOff() {
    _lastNoteIndex = -1;
    oscillateur.noteOff();
}

void AudioGenerator::setAttack(double time) {
    oscillateur.setAttack(time); // ajoute cette méthode dans Oscillateurs si absente
}

void AudioGenerator::setRelease(double time) {
    oscillateur.setRelease(time);
}

void AudioGenerator::setOscStates(bool osc1, bool osc2) {
    _osc1Enabled = osc1;
    _osc2Enabled = osc2;
}
int AudioGenerator::getLastNoteIndex() const {
    return _lastNoteIndex;
}

