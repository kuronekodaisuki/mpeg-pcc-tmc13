#pragma once
#include <cstdint>

#ifdef _WIN32
#  ifdef EXPORT
#    define API __declspec(dllexport)
#  else
#    define API __declspec(dllimport)
#  endif
#else
#  define API
#endif


class API Particle 
{
public:
  Particle(
    int32_t x, int32_t y, int32_t z, uint16_t r, uint16_t g, uint16_t b);

  int32_t* position();
  uint16_t* color();

private:
  int32_t _position[3];
  uint16_t _color[3];
};
