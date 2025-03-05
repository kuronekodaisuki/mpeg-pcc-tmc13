#pragma once
#include <array>

class Particle 
{
public:
  Particle(
    int32_t x, int32_t y, int32_t z, uint16_t r, uint16_t g, uint16_t b);

  int32_t* position();
  uint16_t* color();

private:
  std::array<int32_t, 3> _position;
  std::array<uint16_t, 3> _color;
};
