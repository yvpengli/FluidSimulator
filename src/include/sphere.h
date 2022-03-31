#pragma once
#include <glm/glm.hpp>
#include <deque>


class Sphere
{
public:
    Sphere(glm::vec3 c, float r): center(c), radius(r) {

        vertices = {
            { 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1},
            {-1, 0, 0}, { 0, 0, 1}, { 0, 1, 0},
            { 1, 0, 0}, { 0, 0,-1}, { 0, 1, 0},
            {-1, 0, 0}, { 0, 1, 0}, { 0, 0,-1},
            
            { 1, 0, 0}, { 0, 0, 1}, { 0,-1, 0},
            {-1, 0, 0}, { 0,-1, 0}, { 0, 0, 1},
            { 1, 0, 0}, { 0,-1, 0}, { 0, 0,-1},
            {-1, 0, 0}, { 0, 0,-1}, { 0,-1, 0}
        };

        for (int i = 0; i < vertices.size(); i ++) {
            vertices[i] += center;
        }

        divide(1);
        normalize();
        add_normal_info();
    }

    void divide(int times) {

        while( times -- ) {
            int n = vertices.size();
            while (n) {
                glm::vec3 a = vertices.front();
                vertices.pop_front();
                glm::vec3 b = vertices.front();
                vertices.pop_front();
                glm::vec3 c = vertices.front();
                vertices.pop_front();

                glm::vec3 d = ( (a + b) / 2.f );
                glm::vec3 e = ( (b + c) / 2.f );
                glm::vec3 f = ( (c + a) / 2.f );

                // add new vertices
                vertices.push_back( a ); vertices.push_back( d ); vertices.push_back( f );
                vertices.push_back( b ); vertices.push_back( e ); vertices.push_back( d );
                vertices.push_back( c ); vertices.push_back( f ); vertices.push_back( e );
                vertices.push_back( d ); vertices.push_back( e ); vertices.push_back( f ); 
                
                n -= 3;
            }
        }
    }

    void normalize() {
        for (int i = 0; i < vertices.size(); i ++) {
            glm::vec3 pos = vertices[i];
            glm::vec3 dir = glm::normalize(pos - center);
            vertices[i] = center + dir * radius;
        }
    }

    void add_normal_info() {
        int n = vertices.size();
        for (int i = 0; i < n; i ++) {
            glm::vec3 pos = vertices.front();
            vertices.pop_front();

            glm::vec3 normal = glm::normalize(pos - center);
            vertices.push_back(pos);
            vertices.push_back(normal);
        }
    }


public:
    glm::vec3 center;
    float radius;
    std::deque<glm::vec3> vertices;
};