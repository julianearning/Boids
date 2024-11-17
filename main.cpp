#include "swarm.hpp"
#include "vector.hpp"
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <unistd.h>
#include <chrono>
#include <thread>
#include <boost/program_options.hpp>


unsigned int n_birds = 2000; 
double cohesion = 0.1;
double seperation = 60;
double alignment = 0.1;
double radius=500;
double radius_seperation=300;
unsigned int max_x=1600;
unsigned int max_y=900;
double width_tail=10;
double length_bird=20;
unsigned int fps=60;   


//char * help_msg = "Use: \n"; // TODO

void draw_birdies(sf::RenderWindow * window, Swarm * swarm) {
    Vector<int> position;
    Vector<double> velocity;
    char r;
    char g;
    char b;

    sf::ConvexShape convex;
    //convex.setFillColor(sf::Color::Green);
    convex.setPointCount(3);

    for(int i = 0; i<n_birds; i++) {
        if(swarm->get_nth_bird(i, &position, &velocity, &r, &g, &b)!=0) { break; }
        velocity.normalize();
        velocity.multiply(length_bird);
        convex.setPoint(0, sf::Vector2f((double)position.get_x()+velocity.get_x(), (double)position.get_y()+velocity.get_y()));
        velocity.degree90();
        velocity.normalize();
        velocity.multiply(width_tail/2);
        convex.setPoint(1, sf::Vector2f((double)position.get_x()+velocity.get_x(), (double)position.get_y()+velocity.get_y()));
        convex.setPoint(2, sf::Vector2f((double)position.get_x()-velocity.get_x(), (double)position.get_y()-velocity.get_y()));
        convex.setFillColor(sf::Color(r,g,b));
        window->draw(convex);
    } 
}


int main(int argc, char * argv []) {

    // boost command line parsing
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("n", boost::program_options::value<unsigned int>(&n_birds)->default_value(1000), "Number of birds/fishes.")
        ("cohesion", boost::program_options::value<double>(&cohesion)->default_value(1), "Weight of cohesion vector.")
        ("seperation", boost::program_options::value<double>(&seperation)->default_value(1), "Weight of seperation vector.")
        ("alignment", boost::program_options::value<double>(&alignment)->default_value(1), "Weight of alignment vector.")
        ("radius", boost::program_options::value<double>(&radius)->default_value(400.0), "Radius for fishes that influence cohesion and alignment.")
        ("radius_seperation", boost::program_options::value<double>(&radius_seperation)->default_value(100.0), "Radius for fishes that influence seperation.")
        ("max_x", boost::program_options::value<unsigned int>(&max_x)->default_value(1600), "Size x-axis of graphics window.")
        ("max_y", boost::program_options::value<unsigned int>(&max_y)->default_value(900), "Size y-axis of graphics window.")
        ("width_tail", boost::program_options::value<double>(&width_tail)->default_value(10.0), "Width of tail of bird graphics." )
        ("length_bird", boost::program_options::value<double>(&length_bird)->default_value(20), "Length of bird graphics.")
        ("fps", boost::program_options::value<unsigned int>(&fps)->default_value(60), "Desired FPS.")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }



    double ms_per_frame = 1000/fps;

    std::cout<<"Initializing Swarm...";
    Swarm swarm(max_x, max_y, n_birds, cohesion, seperation, alignment,radius, radius_seperation);
    std::cout<<"[Done]\n";
    std::cout<<"Creating Window...";
    sf::RenderWindow window(sf::VideoMode(max_x, max_y), "Boids");
    std::cout<<"[Done]\n";

    std::cout<<"Starting Animation..."<<std::endl;
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        draw_birdies(&window, &swarm);
        window.display();

        auto l1 = std::chrono::high_resolution_clock::now();
        swarm.update();
        auto l2 = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(l2 - l1);

        if(ms_int.count()<=16.6) {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(ms_per_frame-ms_int.count())));
            //std::cout<<"\n[FPS at 60]";
        } else {
            std::cout<<"\n[FPS dropping to "<<1000/((double)(ms_int.count()))<<"]";
        }
    }

    return 0;
}