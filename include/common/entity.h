#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "common/vector3.h"

class Entity
{
public:
    Vector3 LastPos, Pos;
    Vector3 Rotation;
    Vector3 Hitbox;
    virtual void Render();
    Entity(Vector3 initpos, Vector3 initrotation, Vector3 inithitbox) : LastPos(initpos), Pos(initpos), Rotation(initrotation), Hitbox(inithitbox) {};
};
#endif


