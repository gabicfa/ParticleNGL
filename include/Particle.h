#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <ngl/Vec3.h>

struct Particle
{
    int lifetime = 0;
    ngl::Vec3 position;
    ngl::Vec3 colour;
    ngl::Vec3 direction={0.0f,1.0f,0.0f};
    float scale=0.01f;
};

#endif