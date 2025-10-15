#ifndef SIMULATIONRESULT_HPP
#define SIMULATIONRESULT_HPP

#include "Vector2D.hpp"
#include <vector>

struct SimulationResult{

    // initial conditions
    double eccentricity;
    Vector2D initial_position;
    Vector2D initial_velocity;

    // simulation information
    double time_step;
    
    // position and velocities
    std::vector<Vector2D> positions;
    std::vector<Vector2D> velocities;

    // for conservation of angular momentum and energy
    std::vector<double> energies;
    std::vector<double> angular_moments;


    SimulationResult(
        const Vector2D & initial_position,
        const Vector2D & initial_velocity, 
        double eccentricity, 
        double dt, 
        double initial_energy, 
        double initial_angular_momentum
    );

    void export_to_file(const std::string& filename, const std::string& method_name) const;
};


#endif