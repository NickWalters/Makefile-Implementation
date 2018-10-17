#include "haversine.h"

double degrees_to_radians(double degrees)
{
    return (degrees * M_PI / 180.0);
}

double radians_to_degrees(double radians)
{
    return (radians * 180.0 / M_PI);
}
