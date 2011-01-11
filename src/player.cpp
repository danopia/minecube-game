#include "player.h"
#include <cmath>

const double PI = 3.14159265358979323846264338329750288419716939937510582;
const double CROUCH_OFFSET = 0.30;
const double CRAWL_OFFSET  = 0.70;

const double RUN_SPEED     = 7.0;
const double WALK_SPEED    = 5.0;
const double CROUCH_SPEED  = 3.0;
const double CRAWL_SPEED   = 2.0;

Player::Player() : Speed(0), Forward(false), Backward(false), Left(false), Right(false), Moving(false), Running(false), Walking(false), Crouching(false), Crawling(false), Name("Untitled"), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f), Entity(Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0)) {};

Player::Player(float initspeed, Vector3 initrot, Vector3 initpos, std::string Name) : Speed(initspeed), Forward(false), Backward(false), Left(false), Right(false), Moving(false), Running(false), Walking(false), Crouching(false), Crawling(false), Name(Name), StandingOn(NULL), GravitySpeed(0.f), SurfaceZ(0.f), Entity(initpos, initrot, Vector3(1, 1, 2)) {};

/*void Player::Forward(float amount)
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
}*/

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

bool Player::Jump()
{
    if (StandingOn) {
        GravitySpeed = 5.f;
        StandingOn = NULL;
        return true;
    }
    return false;
}

/*void Player::Walk()
{
    Speed = WALK_SPEED;
    Walking   = true;
    Crouching = false;
    Crawling  = false;
    Running   = false;
}

void Player::Run()
{
    Speed = RUN_SPEED;
    Walking   = false;
    Crouching = false;
    Crawling  = false;
    Running   = true;
}
*/
bool Player::toggleRun()
{
    Running = !Running;
    return Running;
}
/*
void Player::Crouch()
{
    Speed = CROUCH_SPEED;
    Walking   = false;
    Crouching = true;
    Crawling  = false;
    Running   = false;
}
*/
bool Player::toggleCrouch()
{
    Crouching = !Crouching;
    return Crouching;
}
/*
void Player::Crawl()
{
    Speed = CRAWL_SPEED;
    Walking   = false;
    Crouching = false;
    Crawling  = true;
    Running    = false;
}
*/
bool Player::toggleCrawl()
{
    Crawling = !Crawling;
    return Crawling;
}

void Player::DoStep(float amount)
{
    float xStepOffset, yStepOffset,
          SinStep, CosStep,
          SinStep_Pos, CosStep_Pos,
          SinStep_Neg, CosStep_Neg;
    
    //GravitySpeed = 0.f;
    Pos.Z = SurfaceZ;
    Hitbox.Z = 2;
    Speed = 0;
    
    if (Forward && Backward) {
        // Stay still if both forward + backward
    } else if (Running) {
        Speed = RUN_SPEED;
    } else if (Crouching) {
        Speed = CROUCH_SPEED;
        Pos.Z -= CROUCH_OFFSET;
        Hitbox.Z -= CROUCH_OFFSET;
    } else if (Crawling) {
        Speed = CRAWL_SPEED;
        Pos.Z -= CRAWL_OFFSET;
        Hitbox.Z -= CRAWL_OFFSET;
    } else if (Forward || Backward || Left || Right)  {
        Speed = WALK_SPEED;
    }

    SinStep = Speed * sin((PI * Rotation.Z) / 180) * amount;
    CosStep = Speed * cos((PI * Rotation.Z) / 180) * amount;
    
    if (Forward) {
        Pos.X += SinStep;
        Pos.Y += CosStep;
    } else if (Backward) {
        Pos.X -= SinStep;
        Pos.Y -= CosStep;
    }
    if ((Left || Right) && (Forward || Backward)) {
        xStepOffset = 0.707106;
        yStepOffset = 0.707106;
    }
    if (Left) {
        Pos.X -= CosStep * xStepOffset;
        Pos.Y += SinStep * yStepOffset;
    } else if (Right) {
        Pos.X -= -CosStep * xStepOffset;
        Pos.Y += -SinStep * yStepOffset;
    }

    
    if (!StandingOn) {
        GravitySpeed -= 9.8f * amount;
        if (Pos.Z < 0) GravitySpeed = -GravitySpeed;
        Pos.Z += GravitySpeed * amount;
    }
}

void Player::Render()
{
    //TODO: Write me
}

