#include <iostream>
#include <vector>
#include <iostream>
#include "utility.h"
#include "material.h"
#include "ray.h"
#include "entity_list.h"
#include "sphere.h"
#include "camera.h"

using namespace std;


pixel ray_color(const ray& r, entity_list& world, int depth) {
    if (depth < 0){
        return pixel(0,0,0);
    }

    //checking if ray hits any part of world
    hit_record h_record;
    if (world.hit(r, 0.001, INF, h_record)){
        ray scattered;
        pixel attenuation;

        if (h_record.mat_ptr->scatter(r, h_record, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);

        return pixel(0,0,0);

    }
    triple unit_direction = unit_vector(r.get_direction());
    auto t = 0.5 * (unit_direction[1] + 1.0);
    return (1.0-t)*pixel(1.0, 1.0, 1.0) + t*pixel(0.5, 0.7, 1.0);}

int main() {

    //Image description
    const double aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = 225;
    const int num_samples = 100;
    const int max_ray_depth = 50;

    //World (objects in the landscape)
    entity_list world;
    auto material_ground = make_shared<diffuse>(pixel(0.8, 0.8, 0.0));
    auto material_center = make_shared<diffuse>(pixel(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(pixel(0.8, 0.8, 0.8));
    auto material_right  = make_shared<metal>(pixel(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(triple( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(triple( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(triple(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(triple( 1.0,    0.0, -1.0),   0.5, material_right));

    //Camera and viewport
    camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; i++) {
            cerr << "PRINTING PIXEL " << j << " " << i << endl;
            pixel pixel_color(0, 0, 0);
            for (int s = 0; s < num_samples; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_ray_depth).get_vec();
            }
            print_ppm(std::cout, pixel_color, num_samples);
        }
    }
}