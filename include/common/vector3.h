#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3
{
    public:
        Vector3() : X(0), Y(), Z(0) {}
        Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

        float X,Y,Z;

        bool operator <(const Vector3& x) const
        {
            return sqrtf(powf(X,2) + powf(Y,2) + powf(Z,2)) < sqrtf(powf(x.X,2) + powf(x.Y, 2) + powf(x.Z, 2));
        }
};

#endif
