#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#define PI 3.14159265358979323846264338329750288419716939937510582

class Player
{
public:
    float Speed;
    float Yrot, Zrot; //Pitch and yaw, respectively
    float X, Y, Z;
    bool Moving;

    Player();
    Player(float initspeed, float inityrot, float initzrot, float initX, float initY, float initZ);
    void Forward(float amount);
    void Strafe(float amount);
    void ChangeRotation(float deltaYrot, float deltaZrot);
};

#endif
