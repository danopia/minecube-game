#include "common/player.h"
#include <cmath>

const double PI = 3.14159265358979323846264338329750288419716939937510582;
const double CROUCH_OFFSET = 0.30;
const double CRAWL_OFFSET  = 0.70;

const double RUN_SPEED     = 7.0;
const double WALK_SPEED    = 5.0;
const double CROUCH_SPEED  = 3.0;
const double CRAWL_SPEED   = 2.0;

Player::Player() : Speed(0), WasRunning(false), WasCrouching(false), WasCrawling(false), Forward(false), Backward(false), Left(false), Right(false), Jumping(false), Running(false), Walking(false), Crouching(false), Crawling(false), Dirty(true), Name("Untitled"), Entity(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 1.7f)) {};

Player::Player(float initspeed, Vector3 initrot, Vector3 initpos, std::string Name) : Speed(initspeed), WasRunning(false), WasCrouching(false), WasCrawling(false), Forward(false), Backward(false), Left(false), Right(false), Jumping(false), Running(false), Walking(false), Crouching(false), Crawling(false), Dirty(true), Name(Name), Entity(initpos, initrot, Vector3(0.5f, 0.5f, 1.7f)) {};

void Player::ChangeRotation(float deltaYRotation, float deltaZRotation)
{
    Rotation.Y += deltaYRotation;
    Rotation.Z += deltaZRotation;
    
    Dirty = true;

    if (Rotation.Z >= 360)
        Rotation.Z -= 360;
    else if (Rotation.Z < 0)
        Rotation.Z += 360;

    if (Rotation.Y < -90)
        Rotation.Y = -90;
    else if (Rotation.Y >= 90)
        Rotation.Y = 90;
}

bool Player::Jump()
{
    if (LocatedOn) {
        GravitySpeed = 10.f;
        LocatedOn = NULL;
        Dirty = true;
        return true;
    }
    
    return false;
}

bool Player::toggleRun()
{
    Running = !Running;
    Dirty = true;
    return Running;
}

bool Player::toggleCrouch()
{
    Crouching = !Crouching;
    Dirty = true;
    return Crouching;
}

bool Player::toggleCrawl()
{
    Crawling = !Crawling;
    Dirty = true;
    return Crawling;
}

void Player::DoStep(float amount)
{
    float xStepOffset, yStepOffset,
          SinStep, CosStep,
          SinStep_Pos, CosStep_Pos,
          SinStep_Neg, CosStep_Neg;
    
    Speed = 0;
    LastPos = Pos;
    
    if (Crouching && !WasCrouching) {
        Pos.Z    -= CROUCH_OFFSET;
        Hitbox.Z -= CROUCH_OFFSET;
        WasCrouching = true;
    } else if (!Crouching && WasCrouching) {
        Pos.Z    += CROUCH_OFFSET;
        Hitbox.Z += CROUCH_OFFSET;
        WasCrouching = false;
    }
    
    if (Crawling && !WasCrawling) {
        Pos.Z    -= CRAWL_OFFSET;
        Hitbox.Z -= CRAWL_OFFSET;
        WasCrawling = true;
    } else if (!Crawling && WasCrawling) {
        Pos.Z    += CRAWL_OFFSET;
        Hitbox.Z += CRAWL_OFFSET;
        WasCrawling = false;
    }
    
    if (Running || (Forward && Left && Right))
        Speed = RUN_SPEED;
    else if (Crouching)
        Speed = CROUCH_SPEED;
    else if (Crawling)
        Speed = CRAWL_SPEED;
    else if (Forward || Backward || Left || Right) 
        Speed = WALK_SPEED;

    SinStep = Speed * sin((PI * Rotation.Z) / 180) * amount;
    CosStep = Speed * cos((PI * Rotation.Z) / 180) * amount;
    
    if (Forward && Backward) {
        // Ignore if both forward + backward
    } else if (Forward) {
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
    if (Left && Right) {
        // Ignore if both left + right
    } else if (Left) {
        Pos.X -= CosStep * xStepOffset;
        Pos.Y += SinStep * yStepOffset;
    } else if (Right) {
        Pos.X += CosStep * xStepOffset;
        Pos.Y -= SinStep * yStepOffset;
    }
    
    if (Jumping)
        Jump();
    
    /*
    if (StandingOn && Pos.Z - 0.5f > StandingOn->pos.Z + StandingOn->sideLength) {
        Pos.Z = StandingOn->pos.Z + StandingOn->sideLength;
    } else { // TODO: if still standingon, then should go back to prev spot
    */
    
    Entity::DoStep(amount);
    
    if (Forward || Backward || Left || Right) Dirty = true;
}

