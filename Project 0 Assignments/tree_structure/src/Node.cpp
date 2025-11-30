#include "Node.hpp"
#include <Eigen/Dense>
#include "Planet.hpp"
#include <cmath>
#include <iostream>

using Eigen::Matrix2d;
using Eigen::Vector2d;
using Eigen::Vector4d;

/***
 * Function to compute the mass of the entire node
 * For nodes, we sum over all child nodes
 * For leaf, we compute the sum over all planets
 */
double Node::compute_mass()
{
    double total_mass = 0.0;
    if (this->is_leaf)
        for (auto &planet : this->planets)
            total_mass += planet.get_mass();
    else
        for (auto &child : this->child_nodes)
            total_mass += child.get_mass();

    return total_mass;
}

Vector2d Node::compute_com_position()
{
    Vector2d com_position(0, 0);
    if (this->is_leaf)
        for (auto &p : this->planets)
            com_position += p.get_mass() * p.get_position();
    else
        for (auto &c : this->child_nodes)
            com_position += c.get_mass() * c.get_com_position();

    return com_position / this->compute_mass();
}

/***
 * Since we use a grid approach, where we divide everything
 * into equal subgrids the expansion coefficient ot the "size"
 * is simply given by the length
 */
double Node::compute_expansion_coeff()
{
    // expansion coeff should be positive and represent half the cell size
    // hard coded into this is the sqrt(2), therefore we get the "radius" of the cell and not just half the length
    return 0.5 * 1.4142135624 * std::abs(this->top_left[0] - this->bottom_right[0]);
}

Matrix2d Node::compute_Q()
{
    Matrix2d q = Matrix2d::Zero();
    Matrix2d identity = Matrix2d::Identity();
    if (this->is_leaf)
        for (auto p : this->planets)
        {
            double m = p.get_mass();
            Vector2d r = p.get_position() - this->get_com_position();
            double r_mag = r.norm();

            q += m * (3 * r * r.transpose() - r_mag * r_mag * identity);
        }
    else
        for (auto c : this->child_nodes)
        {
            double m = c.get_mass();
            Vector2d r = c.get_com_position() - this->get_com_position();
            double r_mag = r.norm();

            q += m * (3 * r * r.transpose() - r_mag * r_mag * identity);
        }
    return q;
}

void Node::subdivide(std::vector<Planet> &plan)
{
    this->planets = plan; // is this a reference or a copy?

    // firstly, we check weather we go down one more level
    //if (this->planets.size() <= this->limit)

    if(this->depth >= this->limit)
    {
        // if we are @ depth then we don't go any further
        this->is_leaf = true;
        return;
    }
    else
    {
        this->is_leaf = false;
        // we need to subdivide the planets into 4 quadrants
        std::vector<Planet> q1, q2, q3, q4;

        Vector2d centre = 0.5 * (this->top_left + this->bottom_right);
        double centre_x = centre[0];
        double centre_y = centre[1];

        // next we generate our sub_"grids"
        // this has to be a single for loop
        for (auto &p : this->planets)
        {

            // A NOTE:
            // this can lead to problems if a planet is exactly on the centre lines
            // but since we are dealing with a) floating point number and b) real life
            // this case is very unlikely to happen -> so we ignore it
            bool left = p.get_position()[0] <= centre_x;
            bool bottom = p.get_position()[1] <= centre_y;

            // assigning the following quadrants to the position
            // q1 = left + top
            // q2 = right + top
            // q3 = left + bottom
            // q4 = right + bottom

            if (left && !bottom)
                q1.push_back(p);
            else if (!left && !bottom)
                q2.push_back(p);
            else if (left && bottom)
                q3.push_back(p);
            else
                q4.push_back(p);
        }
        // create the four qudrants
        // we only create a child node if there is a planet in that quadrant
        if (!q1.empty())
        {
            this->child_nodes.emplace_back(
                this->top_left,
                centre,
                this->limit,
                this->depth + 1,
                this->G,
                this->theta
            );
            this->child_nodes.back().subdivide(q1);
        }
        if (!q2.empty())
        {
            this->child_nodes.emplace_back(
                Vector2d(centre_x, this->top_left[1]),
                Vector2d(this->bottom_right[0], centre_y),
                this->limit,
                this->depth + 1,
                this->G,
                this->theta
            );
            this->child_nodes.back().subdivide(q2);
        }
        if (!q3.empty())
        {
            this->child_nodes.emplace_back(
                Vector2d(this->top_left[0], centre_y),
                Vector2d(centre_x, this->bottom_right[1]),
                this->limit,
                this->depth + 1,
                this->G,
                this->theta
            );
            this->child_nodes.back().subdivide(q3);
        }
        if (!q4.empty())
        {
            this->child_nodes.emplace_back(
                centre,
                this->bottom_right,
                this->limit,
                this->depth + 1,
                this->G,
                this->theta
            );
            this->child_nodes.back().subdivide(q4);
        }
    }
}

Vector2d Node::acceleration(const Planet& p)
{
    // first we need to check how far away we are
    Vector2d y = p.get_position() - this->get_com_position();
    double y_mag = y.norm();

    Vector2d force(0, 0);

    // if this is a leaf, then we fell completely down the tree
    // to only the node containing the planet itself
    if (this->is_leaf)
    {
        // direct calculation of the attraction
        for (Planet& other : planets)
        {
            // compute the attraction directly
            Vector2d r = p.get_position() - other.get_position();
            double r_mag = r.norm();
            if (r_mag > 0.0) // this excludes the attraction to one self
                force += -G * other.get_mass() * r / std::pow(r_mag, 3);
        }
    }
    else
    {
        // here we check weather we can use the expansion
        if ((this->get_expansion_coeff() / y.norm()) < this->theta)
        {
            // monopol term
            Vector2d F_mono = -G * this->get_mass() * y / (pow(y_mag, 3));

            // quadropol term
            Vector2d Qy = this->get_Q() * y;
            double yQy = y.transpose() * Qy;
            Vector2d F_quad = -G * (Qy / pow(y_mag, 5) - y * ((5.0/2.0) * yQy / pow(y_mag, 7)));

            force = F_mono + F_quad;
        }
        else
        {
            // now here we have to open the node
            for(Node & child : this->child_nodes){
                force += child.acceleration(p);
            }
        }
    }

    // 'force' already represents the acceleration contribution (G * m_other * r / r^3)
    // so do NOT divide by the particle mass here. DirectSolver returns acceleration
    // with the same convention. Returning force/p.get_mass() produced a systematic
    // scaling of the result by 1/m_p and thus made the relative error independent
    // of the tree leaf capacity.
    return force;
}

int Node::get_max_depth()
{
    int current_depth = this->depth;
    for(auto & c : this->child_nodes){
        if(c.get_max_depth() > current_depth)
            current_depth = c.get_max_depth();
    }
    return current_depth;
}

int Node::get_max_planets_in_node()
{
    if(this->is_leaf) return this->planets.size();
    else{
        int current_num = 0;
        for(auto & c : this->child_nodes){
            int node_value = c.get_max_planets_in_node();
            if(node_value > current_num)
                current_num = node_value;
        }
        return current_num;
    }
}