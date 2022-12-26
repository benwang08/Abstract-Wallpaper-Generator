#include "ray.h"
#include "triple.h"
#include "entity_list.h"
#include "sphere.h"
#include "camera.h"

//file contains utility functions and includes for easier access

//Declaring useful constants/functions

const double PI = atan(1) * 4;
const double INF = numeric_limits<double>::infinity();

inline double deg_to_rad(double degrees){
    return degrees * PI / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

//This function rounds to int(0-255) for output to .ppm file
//includes samples for antialising averaging
inline void print_ppm(std::ostream &out, pixel &p, int samples){
    pixel temp = p;
    temp /= samples;

    //sqrt RGB values for gamma correction (gamma 2)
    temp.mod_value(0) = sqrt(temp[0]);
    temp.mod_value(1) = sqrt(temp[0]);
    temp.mod_value(2) = sqrt(temp[0]);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(temp[0], 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(temp[1], 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(temp[2], 0, 0.999)) << '\n';
}
