#include "Boundary.h"

float Boundary::getTop()
{
    return fmax(a.y, b.y);
}

float Boundary::getBottom()
{
    return fmin(a.y, b.y);
}
