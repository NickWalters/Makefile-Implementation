#include "haversine.h"

bool valid_location(double latitude, double longitude)
{
/*  IN-CLASS VERSION:
    if(latitude < MIN_LAT || latitude > MAX_LAT)
    {
        return false;
    }
    if(longitude < MIN_LON || longitude > MAX_LON)
    {
        return false;
    }
    return true;
 */

//  A BETTER VERSION:
    return (latitude  >= MIN_LAT && latitude  <= MAX_LAT   &&
            longitude >= MIN_LON && longitude <= MAX_LON);
}
