// basic c++ includes
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

// includes of some of our helper classes
#include "Vector2D.hpp"
#include "SimulationResult.hpp"
#include "Phase.hpp"

// inclusion of the time integrators we want to use
#include "ExplicitEuler.hpp"
#include "RungeKutta.hpp"
#include "LeapFrog.hpp"
#include "SemiImplicitEuler.hpp"

double G = 1.0;
double M = 1.0;
double m = 1.0;

Phase rdotdot(const Phase &current_phase)
{

    double magnitude = current_phase.position.magnitude();
    double amplification = -G * M / (magnitude * magnitude * magnitude);

    return Phase(
        current_phase.velocity,
        current_phase.position * amplification);
}

double total_energy(const Vector2D &r, const Vector2D &v)
{
    double kinetic = 0.5 * m * v.magnitude() * v.magnitude();
    double potential = -G * M * m / r.magnitude();

    return kinetic + potential;
}

double angular_momentum(const Vector2D &r, const Vector2D &v)
{
    // in 2D the angular momentum is a scalar
    return (r.x * v.y - r.y * v.x);
}

int main()
{
    // create instances of our integrators
    ExplicitEuler explicit_euler(rdotdot, total_energy, angular_momentum);
    RungeKutta runge_kutta(rdotdot, total_energy, angular_momentum);
    LeapFrog leap_frog(rdotdot, total_energy, angular_momentum);
    SemiImplicitEuler semi_implicit_euler(rdotdot, total_energy, angular_momentum);

    // create our initial position
    int t_max = 1000;
    double eccentricity = 0.5;

    Vector2D initial_position(1.0, 0.0);
    Vector2D initial_velocity(0.0, std::sqrt(1 + eccentricity));

    // define some timesteps, to use
    std::vector<double> time_steps = {1, 0.1, 0.01, 0.001};

    // perform the simulations
    for (const auto &dt : time_steps)
    {
        std::cout << "\nSimulating method with time step " << dt << "\n";

        std::cout << "  Using Euler Integration";
        auto start_euler_e = std::chrono::high_resolution_clock::now();
        auto result_euler_e = explicit_euler.integrate(initial_position, eccentricity, t_max, dt, m);
        auto duration_euler_e = std::chrono::high_resolution_clock::now() - start_euler_e;
        std::cout << " - complete (" << std::chrono::duration<double>(duration_euler_e).count() << " s)\n";
        result_euler_e.export_to_file("explicit_euler_", "Explicit Euler");

        std::cout << "  Using RK2 Integration";
        auto start_rk2 = std::chrono::high_resolution_clock::now();
        auto result_rk2 = runge_kutta.integrate(2, initial_position, eccentricity, t_max, dt, m);
        auto duration_rk2 = std::chrono::high_resolution_clock::now() - start_rk2;
        std::cout << " - complete (" << std::chrono::duration<double>(duration_rk2).count() << " s)\n";
        result_rk2.export_to_file("runge_kutta_2_", "Runge Kutta 2");

        std::cout << "  Using RK4 Integration";
        auto start_rk4 = std::chrono::high_resolution_clock::now();
        auto result_rk4 = runge_kutta.integrate(4, initial_position, eccentricity, t_max, dt, m);
        auto duration_rk4 = std::chrono::high_resolution_clock::now() - start_rk4;
        std::cout << " - complete (" << std::chrono::duration<double>(duration_rk4).count() << " s)\n";
        result_rk4.export_to_file("runge_kutta_4_", "Runge Kutta 4");

        std::cout << "  Using LeapFrog Integration";
        auto start_leap_frog = std::chrono::high_resolution_clock::now();
        auto result_leap_frog = leap_frog.integrate(initial_position, eccentricity, t_max, dt, m);
        auto duration_leap_frog = std::chrono::high_resolution_clock::now() - start_leap_frog;
        std::cout << " - complete (" << std::chrono::duration<double>(duration_leap_frog).count() << " s)\n";
        result_leap_frog.export_to_file("leap_frog_", "LeapFrog");

        std::cout << "  Using SemiImplicitEuler Integration";
        auto start_se = std::chrono::high_resolution_clock::now();
        auto result_se = semi_implicit_euler.integrate(initial_position, eccentricity, t_max, dt, m);
        auto duration_se = std::chrono::high_resolution_clock::now() - start_se;
        std::cout << " - complete (" << std::chrono::duration<double>(duration_se).count() << " s)\n";
        result_leap_frog.export_to_file("semi_implicit_euler_", "Semi Implicit Euler");
    }

    return 0;
}