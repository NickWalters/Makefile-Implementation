#include "haversine.h"

double haversine(double lat1, double lon1, double lat2, double lon2)
{
    double deltalat = (lat2 - lat1) / 2.0;
    double deltalon = (lon2 - lon1) / 2.0;

    double sin1     = sin( degrees_to_radians(deltalat) );

    double cos1     = cos( degrees_to_radians(lat1) );
    double cos2     = cos( degrees_to_radians(lat2) );

    double sin2     = sin( degrees_to_radians(deltalon) );

    double x        = sin1*sin1 + cos1*cos2 * sin2*sin2;

    return (2.0 * EARTH_RADIUS_IN_METRES * asin( sqrt(x) ) );	// in metres
}


//ddddddddddd
