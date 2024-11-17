#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <math.h>
#include <ostream>

template <class T>
class Vector {
private:
    T x = 0; 
    T y = 0;
public:
    Vector() : x(0), y(0) {}
    Vector(T x1, T y1) {x=x1; y=y1;};
    Vector(const Vector & a) {x=a.x; y=a.y;};
    Vector& operator=(const Vector other) { x=other.x; y=other.y; return *this; }
    Vector& operator+(const Vector other) { x=x+other.x; y=y+other.y; return *this; };
    void normalize() {double inv_magnitude=1/std::sqrt(x*x+y*y); x*=inv_magnitude; y*=inv_magnitude; };
    void multiply(double scalar) { x*=scalar; y*=scalar; }; 
    void rotate(double angle) { x = x* std::cos(angle) + y*std::sin(angle); y = -x*std::sin(angle) + y*std::cos(angle); }; 
    void degree90() { double temp_x=x; x=y, y=-temp_x; };

    T get_x() { return x; }
    T get_y() { return y; }
    void set_x(T x) { this->x=x; }
    void set_y(T y) { this->y=y; }
};



#endif