#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "sphere.h"
#include <map>
#include <tuple>

class ParticleSystem {

public:
    ParticleSystem( float l ) : system_length(l) {
        // for simplicity the Particle System is a cube
        // coords of system is [0, 0, 0] to [system_length, system_length, system_length]
        screen_to_world_ratio = 400.0f;     // because screen size is (800, 800)
        boundary = system_length / screen_to_world_ratio;

        particle_radius = 0.05f;
        particle_diameter = 2 * particle_radius;
        particle_num = 0;
        m_V = 0.8 * pow(particle_diameter, 3.0f);
        support_radius = particle_radius * 4.0;

        grid_size = support_radius;

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
        for (float x = lower_corner[0]; x < upper_corner[0]; x += particle_diameter) {
            for (float y = lower_corner[1]; y < upper_corner[1]; y += particle_diameter) {
                for (float z = lower_corner[2]; z < upper_corner[2]; z += particle_diameter) {
                    this->x.push_back(glm::vec3(x, y, z));
                    this->v.push_back(glm::vec3(0.0f));
                    this->particle_num ++;
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

        // std::cout << "first point location: " << position_and_normal[0][0] << " " << position_and_normal[0][1] << " " << position_and_normal[0][2] << std::endl;
        // std::cout << "x.size() = " << x.size() << std::endl;
        // std::cout << "position and normal size = " << position_and_normal.size() << std::endl;
    }

    /**
     * enfore_boundary(): test if the ball hit boundary 
     * if so, use simulate_collisions() to handle collision
     */
    void enfoce_boundary() {
        // TODO: hw frame has an concept padding
        for (int i = 0; i < particle_num; i ++) {
            glm::vec3 pos = x[i];
            // if pos = (-1, 2, -1), it need to move (1, 0, 1);
            glm::vec3 tmp = glm::vec3(0.f);
            if ( pos[0] < 0 || pos[1] < 0 || pos[2] < 0) {
                tmp = pos;
            } else if ( pos[0] > boundary || pos[1] > boundary || pos[2] > boundary) {
                tmp = glm::vec3(boundary) - pos;
            }
            glm::vec3 move = glm::vec3( tmp[0] < 0 ? -tmp[0] : 0.f,
                                        tmp[1] < 0 ? -tmp[1] : 0.f,
                                        tmp[2] < 0 ? -tmp[2] : 0.f );
            if ( move[0] != 0 || move[1] != 0 || move[2] != 0 ) {
                simulate_collisions(i, move);
            }
        }
    }

    void simulate_collisions(int p_i, glm::vec3 move) {
        // Collision factor, assume roughly (1-c_f)*velocity loss after collision
        float c_f = 0.8f;
        x[p_i] += move;
        glm::vec3 dir = glm::normalize(move);
        v[p_i] -= (1.0f + c_f) * glm::dot(v[p_i], dir) * dir;
    }

    glm::ivec3 pos_to_index( int p_i ) {
        int x_offset = static_cast<int>( x[p_i][0] / grid_size );
        int y_offset = static_cast<int>( x[p_i][1] / grid_size );
        int z_offset = static_cast<int>( x[p_i][2] / grid_size );
        return glm::ivec3(x_offset, y_offset, z_offset);
    }

    void initialize_grid() {
        grid.clear();
        for (int i = 0; i < particle_num; i ++) {
            glm::ivec3 index = pos_to_index(i);
            std::tuple<int, int, int> tp = {index[0], index[1], index[2]};
            grid[ tp ].push_back(i);
        }
    }

    void search_neighbors() {
        particle_neighbors.swap(std::vector<std::vector<int>>(particle_num));
        for (int i = 0; i < particle_num; i ++) {
            glm::ivec3 center = pos_to_index(i);
            for (int x_offset = -1; x_offset < 2; x_offset ++)
            for (int y_offset = -1; y_offset < 2; y_offset ++)
            for (int z_offset = -1; z_offset < 2; z_offset ++) {
                std::tuple<int, int, int> cell = {center[0] + x_offset, center[1] + y_offset, center[2] + z_offset};
                vector<int> pos_in_grid = grid[cell];
                for (int p_i : pos_in_grid) {
                    glm::vec3 pos = x[p_i];
                    float distance = glm::length( x[i] - pos );
                    if ( p_i != i && distance < support_radius ) {
                        particle_neighbors[i].push_back(p_i);
                    }
                }
            }
        }
    }


    void initialize_particle_system() {
        initialize_grid();
        search_neighbors();
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
    float m_V;
    float support_radius;

    // int particle_max_num;
    // int particle_max_num_per_cell;
    // int particle_max_num_neighbor;

    // Single Particle Properties
    std::vector<glm::vec3> x;       
    std::vector<glm::vec3> v;
    std::vector<float> density;
    std::vector<float> pressure;
    std::vector<std::vector<int>> particle_neighbors;

    // Grid related properties
    float grid_size;
    std::map<std::tuple<int, int, int>, std::vector<int>> grid; 

    // DataOutput
    std::vector<glm::vec3> position_and_normal;
};