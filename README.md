# Boids  
Implementation of Boids in C++ using SFML for graphics and Boost for command line parsing. 
![alt text](boids.png "Screenshot of Program")

Boids is a simple simulation of flock/swarm-like behavior. Every fish wants to do three things:
  
- Cohesion: Fish wants to swim towards the center of all surrounding fishes  
- Alignment: Fish wants to align himself with the direction of all surrounding fishes   
- Seperation: Fish doesn't want to collide with other fishes   

Also necessary: 

- Push fishes away from the border of the screen, otherwise they disappear  
   
Together with many other fishes this causes natural-looking patterns. I added some constraints to make the movements look more natural:  
  
- max speed  
- max turn angle  
  
I still have to test a lot and want to parallelize it, so that more fishes can be simulated without dropping framerate (right now, about 1000 fishes can be done at 60fps). 
The radius search can be optimized too. Right now, I just iterate over all other fishes (O(n^2)) because I'm unsure which data structure would be most advantageous for constantly changing data. Also, the fishes like to circle, which I'm pretty sure is due to some kind of bug? My JavaScript implementation didn't do that. 
