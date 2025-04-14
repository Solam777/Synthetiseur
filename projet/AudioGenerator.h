#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "portaudio.h"
#include "Oscillateurs.h"
class AudioGenerator {
public:
    void init();
    void cleanup();

    AudioGenerator();
    Oscillateurs oscillateur;


private:
    PaStream *stream = nullptr;  // Maintenant un membre de classe


    static int audioCallback(const void *inputBuffer,
                             void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
};

#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
