#ifndef EXPLICIT_EULER_HPP
#define EXPLICIT_EULER_HPP

#include <functional>
#include <cmath>
#include <iostream>
#include <vector>

#include "Vector2D.hpp"
#include "SimulationResult.hpp"
#include "Phase.hpp"

class ExplicitEuler
{
private:
    // the function that we will integrate
    std::function<Phase(const Phase &)> derivative;
    std::function<double(const Vector2D &, const Vector2D &)> total_energy_function;
    std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function;

public:
    // first we have to define the constructor that takes the acceleration function as a parameter
    ExplicitEuler(std::function<Phase(const Phase &)> rdotdot_func, std::function<double(const Vector2D &, const Vector2D &)> energy_func, std::function<double(const Vector2D &, const Vector2D &)> am_func)
    {
        derivative = rdotdot_func;
        total_energy_function = energy_func;
        angular_momentum_function = am_func;
    }

    // now we define the integrate function
    // Defining this inside the header file is now okey, since this will only be included in the main class
    // when this class will be used in multiple files -> then we would take it outside and create a cpp file
    SimulationResult integrate(const Vector2D &initial_position, double eccentricity, double t_max, double dt, double mass)
    {
        //calculate the initial parameters
        Vector2D initial_velocity(0, std::sqrt(1 + eccentricity));
        auto initial_energy = total_energy_function(initial_position, initial_velocity);
        auto initial_angular_momentum = angular_momentum_function(initial_position, initial_velocity);
        
        //prepare the simulation result with the used components
        SimulationResult result(
            initial_position,
            initial_velocity,
            eccentricity,
            dt,
            initial_energy,
            initial_angular_momentum
        );
        
        //calculate the number of time steps we need -> cast to an int and add one more step
        int n_steps = static_cast<int>(t_max / dt);
        
        

        Phase current_phase = Phase(
            result.positions.back(),
            result.velocities.back()
        );

        // now we perform the explicit euler integration
        for(int i = 0; i < n_steps; i++)
        {

            //this is the entire thing for the update of the phase
            current_phase += derivative(current_phase) * dt;


            // store the new position and velocity
            result.positions.push_back(current_phase.position);
            result.velocities.push_back(current_phase.velocity);
            result.energies.push_back(total_energy_function(current_phase.position, current_phase.velocity));
            result.angular_moments.push_back(angular_momentum_function(current_phase.position, current_phase.velocity));
        }
    
        return result;
    }
};

#endif