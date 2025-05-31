#ifndef ENVELOPPE_H
#define ENVELOPPE_H

// Envelope generator for controlling amplitude over time (Attack, Sustain, Release)
class Enveloppe {
public:
    // Constructor
    Enveloppe(double sampleRate);

    // Setters for envelope times
    void setAttack(double attackSeconds);
    void setRelease(double releaseSeconds);

    // Trigger envelope transitions
    void noteOn();   // Begin Attack phase
    void noteOff();  // Begin Release phase

    // Compute current amplitude value (0.0 to 1.0)
    double process();

    // Return whether a note is currently active
    bool isNoteOn();

private:
    double _attackTime;     // Attack duration (seconds)
    double _releaseTime;    // Release duration (seconds)
    double _currentValue;   // Current amplitude (0.0 to 1.0)
    double _sampleRate;     // Audio sample rate (Hz)
    bool _noteOn;           // Is a note currently held

    double _attackIncrement;  // Increment per sample during Attack
    double _releaseIncrement; // Decrement per sample during Release

    // Envelope state machine
    enum class EnveloppeState {
        Idle,     // Resting state, output = 0
        Attack,   // Increasing from 0 to 1
        Sustain,  // Holding at 1
        Release   // Decreasing from 1 to 0
    };

    EnveloppeState _state;  // Current state

    // Internal function to update increment values
    void calculateIncrements();
};

#endif // ENVELOPPE_H
