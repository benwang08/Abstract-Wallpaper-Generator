#ifndef sphere_h
#define sphere_h

#include "entity.h"
#include "triple.h"

//sphere inherits from entity
class sphere : public entity {
    public:
        sphere() {}
        sphere(point cen, double r) : center(cen), radius(r) {};

        bool hit(ray r, double t_min, double t_max, hit_record &rec) override;

    public:
        point center;
        double radius;
};


#endif