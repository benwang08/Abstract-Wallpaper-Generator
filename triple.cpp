#include "triple.h"

triple::triple(){
    vec.resize(3, 0);
}

triple::triple(double x, double y, double z){
    vec.resize(3);
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

triple::triple(vector<double> vec_in){
    vec = vec_in;
}

vector<double> triple::get_vec(){
    return vec;
}

vector<double> triple::operator-(){
    vector<double> copy = {-1 * vec[0], -1 * vec[1], -1 * vec[2]};
    return copy;
}

void triple::operator+=(vector<double> vec_in){
    for (int i = 0; i < 3; i++){
        vec[i] += vec_in[i];
    }
}

void triple::operator*=(double val){
    for (int i = 0; i < 3; i++){
        vec[i] *= val;
    }
}

void triple::operator/=(double val){
    for (int i = 0; i < 3; i++){
        vec[i] /= val;
    }
}

double triple::length(){
    double sqr = pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2);

    return sqrt(sqr);
}    


