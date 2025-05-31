#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "portaudio.h"
#include "Oscillateurs.h"

// The AudioGenerator class handles audio stream initialization,
// real-time audio processing, and note triggering using PortAudio.
class AudioGenerator {
public:
    // Constructor and destructor-related operations
    AudioGenerator();
    void init();                // Initialize PortAudio and start the stream
    void cleanup() const;       // Stop and close the audio stream

    // Note control
    void noteOn(int noteIndex);  // Start a note (0 to 12)
    void noteOff();              // Stop the current note

    // Envelope configuration
    void setAttack(double attackTime);
    void setRelease(double releaseTime);

    // Oscillator state toggles (used by UI)
    void setOscStates(bool osc1, bool osc2);

    // Getters
    int getLastNoteIndex() const;         // Currently active note index
    bool isOsc1Enabled() const { return _osc1Enabled; }
    bool isOsc2Enabled() const { return _osc2Enabled; }
    double getCurrentTime() const { return _currentTimeInSeconds; }

    // Public oscillator object (directly used by UI)
    Oscillateurs oscillateur;

private:
    // Internal audio state
    int _lastNoteIndex = -1;     // -1 = no active note
    bool _osc1Enabled = true;    // Is oscillator 1 enabled
    bool _osc2Enabled = true;    // Is oscillator 2 enabled

    PaStream* stream = nullptr;  // PortAudio stream
    double _currentTimeInSeconds = 0.0; // Global time in seconds

    // PortAudio callback function (static)
    static int audioCallback(const void* inputBuffer,
                             void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);
};

#endif // SIMPLE_SYNTH_AUDIOGENERATOR_H
