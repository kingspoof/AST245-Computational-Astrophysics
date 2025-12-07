#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>
#include "Planet.hpp"
#include "Node.hpp"

using std::vector;
using Eigen::Vector3d;


// implement the actuall interesting function
void Node::subdivide(std::vector<Planet> & planets)
{
    // implementation of the subdivision into 8 child nodes
    // first we need to check if we actually need to subdivide
    if (planets.size() <= limit){
        is_leaf = true;
        this->planets = planets;
        return;
    }
    

    // at first we need to determine in which quadrant each planet goes
    vector<vector<Planet>> quadrant_planets(8);
    auto center = center_position();
    auto half_diagonal = (diag_one - diag_two).norm() / 2;

    // i want to do the following octant mapping:
    // after adjusting the position of all planets to be relative to the center:
    // 7: (+x, +y, +z) 4 + 2 + 1 = 7
    // 6: (+x, +y, -z) 4 + 2 + 0 = 6
    // 5: (+x, -y, +z) 4 + 0 + 1 = 5
    // 4: (+x, -y, -z) 4 + 0 + 0 = 4
    // 3: (-x, +y, +z) 0 + 2 + 1 = 3
    // 2: (-x, +y, -z) 0 + 2 + 0 = 2
    // 1: (-x, -y, +z) 0 + 0 + 1 = 1
    // 0: (-x, -y, -z) 0 + 0 + 0 = 0

    // now we need to create the above mapping as vectors
    vector<Vector3d> directions = {
        Vector3d(-1, -1, -1), // 0
        Vector3d(-1, -1,  1), // 1
        Vector3d(-1,  1, -1), // 2
        Vector3d(-1,  1,  1), // 3
        Vector3d( 1, -1, -1), // 4
        Vector3d( 1, -1,  1), // 5
        Vector3d( 1,  1, -1), // 6
        Vector3d( 1,  1,  1)  // 7
    };


    // now we can put all planets into their respective quadrants
    for (const auto& planet : planets) {
        // compute position relative to center
        Vector3d relative_pos = planet.position - center;

        // determine octant
        int octant = 0;
        if (relative_pos[0] >= 0) octant += 4; // +x
        if (relative_pos[1] >= 0) octant += 2; // +y
        if (relative_pos[2] >= 0) octant += 1; // +z

        quadrant_planets[octant].push_back(planet);
    }

    // now that all the planets are groupes we need to create the planets
    for (int i = 0; i < 8; ++i) {
        // if the quadrant is empty, we don't create a child
        if(quadrant_planets[i].empty()) continue;

        // since we only need to compute two diagonal points, we can do this with the direction vectors
        Vector3d new_second = center + (directions[i].normalized() * half_diagonal);

        this->children.emplace_back(
            center,
            new_second,
            limit,
            depth + 1,
            G,
            theta
        );
    }
}



// implementation of the private compute functions

double Node::_compute_total_mass()
{
    _total_mass = 0.0;
    
    if (is_leaf) {
        // If leaf node, sum mass from all planets
        for (const auto& planet : planets) {
            _total_mass += planet.mass;
        }
    } else {
        // If not a leaf, sum mass from all child nodes
        for (auto& child : children) {
            _total_mass += child.total_mass();
        }
    }
    
    _total_mass_flag = true;
    return _total_mass;
}

Eigen::Vector3d Node::_compute_com()
{
    _com = Eigen::Vector3d::Zero();
    double total_mass_val = total_mass();
    
    // If node is empty, return zero
    if (total_mass_val == 0.0) {
        _com_flag = true;
        return _com;
    }
    
    if (is_leaf) {
        // If leaf node, sum weighted positions from all planets
        for (const auto& planet : planets) {
            _com += planet.mass * planet.position;
        }
    } else {
        // If not a leaf, sum weighted center of mass from all child nodes
        for (auto& child : children) {
            _com += child.total_mass() * child.com();
        }
    }
    
    // Normalize by total mass
    _com /= total_mass_val;
    
    _com_flag = true;
    return _com;
}

Eigen::Vector3d Node::_compute_center_position()
{
    // The center position is simply the geometric center of the node's bounding box

    _center_position = (diag_one + diag_two) / 2.0;
    _center_position_flag = true;
    return _center_position;
}

double Node::_compute_expansion_coefficient()
{
    // The expansion coefficient is based on the size of the node's bounding box
    // Typically: half the side length of the cell
    Eigen::Vector3d extent = diag_one - diag_two;
    _expansion_coefficient = extent.norm() / 2.0;
    
    _expansion_coefficient_flag = true;
    return _expansion_coefficient;
}

Eigen::Matrix3d Node::_compute_Q()
{
    _Q = Eigen::Matrix3d::Zero();
    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();
    double total_mass_val = total_mass();
    
    if (this->is_leaf) {
        for (auto p : this->planets) {
            double m = p.mass;
            Eigen::Vector3d r = p.position - this->com();
            double r_mag = r.norm();
            _Q += m * (3 * r * r.transpose() - r_mag * r_mag * identity);
        }
    } else {
        for (auto & c : this->children) {
            double m = c.total_mass();
            Eigen::Vector3d r = c.com() - this->com();
            double r_mag = r.norm();
            _Q += m * (3 * r * r.transpose() - r_mag * r_mag * identity);
        }
    }
    
    // Normalize by total mass
    if (total_mass_val > 0.0) {
        _Q /= total_mass_val;
    }
    
    _Q_flag = true;
    return _Q;
}