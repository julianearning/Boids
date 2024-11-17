#ifndef _SWARM_HPP
#define _SWARM_HPP

#include <vector>
#include "bird.hpp"
#include "vector.hpp"

class Swarm {
private:
    std::vector<Bird> birds;
    int n_birds;
    double cohesion_param;
    double seperation_param;
    double alignment_param;
    double radius;
    double radius_seperation;
    int max_x;
    int max_y;
public:
    Swarm(int max_x, int max_y, int n_birds, double cohesion, double seperation, double alignment, double radius, double radius_seperation);
    int get_nth_bird(int n, Vector<int> * position, Vector<double> * velocity, char* r, char* g, char* b);
    void update();
    void get_neighbours(int p, std::vector<int> * neighbours);
    void get_cohesion(int p, std::vector<int> & neighbours, Vector<double> * cohesion_vector);
    void get_alignment(int p, std::vector<int> & neighbours, Vector<double> * alignment_vector);
    void get_seperation(int p, std::vector<int> & neighbours, Vector<double> * seperation_vector);
    void get_draft(int p, std::vector<int> & neighbours, Vector<double> * draft_vector); // when no other birds are near, bird will fly exactly straight (not very realisic!)
    void get_border_push(int p, std::vector<int> & neighbours, Vector<double> * border_push_vector); // pushes birds into frame
};


#endif