#pragma once
#include <vector>
#include <queue>
#include <glm/glm.hpp>


class Sphere
{
public:
    Sphere(glm::vec3 c, float r): center(c), radius(r) {


    }

    std::vector<glm::vec3> divide(std::vector<glm::vec3> &vertices, int times) {
        std::queue<glm::vec3> q;
        for ( int i = 0; i < vertices.size(); i ++)
        {
            q.push(vertices[i]);
        }
        
        while( --times ) {
            int n = q.size();
            while (n) {
                glm::vec3 a = q.front();
                q.pop();
                glm::vec3 b = q.front();
                
                n -= 3;
            }
        }

    }


public:
    glm::vec3 center;
    float radius;
    std::vector<glm::vec3> vertices;
};