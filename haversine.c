#include "haversine.h"

int main(int argc, char *argv[])
{
    int result;

//  CHECK THE NUMBER OF ARGUMENTS
    if(argc != 5)
    {
        printf("Usage: %s lat1 lon1 lat2 lon2\n", argv[0]);
        result  = EXIT_FAILURE;
    }
    else
    {
//  CONVERT EACH COMMAND-LINE ARGUMENT TO A FLOATING-POINT NUMBER
        double lat1 = atof(argv[1]);
        double lon1 = atof(argv[2]);

        double lat2 = atof(argv[3]);
        double lon2 = atof(argv[4]);

//  ENSURE THAT THE 2 PROVIDED LOCATIONS ARE EACH VALID
        if(valid_location(lat1, lon1) == false)
        {
            printf("%s: location 1 is invalid!\n", argv[0]);
            result = EXIT_FAILURE;
        }
        else if(valid_location(lat2, lon2) == false)
        {
            printf("%s: location 2 is invalid!\n", argv[0]);
            result = EXIT_FAILURE;
        }
//  ALL GOOD - CALCULATE AND PRINT DISTANCE BETWEEN LOCATIONS
        else
        {
            printf("distance = %lfm\n", haversine(lat1, lon1, lat2, lon2));
            result = EXIT_SUCCESS;
        }
    }
    return result;
}
