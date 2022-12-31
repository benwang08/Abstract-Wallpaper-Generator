#include <iostream>
#include <vector>
#include <iostream>
#include "utility.h"
#include "material.h"
#include "ray.h"
#include "entity_list.h"
#include "sphere.h"
#include "camera.h"
#include <fstream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;


//thread variables
void calculate_pixel(const int num_samples, camera* cam, entity_list* world, int image_width,
                    int image_height, const int max_ray_depth);
bool DONE = false;
mutex mx;
condition_variable cv;
condition_variable wait_for_jobs;
queue<vector<int>> jobs;
vector<vector<pixel>> IMAGE_STORE;
int done = 0;



entity_list random_scene() {
    entity_list world;

    auto ground_material = make_shared<diffuse>(pixel(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_triple() * random_triple();
                    sphere_material = make_shared<diffuse>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_triple(.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<glass>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<glass>(1.5);
    world.add(make_shared<sphere>(point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<diffuse>(pixel(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(pixel(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point(4, 1, 0), 1.0, material3));

    return world;
}

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

    //read in data
    fstream fs("data.txt");

    int image_width;
    int image_height;

    fs >> image_width >> image_height;

    vector<pixel> color_palette;
    while (!fs.eof()){
        int r, g, b;
        double prob;
        fs >> r >> g >> b >> prob;
        int entries = int(prob * 100.0);
        entries++;
        
        pixel temp(r, g, b);
        for (int i = 0; i < entries; i++){
            color_palette.push_back(temp);
        }
    }

    fs.close();

    //Image description
    const double aspect_ratio = double(image_width)/image_height;
    const int num_samples = 600;
    const int max_ray_depth = 50;

    //World (objects in the landscape)
    auto world = random_scene();

    //Camera and viewport
    point lookfrom(13,2,3);
    point lookat(0,0,0);
    triple vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    //store calculations from threads in a 2d vector
    vector<pixel> vec_width(image_width, pixel(0,0,0));
    vec_width.reserve(image_width);

    IMAGE_STORE.resize(image_height, vec_width);
    cerr << IMAGE_STORE.size();
    cerr << " " << IMAGE_STORE[0].size();

    //start 12 worker threads
    for(int i = 0; i < 12; i++){
        thread temp = thread(calculate_pixel, num_samples, &cam, &world, 
                            image_width, image_height, max_ray_depth);
        temp.detach();
    }

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; i++) {
            jobs.push({j, i});
            cv.notify_one();
        }
    } 
    DONE = true;

    unique_lock<mutex> guard(mx);

    while(done < 12){
        wait_for_jobs.wait(guard);
    }


    ofstream of("wallpaper.ppm");
    of << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; i++) {
            print_ppm(of, IMAGE_STORE[j][i], num_samples);
        }
    } 

    of.close();

}

void calculate_pixel(const int num_samples, camera* cam, entity_list* world, int image_width,
                    int image_height, const int max_ray_depth){

    int i = -1;
    int j = -1;

    while(true){
        unique_lock<mutex> guard(mx);

        while(jobs.empty()){
            if (DONE){
                done++;
                wait_for_jobs.notify_one();
                return;
            }
            cv.wait(guard);
        }

        vector<int> i_j = jobs.front();
        i = i_j[1];
        j = i_j[0];

        cerr << "CALCULATING PIXEL " << j << " " << i << endl;

        jobs.pop();
        guard.unlock();

        //run job and put calculated pixel into 2d vector
        pixel pixel_color(0, 0, 0);
        for (int s = 0; s < num_samples; ++s) {
            auto u = (i + random_double()) / (image_width-1);
            auto v = (j + random_double()) / (image_height-1);
            ray r = cam->get_ray(u, v);
            pixel_color += ray_color(r, *world, max_ray_depth).get_vec();
        }

        try{
            IMAGE_STORE[j][i] = pixel_color;
        }
        catch(int ja){
            cerr << j << ' ' << i;
        }

    }

}