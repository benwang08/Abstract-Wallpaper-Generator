#ifndef camera_h
#define camera_h

#include "ray.h"
#include "triple.h"
#include "utility.h"

class camera {
    public:
        //Need to add custom constructor for various aspect ratios
        camera(
            point lookfrom,
            point lookat,
            triple vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist
            ) {
            auto theta = deg_to_rad(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) {
            triple rd = lens_radius * random_in_unit_disk();
            triple offset = u * rd[1] + v * rd[2];

            return ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }

    private:
        point origin;
        point lower_left_corner;
        triple horizontal;
        triple vertical;
        triple u, v, w;
        double lens_radius;
};
#endif