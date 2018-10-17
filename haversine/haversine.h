#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//  THE FOLLOWING 3 LINES ARE REQUIRED TO GET ACCESS TO  M_PI  UNDER Linux
#if     defined(__linux__)
#define __USE_XOPEN
#endif

#include <math.h>

//  written by Chris.McDonald@uwa.edu.au

#define EARTH_RADIUS_IN_METRES          6372797

#define	MIN_LAT				-90.0
#define	MAX_LAT				 90.0
#define	MIN_LON				-180.0
#define	MAX_LON				 180.0

//  DECLARATIONS OF FUNCTIONS THAT ARE DEFINED IN OTHER FILES:
extern double degrees_to_radians(double degrees);
extern double radians_to_degrees(double radians);
extern bool   valid_location(double latitude, double longitude);
extern double haversine(double lat1, double lon1, double lat2, double lon2);

