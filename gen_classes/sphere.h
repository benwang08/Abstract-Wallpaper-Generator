#ifndef sphere_h
#define sphere_h

#include "entity.h"
#include "triple.h"
#include <memory>

//sphere inherits from entity
class sphere : public entity {
    public:
        sphere() {}
        sphere(point cen, double r, shared_ptr<material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        bool hit(ray r, double t_min, double t_max, hit_record &rec) override;

    public:
        point center;
        double radius;
        shared_ptr<material> mat_ptr;
};


#endif