#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>


class Player
{
public:
    float Speed;
    float Yrot, Zrot; //Pitch and yaw, respectively
    float X, Y, Z;
    bool Moving;
    std::string Name;
    Player();
    Player(float initspeed, float inityrot, float initzrot, float initX, float initY, float initZ, std::string Name);
    void Forward(float amount);
    void Strafe(float amount);
    void ChangeRotation(float deltaYrot, float deltaZrot);
};

#endif
