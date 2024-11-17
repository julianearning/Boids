#ifndef _BIRD_HPP
#define _BIRD_HPP

#include "vector.hpp"

class Bird {
public:
    bool in_border;
    double max_speed;
    double min_cosine_similarity;
    double cosangle;
    double sinangle;
    double angle;
    char r;
    char g;
    char b;
    int max_x;
    int max_y;
    Vector<int> position;
    Vector<double> velocity;
    Bird(int initial_position_x, int initial_position_y, double initial_velocity_x, double initial_velocity_y, int max_x, int max_y);
    void update(double x, double y);
    void reset_velocity();
};


#endif