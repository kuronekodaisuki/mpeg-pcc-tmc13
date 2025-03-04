#include "Particle.h"

Particle::Particle(int32_t x, int32_t y, int32_t z, uint16_t r, uint16_t g, uint16_t b)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
  color[0] = r;
  color[1] = g;
  color[2] = b;
}