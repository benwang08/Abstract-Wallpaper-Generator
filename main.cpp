#include <iostream>
#include <vector>
#include <iostream>
#include "triple.h"
#include "ray.h"

using namespace std;

pixel ray_color(ray& r) {
    triple unit_direction = unit_vector(r.get_direction());
    auto t = 0.5 * (unit_direction[1] + 1.0);
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