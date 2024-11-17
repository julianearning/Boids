#include "bird.hpp"
#include "vector.hpp"
#include <random>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm>

Bird::Bird(int initial_position_x, int initial_position_y, double initial_velocity_x, double initial_velocity_y, int max_x, int max_y) {
    this->in_border=false;
    this->min_cosine_similarity=0.9;
    this->angle=std::acos(min_cosine_similarity);
    this->cosangle=std::cos(angle);
    this->sinangle=std::sin(angle);
    this->max_speed=8;
    this->position=Vector<int>(initial_position_x, initial_position_y);
    this->velocity=Vector<double>(initial_velocity_x, initial_velocity_y);
    this->r=rand()%256;
    this->g=rand()%256;
    this->b=rand()%256;
    this->max_x=max_x;
    this->max_y=max_y;
}


void Bird::update(double x, double y) {
    double new_x=this->velocity.get_x()+x;
    double new_y=this->velocity.get_y()+y;
    double corrected_x=0.0;
    double corrected_y=0.0;
    double new_magnitude=std::sqrt(new_x*new_x+new_y*new_y);
    double old_magnitude=std::sqrt(this->velocity.get_x()*this->velocity.get_x()+this->velocity.get_y()*this->velocity.get_y());
    Vector<double> unit_vector(this->velocity.get_x()/old_magnitude,this->velocity.get_y()/old_magnitude);
    // clamp turn angle
    double nenner=(new_magnitude*old_magnitude);
    double factor=1.0;
    if(nenner!=0) {
        double cos_similarity=(this->velocity.get_x()*new_x+this->velocity.get_y()*new_y)/nenner;
        if((cos_similarity<min_cosine_similarity)) { // clamp angle
            if((this->velocity.get_x()*new_y - this->velocity.get_y()*new_x) < 0) factor = -factor;
            new_x=(cosangle*unit_vector.get_x()-sinangle*unit_vector.get_y())*new_magnitude*factor;
            new_y=(cosangle*unit_vector.get_y()+sinangle*unit_vector.get_x())*new_magnitude*factor;
        }
    }

    this->velocity.set_x(new_x);
    this->velocity.set_y(new_y);

    // clamp speed
    double speed=std::sqrt((this->velocity.get_x()*this->velocity.get_x())+(this->velocity.get_y()*this->velocity.get_y()));
    if((speed>max_speed)) {
        this->velocity.set_x((this->velocity.get_x()/speed)*max_speed);
        this->velocity.set_y((this->velocity.get_y()/speed)*max_speed);
    }
    
    if((velocity.get_x()!=velocity.get_x()) || (velocity.get_y()!=velocity.get_y())) {
        this->velocity.set_x(1.0);
        this->velocity.set_y(1.0);
    }

    this->position.set_x(this->position.get_x()+this->velocity.get_x());
    this->position.set_y(this->position.get_y()+this->velocity.get_y());
    //in_border=false;

}


void Bird::reset_velocity() {
    this->velocity.set_x(0.0);
    this->velocity.set_y(0.0);
    in_border=true;
}