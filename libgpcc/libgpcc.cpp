// libgpcc.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "SequenceEncoder.h"
#include "libgpcc.h"

using namespace std;
using namespace pcc;

int
compress(Particle* particles, int count)
{
  std::vector<Particle> particle_vector(particles, particles + count);
  Parameters parameter;
  SequenceEncoder encoder(&parameter);

  return encoder.compress(particle_vector);
}

/*
int
compress(std::vector<Particle> particles)
{
  Parameters parameter;	
  SequenceEncoder encoder(&parameter);
  return encoder.compress(particles);
}
*/
