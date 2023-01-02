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


//get random material, more likely to return diffuse
int get_material(){
    int seed = int(random_double(0,101));

    if (seed < 70){
        return 1;
    }
    if (seed < 85){
        return 2;
    }
    return 3;

}

pixel get_color(vector<pixel> &color_palette){
    int index = rand();
    index = index % color_palette.size();
    return color_palette[index];
}

/*entity_list random_scene(vector<pixel> &color_palette) {
    entity_list world;

    int num_ground = int(random_double(1,2.9));
    for(int i = 0; i < num_ground; i++){
        int mat = get_material();
        pixel color = get_color(color_palette);
        if (mat == 1){
            auto ground_material = make_shared<diffuse>(get_color(color_palette));
            world.add(make_shared<sphere>(point(0, random_double(-1000,-900),0), random_double(1000,1400), ground_material)); 
        }
        if (mat == 2){
            auto ground_material = make_shared<metal>(get_color(color_palette), 0.1);
            world.add(make_shared<sphere>(point(0,0, random_double(-1000,-900)), random_double(1000,1400), ground_material)); 
        }
    }
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            triple center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - triple(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                double rad = random_double(0.1 ,0.3);
                if (choose_mat < 0.8) {
                    // diffuse
                    auto sphere_mat = make_shared<diffuse>(get_color(color_palette));
                    world.add(make_shared<sphere>(center, rad, sphere_mat));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto fuzz = random_double(0, 0.7);
                    auto sphere_mat = make_shared<metal>(get_color(color_palette), fuzz);
                    world.add(make_shared<sphere>(center, rad, sphere_mat));     
                } else {
                    // glass
                    auto sphere_mat = make_shared<glass>(random_double(1,2));
                    world.add(make_shared<sphere>(center, rad, sphere_mat));    
                }
            }
        }
    }


    int num_big = int(random_double(1, 4.1));

    for(int i = 0; i < num_big; i++){
        int mat = get_material();
        pixel color = get_color(color_palette);
        int x = random_double(1,4);
        int y = random_double(1,4);
        int z = random_double(1,4);

        if (mat == 1){
            auto big_mat = make_shared<diffuse>(get_color(color_palette));
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.6, 1.3), big_mat)); 
        }
        else if (mat == 2){
            auto big_mat = make_shared<metal>(get_color(color_palette), 0.0);
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.6, 1.3), big_mat)); 
        }
        else{
            auto big_mat = make_shared<glass>(1.5);
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.6, 1.3), big_mat)); 
        }
    }


    return world;
}*/

entity_list random_scene(vector<pixel> & color_palette) {
    entity_list world;


    int mat = get_material();
    pixel light_color = get_color(color_palette) + pixel(2,2,2);
    for (int i =0 ; i < 3; i++){
        light_color.mod_value(i) = light_color[i] / 3;

    }
    if (mat == 1){
        auto ground_material = make_shared<diffuse>(light_color);
        world.add(make_shared<sphere>(point(0, -1000,0), 1000, ground_material)); 
    }
    if (mat == 2){
        auto ground_material = make_shared<metal>(light_color, 0.1);
        world.add(make_shared<sphere>(point(0,0, -1000), 1000, ground_material)); 
    }
    for (int a = -13; a < 13; a++) {
        for (int b = -13; b < 13; b++) {
            auto choose_mat = random_double();
            triple center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - triple(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                double rad = random_double(0.1 ,0.3);
                if (choose_mat < 0.8) {
                    // diffuse
                    auto sphere_mat = make_shared<diffuse>(get_color(color_palette));
                    world.add(make_shared<sphere>(center, rad, sphere_mat));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto fuzz = random_double(0, 0.7);
                    auto sphere_mat = make_shared<metal>(get_color(color_palette), fuzz);
                    world.add(make_shared<sphere>(center, rad, sphere_mat));     
                } else {
                    // glass
                    auto sphere_mat = make_shared<glass>(random_double(1,2));
                    world.add(make_shared<sphere>(center, rad, sphere_mat));    
                }
            }
        }
    }

    int num_big = int(random_double(1, 6.1));

    for(int i = 0; i < num_big; i++){
        int mat = get_material();
        pixel color = get_color(color_palette);
        int x = random_double(1,4);
        int y = random_double(1,4);
        int z = random_double(1,4);

        if (mat == 1){
            auto big_mat = make_shared<diffuse>(get_color(color_palette));
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.5, 1.3), big_mat)); 
        }
        else if (mat == 2){
            auto big_mat = make_shared<metal>(get_color(color_palette), 0.0);
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.5, 1.3), big_mat)); 
        }
        else{
            auto big_mat = make_shared<glass>(1.5);
            world.add(make_shared<sphere>(point(x, y, z), random_double(0.5, 1.3), big_mat)); 
        }
    }

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
        double r, g, b;
        double prob;
        fs >> r >> g >> b >> prob;
        int entries = int(prob * 100.0);
        entries++;
        
        pixel temp(r/255.0, g/255.0, b/255.0);
        //push_back 10 if color is black or white
        if (entries > 10){
            entries = 10;
        }
        for (int i = 0; i < entries; i++){
            color_palette.push_back(temp);
        }
    }

    fs.close();

    //Image description
    const double aspect_ratio = double(image_width)/image_height;
    const int num_samples = 500;
    const int max_ray_depth = 50;

    //World (objects in the landscape)
    auto world = random_scene(color_palette);

    //Camera and viewport
    triple lookfrom(13,2,3);
    triple lookat(0,0,0);
    triple vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    int zoom = random_double (15,25);

    camera cam(lookfrom, lookat, vup, zoom, aspect_ratio, aperture, dist_to_focus);

    // Render

    //store calculations from threads in a 2d vector
    vector<pixel> vec_width(image_width, pixel(0,0,0));
    vec_width.reserve(image_width);

    IMAGE_STORE.resize(image_height, vec_width);
    std::cerr << IMAGE_STORE.size();
    std::cerr << " " << IMAGE_STORE[0].size();
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

        std::cerr << "CALCULATING PIXEL " << j << " " << i << endl;

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
            cout << pixel_color[0] << " " << pixel_color[1] << " " << pixel_color[2] << endl;
        }
        catch(int ja){
            std::cerr << j << ' ' << i;
        }

    }

}