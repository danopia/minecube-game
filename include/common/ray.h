#pragma once
#ifndef RAY_H
#define RAY_H

#include <cmath>

#include "common/positionedblock.h"
#include "common/entity.h"
#include "common/vector3.h"

class Ray
{
    public:
        Ray() : Origin(0, 0, 0), Direction(0, 0, 0) {};
        Ray(Entity *entity);
        Ray(Vector3 origin, Vector3 direction) : Origin(origin), Direction(direction) {};
        
        bool CheckCollision(PositionedBlock *block);
        
        Vector3 Origin, Direction;
        
        const double static PI = 3.14159265358979323846264338329750288419716939937510582;
};

#endif

