#ifndef triple_h
#define triple_h

#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <assert.h>
#include "utility.h"

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

//returns random triple with vals between 0 and 1
inline static triple random_triple() {
    return triple(random_double(), random_double(), random_double());
}

//returns random triple with vals between min and max
inline static triple random_triple(double min, double max) {
    return triple(random_double(min,max), random_double(min,max), random_double(min,max));
}

//returns random point in a unit sphere (radius = 1)
inline triple random_in_unit_sphere() {
    while (true) {
        //select a random point
        auto p = random_triple(-1,1);

        //if point is not in unit circle, continue without returning
        if (pow(p.length(), 2) >= 1){
            continue;
        }

        //point is in unit circle
        return p;
    }
}


#endif