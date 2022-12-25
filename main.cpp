#include <iostream>
#include <vector>
#include <iostream>
#include "triple.h"
#include "ray.h"
#include "sphere.h"
#include "entity_list.h"

using namespace std;


//sees if a given ray hits(intersects) a 3d sphere
//this is a quadratic equation
double hit_sphere(const point& center, double radius, ray r) {
    triple oc = r.get_origin() - center;
    auto a = dot(r.get_direction(), r.get_direction());
    auto b = 2.0 * dot(oc, r.get_direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    //if discriminant >= 0, there exists a solution
    if (discriminant < 0) {
        return -1.0;
    } else {
        //There exists two answers to this equation
        //Return the smallest one, which is the point the camera "sees"
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}
pixel ray_color(ray& r) {
    //checking if ray hits sphere
    auto sphere_center = point(0,0,-1);
    auto t = hit_sphere(sphere_center, 0.6, r);
    if (t > 0){
        //normal_ray is a normal(perpendicular) ray at the point the ray hits the sphere
        triple normal_ray = unit_vector(r.at(t) - sphere_center);
        return 0.5*pixel(normal_ray[0]+1, normal_ray[1]+1, normal_ray[2]+1);
    }

    triple unit_direction = unit_vector(r.get_direction());
    t = 0.5 * (unit_direction[1] + 1.0);
    return (1.0 - t) * triple(0.1, 0, 0.6) + t * triple(0.9, 1, 0.1);
}

int main() {

    // Image
    const double aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = 225;

    //Camera and viewport
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point(0, 0, 0);
    auto horizontal = triple(viewport_width, 0, 0);
    auto vertical = triple(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - triple(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; i++) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            pixel pixel_color = ray_color(r);
            print_ppm(std::cout, pixel_color);
        }
    }
}