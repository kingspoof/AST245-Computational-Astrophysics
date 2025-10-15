#ifndef RUNGE_KUTTA_HPP
#define RUNGE_KUTTA_HPP

#include <functional>
#include <cmath>
#include <iostream>
#include <vector>

#include "Vector2D.hpp"
#include "SimulationResult.hpp"
#include "Phase.hpp"

class RungeKutta
{
private:
    // the function that we will integrate
    std::function<Phase(const Phase &)> derivative;
    std::function<double(const Vector2D &, const Vector2D &)> total_energy_function;
    std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function;


    Phase k1(const Phase & phase) const
    {
        return derivative(phase);
    }
    Phase k2(const Phase & phase, double dt) const
    {
        return derivative(phase + k1(phase) * dt/2);
    }
    Phase k3(const Phase & phase, double dt) const
    {
        return derivative(phase + k2(phase, dt) * dt/2);
    }
    Phase k4(const Phase & phase, double dt) const
    {
        return derivative(phase + k3(phase, dt) * dt);
    }

public:
    RungeKutta(std::function<Phase(const Phase &)> rdotdot_func, std::function<double(const Vector2D &, const Vector2D &)> energy_func, std::function<double(const Vector2D &, const Vector2D &)> am_func)
    {
        derivative = rdotdot_func;
        total_energy_function = energy_func;
        angular_momentum_function = am_func;
    }

    SimulationResult integrate(int level, const Vector2D &initial_position, double eccentricity, double t_max, double dt, double mass) const
    {
        // calculate the initial parameters
        Vector2D initial_velocity(0, std::sqrt(1 + eccentricity));
        auto initial_energy = total_energy_function(initial_position, initial_velocity);
        auto initial_angular_momentum = angular_momentum_function(initial_position, initial_velocity);

        // prepare the simulation result with the used components
        SimulationResult result(
            initial_position,
            initial_velocity,
            eccentricity,
            dt,
            initial_energy,
            initial_angular_momentum);

        int n_steps = static_cast<int>(t_max / dt);

        Phase current_phase(result.positions.back(), result.velocities.back());

        for(int i = 0; i < n_steps; i++)
        {
            if(level == 2)
            {
                auto k1_result = k1(current_phase);
                auto k2_result = k2(current_phase, dt);
                current_phase += (k1_result + k2_result) * dt/2;
            }
            else if(level == 4)
            {
                auto k1_result = k1(current_phase);
                auto k2_result = k2(current_phase, dt);
                auto k3_result = k3(current_phase, dt);
                auto k4_result = k4(current_phase, dt);
                current_phase += (k1_result/6 + k2_result/3 + k3_result/3 + k4_result/6) * dt;
            }

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