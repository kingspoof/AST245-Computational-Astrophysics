// basic c++ includes
#include <iostream>
#include <cmath>
#include <vector>


// includes of some of our helper classes
#include "Vector2D.hpp"
#include "SimulationResult.hpp"

// inclusion of the time integrators we want to use
#include "ExplicitEuler.hpp"
#include "RungeKutta.hpp"
#include "LeapFrog.hpp"
#include "SemiImplicitEuler.hpp"

double G = 1.0;
double M = 1.0;
double m = 1.0;

Vector2D rdotdot(const Vector2D& r){
    double magnitude = r.magnitude();

    //using magnitude*magnitude*magnitude is a lot faster than pow(magnitude, 3)
    double amplification = -G * M / (magnitude * magnitude * magnitude);

    return r * amplification;
}

double total_energy(const Vector2D& r, const Vector2D& v)
{
    double kinetic = 0.5 * m * v.magnitude() * v.magnitude();
    double potential = - G * M * m / r.magnitude();

    return kinetic + potential;
}

double angular_momentum(const Vector2D& r, const Vector2D& v)
{
    // in 2D the angular momentum is a scalar
    return (r.x * v.y - r.y * v.x);
}




int main() {
    // create instances of our integrators
    ExplicitEuler explicit_euler(rdotdot, total_energy, angular_momentum);
    RungeKutta runge_kutta(rdotdot, total_energy, angular_momentum);
    LeapFrog leap_frog(rdotdot, total_energy, angular_momentum);
    SemiImplicitEuler semi_implicit_euler(rdotdot, total_energy, angular_momentum);
    

    // create our initial position
    int max_iter = 10000;
    double eccentricity = 1;

    Vector2D initial_position(1.0, 0.0);
    Vector2D initial_velocity(0.0, std::sqrt(1 + eccentricity));

    // define some timesteps, to use
    std::vector<double> time_steps = {1, 0.1, 0.01, 0.001};

    //perform the simulations
    for (const auto & dt : time_steps)
    {
        std::cout << "\nSimulating method with time step " << dt << "\n";

        std::cout << "  Using Euler Integration";
        explicit_euler.integrate(initial_position, eccentricity, (dt * max_iter), dt, m).export_to_file("explicit_euler_", "Explicit Euler");
        std::cout << " - complete\n";

        std::cout << "  Using RK2 Integration";
        runge_kutta.integrate(2, initial_position, eccentricity, (dt * max_iter), dt, m).export_to_file("runge_kutta_2_", "Runge Kutta 2");
        std::cout << " - complete\n";

        std::cout << "  Using RK4 Integration";
        runge_kutta.integrate(4, initial_position, eccentricity, (dt * max_iter), dt, m).export_to_file("runge_kutta_4_", "Runge Kutta 4");
        std::cout << " - complete\n";

        std::cout << "  Using LeapFrog Integration";
        leap_frog.integrate(initial_position, eccentricity, (dt * max_iter), dt, m).export_to_file("leap_frog_", "LeapFrog");
        std::cout << " - complete\n";

        std::cout << "  Using SemiImplicitEuler Integration";
        semi_implicit_euler.integrate(initial_position, eccentricity, (dt * max_iter), dt, m).export_to_file("semi_implicit_euler_", "SemiImplicitEuler");
        std::cout << " - complete\n";

    }

    return 0;
}