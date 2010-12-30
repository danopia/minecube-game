#ifndef _MINEAPP_H_
    #define _MINEAPP_H_
#include <SDL.h>

class MineApp {

    private:
        bool         Running;
        SDL_Surface* Surf_Display;

    public:
        MineApp();
        int OnExecute();

    public:
        bool OnInit();
        void OnEvent(SDL_Event* Event);
        void OnLoop();
        void OnRender();
        void OnCleanup();
};

#endif
