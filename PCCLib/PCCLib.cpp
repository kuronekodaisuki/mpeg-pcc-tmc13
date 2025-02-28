// PCCLib.cpp : スタティック ライブラリ用の関数を定義します。
//

#include <array>
#include <string>
#include "ply.h"

std::array<const char*, 3>
axisOrderToPropertyNames(uint16_t order)
{
  static const std::array<const char*, 3> kAxisOrderToPropertyNames[] = {
    {"z", "y", "x"}, {"x", "y", "z"}, {"x", "z", "y"}, {"y", "z", "x"},
    {"z", "y", "x"}, {"z", "x", "y"}, {"y", "x", "z"}, {"x", "y", "z"},
  };

  return kAxisOrderToPropertyNames[order];
}

namespace pcc {

// TODO: これは、ライブラリ関数の例です
bool
PlyRead(std::string& filepath)
{
  pcc::PCCPointSet3 pointCloud;
  pcc::ply::PropertyNameMap propertyNames;

  propertyNames.position= axisOrderToPropertyNames(1);

  return pcc::ply::read(filepath, propertyNames, 1, pointCloud);
}

}  // namespace pcc
