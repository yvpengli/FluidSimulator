#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "sphere.h"

class ParticleSystem {

public:
    ParticleSystem( float l ) : system_length(l) {
        // for simplicity the Particle System is a cube
        // coords of system is [0, 0, 0] to [system_length, system_length, system_length]
        boundary = system_length / screen_to_world_ratio;
        screen_to_world_ratio = 400.0f;     // because screen size is (800, 800)

        particle_radius = 0.03;
        particle_diameter = 2 * particle_radius;
    }

    void add_cube(glm::vec3 lower_corner, float cube_size) {
        // check out of boundary
        for (int i = 0; i < 3; i ++) {
            if (lower_corner[i] + cube_size > system_length) {
                std::cout << "ParticleSystem.add_cube() error, out of boundary" << std::endl;
                return;
            }
        }
        lower_corner /= screen_to_world_ratio;
        cube_size /= screen_to_world_ratio;

        glm::vec3 upper_corner = lower_corner + glm::vec3(cube_size, cube_size, cube_size);
        for (float x = lower_corner[0] + particle_radius; x < upper_corner[0]; x += particle_diameter) {
            for (float y = lower_corner[1] + particle_radius; y < upper_corner[1]; y += particle_diameter) {
                for (float z = lower_corner[2] + particle_radius; z < upper_corner[2]; z += particle_diameter) {
                    this->x.push_back(glm::vec3(x, y, z));
                }
            }
        }

    }

    void create_balls() {
        // clear vector
        position_and_normal.swap(std::vector<glm::vec3>());
        for (int i = 0 / 2; i < x.size(); i ++) {
            Sphere ball(x[i], particle_radius);
            // std::cout << i << ".size() = " << ball.vertices.size() << std::endl;
            for ( int j = 0; j < ball.vertices.size(); j ++ )
                position_and_normal.push_back(ball.vertices[j]);            
        }

        // std::cout << "x.size() = " << x.size() << std::endl;
        // std::cout << "position and normal size = " << position_and_normal.size() << endl;
    }

public:
    float system_length;
    float screen_to_world_ratio;
    float boundary;

    // Particles Global Properties
    float particle_radius;
    float particle_diameter;
    int particle_max_num;
    int particle_num;
    // Single Particle Properties
    std::vector<glm::vec3> x;       
    std::vector<glm::vec3> v;

    // DataOutput
    std::vector<glm::vec3> position_and_normal;



};