#ifndef entity_h
#define entity_h

#include "ray.h"

struct hit_record {
    point p;
    triple normal;
    double t;

    //Records whether the ray hits an outside surface or inside surface
    bool hits_outside;

    //normal is always initialized against the ray direction
    void init_normal(triple normal_in, ray r){
        if (dot(r.get_direction(), normal_in) > 0.0){
            normal = -normal_in;
            hits_outside = false;
        }
        else{
            normal = normal_in;
            hits_outside = true;
        }
    }
};

//abstract class for anything a ray might hit
class entity {
    public:
        virtual bool hit(ray r, double t_min, double t_max, hit_record &rec){
            return false;
        }
};

#endif