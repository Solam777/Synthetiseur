#include "Enveloppe.h"

// Constructor
Enveloppe::Enveloppe(double sampleRate)
    : _sampleRate(sampleRate),
      _state(EnveloppeState::Idle),
      _attackTime(0.5),
      _releaseTime(0.75),
      _currentValue(0.0),
      _noteOn(false)
{
    calculateIncrements();
}

// Compute per-sample increments based on attack/release times
void Enveloppe::calculateIncrements() {
    _attackIncrement = 1.0 / (_attackTime * _sampleRate);
    _releaseIncrement = 1.0 / (_releaseTime * _sampleRate);
}

// Set attack time and update increment
void Enveloppe::setAttack(double time) {
    _attackTime = time;
    calculateIncrements();
}

// Set release time and update increment
void Enveloppe::setRelease(double time) {
    _releaseTime = time;
    calculateIncrements();
}

// Begin attack phase if not already in Attack or Sustain
void Enveloppe::noteOn() {
    if (_state == EnveloppeState::Attack || _state == EnveloppeState::Sustain)
        return;

    _noteOn = true;
    _state = EnveloppeState::Attack;
}

// Begin release phase
void Enveloppe::noteOff() {
    _noteOn = false;
    _state = EnveloppeState::Release;
}

// Main processing function: returns amplitude based on envelope state
double Enveloppe::process() {
    switch (_state) {
        case EnveloppeState::Idle:
            return 0.0;

        case EnveloppeState::Attack:
            _currentValue += _attackIncrement;
            if (_currentValue >= 1.0) {
                _currentValue = 1.0;
                _state = EnveloppeState::Sustain;
            }
            return _currentValue;

        case EnveloppeState::Sustain:
            return 1.0;

        case EnveloppeState::Release:
            _currentValue -= _releaseIncrement;
            if (_currentValue <= 0.0) {
                _currentValue = 0.0;
                _state = EnveloppeState::Idle;
            }
            return _currentValue;

        default:
            return 0.0;
    }
}

// Return whether a note is currently active
bool Enveloppe::isNoteOn() {
    return _noteOn;
}
