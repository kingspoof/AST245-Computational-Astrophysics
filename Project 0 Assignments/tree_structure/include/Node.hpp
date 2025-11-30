#ifndef NODE_HPP
#define NODE_HPP

#include <Eigen/Dense>
#include <vector>
#include "Planet.hpp"

using Eigen::Matrix2d;
using Eigen::Vector2d;
using Eigen::Vector4d;

class Node
{
public:
/***
 * The node function
 * Experimentally found perfect values:
 * 10'000 -> depth of 6
 * 100'000 -> depth of 8
 * 1'000'000 -> depth of 8
 * Then it maybe stays at 8??? 
 */
    Node(const Vector2d & top_left, const Vector2d & bottom_right, int lim=0, int d=0, double G=1, double theta=0.1)
    {
        this->top_left = top_left;
        this->bottom_right = bottom_right;
        this->limit = lim;
        this->depth = d;
        this->G = G;
        this->theta = theta;

        // set the flags for initialisation
        this->mass_flag = false;
        this->com_flag = false;
        this->expansion_flag = false;
        this->Q_flag = false;
    }


    /***
     * Function to build the entire node structure recursively
     */
    void subdivide(std::vector<Planet>& planets);
    Vector2d acceleration(const Planet& p);
    int get_max_depth();
    int get_max_planets_in_node();



    // define the functions for the once properties
    double get_mass(){
        if(!mass_flag) {
            this->_mass = compute_mass();
            this->mass_flag = true;
        }
        return _mass;
    }
    Vector2d get_com_position(){
        if(!com_flag){
            this->_com_position = compute_com_position();
            this->com_flag = true;
        }
        return _com_position;
    } 
    double get_expansion_coeff(){
        if(!expansion_flag){
            this->_expansion_coeff = compute_expansion_coeff();
            this->expansion_flag = true;
        }
        return _expansion_coeff;
    }
    Matrix2d get_Q(){
        if(!Q_flag){
            this->_Q = compute_Q();
            this->Q_flag = true;
        }
        return _Q;
    }
    


private:
    // bounds information is generated from two points
    // the top left points as well as the bottom right point
    Vector2d top_left;
    Vector2d bottom_right;


    // specific information for the treen
    int limit; // defines the minimum number of planets in a node
    int depth; // how "deep" are we in the tree

    // a leaf node is the bottom most node in the tree
    bool is_leaf;

    double G;
    double theta;

    std::vector<Planet> planets;
    std::vector<Node> child_nodes; 



    // define the properties to be only computed once
    double _mass;
    Vector2d _com_position;
    double _expansion_coeff;
    Matrix2d _Q;

    // define the functions which will calculate these properties
    double compute_mass();
    Vector2d compute_com_position();
    double compute_expansion_coeff();
    Matrix2d compute_Q();

    // define the flags for the above properties
    bool mass_flag;
    bool com_flag;
    bool expansion_flag;
    bool Q_flag;

};

#endif // NODE_HPP