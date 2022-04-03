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

    float cubic_kernel( float r_norm ) {
        float res = 0.0f;
        float h = ps.support_radius;

        float k = 8 / (PI * powf( h , 3.0f) );
        float q = r_norm / h;
        if ( q <= 1.0 ) {
            if ( q <= 0.5 ) {
                float q2 = q * q;
                float q3 = q2 * q;
                res = k * (6.0f * q3 - 6.0f * q2 + 1);
            } else {
                res= k * 2 * powf(1 - q, 3.0f);
            }
        }
        return res;
    }

    glm::vec3 cubic_kernel_derivative(glm::vec3 r) {
        float h = ps.support_radius;
        float k = 8.0f / PI;
        k = 6.0f * k / powf( h, 3.0f );
        float r_norm = glm::length(r);
        float q = r_norm / h;
        glm::vec3 res = glm::vec3(0.0f);

        if ( r_norm > 1e-5 && q <= 1.0f ) {
            glm::vec3 grad_q = r / (r_norm * h);
            if (q <= 0.5) {
                res = k * q * (3.0f * q - 2.0f) * grad_q;
            } else {
                float factor = 1.0f - q;
                res = k * (-factor * factor) * grad_q;
            }
        }
        return res;
    }

    glm::vec3 pressure_force(int p_i, int p_j, glm::vec3 r) {
        glm::vec3 res = -density_ref * ps.m_V * ( powf( ps.pressure[p_i] / ps.density[p_i], 2.0f)
                + powf(ps.pressure[p_j] / ps.density[p_j], 2.0f) ) 
                * cubic_kernel_derivative(r);
        return res;
    }

    void compute_density() {
        ps.density.swap(std::vector<float>(ps.particle_num, 0.0f));
        for (int i = 0; i < ps.particle_num; i ++) {
            glm::vec3 x_i = ps.x[i];
            for ( int p_j : ps.particle_neighbors[i] ) {
                glm::vec3 x_j = ps.x[p_j];
                ps.density[i] += ps.m_V * cubic_kernel(glm::length(x_i - x_j));
            }
            ps.density[i] *= density_ref;
        }
    }

    void compute_pressure_forces() {
        ps.pressure.swap(std::vector<float>(ps.particle_num, 0.0f));
        for (int i = 0; i < ps.particle_num; i ++) {
            ps.density[i] = max(ps.density[i], density_ref);
            ps.pressure[i] = stiffness * ( powf( ps.density[i] / density_ref, exponent) - 1.0f );
        }

        for (int i = 0; i < ps.particle_num; i ++) {
            glm::vec3 x_i = ps.x[i];
            glm::vec3 d_v = glm::vec3(0.0f);
            for ( int p_j : ps.particle_neighbors[i] ) {
                glm::vec3 x_j = ps.x[p_j];
                d_v += pressure_force(i, p_j, x_i - x_j);
            }
            d_velocity[i] += d_v;
        }

    }

    void substep() {
        compute_density();
        compute_non_pressure_forces();
        compute_pressure_forces();
        advert();
    }

    void step() {
        ps.initialize_particle_system();
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
    float stiffness = 50.0f;
    float exponent = 7.0f;
    float mass;
    float dt = 2e-4;
    const float PI = 3.14159265358979323846;

    std::vector<glm::vec3> d_velocity;
};