#ifndef material_h
#define material_h

#include "utility.h"
#include "entity.h"

class material {
    public:
        virtual bool scatter(ray r_in, hit_record rec, pixel &attenuation, ray &scattered){
            return false;
        }
};

//class representing diffuse material (matte)
class diffuse : public material{
    public:
        diffuse(pixel color): albedo(color.get_vec()) {}

        virtual bool scatter(ray r_in, hit_record rec, pixel &attenuation, ray &scattered) override {
            //scatter ray to a random direction
            auto scattered_dir = rec.normal + random_unit_vec();

            // Catch degenerate scatter direction
            if (near_zero(scattered_dir))
                scattered_dir = rec.normal;

            //set new ray
            scattered = ray(rec.p, scattered_dir);
                        

            attenuation = albedo;
            return true;
        }


    private:
        pixel albedo;
};

//class representing reflective material (metal)
class metal : public material{
    public:
        metal(pixel color): albedo(color.get_vec()) {}

        virtual bool scatter(ray r_in, hit_record rec, pixel &attenuation, ray &scattered) override {
            //reflect ray
            auto reflected_dir = reflect(unit_vector(r_in.get_direction()), rec.normal);

            //set new ray
            scattered = ray(rec.p, reflected_dir);
                    
            attenuation = albedo;

            return (dot(scattered.get_direction(), rec.normal) > 0);
        }


    private:
        pixel albedo;

};

#endif