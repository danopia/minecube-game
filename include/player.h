#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "octree.h"
#include "block.h"

class Player
{
public:
    float Speed;
    float Yrot, Zrot; // Pitch and yaw, respectively
    float X, Y, Z;
    bool Moving;
    std::string Name;
    Octree<Block*> *StandingOn;
    float SurfaceZ;
    float GravitySpeed;
    
    Player();
    Player(float initspeed, float inityrot, float initzrot, float initX, float initY, float initZ, std::string Name);
    
    // Moves the player around
    void Forward(float amount);
    void Strafe(float amount);
    void ChangeRotation(float deltaYrot, float deltaZrot);
    void Jump(); // Begins a jump
    
    void DoStep(float amount); // Gets called every frame
};

#endif
