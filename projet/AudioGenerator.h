#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "portaudio.h"
#include "Oscillateurs.h"
class AudioGenerator {
public:
    void init();
    void cleanup() const;

    AudioGenerator();
    Oscillateurs oscillateur;
    void setAttack(double attacktime);
    void setRelease(double releasetime);
    void noteOn(int noteIndex);
    void noteOff();
    void setOscStates(bool osc1, bool osc2);

    int getLastNoteIndex() const;

    bool isOsc1Enabled() const { return _osc1Enabled; }
    bool isOsc2Enabled() const { return _osc2Enabled; }

private:
    int _lastNoteIndex = -1;  // -1 = aucune note
    bool _osc1Enabled = true;
    bool _osc2Enabled = true;
    PaStream *stream = nullptr;  // Maintenant un membre de classe
    static int audioCallback(const void *inputBuffer,
                             void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
};

#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
