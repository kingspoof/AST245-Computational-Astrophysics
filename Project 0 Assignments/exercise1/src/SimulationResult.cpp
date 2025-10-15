#include "SimulationResult.hpp"
#include <fstream>
#include <iostream>




SimulationResult::SimulationResult(const Vector2D & initial_position, const Vector2D & initial_velocity, double eccentricity, double dt, double initial_energy, double initial_angular_momentum)
{
    this->time_step = dt;
    this->eccentricity = eccentricity;
    this->initial_position = initial_position;
    this->initial_velocity = initial_velocity;

    this->positions.push_back(initial_position);
    this->velocities.push_back(initial_velocity);

    this->energies.push_back(initial_energy);
    this->angular_moments.push_back(initial_angular_momentum);

}

void SimulationResult::export_to_file(const std::string &filename, const std::string &method_name) const
{
    // add some basic information to the file
    auto file_name = filename + std::to_string(time_step) + ".txt";
    std::string output = "";
    output += std::to_string(time_step) + "\n";
    output += method_name + "\n";

    // append the positions to the output string
    std::string pos_str = "";
    for (const Vector2D &pos : positions)
    {
        pos_str += std::to_string(pos.x) + ";" + std::to_string(pos.y) + " ";
    }
    output += pos_str + "\n";

    // append the velocities to the output string
    std::string vel_str = "";
    for (const Vector2D &vel : velocities)
    {
        vel_str += std::to_string(vel.x) + ";" + std::to_string(vel.y) + " ";
    }
    output += vel_str + "\n";

    // append the energies to the output string
    std::string energy_str = "";
    for (const auto &e : energies)
    {
        energy_str += std::to_string(e) + ";";
    }
    output += energy_str + "\n";

    // append the angular momentum to the output string
    std::string angular_moment_str = "";
    for(const auto & l : angular_moments)
    {
        angular_moment_str += std::to_string(l) + ";";
    }
    output += angular_moment_str + "\n";


    std::ofstream file("results/" + file_name);
    if (file.is_open())
    {
        file << output;
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open file " << file_name << " for writing.\n";
    }
}
