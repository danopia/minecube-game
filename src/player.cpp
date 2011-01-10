#include "player.h"
#include <cmath>

const double PI = 3.14159265358979323846264338329750288419716939937510582;
const double CROUCH_OFFSET = 0.30;
const double CRAWL_OFFSET  = 0.70;

Player::Player() : Speed(0), Moving(false), Crouching(false), Crawling(false), Name("Untitled"), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f), Entity(Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0)) {};

Player::Player(float initspeed, Vector3 initrot, Vector3 initpos, std::string Name) : Speed(initspeed), Moving(false), Crouching(false), Crawling(false), Name(Name), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f), Entity(initpos, initrot, Vector3(1, 1, 2)) {};

void Player::Forward(float amount)
{
    Moving = true;
    float xStep = Speed * sin((PI * Rotation.Z) / 180) * amount;
    float yStep = Speed * cos((PI * Rotation.Z) / 180) * amount;
    //float zStep = -Speed * sin((PI * Rotation.Y) / 180) * amount;
    Pos.X += (xStep);// * cos((PI * Rotation.Y) / 180));
    Pos.Y += (yStep);// * cos((PI * Rotation.Y) / 180));
    //Z += zStep;
}

void Player::Strafe(float amount)
{
    float yStep = Speed * sin((PI * Rotation.Z) / 180) * amount;
    float xStep = Speed * cos((PI * Rotation.Z) / 180) * amount;
    if(Moving == true)
    {
        xStep *= 0.707106;        
        yStep *= 0.707106;
    }

    Pos.X -= xStep;
    Pos.Y += yStep;
}

void Player::ChangeRotation(float deltaYRotation, float deltaZRotation)
{
    Rotation.Y += deltaYRotation;
    Rotation.Z += deltaZRotation;

    if(Rotation.Z >= 360)
        Rotation.Z -= 360;
    else if (Rotation.Z < 0)
        Rotation.Z += 360;

    if(Rotation.Y < -90)
        Rotation.Y = -90;
    else if(Rotation.Y >= 90)
        Rotation.Y = 90;
}

void Player::Jump()
{
    if (StandingOn) {
        GravitySpeed = 5.f;
        StandingOn = NULL;
        Crouching = false;
    }
}

void Player::Stand()
{
    Crouching = false;
    Crawling = false;
}

void Player::Crouch()
{
    Crouching = true;
}

bool Player::toggleCrouch()
{
    Crouching = !Crouching;
    return Crouching;
}

void Player::Crawl()
{
    Crawling = true;
}

bool Player::toggleCrawl()
{
    Crawling = !Crawling;
    return Crawling;
}

void Player::DoStep(float amount)
{
    Moving = false;
    
    if (!StandingOn)
    {
        GravitySpeed -= 9.8f * amount;
        if (Pos.Z < 0) GravitySpeed = -GravitySpeed;
        Pos.Z += GravitySpeed * amount;
    }
    else
    {
        GravitySpeed = 0.f;
        Pos.Z = SurfaceZ;
        Hitbox.Z = 2;
        if (Crawling) {
            Pos.Z -= CRAWL_OFFSET;
            Hitbox.Z -= CRAWL_OFFSET;
        } else if (Crouching) {
            Pos.Z -= CROUCH_OFFSET;
            Hitbox.Z -= CROUCH_OFFSET;
        }
    }
}

void Player::Render()
{
    //TODO: Write me
}

