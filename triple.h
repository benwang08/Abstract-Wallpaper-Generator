#ifndef triple_h
#define triple_h

#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <assert.h>

using namespace std;


//Class represents both 3d point and RGB color
class triple{
    public:
        //Constructors
        triple();
        triple(double x, double y, double z);
        triple(vector<double> vec_in);

        //Getters
        vector<double> get_vec();
        double operator[](int in) const{
            return vec[in];
        }

        double& mod_value(int in){
            assert(in >= 0 && in <= 3);

            return vec[in];
        }

        //Overloaded operators for more convenient vector arithmetic
        vector<double> operator-();
        void operator+=(vector<double> vec_in);
        void operator*=(double val);
        void operator/=(double val);

        double length();    
   
    private:
        vector<double> vec;

};

//Aliases for this class
using point = triple;
using pixel = triple;

//More helper functions for arithmetic involving 2 triples
inline std::ostream& operator<<(std::ostream &out, triple &v){
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline triple operator+(const triple &left, const triple &right){
    return triple(left[0] + right[0], left[1] + right[1], left[2] + right[2]);
}

inline triple operator-(const triple &left, const triple &right){
    return triple(left[0] - right[0], left[1] - right[1], left[2] - right[2]);
}

inline triple operator*(const triple &left, const triple &right){
    return triple(left[0] * right[0], left[1] * right[1], left[2] * right[2]);
}

inline triple operator*(const triple &left, double right){
    return triple(left[0] * right, left[1] * right, left[2] * right);
}

inline triple operator*(double left, const triple &right){
    return triple(right[0] * left, right[1] * left, right[2] * left);

}

inline triple operator/(const triple &left, double right){
    return triple(left[0] / right, left[1] / right, left[2] / right);
}

inline double dot(const triple &left, const triple &right){
    return left[0] * right[0]
         + left[1] * right[1]
         + left[2] * right[2];
}

inline triple cross(const triple &left, const triple &right){
    return triple(left[1] * right[2] - left[2] * right[1],
                left[2] * right[0] - left[0] * right[2],
                left[0] * right[1] - left[1] * right[0]);
}

inline triple unit_vector(triple v){
    return v / v.length();
}

//calculate reflection off metal material using vector math
inline triple reflect(triple v, triple n) {
    return v - 2*dot(v,n)*n;
}

//function to calculate ray refraction
//etai_over_etat is a variable dependant on the material
triple refract(triple uv, triple n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    triple r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    triple r_out_parallel = -sqrt(fabs(1.0 - pow(r_out_perp.length(),2))) * n;
    return r_out_perp + r_out_parallel;
}


#endif