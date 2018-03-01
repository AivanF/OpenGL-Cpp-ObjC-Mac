//
//  WorldDraw.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#include "World.hpp"
#include "helper.hpp"


void World::draw(float w, float h) {
    if (moving) {
        par = (par + 1) % 360;
        for (int i = 0; i < _items.size(); i++) {
            _items[i]->processStep();
        }
    }
    
    prepareFX();
    renderShadows();
    glViewport(0, 0, w, h);
    draw3D(w, h);
    draw2D(w, h);
    // Draw shadow map
    {
        float ww = w / 2;
        float hh = h / 2;
        _shpc2p->use();
        c2p.draw(_points[light_target]->shadowMap._shadowMapTex, _shpc2p,
                 w, h, // screen size
                 0, hh, // shift
                 ww, hh); // scale
    }
    
    glFlush();
    GetError();
}
