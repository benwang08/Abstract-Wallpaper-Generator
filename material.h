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
        metal(pixel color, double fuzz_in): albedo(color.get_vec()) {
            fuzz = fuzz_in;
        }

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

        //fuzz value for fuzzy reflection
        double fuzz;

};

//class representing dielectric material (glass)
class glass : public material {
    public:
        glass(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(ray r_in, hit_record rec, pixel &attenuation, ray &scattered) override {
            attenuation = pixel(1.0, 1.0, 1.0);
            double refraction_ratio = rec.hits_outside ? (1.0/ir) : ir;

            triple unit_direction = unit_vector(r_in.get_direction());

            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);

            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            triple direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
                direction = reflect(unit_direction, rec.normal);
            }
            else{
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double ir; // Index of Refraction

        //Use Schlick's approximation for reflectance.
        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif