#ifndef UNIVERSE_hpp
#define UNIVERSE_hpp

#include <vector>
#include <Eigen/Dense>
#include "Planet.hpp"

class Universe
{
private:
    double _half_mass_radius;

    void _compute_magnitudes();
    void _compute_half_mass_radius();
    void _relist_positions();

public:
    std::vector<Planet> _planets;
    std::vector<Eigen::Vector3d> _positions;
    std::vector<double> _magnitudes;

    Universe(const std::vector<Planet> planets) : _planets(planets)
    {
        _compute_magnitudes();
        _compute_half_mass_radius();
        _relist_positions();
    }

    double calculate_mean_interparticle_separation() const;

    std::vector<Eigen::Vector3d> calculate_direct_nbody_forces(double softening, double G) const;
};

#endif