// libgpcc.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <vector>

#include "SequenceCodec.h"
#include "Particle.h"


/// <summary>
/// Compress particles from Swift
/// </summary>
/// <param name="particles"></param>
/// <param name="count"></param>
/// <returns></returns>
extern "C" int compress(Particle* particles, int count);

/// <summary>
/// Read PLY file
/// </summary>
/// <param name="fileName"></param>
/// <param name="cloud"></param>
/// <returns></returns>
bool readPly(const std::string& fileName, pcc::PCCPointSet3& cloud);


