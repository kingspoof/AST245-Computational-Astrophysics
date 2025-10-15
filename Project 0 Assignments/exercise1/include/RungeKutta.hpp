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
    std::function<Vector2D(const Vector2D &)> rdotdot;
    std::function<double(const Vector2D &, const Vector2D &)> total_energy_function;
    std::function<double(const Vector2D &, const Vector2D &)> angular_momentum_function;


    Phase k1(const Vector2D & position, const Vector2D & velocity) const;
    Phase k2(const Vector2D & position, const Vector2D & velocity, double dt) const;
    Phase k3(const Vector2D & position, const Vector2D & velocity, double dt) const;
    Phase k4(const Vector2D & position, const Vector2D & velocity, double dt) const;

public:
    RungeKutta(std::function<Vector2D(const Vector2D &)> rdotdot_func, std::function<double(const Vector2D &, const Vector2D &)> energy_func, std::function<double(const Vector2D &, const Vector2D &)> am_func)
    {
        rdotdot = rdotdot_func;
        total_energy_function = energy_func;
        angular_momentum_function = am_func;
    }

    SimulationResult integrate(int level, const Vector2D &initial_position, double eccentricity, double t_max, double dt, double mass) const;
};

#endif