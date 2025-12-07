#ifndef NODE_HPP
#define NODE_HPP


#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>
#include "Planet.hpp"

class Node
{
public:
    // create the function to access the below values
    double total_mass() {return _total_mass_flag ? _total_mass : _compute_total_mass();}
    Eigen::Vector3d com() {return _com_flag ? _com : _compute_com();}
    Eigen::Vector3d center_position() {return _center_position_flag ? _center_position : _compute_center_position();}
    double expansion_coefficient() {return _expansion_coefficient_flag ? _expansion_coefficient : _compute_expansion_coefficient();}
    Eigen::Matrix3d Q() {return _Q_flag ? _Q : _compute_Q();}
    
    /**
     * Subdivides the node into 8 child nodes
     * \param planets The planets contained in this node
     */
    void subdivide(std::vector<Planet> & planets);

    /**
     * Computes the acceleration on a planet due to all planets in this node
     * \param p The planet
     */
    Eigen::Vector3d compute_acceleration(const Planet & p);

    /**
     * Constructor for the Node class
     * \param min_ The minimum corner of the node
     * \param max_ The maximum corner of the node
     * \param limit_ The maximum number of planets in a leaf node
     * \param depth_ The depth of the node in the tree
     * \param G_ The gravitational constant
     * \param theta_ The opening angle for the Barnes-Hut criterion
     */
    Node(Eigen::Vector3d dia1, Eigen::Vector3d dia2, int limit_, int depth_, double G_, double theta_)
        : diag_one(dia1), diag_two(dia2), limit(limit_), depth(depth_), is_leaf(true), G(G_), theta(theta_)
    {}

private:
    // better internal properties
    Eigen::Vector3d diag_one;
    Eigen::Vector3d diag_two;
    int limit;
    int depth;

    bool is_leaf;

    double G;
    double theta;

    std::vector<Planet> planets;
    std::vector<Node> children;


    // internal properties
    double _total_mass;
    Eigen::Vector3d _com;
    Eigen::Vector3d _center_position;
    double _expansion_coefficient;
    Eigen::Matrix3d _Q;


    // create flags for once computed values
    bool _total_mass_flag = false;
    bool _com_flag = false;
    bool _center_position_flag = false;
    bool _expansion_coefficient_flag = false;
    bool _Q_flag = false;


    // create the functions, which are needed to compute these values
    double _compute_total_mass();
    Eigen::Vector3d _compute_com();
    Eigen::Vector3d _compute_center_position();
    double _compute_expansion_coefficient();
    Eigen::Matrix3d _compute_Q();
};

#endif