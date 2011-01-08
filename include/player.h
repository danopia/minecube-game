#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "octree.h"
#include "block.h"
#include "entity.h"
#include "vector3.h"

class Player : public Entity
{
public:
    float Speed;
    bool Moving;
    std::string Name;
    Octree<Block*> *StandingOn;
    float SurfaceZ;
    float GravitySpeed;
    
    Player();
    Player(float initspeed, Vector3 initrot, Vector3 initpos, std::string Name);
    
    // Moves the player around
    void Forward(float amount);
    void Strafe(float amount);
    void ChangeRotation(float deltaYrot, float deltaZrot);
    void Jump(); // Begins a jump
    
    void DoStep(float amount); // Gets called every frame

    void Render(); //Render!
};

#endif
