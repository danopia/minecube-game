#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>
#include "octree.h"

class Renderer {
    public:
        Renderer(Octree<bool> terrain);
        void render(float Left, float Top, float Up);
        
        Octree<bool> terrain;
};

#endif
