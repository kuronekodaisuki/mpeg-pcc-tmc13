cmake_minimum_required(VERSION 3.10)
enable_language(C)

if (UNIX)
  add_definitions(-D_POSIX_C_SOURCE=200809L)
elseif (WIN32)
  add_definitions(-D_USE_MATH_DEFINES)
endif()

include(CheckSymbolExists)
check_symbol_exists(getrusage sys/resource.h HAVE_GETRUSAGE)

##
# Determine the software version from VCS
# Fallback to descriptive version if VCS unavailable
#  NB: the "unconfirmed-" part refers to this being a fallback option
#      and the reported version may or may not be accurate.
set(VERSION_FALLBACK "unconfirmed-release-23.0-rc2")
set(VERSION_FILE ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(VERSION_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in)
set(VERSION_CACHE_FILE ${CMAKE_CURRENT_BINARY_DIR}/version.cache)
find_package(Git)

#add_custom_target(
#  genversion
#  COMMAND ${CMAKE_COMMAND} -D OUTPUT=${VERSION_FILE}
#                           -D TEMPLATE=${VERSION_FILE_IN}
#                           -D VERSION_FALLBACK=${VERSION_FALLBACK}
#                           -D VERSION_EXTRA=${VERSION_EXTRA}
#                           -D VERSION_CACHE_FILE=${VERSION_CACHE_FILE}
#                           -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
#                           -P ${CMAKE_SOURCE_DIR}/scripts/genversion.cmake
#  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)

add_custom_command(
  OUTPUT ${VERSION_FILE}
  DEPENDS genversion
)

#configure_file (
#  "${CMAKE_CURRENT_SOURCE_DIR}/TMC3Config.h.in"
#  "${PROJECT_BINARY_DIR}/tmc3/TMC3Config.h"
#)

#
# NB: avoid the use of wild cards for source files in this directory,
# since cmake is unable to detect additions or removals in such cases
# resulting in obscure broken builds when switching branches or
# otherwise adding/removing files.
#
# Dependencies are permitted to use the wild card since they are
# not expected to change without modifying this CMakeLists.txt.
#

file(GLOB PROJECT_IN_FILES
  "TMC3Config.h.in"
  "version.cpp.in"
)

file(GLOB PROJECT_INC_FILES
  "../tmc3/Attribute.h"
  "../tmc3/AttributeCommon.h"
  "../tmc3/AttributeDecoder.h"
  "../tmc3/AttributeEncoder.h"
  "../tmc3/BitReader.h"
  "../tmc3/BitWriter.h"
  "../tmc3/DualLutCoder.h"
  "../tmc3/FixedPoint.h"
  "../tmc3/OctreeNeighMap.h"
  "../tmc3/PCCKdTree.h"
  "../tmc3/PCCMath.h"
  "../tmc3/PCCMisc.h"
  "../tmc3/PCCPointSet.h"
  "../tmc3/PCCTMC3Common.h"
  "../tmc3/PCCTMC3Decoder.h"
  "../tmc3/PCCTMC3Encoder.h"
  "../tmc3/RAHT.h"
  "../tmc3/TMC3.h"
  "../tmc3/attribute_raw.h"
  "../tmc3/colourspace.h"
  "../tmc3/coordinate_conversion.h"
  "../tmc3/constants.h"
  "../tmc3/entropy.h"
  "../tmc3/entropychunk.h"
  "../tmc3/entropydirac.h"
  "../tmc3/entropyo3dgc.h"
  "../tmc3/entropyutils.h"
  "../tmc3/frame.h"
  "../tmc3/framectr.h"
  "../tmc3/geometry.h"
  "../tmc3/geometry_intra_pred.h"
  "../tmc3/geometry_octree.h"
  "../tmc3/geometry_params.h"
  "../tmc3/geometry_predictive.h"
  "../tmc3/geometry_trisoup.h"
  "../tmc3/hls.h"
  "../tmc3/io_hls.h"
  "../tmc3/io_tlv.h"
  "../tmc3/motionWip.h"
  "../tmc3/osspecific.h"
  "../tmc3/partitioning.h"
  "../tmc3/pcc_chrono.h"
  "../tmc3/ply.h"
  "../tmc3/pointset_processing.h"
  "../tmc3/quantization.h"
  "../tmc3/ringbuf.h"
  "../tmc3/tables.h"
  "../tmc3/version.h"
  "../dependencies/nanoflann/*.hpp"
  "../dependencies/nanoflann/*.h"
  "../dependencies/program-options-lite/*.h"
  "../dependencies/schroedinger/schroarith.h"
)

file(GLOB PROJECT_CPP_FILES
  "../tmc3/AttributeCommon.cpp"
  "../tmc3/AttributeDecoder.cpp"
  "../tmc3/AttributeEncoder.cpp"
  "../tmc3/DualLutCoder.cpp"
  "../tmc3/FixedPoint.cpp"
  "../tmc3/OctreeNeighMap.cpp"
  "../tmc3/RAHT.cpp"
  "../tmc3/TMC3.cpp"
  "../tmc3/attribute_raw_decoder.cpp"
  "../tmc3/attribute_raw_encoder.cpp"
  "../tmc3/coordinate_conversion.cpp"
  "../tmc3/decoder.cpp"
  "../tmc3/encoder.cpp"
  "../tmc3/entropydirac.cpp"
  "../tmc3/frame.cpp"
  "../tmc3/geometry_intra_pred.cpp"
  "../tmc3/geometry_octree.cpp"
  "../tmc3/geometry_octree_decoder.cpp"
  "../tmc3/geometry_octree_encoder.cpp"
  "../tmc3/geometry_predictive_decoder.cpp"
  "../tmc3/geometry_predictive_encoder.cpp"
  "../tmc3/geometry_trisoup_decoder.cpp"
  "../tmc3/geometry_trisoup_encoder.cpp"
  "../tmc3/io_hls.cpp"
  "../tmc3/io_tlv.cpp"
  "../tmc3/misc.cpp"
  "../tmc3/motionWip.cpp"
  "../tmc3/osspecific.cpp"
  "../tmc3/partitioning.cpp"
  "../tmc3/pcc_chrono.cpp"
  "../tmc3/ply.cpp"
  "../tmc3/pointset_processing.cpp"
  "../tmc3/quantization.cpp"
  "../tmc3/tables.cpp"
  "../dependencies/program-options-lite/*.cpp"
  "../dependencies/schroedinger/schroarith.c"
)

source_group (inc FILES ${PROJECT_INC_FILES})
source_group (input FILES ${PROJECT_IN_FILES})
source_group (cpp FILES ${PROJECT_CPP_FILES})

include_directories(
  "${PROJECT_BINARY_DIR}/tmc3"
  "${PROJECT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/nanoflann"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/schroedinger/"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/program-options-lite"
)

add_library (libgpcc STATIC
  ${PROJECT_CPP_FILES}
  ${PROJECT_INC_FILES}
  ${PROJECT_IN_FILES}
  ${VERSION_FILE}
)
add_dependencies(libgpcc genversion)

#add_executable (ply-merge EXCLUDE_FROM_ALL
#  "../tools/ply-merge.cpp"
#  "misc.cpp"
#  "ply.cpp"
#  "../dependencies/program-options-lite/program_options_lite.cpp"
#  ${VERSION_FILE}
#)
#add_dependencies(ply-merge genversion)

#install (TARGETS tmc3 DESTINATION bin)
