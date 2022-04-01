#pragma once
#include "particle_system.h"

class SPHBase {
public:
    SPHBase(ParticleSystem particle_system) : ps(particle_system) {
        mass = ps.m_V * density_ref;

    }




public:
    ParticleSystem& ps;
    float g = -9.8f;
    float viscosity = 0.05f;
    float density_ref = 1000.0f;
    float mass;
    float dt = 2e-4;
};