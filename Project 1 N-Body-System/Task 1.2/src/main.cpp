#include <iostream>
#include <chrono> 
#include "FileReader.hpp"
#include "Universe.hpp"
#include "ResultExporter.hpp"


int main(){
  std::cout << "Hello World\n";

  FileReader reader("data/data.txt"); 
  std::vector<Planet> data = reader.read_file();

  Universe universe(data);

  // compute the interparticle separation
  auto interparticle_separation = universe.calculate_mean_interparticle_separation();
  std::cout << interparticle_separation << "\n";



  // compute the force
  auto forces_0001 = universe.calculate_direct_nbody_forces(0.001, 1.0);
  std::cout << "finished computing forces_0001" << "\n";
  auto forces_001 = universe.calculate_direct_nbody_forces(0.01, 1.0);
  std::cout << "finished computing forces_001" << "\n";
  auto forces_01 = universe.calculate_direct_nbody_forces(0.1, 1.0);
  std::cout << "finished computing forces_01" << "\n";
  auto forces_1 = universe.calculate_direct_nbody_forces(1, 1.0);
  std::cout << "finished computing forces_1" << "\n";
  auto forces_10 = universe.calculate_direct_nbody_forces(10, 1.0);
  std::cout << "finished computing forces_10" << "\n";
  auto forces_100 = universe.calculate_direct_nbody_forces(100, 1.0);
  std::cout << "finished computing forces_100" << "\n";


  ResultExporter::export_force_computation(universe._positions, forces_0001, "output/data/direct_force_0001_softening.txt");
  ResultExporter::export_force_computation(universe._positions, forces_001, "output/data/direct_force_001_softening.txt");
  ResultExporter::export_force_computation(universe._positions, forces_01, "output/data/direct_force_01_softening.txt");
  ResultExporter::export_force_computation(universe._positions, forces_1, "output/data/direct_force_1_softening.txt");
  ResultExporter::export_force_computation(universe._positions, forces_10, "output/data/direct_force_10_softening.txt");
  ResultExporter::export_force_computation(universe._positions, forces_100, "output/data/direct_force_100_softening.txt");
}