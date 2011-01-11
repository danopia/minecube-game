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
    bool Forward;
    bool Backward;
    bool Left;
    bool Right;
    bool Moving;
    bool Walking;
    bool Crouching;
    bool Crawling;
    bool Running;
    std::string Name;
    Octree<Block*> *StandingOn;
    float SurfaceZ;
    float GravitySpeed;
    
    Player();
    Player(float initspeed, Vector3 initrot, Vector3 initpos, std::string Name);
    
    // Moves the player around
    /*void Forward(float amount);
    void Strafe(float amount);*/
    void ChangeRotation(float deltaYrot, float deltaZrot);
    bool Jump();          // Begins a jump
    
    // Speed/height adjustment
    void Walk();          // Stands up, walks at default speed
    
    void Run();           // Start running
    bool toggleRun();     // Begin or end running
    
    void Crouch();        // Begin crouching
    bool toggleCrouch();  // Begin or end crouching
    
    void Crawl();         // Begin crawling
    bool toggleCrawl();   // Begin or end crawling
    
    void DoStep(float amount); // Gets called every frame

    void Render(); //Render!
};

#endif
