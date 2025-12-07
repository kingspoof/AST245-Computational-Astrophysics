#include <iostream>
#include <chrono> 
#include <Eigen/Dense>
#include <Eigen/Core>
#include "FileReader.hpp"
#include "Universe.hpp"
#include "ResultExporter.hpp"
#include "Node.hpp"



int main(){
  std::cout << "Hello World\n";

  FileReader reader("data/data.txt"); 
  std::vector<Planet> data = reader.read_file();

  // we know that all points lie withtin a cube of side length 1000 centered at the origin
  // this has to be done better later but actually we don't care :) 
  Eigen::Vector3d diag1(-1000, -1000, -1000);
  Eigen::Vector3d diag2( 1000,  1000,  1000);

  Node root(
    diag1, 
    diag2,
    10, // limit
    0,  // depth
    1,
    0.5 // theta
  );

  root.subdivide(data);

}