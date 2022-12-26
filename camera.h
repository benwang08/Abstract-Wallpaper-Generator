#ifndef camera_h
#define camera_h

#include "ray.h"
#include "triple.h"

class camera {
    public:
        //Need to add custom constructor for various aspect ratios
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = point(0, 0, 0);
            horizontal = triple(viewport_width, 0.0, 0.0);
            vertical = triple(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - triple(0, 0, focal_length);
        }

        ray get_ray(double u, double v) {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point origin;
        point lower_left_corner;
        triple horizontal;
        triple vertical;
};
#endif