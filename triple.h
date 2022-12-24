#ifndef triple_h
#define triple_h

#include <cmath>
#include <vector>
#include <iostream>

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

//This function rounds to int(0-255) for output to .ppm file
inline void print_ppm(std::ostream &out, pixel &p){
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * p[0]) << ' '
        << static_cast<int>(255.999 * p[1]) << ' '
        << static_cast<int>(255.999 * p[2]) << '\n';
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
         * left[1] * right[1]
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



#endif