#ifndef SEMI_IMPLICIT_EULER_HPP
#define SEMI_IMPLICIT_EULER_HPP

#include <iostream>
#include "Vector2D.hpp"
#include "SimulationResult.hpp"
#include <functional>

class SemiImplicitEuler
{
private:
    // the function that we will integrate
    std::function<Phase(const Phase &)> derivative;

    // helper functions
    std::function<double(const Vector2D &, const Vector2D &)> total_energy_function;
    std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function;

public:
    SemiImplicitEuler(
        std::function<Phase(const Phase &)> function,
        std::function<double(const Vector2D &, const Vector2D &)> energy_function,
        std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function
    )
    {
        this->derivative = function;
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


        Phase current_phase(
            result.positions.back(),
            result.velocities.back()
        );

        for(int i = 0; i < n_steps; i++)
        {
            current_phase.position += current_phase.velocity * dt;
            current_phase.velocity += derivative(current_phase).velocity * dt;


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