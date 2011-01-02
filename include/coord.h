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
            return sqrt(pow(X,2) + pow(Y,2) + pow(Z,2)) < sqrt(pow(x.X,2) + pow(x.Y, 2) + pow(x.Z, 2));
        }
};

#endif
