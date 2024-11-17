#include <random>
#include <ctime>
#include "swarm.hpp"
#include <iostream>
#include <algorithm>

Swarm::Swarm(int max_x, int max_y, int n_birds, double cohesion, double seperation, double alignment, double radius, double radius_seperation) {
    this->n_birds=n_birds;
    this->cohesion_param=cohesion;
    this->seperation_param=seperation;
    this->alignment_param=alignment;
    this->radius=radius;
    this->radius_seperation=radius_seperation;
    this->max_x=max_x;
    this->max_y=max_y;
    birds.reserve(n_birds);
    std::srand(std::time(nullptr));
    for(int i = 0; i<n_birds; i++) {
        birds.push_back(Bird((int)(rand()%max_x),(int)(rand()%max_y), ((double)((rand()%200))-100.0)/200, ((double)((rand()%200))-100.0)/200, max_x, max_y));
    }
}


int Swarm::get_nth_bird(int n, Vector<int> * position, Vector<double> * velocity, char* r, char* g, char* b) {
    if(n>=n_birds) return 1;

    *position=birds.at(n).position;
    *velocity=birds.at(n).velocity;
    *r=birds.at(n).r;
    *g=birds.at(n).g;
    *b=birds.at(n).b;

    return 0; 
}


void Swarm::update() {
    std::vector<int> neighbours;
    Vector<double> cohesion;
    Vector<double> alignment;
    Vector<double> seperation;
    Vector<double> border_push;
    Vector<double> random_push;
    double curr_x=0.0;
    double curr_y=0.0;
    for(int i = 0; i<n_birds; i++) {
        get_neighbours(i,&neighbours);
        get_cohesion(i,neighbours,&cohesion);
        curr_x+=cohesion.get_x();
        curr_y+=cohesion.get_y();
        get_alignment(i,neighbours,&alignment);
        curr_x+=alignment.get_x();
        curr_y+=alignment.get_y();  
        get_seperation(i,neighbours,&seperation);
        curr_x+=seperation.get_x();
        curr_y+=seperation.get_y();
        //get_random_variation(i, neighbours, &random_push);
        //curr_x+=random_push.get_x();
        //curr_y+=random_push.get_y();

        get_border_push(i, neighbours, &border_push);
        if((border_push.get_x()!=0.0)||(border_push.get_y()!=0.0)) { 
            //birds.at(i).reset_velocity();
            curr_x=border_push.get_x();
            curr_y=border_push.get_y();
        } 
        birds.at(i).update(curr_x, curr_y);
    }
}

// for now just iterate over all other birds... not sure if more efficient method is even necessary
void Swarm::get_neighbours(int p, std::vector<int> * neighbours) {
    neighbours->clear();
    std::vector<double> distances;
    int x=birds.at(p).position.get_x();
    int y=birds.at(p).position.get_y();
    int curr_x=x;
    int curr_y=y;
    double diff_x=0.0;
    double diff_y=0.0;
    for(int i = 0; i<p; i++) {
        curr_x=birds.at(i).position.get_x();
        curr_y=birds.at(i).position.get_y();
        diff_x=(double)(curr_x-x);
        diff_y=(double)(curr_y-y);
        distances.push_back(std::sqrt((diff_x*diff_x)+(diff_y*diff_y)));
    }
    for(int i=p+1; i<n_birds; i++) {
        curr_x=birds.at(i).position.get_x();
        curr_y=birds.at(i).position.get_y();
        diff_x=(double)(curr_x-x);
        diff_y=(double)(curr_y-y);
        distances.push_back(std::sqrt(diff_x*diff_x+diff_y*diff_y));
    }

    for(int i = 0; i<p; i++) {
        if(distances.at(i)<=radius) {
            neighbours->push_back(i);
        } 
    }
    for(int i=p; i<n_birds-1; i++) {
        if(distances.at(i)<=radius) {
            neighbours->push_back(i+1);
        } 
    }
}

void Swarm::get_cohesion(int p, std::vector<int> & neighbours, Vector<double> * cohesion_vector) {
    if(neighbours.size()<=0) {
        cohesion_vector->set_x(0);
        cohesion_vector->set_y(0);
    }
    double mean_x=0.0;
    double mean_y=0.0;
    double inv_magnitude=1.0;
    for(int i = 0; i<neighbours.size(); i++) {
        mean_x+=birds.at(neighbours.at(i)).position.get_x();
        mean_y+=birds.at(neighbours.at(i)).position.get_y();
    }
    if(neighbours.size()>0) { 
        mean_x/=neighbours.size();
        mean_y/=neighbours.size();
        mean_x=mean_x-(double)birds.at(p).position.get_x();
        mean_y=mean_y-(double)birds.at(p).position.get_y();
        inv_magnitude=1/std::sqrt(mean_x*mean_x+mean_y*mean_y);
        cohesion_vector->set_x(mean_x*inv_magnitude*cohesion_param);
        cohesion_vector->set_y(mean_y*inv_magnitude*cohesion_param);
    }
}

void Swarm::get_alignment(int p, std::vector<int> & neighbours, Vector<double> * alignment_vector) {
    double mean_x=0.0;
    double mean_y=0.0;
    double inv_magnitude=1.0;
    for(int i = 0; i<neighbours.size(); i++) {
        mean_x+=birds.at(neighbours.at(i)).velocity.get_x();
        mean_y+=birds.at(neighbours.at(i)).velocity.get_y();
    }
    if(neighbours.size()>0) {
        mean_x/=neighbours.size();
        mean_y/=neighbours.size();
        mean_x=birds.at(p).velocity.get_x()-mean_x;
        mean_y=birds.at(p).velocity.get_y()-mean_y;
        inv_magnitude=1/std::sqrt(mean_x*mean_x+mean_y*mean_y);
        alignment_vector->set_x(mean_x*inv_magnitude*alignment_param);
        alignment_vector->set_y(mean_y*inv_magnitude*alignment_param);
    }
}

// assumes that radius > seperation_vector
void Swarm::get_seperation(int p, std::vector<int> & neighbours, Vector<double> * seperation_vector) {
    int curr_x;
    int curr_y;
    int x=birds.at(p).position.get_x();
    int y=birds.at(p).position.get_y();
    double min_close_dx=-1.0;
    double min_close_dy=-1.0;
    double min_dist=radius_seperation;
    double curr_distance=0.0;
    for(int i = 0; i<neighbours.size(); i++) {
        curr_x=birds.at(i).position.get_x();
        curr_y=birds.at(i).position.get_y();
        curr_distance=std::sqrt((curr_x-x)*(curr_x-x)+(curr_y-y)*(curr_y-y));
        if((curr_distance <= min_dist)) {
            //close_dx+=(curr_x-x);
            //close_dy+=(curr_y-y);

            min_close_dx=(curr_x-x);
            min_close_dy=(curr_y-y);
            min_dist=curr_distance;
        }
    }
    //double normalization=1/std::sqrt((close_dx*close_dx)+(close_dy+close_dy));
    if(min_close_dx>0) {
        seperation_vector->set_x(min_close_dx*seperation_param);
        seperation_vector->set_y(min_close_dy*seperation_param);
    }
}


void Swarm::get_random_variation(int p, std::vector<int> & neighbours, Vector<double> * random_vector) {
    int random_push_x=rand()%max_x;
    int random_push_y=rand()%max_y;
    random_vector->set_x(birds.at(p).position.get_x()-random_push_x);
    random_vector->set_y(birds.at(p).position.get_x()-random_push_y);
}

void Swarm::get_border_push(int p, std::vector<int> & neighbours, Vector<double> * border_push_vector) {
    double x=0.0;
    double y=0.0;
    double value=200.0;
    if(birds.at(p).position.get_x()<-10) {
        x+=value;
    }
    if((birds.at(p).position.get_y()<-10)) {
        y+=value;
    }
    if(birds.at(p).position.get_x()>max_x+10) {
        x-=value;
    }
    if(birds.at(p).position.get_y()>max_y+10) {
        y-=value;
    }
    border_push_vector->set_x(x);
    border_push_vector->set_y(y);
}