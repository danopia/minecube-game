#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "vector3.h"

class Entity
{
public:
    Vector3 Pos; 
    Vector3 Rotation;
    Vector3 Hitbox;
    virtual void Render();
    Entity(Vector3 initpos, Vector3 initrotation, Vector3 inithitbox) : Pos(initpos), Rotation(initrotation), Hitbox(inithitbox) {};
};
#endif


