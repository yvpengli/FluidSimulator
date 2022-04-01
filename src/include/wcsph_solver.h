#pragma once
#include "sph_base.h"

class WCSPHSolver {
public:

    WCSPHSolver(ParticleSystem& particle_system) : ps(particle_system) {

        mass = ps.m_V * density_ref;
        d_velocity = std::vector<glm::vec3>(ps.particle_num, {0.f, 0.f, 0.f});
    }

    void compute_non_pressure_forces() {
        for (int i = 0; i < ps.particle_num; i ++) {
            // gravity
            glm::vec3 d_v = glm::vec3(0.0);
            d_v[1] = g;


            d_velocity[i] += d_v;
        }
    }

    void advert() {
        // Symplectic Euler
        for (int i = 0; i < ps.particle_num; i ++) {
            ps.v[i] += dt * d_velocity[i];
            ps.x[i] += dt * ps.v[i];
        }

        // std::cout << "first point location: " << ps.position_and_normal[0][0] << " " << ps.position_and_normal[0][1] << " " << ps.position_and_normal[0][2] << std::endl;
    }

    void substep() {
        compute_non_pressure_forces();
        advert();
    }

    void step() {
        // update x[] and v[] by forces
        substep();
        // boundary test
        ps.enfoce_boundary();
    }

public:
    ParticleSystem& ps;
    float g = -9.8f;
    float viscosity = 0.05f;
    float density_ref = 1000.0f;
    float mass;
    float dt = 2e-4;

    float exponent = 7.0f;
    float stiffness = 50.0f;
    std::vector<glm::vec3> d_velocity;
};