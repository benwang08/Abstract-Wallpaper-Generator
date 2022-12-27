#ifndef ray_h
#define ray_h

#include <cmath>
#include <vector>
#include <iostream>
#include "triple.h"

using namespace std;

//representation of a ray
//ray includes a triple for its point of origin and for its direction
class ray{
    public:
        ray(){
            
        }
        ray(point &p, triple d) :
            origin(p.get_vec()), direction(d.get_vec())
        {}

        triple get_origin(){
            return origin;
        }

        triple get_direction() const{
            return direction;
        }

        //returns a point along the ray
        //t is a value that is multiplied on the direction vector
        point at(double t){
            auto line = direction * t;
            return origin + line;
        }

    private:
        point origin;
        triple direction;

};

#endif