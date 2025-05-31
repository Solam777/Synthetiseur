#ifndef FILTRE_H
#define FILTRE_H

// Simple stereo low-pass filter using biquad formula
class Filtre {
public:
    // Constructor
    Filtre(double sampleRate);

    // Set cutoff frequency (Hz)
    void setCutoff(double cutoff);

    // Set resonance (0.0 to 1.0)
    void setResonance(double resonance);

    // Process a single sample for the given channel (0 = left, 1 = right)
    float process(float input, int channel);

private:
    // Update filter coefficients
    void updateParams();

    double _sampleRate;     // Sample rate (Hz)
    double _cutoff = 10000; // Cutoff frequency (Hz)
    double _resonance = 0.5;// Resonance (0.0 - 1.0)

    // Filter coefficients (biquad)
    double a0, a1, a2;
    double b1, b2;

    // Delay buffers for left and right channels
    float x1[2]{}, x2[2]{}; // Previous inputs
    float y1[2]{}, y2[2]{}; // Previous outputs
};

#endif // FILTRE_H
