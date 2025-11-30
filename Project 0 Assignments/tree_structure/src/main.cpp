#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "Planet.hpp"
#include "Node.hpp"
#include "DirectSolver.hpp"

using Eigen::MatrixXd;
using Eigen::Vector2d;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

/***
 * Generates a random double between 0 and 1
 */
double random_01()
{
  return rand() / double(RAND_MAX);
}

/***
 * Generates a list of planets with random position between 0 and 1000
 */
std::vector<Planet> generate_planets(int num)
{
  std::vector<Planet> planets;
  for (int i = 0; i < num; i++)
  {
    double mass = random_01() * 10.0 + 1.0;                        // mass between 1 and 10
    Vector2d position(random_01() * 1000.0, random_01() * 1000.0); // position in 1000x1000 box
    Vector2d velocity(0, 0);                                       // initial velocity zero

    planets.emplace_back(
        Planet(
            mass,
            position,
            velocity));
  }

  return planets;
}

void test_different_tree_depths()
{
  int num_of_planets = 10000;
  auto planets = generate_planets(num_of_planets);

  Vector2d top_left(0, 1000);
  Vector2d bottom_right(1000, 0);

  DirectSolver solver(1);

  std::cout << "" << "limit " << "" << "avg_deviation " << "" << "time " << "max depth " << "max_planets_in_leaf" << "\n";
  for (int limit = 1; limit < 21; limit++)
  {
    Node root(top_left, bottom_right, limit, 0, 1, 0.1);
    root.subdivide(planets);

    double deviation = 0.0;

    auto start = high_resolution_clock::now();
    int current = 0;
    for (const auto &p : planets)
    {
      current++;
      if (current % 1 == 0)
      {

        Vector2d acc_direct = solver.acceleration(p, planets);
        Vector2d acc_tree = root.acceleration(p);

        deviation += (acc_direct - acc_tree).norm() / acc_direct.norm();
      }
    }
    auto stop = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(stop - start).count();

    std::cout << "" << limit << " " << deviation / num_of_planets << " " << time << " " << root.get_max_depth() << " " << root.get_max_planets_in_node() << "\n";
  }
}

void test_different_opening_anlges()
{
  int num_planets = 10000;
  auto planets = generate_planets(num_planets);

  Vector2d top_left(0, 1000);
  Vector2d bottom_right(1000, 0);

  DirectSolver solver(1);

  for (double i = 1; i < 100; i++)
  {
    double t = i / 100;
    Node root(top_left, bottom_right, 8, 0, 1, t);
    root.subdivide(planets);

    double deviation = 0.0;
    auto start = high_resolution_clock::now();

    for (const auto &p : planets)
    {

      Vector2d acc_direct = solver.acceleration(p, planets);
      Vector2d acc_tree = root.acceleration(p);

      deviation += (acc_direct - acc_tree).norm() / acc_direct.norm();
    }
    auto stop = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(stop - start).count();

    std::cout << "[" << t << "," << deviation / num_planets << "," << time << "]," << "\n";
  }
}

int main()
{
  test_different_opening_anlges();
  return 0;
}