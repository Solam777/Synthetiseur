#include <iostream>
#include <cmath>
#include <vector>

#include "AudioGenerator.h"
#include "Oscillateurs.h"

// Constants
constexpr int FRAMES_PER_BUFFER = 1024;
constexpr int SAMPLE_RATE = 44100;

// Constructor: Initialize the oscillator with the sample rate
AudioGenerator::AudioGenerator() : oscillateur(SAMPLE_RATE) {
    // No additional setup here
}

// Initializes PortAudio and opens the audio stream
void AudioGenerator::init() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    // Configure default oscillator
    oscillateur.SetWaveForm(WaveForm::Sine);
    oscillateur.SetFrequency(440.0); // Standard A4 (440Hz)
    oscillateur.SetIsActive(true);

    // Open default output stream
    err = Pa_OpenDefaultStream(&stream,
                               0,                // No input channels
                               2,                // Stereo output
                               paFloat32,        // 32-bit floating point samples
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               audioCallback,    // Static callback
                               this);            // Pass current object as userData

    if (err != paNoError) {
        std::cerr << "PortAudio stream open error: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    // Start audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio start error: " << Pa_GetErrorText(err) << std::endl;
    }
}

// Cleans up audio stream and shuts down PortAudio
void AudioGenerator::cleanup() const {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}

// PortAudio callback: generates audio and fills output buffer
int AudioGenerator::audioCallback(const void* inputBuffer,
                                  void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void* userData) {
    auto* audio = static_cast<AudioGenerator*>(userData);
    auto* out = static_cast<float*>(outputBuffer);

    std::vector<float> buffer;

    // Fill audio buffer using oscillators
    audio->oscillateur.fillBuffer(buffer,
                                  static_cast<int>(framesPerBuffer),
                                  audio->isOsc1Enabled(),
                                  audio->isOsc2Enabled());

    // Copy audio buffer to output (stereo: left/right)
    for (unsigned int i = 0; i < framesPerBuffer * 2; i++) {
        out[i] = buffer[i];
    }

    // Increment global time based on processed samples
    audio->_currentTimeInSeconds += static_cast<double>(framesPerBuffer) / SAMPLE_RATE;

    return paContinue;
}

// Triggers a new note if it’s different from the currently active one
void AudioGenerator::noteOn(int noteIndex) {
    if (noteIndex == _lastNoteIndex && oscillateur.isActive()) {
        return; // Do not restart the same note
    }

    _lastNoteIndex = noteIndex;

    // Convert note index to frequency
    double frequency = 220.0 * std::pow(2.0, noteIndex / 12.0);
    oscillateur.SetFrequency(frequency);
    oscillateur.SetIsActive(true);
    oscillateur.noteOn(); // Start envelope
}

// Stops the current note and releases the envelope
void AudioGenerator::noteOff() {
    _lastNoteIndex = -1;
    oscillateur.noteOff();
}

// Passes attack value to the oscillator's envelope
void AudioGenerator::setAttack(double time) {
    oscillateur.setAttack(time);
}

// Passes release value to the oscillator's envelope
void AudioGenerator::setRelease(double time) {
    oscillateur.setRelease(time);
}

// Stores oscillator activation states
void AudioGenerator::setOscStates(bool osc1, bool osc2) {
    _osc1Enabled = osc1;
    _osc2Enabled = osc2;
}

// Getter for currently active note index
int AudioGenerator::getLastNoteIndex() const {
    return _lastNoteIndex;
}
