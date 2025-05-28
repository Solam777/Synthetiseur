
#include "MainWindow.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);
//checkbox1
const char* osc1 ="OSC 1";
bool checkbox = true;
//slider
static float sineValue = 2; // Variable pour stocker la valeur du slider
//Combobox
const char* waveforms[] = { "SINE", "SQUARE", "SAW" };
static int waveformIndex = 0; // Index de la forme d'onde sélectionnée
//checkbox2
const char* osc2 ="OSC 2";
bool checkbox2 = true;
// slider 2 attack
static float attack = 0.5;

// slider 3 release
static float release = 0.75;

// slider 4 FilterCutof
static float FilterCutof = 20000;

// slider 5 FilterResonance
static float FilterResonance = 0.5;

// slider 6 DelayTime
static float DelayTime = 1;

// slider 7 DelayMix : intensité de l’effet delay
static float DelayMix = 1;

//bouton

const ImVec2 &size = ImVec2(0,10);



void MainWindow::init() {
    // Initialiser PortAudio
    audio.init();
    audio.setAttack(attack);
    audio.setRelease(release);


    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return;
    }
    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("", 800, 600, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }
    SDL_SetWindowPosition(
            window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup DearImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

std::unordered_map<SDL_Scancode, int> keyToNote = { //association touche bouton
    {SDL_SCANCODE_Q, 0},
    {SDL_SCANCODE_Z, 1},
    {SDL_SCANCODE_S, 2},
    {SDL_SCANCODE_E, 3},
    {SDL_SCANCODE_D, 4},
    {SDL_SCANCODE_F, 5},
    {SDL_SCANCODE_T, 6},
    {SDL_SCANCODE_G, 7},
    {SDL_SCANCODE_Y, 8},
    {SDL_SCANCODE_H, 9},
    {SDL_SCANCODE_U, 10},
    {SDL_SCANCODE_J, 11},
    {SDL_SCANCODE_K, 12}
};

void MainWindow::run() {
    constexpr auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done { false };
    while (!done){
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            // Quit
            if (event.type == SDL_EVENT_QUIT)
                done = true;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                auto it = keyToNote.find(event.key.scancode);
                if (it != keyToNote.end()) {
                    audio.noteOn(it->second);
                }
            }

            if (event.type == SDL_EVENT_KEY_UP) {
                auto it = keyToNote.find(event.key.scancode);
                if (it != keyToNote.end()) {
                    audio.noteOff();
                }
            }

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                SDL_GetWindowID(window) == event.window.windowID)
                done = true;
        }


        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // all the UI code description
        draw();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer,
                                    clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Calculate time spent and sleep if needed
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void MainWindow::draw() {
    ImGui::Begin("My Window");

    // OSC 1 actif
    ImGui::Checkbox(osc1, &checkbox);
    audio.oscillateur.SetIsActive(checkbox); // on/off
    audio.setOscStates(checkbox, checkbox2);

    // Forme d’onde
    if (ImGui::Combo("OSC1 Waveform", &waveformIndex, waveforms, 3)) {
        switch (waveformIndex) {
            case 0: audio.oscillateur.SetWaveForm(WaveForm::Sine); break;
            case 1: audio.oscillateur.SetWaveForm(WaveForm::Square); break;
            case 2: audio.oscillateur.SetWaveForm(WaveForm::Saw); break;
        }
    }

    // Frequency Offset
    if (ImGui::SliderFloat("OSC1 Frequency Offset", &sineValue, -5, 5)) {
        audio.oscillateur.SetFrequencyO0ffset(sineValue);
    }

    // OSC 2
    ImGui::Checkbox(osc2, &checkbox2);


    // Attack
    if (ImGui::SliderFloat("Attack", &attack, 0, 2)) {
        audio.setAttack(attack);
    }

    // Release
    if (ImGui::SliderFloat("Release", &release, 0, 2)) {
        audio.setRelease(release);
    }

    //cutoff attenue le song
    if (ImGui::SliderFloat("FilterCutoff", &FilterCutof, 20, 20000)) {
        audio.oscillateur.setCutoff(FilterCutof);
    }

    //resonance
    if (ImGui::SliderFloat("FilterResonance", &FilterResonance, 0, 1)) {
        audio.oscillateur.setResonance(FilterResonance);
    }

    ImGui::SliderFloat("DelayTime", &DelayTime, 0.1, 2);

    // Espacement
    ImGui::Spacing();
    ImGui::Spacing();

    // Presets / notes
    ImGui::Text("Presets");
    float buttonWidth = 40.0f;
    float spacing = 10.0f;

    for (int i = 0; i < 12 ; i++) {
        // Bouton de note
        if (ImGui::Button(std::to_string(i + 1).c_str(), ImVec2(buttonWidth, 0))) {
            // Tu peux garder ça si tu veux une réaction immédiate au clic
            audio.noteOn(i);
        }

        // Début de clic => début de note
        if (ImGui::IsItemActivated()) {
            audio.noteOn(i);
        }

        // Fin du clic => relâchement de note
        if (ImGui::IsItemDeactivated()) {
            audio.noteOff();
        }

        if (i < 11) ImGui::SameLine(0, spacing);
    }


    ImGui::End();
}
