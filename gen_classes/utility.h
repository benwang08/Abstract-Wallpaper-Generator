#ifndef utility_h //ensuring that this object is only initialized once
#define utility_h 

#include "triple.h"

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
    temp.mod_value(1) = sqrt(temp[1]);
    temp.mod_value(2) = sqrt(temp[2]);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(temp[0], 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(temp[1], 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(temp[2], 0, 0.999)) << '\n';
}

//returns random triple with vals between 0 and 1
inline static triple random_triple() {
    return triple(random_double(), random_double(), random_double());
}

//returns random triple with vals between min and max
inline static triple random_triple(double min, double max) {
    return triple(random_double(min,max), random_double(min,max), random_double(min,max));
}

//returns random point in a unit sphere (radius = 1)
inline triple random_in_unit_sphere() {
    while (true) {
        //select a random point
        auto p = random_triple(-1,1);

        //if point is not in unit circle, continue without returning
        if (pow(p.length(), 2) >= 1){
            continue;
        }

        //point is in unit circle
        return p;
    }
}

//returns random unit vector
//taking advantage of random_in_unit_sphere() function
inline triple random_unit_vec(){
    return unit_vector(random_in_unit_sphere());
}

// Return true if the vector is close to zero in all dimensions.
//necessary to prevent bugs caused by very small values
inline bool near_zero(triple t) {
    const auto s = 1e-8;
    return (fabs(t[0]) < s) && (fabs(t[1]) < s) && (fabs(t[2]) < s);
}

inline triple random_in_unit_disk() {
    while (true) {
        auto p = triple(random_double(-1,1), random_double(-1,1), 0);
        if (pow(p.length(), 2) >= 1) continue;
        return p;
    }
}


#endif