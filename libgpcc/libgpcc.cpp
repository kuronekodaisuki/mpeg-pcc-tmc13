// libgpcc.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "SequenceEncoder.h"
#include "libgpcc.h"

using namespace std;
using namespace pcc;

int
compress(std::vector<Particle> particles)
{
  Parameters parameter;	
  SequenceEncoder encoder(&parameter);
  return encoder.compress(particles);
}
