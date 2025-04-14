
#ifndef ENVELOPPE_H
#define ENVELOPPE_H
#include <windows.h>

class Enveloppe {
public:
    Enveloppe (double SmapleRate);
    void setAttack(double attack);
    void setRelease(double release);
    void noteOn(); //call attack when an note is playing
    void noteOf(); //call release when an note is releasing
    double processus(); // calcul l'amplitute
    bool isNoteOn();

private:
    double _attackTime; //  time of attack (seconde)
    double  _releaseTime; //time of release (seconde)
    double _currentValue; // current value between 0-1
    double _sampleRate; //echantillons d'un song


    bool _noteOn; //state of note
    double _attackKey; // increment sample during attack
    double _releaseKey; //decrement sample during release

    void calculateIncrements(); //calcul les increment par echantillons

    enum class EnveloppeState {
        defaultVAl, // rest(repo) val = 0
        Attack, // key pressed
        Sustain, // maintin (val = 1)
        Release //key release
    };

    EnveloppeState _state;

};



#endif //ENVELOPPE_H
