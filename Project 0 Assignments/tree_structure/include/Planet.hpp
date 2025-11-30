#ifndef PLANET_HPP
#define PLANET_HPP

#include <Eigen/Dense>
using Eigen::Vector2d;

struct Planet{
private:
    double mass;
    Vector2d position;
    Vector2d velocity;

    
    
    public:
        Planet(double m, Vector2d pos, Vector2d vel)
            : mass(m), position(pos), velocity(vel) {}

            
        double get_mass() const { return mass; }
        Vector2d get_position() const { return position; }
        Vector2d get_velocity() const { return velocity; }
    };


#endif // PLANET_HPP