#include "MineApp.h"

MineApp::MineApp() {
    Surf_Display = NULL;
    Running = true;
}

int MineApp::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}

bool MineApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }

    return true;
}

void MineApp::OnEvent(SDL_Event* Event) {
    if(Event->type == SDL_QUIT) {
        Running = false;
    }
}

void MineApp::OnLoop() {
}

void MineApp::OnRender() {
}

void MineApp::OnCleanup() {
    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
}
