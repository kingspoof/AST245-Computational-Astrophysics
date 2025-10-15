#include "RungeKutta.hpp"
#include "Phase.hpp"

Phase RungeKutta::k1(const Vector2D &position, const Vector2D &velocity) const
{
    // position has as the function velocity and the velocity has the acceleration
    Phase k1;
    k1.position = velocity;
    k1.velocity = rdotdot(position);
    return k1;
}

Phase RungeKutta::k2(const Vector2D &position, const Vector2D &velocity, double dt) const
{
    Phase k1 = this->k1(position, velocity);

    auto temporary_position = position + k1.position * dt / 2;
    auto temporary_velocity = velocity + k1.velocity * dt / 2;

    return Phase(
        temporary_velocity,
        rdotdot(temporary_position));
}

Phase RungeKutta::k3(const Vector2D &position, const Vector2D &velocity, double dt) const
{

    auto k2 = this->k2(position, velocity, dt);

    auto temporary_position = position + k2.position * dt / 2;
    auto temporary_velocity = velocity + k2.velocity * dt / 2;

    return Phase(
        temporary_velocity,
        rdotdot(temporary_position));
}

Phase RungeKutta::k4(const Vector2D &position, const Vector2D &velocity, double dt) const
{
    auto k3 = this->k3(position, velocity, dt);

    auto temporary_position = position + k3.position * dt;
    auto temporary_velocity = velocity + k3.velocity * dt;

    return Phase(
        temporary_velocity,
        rdotdot(temporary_position));
}

SimulationResult RungeKutta::integrate(int level, const Vector2D &initial_position, double eccentricity, double t_max, double dt, double mass) const
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

    Vector2D current_position = result.positions.back();
    Vector2D current_velocity = result.velocities.back();

    for (int i = 0; i < n_steps; i++)
    {

        Vector2D position_increment(0, 0);
        Vector2D velocity_increment(0, 0);

        if (level == 2)
        {
            Phase k1 = this->k1(current_position, current_velocity);
            Phase k2 = this->k2(current_position, current_velocity, dt);

            position_increment = (k1.position + k2.position) * dt / 2;
            velocity_increment = (k1.velocity + k2.velocity) * dt / 2;
        }
        else if (level == 4)
        {
            Phase k1 = this->k1(current_position, current_velocity);
            Phase k2 = this->k2(current_position, current_velocity, dt);
            Phase k3 = this->k3(current_position, current_velocity, dt);
            Phase k4 = this->k4(current_position, current_velocity, dt);

            position_increment = (k1.position + k2.position * 2 + k3.position * 2 + k4.position) * dt/6;
            velocity_increment = (k1.velocity + k2.velocity * 2 + k3.velocity * 2 + k4.velocity) * dt/6;
        }

        current_position += position_increment;
        current_velocity += velocity_increment;

        // append new positions to the grid
        result.positions.push_back(current_position);
        result.velocities.push_back(current_velocity);

        result.energies.push_back(total_energy_function(current_position, current_velocity));
        result.angular_moments.push_back(angular_momentum_function(current_position, current_velocity));
    }

    // perform the runge kutta 2 integration

    return result;
}
