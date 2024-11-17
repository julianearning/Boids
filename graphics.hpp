#ifndef _GRAPHICS_HPP
#define _GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include "swarm.hpp"

class Graphics {
private:
    sf::RenderWindow window;
    Swarm * swarm;
public:
    Graphics(Swarm * swarm, unsigned int max_x, unsigned int max_y);
};


#endif


