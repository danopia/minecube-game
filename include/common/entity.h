#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "common/vector3.h"
#include "common/positionedblock.h"

class Entity
{
public:
    Vector3 LastPos, Pos;
    Vector3 Rotation;
    Vector3 Hitbox;
    
    PositionedBlock *LocatedOn;
    float GravitySpeed;

    Entity(Vector3 initpos, Vector3 initrotation, Vector3 inithitbox) : LastPos(initpos), Pos(initpos), Rotation(initrotation), Hitbox(inithitbox), GravitySpeed(0.f) {};
        
    void DoStep(float amount);
    void Collide(PositionedBlock& block);
};
#endif

