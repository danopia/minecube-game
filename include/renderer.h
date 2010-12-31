#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>

class Renderer {
    public:
        Renderer();
        void render(float Left, float Top, float Up);
};

#endif
