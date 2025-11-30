#ifndef RESULTEXPORTER_hpp
#define RESULTEXPORTER_hpp


#include <iostream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>

class ResultExporter
{
public:
    static void export_force_computation(std::vector<Eigen::Vector3d> positions, std::vector<Eigen::Vector3d> forces, std::string filename)
    {
        // quickly check that both vectors have the same length
        if(positions.size() != forces.size())
        {
            std::cout << "export for file " << filename << " did not work, as positions and forces have not same dimension";
            std::cout << "positions has dimension " << positions.size() << ", forces has dimension " << forces.size() << "\n";
            return;
        }


        // here we can actually do the export
        std::ofstream file(filename);
        if(file.is_open())
        {
            for(int i = 0; i < positions.size(); i++)
            {
                file << positions[i](0) << "," << positions[i](1) << "," << positions[i](2) << ",";
                file << forces[i](0) << "," << forces[i](1) << "," << forces[i](2) << "\n";
            }
        }

        file.close();
    }
};





#endif //RESULTEXPORTER_hpp