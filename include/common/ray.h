#pragma once
#ifndef RAY_H
#define RAY_H

#include <cmath>
#include <list>

#include "common/positionedblock.h"
#include "common/entity.h"
#include "common/vector3.h"

class Ray
{
    public:
        Ray() : Origin(0, 0, 0), Direction(0, 0, 0) {};
        Ray(Entity *entity);
        Ray(Vector3 origin, Vector3 direction) : Origin(origin), Direction(direction) {};

        float CheckCollision(PositionedBlock *block);

        bool FindClosest(std::list<PositionedBlock*> blocks);
        PositionedBlock* Closest;
        float Distance;
        char Face;

        Vector3 Origin, Direction;

        const double static PI;
};

#endif
