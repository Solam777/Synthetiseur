#include <iostream>
#include "AudioGenerator.h"
#include "MainWindow.h"

int main() {
    // MainWindow mainWindow;
    // mainWindow.init();
    // mainWindow.run();

    AudioGenerator audio;
    audio.init();

    audio.oscillateur.SetWaveForm(WaveForm::Square);
    audio.oscillateur.SetFrequency(440.0);
    audio.oscillateur.SetIsActive(true);

    // Attendre l'entrée utilisateur pour terminer
    std::cout << "Appuyez sur Entrée pour arrêter..." << std::endl;
    std::cin.get();

    audio.cleanup();
    return 0;
}