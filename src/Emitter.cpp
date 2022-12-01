#include "Emitter.h"
#include <ngl/Random.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>

ngl::Vec3 randomVectorOnSphere(float _radius=1)
{
    float phi = ngl::Random::randomPositiveNumber(static_cast<float>(M_PI * 2.0f));
    float costheta = ngl::Random::randomNumber();
    float u =ngl::Random::randomPositiveNumber();
    float theta = acos(costheta);
    float r = _radius * std::cbrt(u);
    return ngl::Vec3(r * sin(theta) * cos(phi),
                                        r * sin(theta) * sin(phi),
                                        r * cos(theta)
                                    );
}

void Emitter::createParticle(Particle &io_p)
{
    ngl::Vec3 emitDir(0,10.0f,0);
    io_p.position.set(0.0f,0.0f,0.0f);
    io_p.direction = emitDir * ngl::Random::randomPositiveNumber()+
    randomVectorOnSphere()*10;
    io_p.direction.m_y=std::abs(io_p.direction.m_y);
    io_p.colour=ngl::Random::getRandomColour3();
}

Emitter::Emitter(size_t _numParticles)
{
    m_particles.resize(_numParticles);
    for(auto &p : m_particles)
    {
        createParticle(p);
    }
    m_vao=ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_POINTS);
}

size_t Emitter::getNumParticles() const
{
    return m_particles.size();
}

void Emitter::update()
{
    //std::cout<<"update\n";
    float _dt=0.01f;
    ngl::Vec3 gravity(0,-9.81f,0);
    for(auto &p : m_particles)
    {
        p.direction += gravity * _dt * 0.5f;
        p.position += p.direction * _dt;
        p.scale += 0.001f;
        if(p.direction.m_y <= 0.0f)
            createParticle(p);
    }
}

void Emitter::render() const
{
    glPointSize(4);
    m_vao->bind();
    m_vao->setData(ngl::SimpleVAO::VertexData(m_particles.size()*sizeof(Particle), m_particles[0].position.m_x));
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(Particle),0);
    m_vao->setVertexAttributePointer(1,3,GL_FLOAT,sizeof(Particle),3);

    m_vao->setNumIndices(m_particles.size());
    m_vao->draw();

    m_vao->unbind();

}