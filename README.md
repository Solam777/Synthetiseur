# 🎹 Oxono Synth

Oxono Synth est un **synthétiseur audio numérique** développé en **C++** dans le cadre du projet 4DEV4D.  
Il repose sur les principes de la **synthèse soustractive** et permet de générer et manipuler des sons en temps réel grâce à une interface graphique en **Dear ImGui** et une gestion audio via **PortAudio**.

---

## 🚀 Fonctionnalités

- 🎛 **Deux oscillateurs (OSC1 & OSC2)**  
  - OSC1 : forme d’onde au choix (**Sinus, Carré, Dent de scie**)  
  - OSC2 : toujours en **Dent de scie**  
  - Activation/désactivation individuelle via checkboxes  
  - Paramètre de **Frequency Offset** pour OSC1  

- ⏱ **Enveloppe (ADSR simplifié)**  
  - Attack [0 – 1s]  
  - Release [0 – 2s]  

- 🔊 **Filtre passe-bas**  
  - Cutoff [20Hz – 20kHz]  
  - Résonance [0 – 1]  

- ⏳ **Effet Delay**  
  - Delay Time [0.1 – 2s]  
  - Delay Mix [0 – 1]  

- 🎹 **Clavier virtuel et support clavier physique**  
  - 13 boutons à l’écran pour déclencher des notes  
  - Mapping AZERTY (`q z s e d f t g y h u j k`) simulant les touches d’un piano  

- ↩️ **Boucle d’Undo/Redo non nécessaire (contrôles temps réel)**  
- ⚡ Fonctionne en **temps réel** avec un **buffer de 256 frames** et un **sample rate de 44.1 kHz**

---

## 📸 Aperçu


---

## ⚙️ Technologies utilisées

- [SDL3](https://github.com/libsdl-org/SDL) – gestion de la fenêtre et des événements clavier  
- [Dear ImGui](https://github.com/ocornut/imgui) – interface graphique  
- [PortAudio](http://www.portaudio.com/) – gestion audio cross-plateforme  
- **C++23** – langage principal  


