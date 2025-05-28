//
// Created by ssolam on 28/05/2025.
//

#ifndef FILTRE_H
#define FILTRE_H

class Filtre {
public:
    Filtre(double sampleRate);
    void setCutoff(double cutoff);
    void setResonance(double resonance);
    float process(float input, int channel); // channel = 0 (gauche) ou 1 (droite)

private:
    void updateParams();

    double _sampleRate;
    double _cutoff = 10000.0;
    double _resonance = 0.5;

    // Coefficients
    double a0, a1, a2, b1, b2;

    // Historique par canal
    float x1[2]{}, x2[2]{}, y1[2]{}, y2[2]{};
};


#endif //FILTRE_H
