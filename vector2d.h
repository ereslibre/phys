/**
 * Bajo la licencia GPL-3 ó superior.
 *
 * Copyright (C) 2008 Rafael Fernández López
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

class Vector2D
{
public:
    Vector2D()
        : x(0)
        , y(0)
    {
    }

    Vector2D(float x, float y)
        : x(x)
        , y(y)
    {
    }

    float module() const
    {
        return sqrt(x * x + y * y);
    }

    Vector2D normalized() const
    {
        float m = module();
        return Vector2D(x / m, y / m);
    }

    float x;
    float y;
};

#endif
