#include "Particle.h"

Particle::Particle(int32_t x, int32_t y, int32_t z, uint16_t r, uint16_t g, uint16_t b)
{
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
  _color[0] = r;
  _color[1] = g;
  _color[2] = b;
}

int32_t* Particle::position()
{
	return &_position[0];
}

uint16_t* Particle::color()
{
  return &_color[0];
}

void Particle::SetPosition(int32_t x, int32_t y, int32_t z)
{
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
}

void Particle::SetColor(uint16_t r, uint16_t g, uint16_t b)
{
  _color[0] = r;
  _color[1] = g;
  _color[2] = b;
}