#include "player.h"
#include <cmath>

const double PI = 3.14159265358979323846264338329750288419716939937510582;

Player::Player() : Speed(0), Yrot(0), Zrot(0), X(0), Y(0), Z(0), Moving(false), Name("Untitled"), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f) {}
Player::Player(float initspeed, float initYrot, float initZrot, float initX, float initY, float initZ, std::string Name) :
    Speed(initspeed), Yrot(initYrot), Zrot(initZrot), X(initX), Y(initY), Z(initZ), Moving(false), Name(Name), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f) {}

void Player::Forward(float amount)
{
    Moving = true;
    float xStep = Speed * sin((PI * Zrot) / 180) * amount;
    float yStep = Speed * cos((PI * Zrot) / 180) * amount;
    //float zStep = -Speed * sin((PI * Yrot) / 180) * amount;
    X += (xStep);// * cos((PI * Yrot) / 180));
    Y += (yStep);// * cos((PI * Yrot) / 180));
    //Z += zStep;
}

void Player::Strafe(float amount)
{
    float xStep = Speed * sin((PI * Zrot) / 180) * amount;
    float yStep = Speed * cos((PI * Zrot) / 180) * amount;
    if(Moving == true)
    {
        xStep *= 0.707106;        
        yStep *= 0.707106;
    }

    X -= yStep;
    Y += xStep;
}

void Player::ChangeRotation(float deltaYrot, float deltaZrot)
{
    Zrot += deltaZrot;
    Yrot += deltaYrot;

    if(Zrot >= 360)
        Zrot -= 360;
    else if (Zrot < 0)
        Zrot += 360;

    if(Yrot < -90)
        Yrot = -90;
    else if(Yrot >= 90)
        Yrot = 90;
}

void Player::Jump()
{
    if (StandingOn) {
        GravitySpeed = 9.8f;
    }
}

void Player::DoStep(float amount)
{
    Moving = false;
    
    if (!StandingOn)
    {
        GravitySpeed -= 9.8f * amount;
        if (Z < 0) GravitySpeed = -GravitySpeed;
        Z += GravitySpeed * amount;
    }
    else
    {
        GravitySpeed = 0.f;
        Z = SurfaceZ + 2.f;
    }
}

