#ifndef LEAPFROG_HPP
#define LEAPFROG_HPP

#include <iostream>
#include "Vector2D.hpp"
#include "SimulationResult.hpp"
#include <functional>

class LeapFrog
{
private:
    // the function that we will integrate
    std::function<Vector2D(const Vector2D &)> rdotdot;

    // helper functions
    std::function<double(const Vector2D &, const Vector2D &)> total_energy_function;
    std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function;

public:
    LeapFrog(
        std::function<Vector2D(const Vector2D &)> function,
        std::function<double(const Vector2D &, const Vector2D &)> energy_function,
        std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function
    )
    {
        this->rdotdot = function;
        this->total_energy_function = energy_function;
        this->angular_momentum_function = angular_momentum_function;
    }

    // again we implement this here because it is a simple method and is quite short
    SimulationResult integrate(const Vector2D &initial_position, double eccentricity, double t_max, double dt, double mass) const
    {
        Vector2D initial_velocity(0, std::sqrt(1 + eccentricity));
        auto initial_energy = total_energy_function(initial_position, initial_velocity);
        auto initial_angular_momentum = angular_momentum_function(initial_position, initial_velocity);

        SimulationResult result(
            initial_position,
            initial_velocity,
            eccentricity,
            dt,
            initial_energy,
            initial_angular_momentum
        );

        int n_steps = static_cast<int>(t_max / dt);


        auto current_position = result.positions.back();
        auto current_velocity = result.velocities.back();
        
        for(int i = 0; i < n_steps; i++)
        {
            // update the velocity half a step
            auto velocity_half_step = current_velocity + rdotdot(current_position) * dt/2;
            
            // update the total position as well as the velocity
            current_position += velocity_half_step * dt;
            current_velocity = velocity_half_step + rdotdot(current_position) * dt/2;


            // append new positions to the grid
            result.positions.push_back(current_position);
            result.velocities.push_back(current_velocity);
    
            // append energy and total momentum to the grid
            result.energies.push_back(total_energy_function(current_position, current_velocity));
            result.angular_moments.push_back(angular_momentum_function(current_position, current_velocity));
        }


        return result;
    }
};


#endif