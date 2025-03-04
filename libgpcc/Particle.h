#pragma once
#include "PCCMath.h"

class Particle 
{
public:
  pcc::Vec3<int32_t> position;
  pcc::Vec3<uint16_t> color;

  Particle(
    int32_t x, int32_t y, int32_t z, uint16_t r, uint16_t g, uint16_t b);
};
