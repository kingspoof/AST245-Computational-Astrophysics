#ifndef PLANET_hpp
#define PLANET_hpp

#include <Eigen/Dense>

struct Planet
{
    double mass;
    double potential;

    Eigen::Vector3d position;
    Eigen::Vector3d velocity;

    Planet(double m, double pot, Eigen::Vector3d pos, Eigen::Vector3d vel) : mass(m),
                                                                             potential(pot),
                                                                             position(pos),
                                                                             velocity(vel)
    {}
};

#endif