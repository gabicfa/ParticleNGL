#ifndef EMITTER_H_
#define EMITTER_H_

#include <cstddef>
#include <vector>
#include <string_view>

#include <ngl/AbstractVAO.h>
#include <memory>
#include "Particle.h"

class Emitter
{
    public:
        Emitter(size_t _numParticles);
        size_t getNumParticles() const;
        void update();
        void render() const;
    private:
        void createParticle(Particle &io_p);
        std::vector<Particle> m_particles;
        std::unique_ptr<ngl::AbstractVAO> m_vao;
        
};

#endif