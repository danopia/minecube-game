#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3
{
    public:
        Vector3() : X(0), Y(0), Z(0) {}
        Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

        float X, Y, Z;


        float distance(const Vector3& x) const
        {
            return sqrtf(powf(X - x.X,2) + powf(Y - x.Y,2) + powf(Z - x.Z,2));
        }


        bool operator <(const Vector3& x) const
        {
            return sqrtf(powf(X,2) + powf(Y,2) + powf(Z,2)) < sqrtf(powf(x.X,2) + powf(x.Y, 2) + powf(x.Z, 2));
        }


        Vector3 operator +(const Vector3& x) const
        {
            return Vector3(X + x.X, Y + x.Y, Z + x.Z);
        }

        Vector3 operator +(const float x) const
        {
            return Vector3(X + x, Y + x, Z + x);
        }


        Vector3 operator -(const Vector3& x) const
        {
            return Vector3(X - x.X, Y - x.Y, Z - x.Z);
        }

        Vector3 operator -(const float x) const
        {
            return Vector3(X - x, Y - x, Z - x);
        }


        Vector3 operator *(const Vector3& x) const
        {
            return Vector3(X * x.X, Y * x.Y, Z * x.Z);
        }

        Vector3 operator *(const float x) const
        {
            return Vector3(X * x, Y * x, Z * x);
        }


        Vector3 operator /(const Vector3& x) const
        {
            return Vector3(X / x.X, Y / x.Y, Z / x.Z);
        }

        Vector3 operator /(const float x) const
        {
            return Vector3(X / x, Y / x, Z / x);
        }
};

#endif
