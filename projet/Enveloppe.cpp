
#include "Enveloppe.h"

#include <iostream>
#include <ostream>

Enveloppe::Enveloppe(double sampleRate) :

    _sampleRate(sampleRate),
    _state(EnveloppeState::defaultVAl),
    _attackTime(0.5),
    _releaseTime(0.75),
    _currentValue(0.0),
    _noteOn(false) {calculateIncrements();}

void Enveloppe::calculateIncrements() {
    _attackKey = 1.0/(_attackTime*_sampleRate);
    _releaseKey = 1.0/(_releaseTime*_sampleRate);
}

void Enveloppe::setAttack(double time) {
    _attackTime = time;
    calculateIncrements();


}

void Enveloppe::setRelease(double time) {
    _releaseTime = time;
    calculateIncrements();

}

void Enveloppe::noteOn() {
    if (_state == EnveloppeState::Attack || _state == EnveloppeState::Sustain) {
        // Ne redémarre pas si l’enveloppe est déjà en montée ou maintenue
        return;
    }
    _noteOn = true;
    _state = EnveloppeState::Attack;
}
void Enveloppe::noteOff() {

    _noteOn = false;
    _state = EnveloppeState::Release;
}
// Elle retourne la valeur actuelle de l’enveloppe, qui est entre 0 et 1,
// et qui est utilisée pour moduler le volume du signal audio
double Enveloppe::process () {
    switch (_state) {
        case EnveloppeState::defaultVAl:
            return 0.0;

        case EnveloppeState::Attack:
            _currentValue += _attackKey;
            if (_currentValue >= 1.0) {
                _currentValue = 1.0;
                _state = EnveloppeState::Sustain;
            }
            return _currentValue;

        case EnveloppeState::Release :
            _currentValue -= _releaseKey;
            if (_currentValue <= 0.0) {
                _currentValue = 0.0;
                _state = EnveloppeState::defaultVAl;
            }
            return _currentValue;

        case EnveloppeState::Sustain :
            return 1.0;

        default:
            return 0.0;
    }
}

bool Enveloppe::isNoteOn() {
    return _noteOn;
}





