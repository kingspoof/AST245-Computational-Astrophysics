#ifndef FILEREADER_hpp
#define FILEREADER_hpp

#include <iostream>
#include <Eigen/Dense>
#include "Planet.hpp"
#include <vector>
#include <fstream>
#include <sstream>

class FileReader
{
private:
    std::string _filepath;

    std::vector<std::string> split(const std::string& line, const char& delimiter) const
    {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, delimiter)){
            fields.push_back(field);
        }

        return fields;
    }

public:
    FileReader(std::string filepath) : _filepath(filepath) {}

    std::vector<Planet> read_file() const
    {
        std::vector<Planet> planets;

        std::ifstream file(_filepath);
        if (!file.is_open())
        {
            std::cout << "Failed to open file: " << _filepath << "\n";
            return planets;
        }

        std::string line;
        while(std::getline(file, line)){
            auto row = split(line, '\t');

            // transform the row into a planet
            planets.push_back(
                Planet(
                    std::stod(row[1]),
                    std::stod(row[9]),
                    Eigen::Vector3d(
                        std::stod(row[2]),
                        std::stod(row[3]),
                        std::stod(row[4])
                    ),
                    Eigen::Vector3d(
                        std::stod(row[5]),
                        std::stod(row[6]),
                        std::stod(row[7])
                    )
                )
            );
        }

        return planets;
    }
};

#endif