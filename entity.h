#ifndef entity_h
#define entity_h

#include "ray.h"

struct hit_record {
    point p;
    triple normal;
    double t;
};

//abstract class for anything a ray might hit
class entity {
    public:
        virtual bool hit(ray r, double t_min, double t_max, hit_record &rec){
            return false;
        }
};

#endif