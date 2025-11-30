#include <vector>
#include <Eigen/Dense>
#include <math.h>
#include "Planet.hpp"
#include "Universe.hpp"

/** Computes the mean interparticle separation
 * We use the half-mass region
 * For this we compute the half mass radius
 * Then count the number of particles within this region of space
 * Then compute the volume of the sphere which corresponds to the half mass radius
 * Then we compute the mean density which then gives the mean sparation
 */
double Universe::calculate_mean_interparticle_separation() const
{
    // since all particles have the same size
    // the number of particles which are inside the half mass radius
    // es exactely half of all particles
    double num_inside = _magnitudes.size() / 2;
    // 22/7 is used as a quick pi appr
    double half_mass_radius_to_three = _half_mass_radius * _half_mass_radius * _half_mass_radius;
    double half_mass_volume = 4 / 3 * M_PI * half_mass_radius_to_three;

    return std::pow(half_mass_volume / num_inside, 1 / 3.);
}

std::vector<Eigen::Vector3d> Universe::calculate_direct_nbody_forces(double s, double G) const
{
    std::vector<Eigen::Vector3d> forces(_planets.size(), Eigen::Vector3d(0,0,0));
    for (int i = 0; i < _planets.size(); i++)
    {
        for(int j = i + 1; j < _planets.size(); j++)
        {
            // calculate the force for particle i
            auto r_vec = _planets[i].position - _planets[j].position;
            double r_mag = r_vec.norm();

            auto force = -1 * G * _planets[i].mass * _planets[j].mass * std::pow(r_mag*r_mag + s * s, -1.5) * r_vec;

            // we now add this computed force to the total force vector
            // based upon the actio = reactio principle
            forces[i] += force;
            forces[j] -= force;
        }
    }

    return forces;
}

void Universe::_compute_half_mass_radius()
{
    // calculate the half mass radius
    auto sorted_magnitudes = _magnitudes;
    std::sort(sorted_magnitudes.begin(), sorted_magnitudes.end());
    int center_index = sorted_magnitudes.size() / 2;
    _half_mass_radius = sorted_magnitudes[center_index];
}

void Universe::_compute_magnitudes()
{
    // calculate the magnitudes list
    for (auto const &planet : _planets)
    {
        _magnitudes.push_back(planet.position.norm());
    }
}

void Universe::_relist_positions()
{
    for(const auto planet : _planets)
    {
        _positions.push_back(planet.position);
    }
}