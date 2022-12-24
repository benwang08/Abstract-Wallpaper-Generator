#include "sphere.h"


bool sphere::hit(ray r, double t_min, double t_max, hit_record& rec) {
    triple oc = r.get_origin() - center;
    auto a = pow(r.get_direction().length(), 2);
    auto half_b = dot(oc, r.get_direction());
    auto c = pow(oc.length(), 2) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;

    //if smallest root is not valid, check the larger root
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
    }
    
    //if larger root is not valid, return false
    if (root < t_min || root > t_max) {
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
}
