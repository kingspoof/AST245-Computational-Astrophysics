#ifndef DIRECTSOLVER_HPP
#define DIRECTSOLVER_HPP

#include <Eigen/Dense>
#include "Planet.hpp"

using Eigen::Vector2d;

class DirectSolver
{
private:
    double _G;

public:
    DirectSolver(double g) : _G(g) {}

    Vector2d acceleration(const Planet &p, const std::vector<Planet> &planets)
    {
        Vector2d acceleration(0, 0);

        for (const Planet &other : planets)
        {
            // F = - G * m1 * m2 / r^2
            Vector2d r = other.get_position() - p.get_position();
            double r_mag = r.norm();
            if (r_mag != 0)
            {
                double r_mag_cubed = r_mag * r_mag * r_mag;
                acceleration += this->_G * other.get_mass() * r / r_mag_cubed;
            }
        }

        return acceleration;
    }
};

#endif // DIRECTSOLVER_HPP