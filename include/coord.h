#pragma once
#ifndef COORD_H
#define COORD_H
#include <cmath>

class Coord
{
    public:
        Coord() : X(0), Y(), Z(0) {}
        Coord(int x, int y, int z) : X(x), Y(y), Z(z) {}

        int X,Y,Z;

        bool operator <(const Coord& x) const
        {
            return sqrt(powf(X,2) + powf(Y,2) + powf(Z,2)) < sqrt(powf(x.X,2) + powf(x.Y, 2) + powf(x.Z, 2));
        }
};

#endif
