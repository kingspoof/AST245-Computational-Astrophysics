#ifndef UNIVERSE_hpp
#define UNIVERSE_hpp

#include <vector>
#include <Eigen/Dense>
#include "Planet.hpp"

class Universe
{
private:
    double _half_mass_radius;
    double _total_mass;
    double _scale_factor;

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
        // total mass is trivial as all particles have the same mass
        _total_mass = _magnitudes.size() * _planets[0].mass;

        // calculation as defined in the original paper
        _scale_factor = _half_mass_radius / (1 + sqrt(2));
    }

    /*Computes the mean interparticle separation
     * We use the half-mass region
     * For this we compute the half mass radius
     * Then count the number of particles within this region of space
     * Then compute the volume of the sphere which corresponds to the half mass radius
     * Then we compute the mean density which then gives the mean sparation
     */
    double calculate_mean_interparticle_separation() const;

    /*
    Computes the mass using the given positions
    */
    double calculate_mass_specifically(const double &radius) const;

    /*
    Computes the mass using equation 3 from the hernquist paper
    */
    double hq_calculate_mass(const double &radius) const;

    /*
    Computes the forces between all particles using the direct summation
    as well as newtons law of attraction and the law of actio = reactio
    */
    std::vector<Eigen::Vector3d> calculate_direct_nbody_forces(double softening, double G) const;

    /*
    Calculates the force on each of the particles using the equation in the hernquist paper
    The force is calculated between a particle which sits at the center
    with a mass which corresponds to the calculated mass also from the paper
    */
    std::vector<Eigen::Vector3d> hq_calculate_force(const double &G) const;
};

#endif