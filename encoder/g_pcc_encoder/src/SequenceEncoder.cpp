#include <memory>
#include <sys/stat.h>

//#include <ros/ros.h>
//#include <sensor_msgs/PointCloud2.h>

#include "program_options_lite.h"
#include "io_tlv.h"
#include "version.h"

#include "PCCMath.h"

#include "SequenceEncoder.hpp"
#include "constants.h"
#include "ply.h"

#include "PCCPointSet.h"

#include "util.hpp"
#include <climits>

#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <stdio.h>
//#include "FileWatch.hpp"

//namespace fs = std::filesystem;
// #define ENABLE_DUMP_BIN
// #define OUTPUT_DETAIL_MESSAGE
#define ENABLE_SEND_THREAD

using namespace std;
using namespace pcc;
//using namespace filewatch;
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

//============================================================================
bool SequenceEncoder::ParseParameters(int argc, char* argv[])
{
  namespace po = df::program_options_lite;

  struct {
    pcc::AttributeDescription desc;
    pcc::AttributeParameterSet aps;
    pcc::EncoderAttributeParams encoder;
  } params_attr;

  std::function<po::OptionFunc::Func> attribute_setter =
    [&](po::Options&, const std::string& name, po::ErrorReporter) {
      // NB: insert returns any existing element
      const auto& it = encoderParams_.attributeIdxMap.insert(
        {name, int(encoderParams_.attributeIdxMap.size())});

      if (it.second) {
        encoderParams_.sps.attributeSets.push_back(params_attr.desc);
        encoderParams_.aps.push_back(params_attr.aps);
        encoderParams_.attr.push_back(params_attr.encoder);
        return;
      }

      // update existing entry
      encoderParams_.sps.attributeSets[it.first->second] = params_attr.desc;
      encoderParams_.aps[it.first->second] = params_attr.aps;
      encoderParams_.attr[it.first->second] = params_attr.encoder;
    };

  /* clang-format off */
  // The definition of the program/config options, along with default values.
  //
  // NB: when updating the following tables:
  //      (a) please keep to 80-columns for easier reading at a glance,
  //      (b) do not vertically align values -- it breaks quickly
  //
  po::Options opts;
  po::OptionSpecific opt = opts.addOptions();
  SequenceCodec::addCommonOption(opt)
  // This section controls all general geometry scaling parameters
  (po::Section("Coordinate system scaling"))

  ("srcUnitLength",
    encoderParams_.srcUnitLength, 1.,
    "Length of source point cloud x,y,z unit vectors in srcUnits")

  ("srcUnit",
    encoderParams_.sps.seq_geom_scale_unit_flag, ScaleUnit::kDimensionless,
    " 0: dimensionless\n 1: metres")
  ("codingScale",
    encoderParams_.codedGeomScale, 1.,
    "Scale used to represent coded geometry. Relative to inputScale")

  ("sequenceScale",
    encoderParams_.seqGeomScale, 1.,
    "Scale used to obtain sequence coordinate system. "
    "Relative to inputScale")

  // Alias for compatibility with old name.
  ("positionQuantizationScale", encoderParams_.seqGeomScale, 1.,
   "(deprecated)")

  ("externalScale",
    encoderParams_.extGeomScale, 1.,
    "Scale used to define external coordinate system.\n"
    "Meaningless when srcUnit = metres\n"
    "  0: Use srcUnitLength\n"
    " >0: Relative to inputScale")

  (po::Section("Encoder"))
  ("geometry_axis_order",
    encoderParams_.sps.geometry_axis_order, AxisOrder::kXYZ,
    "Sets the geometry axis coding order:\n"
    "  0: (zyx)\n  1: (xyz)\n  2: (xzy)\n"
    "  3: (yzx)\n  4: (zyx)\n  5: (zxy)\n"
    "  6: (yxz)\n  7: (xyz)")

  ("autoSeqBbox",
    encoderParams_.autoSeqBbox, true,
    "Calculate seqOrigin and seqSizeWhd automatically.")

  // NB: the underlying variable is in STV order.
  //     Conversion happens during argument sanitization.
  ("seqOrigin",
    encoderParams_.sps.seqBoundingBoxOrigin, {0},
    "Origin (x,y,z) of the sequence bounding box "
    "(in input coordinate system). "
    "Requires autoSeqBbox=0")

  // NB: the underlying variable is in STV order.
  //     Conversion happens during argument sanitization.
  ("seqSizeWhd",
    encoderParams_.sps.seqBoundingBoxSize, {0},
    "Size of the sequence bounding box "
    "(in input coordinate system). "
    "Requires autoSeqBbox=0")

  ("mergeDuplicatedPoints",
    encoderParams_.gps.geom_unique_points_flag, true,
    "Enables removal of duplicated points")

  ("partitionMethod",
    encoderParams_.partition.method, PartitionMethod::kUniformSquare,
    "Method used to partition input point cloud into slices/tiles:\n"
    "  0: none\n"
    "  2: n Uniform-geometry partition bins along the longest edge\n"
    "  3: Uniform geometry partition at n octree depth\n"
    "  4: Uniform square partition\n"
    "  5: n-point spans of input")

  ("partitionOctreeDepth",
    encoderParams_.partition.octreeDepth, 1,
    "Depth of octree partition for partitionMethod=4")

  ("sliceMaxPoints",
    encoderParams_.partition.sliceMaxPoints, 1100000,
    "Maximum number of points per slice")

  ("sliceMinPoints",
    encoderParams_.partition.sliceMinPoints, 550000,
    "Minimum number of points per slice (soft limit)")

  ("tileSize",
    encoderParams_.partition.tileSize, 0,
    "Partition input into cubic tiles of given size")

  ("cabac_bypass_stream_enabled_flag",
    encoderParams_.sps.cabac_bypass_stream_enabled_flag, false,
    "Controls coding method for ep(bypass) bins")

  ("entropyContinuationEnabled",
    encoderParams_.sps.entropy_continuation_enabled_flag, false,
    "Propagate context state between slices")

  ("disableAttributeCoding",
    params_.disableAttributeCoding, false,
    "Ignore attribute coding configuration")

  ("enforceLevelLimits",
    encoderParams_.enforceLevelLimits, true,
    "Abort if level limits exceeded")

  (po::Section("Geometry"))

  ("geomTreeType",
    encoderParams_.gps.predgeom_enabled_flag, false,
    "Selects the tree coding method:\n"
    "  0: octree\n"
    "  1: predictive")

  ("qtbtEnabled",
    encoderParams_.gps.qtbt_enabled_flag, true,
    "Enables non-cubic geometry bounding box")

  ("maxNumQtBtBeforeOt",
    encoderParams_.geom.qtbt.maxNumQtBtBeforeOt, 4,
    "Max number of qtbt partitions before ot")

  ("minQtbtSizeLog2",
    encoderParams_.geom.qtbt.minQtbtSizeLog2, 0,
    "Minimum size of qtbt partitions")

  ("numOctreeEntropyStreams",
    // NB: this is adjusted by minus 1 after the arguments are parsed
    encoderParams_.gbh.geom_stream_cnt_minus1, 1,
    "Number of entropy streams for octree coding")

  ("bitwiseOccupancyCoding",
    encoderParams_.gps.bitwise_occupancy_coding_flag, true,
    "Selects between bitwise and bytewise occupancy coding:\n"
    "  0: bytewise\n"
    "  1: bitwise")

  ("neighbourAvailBoundaryLog2",
    // NB: this is adjusted by minus 1 after the arguments are parsed
    encoderParams_.gps.neighbour_avail_boundary_log2_minus1, 0,
    "Defines the avaliability volume for neighbour occupancy lookups:\n"
    "<2: Limited to sibling nodes only")

  ("inferredDirectCodingMode",
    encoderParams_.gps.inferred_direct_coding_mode, 1,
    "Early termination of the geometry octree for isolated points:"
    " 0: disabled\n"
    " 1: fully constrained\n"
    " 2: partially constrained\n"
    " 3: unconstrained (fastest)")

  ("jointTwoPointIdcm",
    encoderParams_.gps.joint_2pt_idcm_enabled_flag, true,
    "Jointly code common prefix of two IDCM points")

  ("adjacentChildContextualization",
    encoderParams_.gps.adjacent_child_contextualization_enabled_flag, true,
    "Occupancy contextualization using neighbouring adjacent children")

  ("intra_pred_max_node_size_log2",
    encoderParams_.gps.intra_pred_max_node_size_log2, 0,
    "octree nodesizes eligible for occupancy intra prediction")

  ("planarEnabled",
    encoderParams_.gps.geom_planar_mode_enabled_flag, true,
    "Use planar mode for geometry coding")

  ("planarModeThreshold0",
    encoderParams_.gps.geom_planar_threshold0, 77,
    "Activation threshold (0-127) of first planar mode. "
    "Lower values imply more use of the first planar mode")

  ("planarModeThreshold1",
    encoderParams_.gps.geom_planar_threshold1, 99,
    "Activation threshold (0-127) of second planar mode. "
    "Lower values imply more use of the first planar mode")

  ("planarModeThreshold2",
    encoderParams_.gps.geom_planar_threshold2, 113,
    "Activation threshold (0-127) of third planar mode. "
    "Lower values imply more use of the third planar mode")

   ("planarModeIdcmUse",
    // NB: this is adjusted by minus1 after thearguments are parsed
    encoderParams_.gps.geom_idcm_rate_minus1, 0,
    "Degree (1/32%) of IDCM activation when planar mode is enabled\n"
    "  0 => never, 32 => always")

  ("trisoupNodeSizeLog2",
    encoderParams_.trisoupNodeSizesLog2, {0},
    "Node size for surface triangulation\n"
    " <2: disabled")

  ("trisoup_sampling_value",
    encoderParams_.gps.trisoup_sampling_value, 0,
    "Trisoup voxelisation sampling rate\n"
    "  0: automatic")

  ("positionQuantisationEnabled",
    encoderParams_.gps.geom_scaling_enabled_flag, false,
    "Enable in-loop quantisation of positions")

  ("positionQuantisationMethod",
    encoderParams_.geom.qpMethod, OctreeEncOpts::QpMethod::kUniform,
    "Method used to determine per-node QP:\n"
    "  0: uniform\n"
    "  1: random\n"
    "  2: by node point density")

  ("positionQpMultiplierLog2",
    encoderParams_.gps.geom_qp_multiplier_log2, 0,
    "Granularity of QP to step size mapping:\n"
    "  n: 2^n QPs per doubling interval, n in 0..3")

  ("positionBaseQp",
    encoderParams_.gps.geom_base_qp, 0,
    "Base QP used in position quantisation (0 = lossless)")

  ("positionIdcmQp",
    encoderParams_.idcmQp, 0,
    "QP used in position quantisation of IDCM nodes")

  ("positionSliceQpOffset",
    encoderParams_.gbh.geom_slice_qp_offset, 0,
    "Per-slice QP offset used in position quantisation")

  ("positionQuantisationOctreeSizeLog2",
    encoderParams_.geom.qpOffsetNodeSizeLog2, -1,
    "Octree node size used for signalling position QP offsets "
    "(-1 => disabled)")

  ("positionQuantisationOctreeDepth",
    encoderParams_.geom.qpOffsetDepth, -1,
    "Octree depth used for signalling position QP offsets (-1 => disabled)")

  ("positionBaseQpFreqLog2",
    encoderParams_.gps.geom_qp_offset_intvl_log2, 8,
    "Frequency of sending QP offsets in predictive geometry coding")

  // NB: this will be corrected to be relative to base value later
  ("positionSliceQpFreqLog2",
    encoderParams_.gbh.geom_qp_offset_intvl_log2_delta, 0,
    "Frequency of sending QP offsets in predictive geometry coding")

  ("angularEnabled",
    encoderParams_.gps.geom_angular_mode_enabled_flag, false,
    "Controls angular contextualisation of occupancy")

  // NB: the underlying variable is in STV order.
  //     Conversion happens during argument sanitization.
  ("lidarHeadPosition",
    encoderParams_.gps.gpsAngularOrigin, {0, 0, 0},
    "laser head position (x,y,z) in angular mode")

  ("numLasers",
    encoderParams_.numLasers, 0,
    "Number of lasers in angular mode")

  ("lasersTheta",
    encoderParams_.lasersTheta, {},
    "Vertical laser angle in angular mode")

  ("lasersZ",
    encoderParams_.lasersZ, {},
    "Vertical laser offset in angular mode")

  ("lasersNumPhiPerTurn",
    encoderParams_.gps.angularNumPhiPerTurn, {},
    "Number of sampling poisitions in a complete laser turn in angular mode")

  ("planarBufferDisabled",
    encoderParams_.gps.planar_buffer_disabled_flag, false,
    "Disable planar buffer (when angular mode is enabled)")

  ("positionAzimuthScaleLog2",
    encoderParams_.gps.geom_angular_azimuth_scale_log2_minus11, 5,
    "Additional bits to represent azimuth angle in predictive geometry coding")

  // NB: this will be corrected to be minus 1 later
  ("positionAzimuthSpeed",
    encoderParams_.gps.geom_angular_azimuth_speed_minus1, 363,
    "Scale factor applied to azimuth angle in predictive geometry coding")

  ("positionRadiusInvScaleLog2",
    encoderParams_.gps.geom_angular_radius_inv_scale_log2, 0,
    "Inverse scale factor applied to radius in predictive geometry coding")

  ("predGeomSort",
    encoderParams_.predGeom.sortMode, PredGeomEncOpts::kSortMorton,
    "Predictive geometry tree construction order")

  ("predGeomAzimuthSortPrecision",
    encoderParams_.predGeom.azimuthSortRecipBinWidth, 0,
    "Reciprocal precision used in azimuthal sorting for tree construction")

  ("predGeomTreePtsMax",
    encoderParams_.predGeom.maxPtsPerTree, 1100000,
    "Maximum number of points per predictive geometry tree")

  ("pointCountMetadata",
    encoderParams_.gps.octree_point_count_list_present_flag, false,
    "Add octree layer point count metadata")

  (po::Section("Attributes"))

  // attribute processing
  //   NB: Attribute options are special in the way they are applied (see above)
  ("attribute",
    attribute_setter,
    "Encode the given attribute (NB, must appear after the"
    "following attribute parameters)")

  // NB: the cli option sets +1, the minus1 will be applied later
  ("attrScale",
    params_attr.desc.params.attr_scale_minus1, 1,
    "Scale factor used to interpret coded attribute values")

  ("attrOffset",
    params_attr.desc.params.attr_offset, 0,
    "Offset used to interpret coded attribute values")

  ("bitdepth",
    params_attr.desc.bitdepth, 8,
    "Attribute bitdepth")

  ("defaultValue",
    params_attr.desc.params.attr_default_value, {},
    "Default attribute component value(s) in case of data omission")

  // todo(df): this should be per-attribute
  ("colourMatrix",
    params_attr.desc.params.cicp_matrix_coefficients_idx, ColourMatrix::kBt709,
    "Matrix used in colourspace conversion\n"
    "  0: none (identity)\n"
    "  1: ITU-T BT.709\n"
    "  8: YCgCo")

  ("transformType",
    params_attr.aps.attr_encoding, AttributeEncoding::kPredictingTransform,
    "Coding method to use for attribute:\n"
    "  0: Region Adaptive Hierarchical Transform (RAHT)\n"
    "  1: Hierarchical neighbourhood prediction\n"
    "  2: Hierarchical neighbourhood prediction as lifting transform")

  ("rahtPredictionEnabled",
    params_attr.aps.raht_prediction_enabled_flag, true,
    "Controls the use of transform-domain prediction")

  ("rahtPredictionThreshold0",
    params_attr.aps.raht_prediction_threshold0, 2,
    "Grandparent threshold for early transform-domain prediction termination")

  ("rahtPredictionThreshold1",
    params_attr.aps.raht_prediction_threshold1, 6,
    "Parent threshold for early transform-domain prediction termination")

  // NB: the cli option sets +1, the minus1 will be applied later
  ("numberOfNearestNeighborsInPrediction",
    params_attr.aps.num_pred_nearest_neighbours_minus1, 3,
    "Attribute's maximum number of nearest neighbors to be used for prediction")

  ("adaptivePredictionThreshold",
    params_attr.aps.adaptive_prediction_threshold, 1 << 6,
    "Neighbouring attribute value difference that enables direct "
    "prediction. 8-bit value scaled to attribute bitdeph. "
    "Applies to transformType=0 only")

  ("intraLodSearchRange",
    params_attr.aps.intra_lod_search_range, -1,
    "Intra LoD nearest neighbor search range\n"
    " -1: Full-range\n"
    "  0: Disabled")

  ("interLodSearchRange",
    params_attr.aps.inter_lod_search_range, -1,
    "Inter LoD nearest neighbor search range\n"
    " -1: Full-range")

  // NB: the underlying variable is in STV order.
  //     Conversion happens during argument sanitization.
  ("lod_neigh_bias",
    params_attr.aps.lodNeighBias, {1, 1, 1},
    "Attribute's (x,y,z) component intra prediction weights")

  ("lodDecimator",
    params_attr.aps.lod_decimation_type, LodDecimationMethod::kNone,
    "LoD decimation method:\n"
    " 0: none\n"
    " 1: periodic subsampling using lodSamplingPeriod\n"
    " 2: centroid subsampling using lodSamplingPeriod")

  ("max_num_direct_predictors",
    params_attr.aps.max_num_direct_predictors, 3,
    "Maximum number of nearest neighbour candidates used in direct"
    "attribute prediction")

  ("direct_avg_predictor_disabled_flag",
    params_attr.aps.direct_avg_predictor_disabled_flag, false,
    "Disable average predictor")

  ("predWeightBlending",
    params_attr.aps.pred_weight_blending_enabled_flag, false,
    "Blend prediction weights according to neigbour distances. "
    "Applies to transformType=0 only")

  // NB: this parameter actually represents the number of refinement layers
  ("levelOfDetailCount",
    params_attr.aps.num_detail_levels_minus1, 1,
    "Attribute's number of levels of detail")

  ("dist2",
    params_attr.aps.dist2, 0,
    "Initial squared distance used in LoD generation")

  ("dist2PercentileEstimate",
    params_attr.encoder.dist2PercentileEstimate, 0.85f,
    "Percentile for dist2 estimation during nearest neighbour search")

  ("lodSamplingPeriod",
    params_attr.aps.lodSamplingPeriod, {4},
    "List of per LoD sampling periods used in LoD generation")

  ("intraLodPredictionSkipLayers",
    params_attr.aps.intra_lod_prediction_skip_layers, -1,
    "Number of finest detail levels that skip intra prediction\n"
    " -1: skip all (disables intra pred)")

  ("interComponentPredictionEnabled",
    params_attr.aps.inter_component_prediction_enabled_flag, false,
    "Use primary attribute component to predict values of subsequent "
    "components")

  ("lastComponentPredictionEnabled",
    params_attr.aps.last_component_prediction_enabled_flag, true,
    "Use second attribute component to predict value of the final component")

  ("canonical_point_order_flag",
    params_attr.aps.canonical_point_order_flag, false,
    "Enable skipping morton sort in case of number of LoD equal to 1")

  ("spherical_coord_flag",
     params_attr.aps.spherical_coord_flag, false,
     "Code attributes in spherical domain")

  ("aps_scalable_enable_flag",
    params_attr.aps.scalable_lifting_enabled_flag, false,
    "Enable scalable attritube coding")

  ("max_neigh_range",
    // NB: this is adjusted by minus 1 after the arguments are parsed
    params_attr.aps.max_neigh_range_minus1, 5,
    "maximum nearest neighbour range for scalable lifting")

  ("qp",
    // NB: this is adjusted with minus 4 after the arguments are parsed
    params_attr.aps.init_qp_minus4, 4,
    "Attribute's luma quantisation parameter")

  ("qpChromaOffset",
    params_attr.aps.aps_chroma_qp_offset, 0,
    "Attribute's chroma quantisation parameter offset (relative to luma)")

  ("aps_slice_qp_deltas_present_flag",
    params_attr.aps.aps_slice_qp_deltas_present_flag, false,
    "Enable signalling of per-slice QP values")

  ("qpLayerOffsetsLuma",
    params_attr.encoder.abh.attr_layer_qp_delta_luma, {},
      "Attribute's per layer luma QP offsets")

  ("qpLayerOffsetsChroma",
      params_attr.encoder.abh.attr_layer_qp_delta_chroma, {},
      "Attribute's per layer chroma QP offsets")

  // This section is just dedicated to attribute recolouring (encoder only).
  // parameters are common to all attributes.
  (po::Section("Recolouring"))

  ("recolourSearchRange",
    encoderParams_.recolour.searchRange, 1,
    "")

  ("recolourNumNeighboursFwd",
    encoderParams_.recolour.numNeighboursFwd, 8,
    "")

  ("recolourNumNeighboursBwd",
    encoderParams_.recolour.numNeighboursBwd, 1,
    "")

  ("recolourUseDistWeightedAvgFwd",
    encoderParams_.recolour.useDistWeightedAvgFwd, true,
    "")

  ("recolourUseDistWeightedAvgBwd",
    encoderParams_.recolour.useDistWeightedAvgBwd, true,
    "")

  ("recolourSkipAvgIfIdenticalSourcePointPresentFwd",
    encoderParams_.recolour.skipAvgIfIdenticalSourcePointPresentFwd, true,
    "")

  ("recolourSkipAvgIfIdenticalSourcePointPresentBwd",
    encoderParams_.recolour.skipAvgIfIdenticalSourcePointPresentBwd, false,
    "")

  ("recolourDistOffsetFwd",
    encoderParams_.recolour.distOffsetFwd, 4.,
    "")

  ("recolourDistOffsetBwd",
    encoderParams_.recolour.distOffsetBwd, 4.,
    "")

  ("recolourMaxGeometryDist2Fwd",
    encoderParams_.recolour.maxGeometryDist2Fwd, 1000.,
    "")

  ("recolourMaxGeometryDist2Bwd",
    encoderParams_.recolour.maxGeometryDist2Bwd, 1000.,
    "")

  ("recolourMaxAttributeDist2Fwd",
    encoderParams_.recolour.maxAttributeDist2Fwd, 1000.,
    "")

  ("recolourMaxAttributeDist2Bwd",
    encoderParams_.recolour.maxAttributeDist2Bwd, 1000.,
    "")

#if defined(ENABLE_DEBUG_ROS_OUTPUT)
  ("enablePreview",
    enablePreview_, false,
    "")
#endif // defined(ENABLE_DEBUG_ROS_OUTPUT)

#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
  ("enablePreviewPreProc",
    enablePreviewPreProc_, false,
    "")
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)

#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
  ("enablePreviewRecon",
    enablePreviewRecon_, false,
    "")
#endif // if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
  ("interval_for_fileinput",
    interval_for_fileinput_, 0,
    "")
  ;
  /* clang-format on */

  po::setDefaults(opts);
  po::ErrorReporter err;
  const list<const char*>& argv_unhandled =
    po::scanArgv(opts, argc, (const char**)argv, err);

  for (const auto arg : argv_unhandled) {
    err.warn() << "Unhandled argument ignored: " << arg << "\n";
  }

  if (argc == 1 || params_.print_help) {
    po::doHelp(std::cout, opts, 78);
    return false;
  }

  // set default output units (this works for the decoder too)
  if (params_.outputUnitLength <= 0.)
    params_.outputUnitLength = encoderParams_.srcUnitLength;
  encoderParams_.outputFpBits = params_.outputFpBits;

  sanitizeEncoderOpts(err);

  // check required arguments are specified
  if (params_.uncompressedDataPath.empty() && params_.ros_subscribe_topic.empty()) {
    // err.error() << "uncompressedDataPath and ros_subscribe_topic not set\n";
    params_.ros_subscribe_topic="//kddi_cap//colorpoints";
  }

  if (params_.compressedStreamPath.empty() && params_.ip_address.empty()) {
    err.error() << "compressedStreamPath and ip_address not set\n";
  }

  // report the current configuration (only in the absence of errors so
  // that errors/warnings are more obvious and in the same place).
  if (err.is_errored)
    return false;

  // Dump the complete derived configuration
  cout << "+ Effective configuration parameters\n";

  po::dumpCfg(cout, opts, "General", 4);
  {
    po::dumpCfg(cout, opts, "Cmn Coordinate system scaling", 4);
    po::dumpCfg(cout, opts, "Coordinate system scaling", 4);
    po::dumpCfg(cout, opts, "Encoder", 4);
    po::dumpCfg(cout, opts, "Geometry", 4);
    po::dumpCfg(cout, opts, "Recolouring", 4);

    for (const auto& it : encoderParams_.attributeIdxMap) {
      // NB: when dumping the config, opts references params_attr
      params_attr.desc = encoderParams_.sps.attributeSets[it.second];
      params_attr.aps = encoderParams_.aps[it.second];
      params_attr.encoder = encoderParams_.attr[it.second];
      cout << "    " << it.first << "\n";
      po::dumpCfg(cout, opts, "Attributes", 8);
    }
  }

  cout << endl;

  return true;
}

//----------------------------------------------------------------------------
void SequenceEncoder::sanitizeEncoderOpts(df::program_options_lite::ErrorReporter& err)
{
  // Input scaling affects the definition of the source unit length.
  // eg, if the unit length of the source is 1m, scaling by 1000 generates
  // a cloud with unit length 1mm.
  encoderParams_.srcUnitLength /= params_.inputScale;

  // global scale factor must be positive
  if (encoderParams_.codedGeomScale > encoderParams_.seqGeomScale) {
    err.warn() << "codingScale must be <= sequenceScale, adjusting\n";
    encoderParams_.codedGeomScale = encoderParams_.seqGeomScale;
  }

  // fix the representation of various options
  encoderParams_.gbh.geom_stream_cnt_minus1--;
  encoderParams_.gps.geom_idcm_rate_minus1--;
  encoderParams_.gps.geom_angular_azimuth_speed_minus1--;
  encoderParams_.gps.neighbour_avail_boundary_log2_minus1 =
    std::max(0, encoderParams_.gps.neighbour_avail_boundary_log2_minus1 - 1);
  for (auto& attr_sps : encoderParams_.sps.attributeSets) {
    attr_sps.params.attr_scale_minus1--;
  }
  for (auto& attr_aps : encoderParams_.aps) {
    attr_aps.init_qp_minus4 -= 4;
    attr_aps.num_pred_nearest_neighbours_minus1--;
    attr_aps.max_neigh_range_minus1--;
  }

  // Config options are absolute, but signalling is relative
  encoderParams_.gbh.geom_qp_offset_intvl_log2_delta -=
    encoderParams_.gps.geom_qp_offset_intvl_log2;

  // If idcm rate is configured as 0, disable idcm
  // NB: if user has requested less contrained idcm, warn
  if (encoderParams_.gps.geom_idcm_rate_minus1 < 0) {
    if (encoderParams_.gps.inferred_direct_coding_mode == 1)
      encoderParams_.gps.inferred_direct_coding_mode = 0;
  }

  if (encoderParams_.gps.geom_idcm_rate_minus1 < 31) {
    if (encoderParams_.gps.inferred_direct_coding_mode > 1) {
      encoderParams_.gps.geom_idcm_rate_minus1 = 31;
      err.warn() << "ignoring planarModeIdcmUse < 32: "
                    "contradicts inferredDirectCodingMode > 1\n";
    }
  }

  // convert coordinate systems if the coding order is different from xyz
  convertXyzToStv(&encoderParams_.sps);
  convertXyzToStv(encoderParams_.sps, &encoderParams_.gps);
  for (auto& aps : encoderParams_.aps)
    convertXyzToStv(encoderParams_.sps, &aps);

  // Trisoup is enabled when a node size is specified
  // sanity: don't enable if only node size is 0.
  // todo(df): this needs to take into account slices where it is disabled
  if (encoderParams_.trisoupNodeSizesLog2.size() == 1)
    if (encoderParams_.trisoupNodeSizesLog2[0] < 2)
      encoderParams_.trisoupNodeSizesLog2.clear();

  for (auto trisoupNodeSizeLog2 : encoderParams_.trisoupNodeSizesLog2)
    if (trisoupNodeSizeLog2 < 2)
      err.error() << "Trisoup node size must be greater than 1\n";

  encoderParams_.gps.trisoup_enabled_flag =
    !encoderParams_.trisoupNodeSizesLog2.empty();

  // Certain coding modes are not available when trisoup is enabled.
  // Disable them, and warn if set (they may be set as defaults).
  if (encoderParams_.gps.trisoup_enabled_flag) {
    if (!encoderParams_.gps.geom_unique_points_flag)
      err.warn() << "TriSoup geometry does not preserve duplicated points\n";

    if (encoderParams_.gps.inferred_direct_coding_mode)
      err.warn() << "TriSoup geometry is incompatable with IDCM\n";

    encoderParams_.gps.geom_unique_points_flag = true;
    encoderParams_.gps.inferred_direct_coding_mode = 0;
  }

  // tweak qtbt generation when trisoup is /isn't enabled
  encoderParams_.geom.qtbt.trisoupEnabled =
    encoderParams_.gps.trisoup_enabled_flag;

  // Planar coding mode is not available for bytewise coding
  if (!encoderParams_.gps.bitwise_occupancy_coding_flag) {
    if (encoderParams_.gps.geom_planar_mode_enabled_flag)
      err.warn() << "Bytewise geometry coding does not support planar mode\n";
    encoderParams_.gps.geom_planar_mode_enabled_flag = false;
  }

  // support disabling attribute coding (simplifies configuration)
  if (params_.disableAttributeCoding) {
    encoderParams_.attributeIdxMap.clear();
    encoderParams_.sps.attributeSets.clear();
    encoderParams_.aps.clear();
  }

  // fixup any per-attribute settings
  for (const auto& it : encoderParams_.attributeIdxMap) {
    auto& attr_sps = encoderParams_.sps.attributeSets[it.second];
    auto& attr_aps = encoderParams_.aps[it.second];
    auto& attr_enc = encoderParams_.attr[it.second];

    // default values for attribute
    attr_sps.attr_instance_id = 0;
    auto& attrMeta = attr_sps.params;
    attrMeta.cicp_colour_primaries_idx = 2;
    attrMeta.cicp_transfer_characteristics_idx = 2;
    attrMeta.cicp_video_full_range_flag = true;
    attrMeta.cicpParametersPresent = false;
    attrMeta.attr_frac_bits = 0;
    attrMeta.scalingParametersPresent = false;

    // Enable scaling if a paramter has been set
    //  - pre/post scaling is only currently supported for reflectance
    attrMeta.scalingParametersPresent = attrMeta.attr_offset
      || attrMeta.attr_scale_minus1 || attrMeta.attr_frac_bits;

    // todo(df): remove this hack when scaling is generalised
    if (it.first != "reflectance" && attrMeta.scalingParametersPresent) {
      err.warn() << it.first << ": scaling not supported, disabling\n";
      attrMeta.scalingParametersPresent = 0;
    }

    if (it.first == "reflectance") {
      // Avoid wasting bits signalling chroma quant step size for reflectance
      attr_aps.aps_chroma_qp_offset = 0;
      attr_enc.abh.attr_layer_qp_delta_chroma.clear();

      // There is no matrix for reflectace
      attrMeta.cicp_matrix_coefficients_idx = ColourMatrix::kUnspecified;
      attr_sps.attr_num_dimensions_minus1 = 0;
      attr_sps.attributeLabel = KnownAttributeLabel::kReflectance;
    }

    if (it.first == "color") {
      attr_sps.attr_num_dimensions_minus1 = 2;
      attr_sps.attributeLabel = KnownAttributeLabel::kColour;
      attrMeta.cicpParametersPresent = true;
    }

    // Assume that YCgCo is actually YCgCoR for now
    // This requires an extra bit to represent chroma (luma will have a
    // reduced range)
    if (attrMeta.cicp_matrix_coefficients_idx == ColourMatrix::kYCgCo)
      attr_sps.bitdepth++;

    // Extend the default attribute value to the correct width if present
    if (!attrMeta.attr_default_value.empty())
      attrMeta.attr_default_value.resize(
        attr_sps.attr_num_dimensions_minus1 + 1,
        attrMeta.attr_default_value.back());

    // In order to simplify specification of dist2 values, which are
    // depending on the scale of the coded point cloud, the following
    // adjust the dist2 values according to PQS.  The user need only
    // specify the unquantised PQS value.
    if (params_.positionQuantizationScaleAdjustsDist2) {
      auto delta = log2(encoderParams_.codedGeomScale);
      attr_aps.dist2 =
        std::max(0, int32_t(std::round(attr_aps.dist2 + delta)));
    }

    // derive samplingPeriod values based on initial value
    if (
      !attr_aps.lodParametersPresent()
      || (attr_aps.lod_decimation_type == LodDecimationMethod::kNone)) {
      attr_aps.lodSamplingPeriod.clear();
    } else if (!attr_aps.lodSamplingPeriod.empty()) {
      auto i = attr_aps.lodSamplingPeriod.size();
      attr_aps.lodSamplingPeriod.resize(attr_aps.num_detail_levels_minus1);
      // add any extra values as required
      for (; i < attr_aps.num_detail_levels_minus1; i++)
        attr_aps.lodSamplingPeriod[i] = attr_aps.lodSamplingPeriod[i - 1];
    }

    if (attr_aps.attr_encoding == AttributeEncoding::kLiftingTransform) {
      attr_aps.adaptive_prediction_threshold = 0;
      attr_aps.intra_lod_search_range = 0;
      attr_aps.intra_lod_prediction_skip_layers = -1;
    }

    // For RAHT, ensure that the unused lod count = 0 (prevents mishaps)
    if (attr_aps.attr_encoding == AttributeEncoding::kRAHTransform) {
      attr_aps.num_detail_levels_minus1 = 0;
      attr_aps.adaptive_prediction_threshold = 0;
    }

    if (!encoderParams_.gps.geom_angular_mode_enabled_flag) {
      if (attr_aps.spherical_coord_flag)
        err.warn() << it.first
                   << ".spherical_coord_flag=1 requires angularEnabled=1, "
                      "disabling\n";
      attr_aps.spherical_coord_flag = false;
    }
  }

  // convert floating point values of Lasers' Theta and H to fixed point
  if (encoderParams_.gps.geom_angular_mode_enabled_flag) {
    if (encoderParams_.numLasers == 0)
      err.error() << "numLasers must be at least 1\n";

    for (auto val : encoderParams_.lasersTheta) {
      int one = 1 << 18;
      encoderParams_.gps.angularTheta.push_back(round(val * one));
    }

    for (auto val : encoderParams_.lasersZ) {
      int one = 1 << 3;
      auto scale = encoderParams_.codedGeomScale;
      if (encoderParams_.gps.predgeom_enabled_flag)
        scale = encoderParams_.codedGeomScale / encoderParams_.seqGeomScale;

      encoderParams_.gps.angularZ.push_back(round(val * scale * one));
    }

    if (encoderParams_.gps.angularTheta.size() != encoderParams_.numLasers)
      err.error() << "lasersZ.size() != numLasers\n";

    if (encoderParams_.gps.angularZ.size() != encoderParams_.numLasers)
      err.error() << "lasersTheta.size() != numLasers\n";

    if (
      encoderParams_.gps.angularNumPhiPerTurn.size()
      != encoderParams_.numLasers)
      err.error() << "lasersNumPhiPerTurn.size() != numLasers\n";

    if (encoderParams_.gps.qtbt_enabled_flag) {
      encoderParams_.geom.qtbt.angularMaxNodeMinDimLog2ToSplitV =
        std::max<int>(0, 8 + log2(encoderParams_.codedGeomScale));
      encoderParams_.geom.qtbt.angularMaxDiffToSplitZ =
        std::max<int>(0, 1 + log2(encoderParams_.codedGeomScale));
    }

    if (encoderParams_.gps.predgeom_enabled_flag) {
      auto& gps = encoderParams_.gps;
      int maxSpeed = 1 << (gps.geom_angular_azimuth_scale_log2_minus11 + 12);
      if (encoderParams_.gps.geom_angular_azimuth_speed_minus1 + 1 > maxSpeed)
        err.error() << "positionAzimuthSpeed > max (" << maxSpeed << ")\n";
    }
  }

  // tweak qtbt when angular is / isn't enabled
  encoderParams_.geom.qtbt.angularTweakEnabled =
    encoderParams_.gps.geom_angular_mode_enabled_flag;

  if (!encoderParams_.geom.qtbt.angularTweakEnabled) {
    // NB: these aren't used in this condition
    encoderParams_.geom.qtbt.angularMaxNodeMinDimLog2ToSplitV = 0;
    encoderParams_.geom.qtbt.angularMaxDiffToSplitZ = 0;
  }

  // sanity checks

  if (encoderParams_.gps.geom_qp_multiplier_log2 & ~3)
    err.error() << "positionQpMultiplierLog2 must be in the range 0..3\n";

  if (!encoderParams_.gps.geom_angular_mode_enabled_flag) {
    if (encoderParams_.gps.planar_buffer_disabled_flag) {
      encoderParams_.gps.planar_buffer_disabled_flag = 0;
      err.warn() << "ignoring planarBufferDisabled without angularEnabled\n";
    }
  }

  // The following featues depend upon the occupancy atlas
  if (!encoderParams_.gps.neighbour_avail_boundary_log2_minus1) {
    if (encoderParams_.gps.adjacent_child_contextualization_enabled_flag)
      err.warn() << "ignoring adjacentChildContextualization when"
                    " neighbourAvailBoundaryLog2=0\n";

    if (encoderParams_.gps.intra_pred_max_node_size_log2)
      err.warn() << "ignoring intra_pred_max_node_size_log2 when"
                    " neighbourAvailBoundaryLog2=0\n";

    encoderParams_.gps.adjacent_child_contextualization_enabled_flag = 0;
    encoderParams_.gps.intra_pred_max_node_size_log2 = 0;
  }

  if (
    encoderParams_.partition.sliceMaxPoints
    < encoderParams_.partition.sliceMinPoints)
    err.error()
      << "sliceMaxPoints must be greater than or equal to sliceMinPoints\n";

  for (const auto& it : encoderParams_.attributeIdxMap) {
    const auto& attr_sps = encoderParams_.sps.attributeSets[it.second];
    const auto& attr_aps = encoderParams_.aps[it.second];
    auto& attr_enc = encoderParams_.attr[it.second];

    if (it.first == "color") {
      if (
        attr_enc.abh.attr_layer_qp_delta_luma.size()
        != attr_enc.abh.attr_layer_qp_delta_chroma.size()) {
        err.error() << it.first
                    << ".qpLayerOffsetsLuma length != .qpLayerOffsetsChroma\n";
      }
    }

    if (attr_sps.bitdepth > 16)
      err.error() << it.first << ".bitdepth must be less than 17\n";

    if (attr_aps.lodParametersPresent()) {
      int lod = attr_aps.num_detail_levels_minus1;
      if (lod > 255 || lod < 0) {
        err.error() << it.first
                    << ".levelOfDetailCount must be in the range [0,255]\n";
      }

      // if zero, values are derived automatically
      if (attr_aps.dist2 < 0 || attr_aps.dist2 > 20) {
        err.error() << it.first << ".dist2 must be in the range [0,20]\n";
      }

      if (lod > 0 && attr_aps.canonical_point_order_flag) {
        err.error() << it.first
                    << "when levelOfDetailCount > 0, "
                       "canonicalPointOrder must be 0\n";
      }

      if (attr_aps.attr_encoding == AttributeEncoding::kPredictingTransform) {
        if (lod == 0 && attr_aps.intra_lod_prediction_skip_layers != 0)
          err.error(it.first)
            << "when transformType == 0 (Pred) and levelOfDetailCount == 0, "
               "intraLodPredictionSkipLayers must be 0\n";

        if (lod == 0 && attr_aps.intra_lod_search_range == 0)
          err.error(it.first)
            << "when transformType == 0 (Pred) and levelOfDetailCount == 0, "
               "intraLodSearchRange must not be 0\n";
      }

      if (
        (attr_aps.lod_decimation_type != LodDecimationMethod::kNone)
        && attr_aps.lodSamplingPeriod.empty()) {
        err.error() << it.first
                    << ".lodSamplingPeriod must contain at least one entry\n";
      }

      for (auto samplingPeriod : attr_aps.lodSamplingPeriod) {
        if (samplingPeriod < 2)
          err.error() << it.first << ".lodSamplingPeriod values must be > 1\n";
      }

      if (attr_aps.adaptive_prediction_threshold < 0) {
        err.error() << it.first
                    << ".adaptivePredictionThreshold must be positive\n";
      }

      if (
        attr_aps.num_pred_nearest_neighbours_minus1
        >= kAttributePredictionMaxNeighbourCount) {
        err.error() << it.first
                    << ".numberOfNearestNeighborsInPrediction must be <= "
                    << kAttributePredictionMaxNeighbourCount << "\n";
      }

      if (attr_aps.scalable_lifting_enabled_flag) {
        if (attr_aps.lod_decimation_type != LodDecimationMethod::kNone) {
          err.error() << it.first << ".lod_decimation_type must be 0\n";
        }

        if (encoderParams_.gps.trisoup_enabled_flag) {
          err.error() << it.first
                      << " trisoup_enabled_flag must be disabled\n";
        }

        if (encoderParams_.gps.geom_qp_multiplier_log2 != 3)
          err.error() << it.first << " positionQpMultiplierLog2 must be 3\n";
      }
    }

    if (attr_aps.init_qp_minus4 < 0 || attr_aps.init_qp_minus4 + 4 > 51)
      err.error() << it.first << ".qp must be in the range [4,51]\n";

    if (std::abs(attr_aps.aps_chroma_qp_offset) > 51 - 4) {
      err.error() << it.first
                  << ".qpChromaOffset must be in the range [-47,47]\n";
    }
  }
}

//============================================================================
SequenceEncoder::SequenceEncoder()
                : SequenceCodec()
                , pcc_sock_(-1)
                , input_point_total_(0)
                , input_point_max_(0)
                , input_point_min_(INT_MAX)
                , output_byte_total_(0)
                , output_byte_max_(0)
                , output_byte_min_(INT_MAX)
                , sendbuff_()
#ifdef DO_PARALLEL_CODING
                , encoder(nullptr)
                , parallelEncoder(nullptr)
#endif // DO_PARALLEL_CODING
{
  static const struct sockaddr_in initiator = {};
  peer_ = initiator;
  static const struct sockaddr_in6 initiator6 = {};
  peer6_ = initiator6;
}

SequenceEncoder::~SequenceEncoder()
{
  deinit();
}

void SequenceEncoder::deinit()
{
  closeSocket(pcc_sock_);
  pcc_sock_ = -1;

#if defined(ENABLE_DUMP_BIN)
  if (dumpSendStream.is_open()) {
    std::cout << "Total bitstream size " << dumpSendStream.tellp() << " B" << std::endl;
    dumpSendStream.close();
  }
#endif // defined(ENABLE_DUMP_BIN)

  if(debugLog_.is_open()) {
    debugLog_.close();
  }

  if (th_sender_.joinable()) {
    std::cout << "join Thread [#" << th_sender_.thread_->get_id() << "]" << std::endl;
    th_sender_.join();
  }
#ifdef DO_PARALLEL_CODING

  if (nullptr != encoder) {
    delete encoder;
    encoder = nullptr;
  }

  if (nullptr != parallelEncoder) {
    delete parallelEncoder;
    parallelEncoder = nullptr;
  }
#endif // DO_PARALLEL_CODING
}

bool SequenceEncoder::init()
{
#ifdef DO_PARALLEL_CODING
  if (1 == params_.numOfFrameThreads) {
    try {
      encoder = new pcc::PCCTMC3Encoder3();
    }
    catch (const std::bad_alloc&) {
      return(false);
    }
  }
  else {
    try {
      parallelEncoder = new ParallelEncoder();
    }
    catch (const std::bad_alloc&) {
      return(false);
    }

    if (false == parallelEncoder->init(static_cast<unsigned int>(params_.numOfFrameThreads), params_.doSliceParallelProc)) {
      return(false);
    }
  }
#endif // DO_PARALLEL_CODING
#if defined(ENABLE_DUMP_BIN)
    dumpSendStream.open(".//dumpbin.bin", ios::binary);
    if (!dumpSendStream.is_open()) {
      return -1;
    }
#endif // defined(ENABLE_DUMP_BIN)

  // determine the naming (ordering) of ply properties
  _plyAttrNames.position =
    axisOrderToPropertyNames(encoderParams_.sps.geometry_axis_order);

  // NB: this is the raw origin before the encoder tweaks it
  _angularOrigin = encoderParams_.gps.gpsAngularOrigin;

  if (!params_.ip_address.empty()) {
      cout << "----- sock::init. -----" << endl;

    if (!initializeSocket()) {
      cout << "----- failed to sock::init. -----" << endl;
      // failed to initialize socket
      return (false);
    }
  }

  {
    if (!params_.compressedStreamPath.empty()) {
      bytestreamFile.open(params_.compressedStreamPath, ios::binary);
      if (!bytestreamFile.is_open()) {
        return -1;
      }
    }
  }

  if (( !params_.ros_subscribe_topic.empty()) || enablePreview_ || enablePreviewPreProc_ || enablePreviewRecon_) {
    if (!params_.dumpUncompressedDataPath.empty()) {
      // reconstructedDataPath で良いが、
      // 便宜上、デコーダ側とオプションを合わせるため
      // dumpUncompressedDataPath のオプション指定を使用する。
      params_.reconstructedDataPath = params_.dumpUncompressedDataPath;
    }
    frameNum = params_.firstFrameNum;
    // package_path_ = ros::package::getPath("tmc3");

/** ROS関連は除外 ka-hagiwara
    // todo 入力判定を行い、 ROS 経由の入力なら初期化を行うようにする。
    if (!ros::isInitialized()) {
      cout << "----- ros::init. -----" << endl;

      int argc = 0;
      // todo 引数によるオプションオーバーロードが必要になった場合は、
      // parse parameter の unhandled な文字列を stringstremで保持するなどしておき、
      // ros::init のパラメータで指定するようにする。
      // ros::init(argc, nullptr, params_.ros_node_name);
      ros::init(argc, nullptr, "g_pcc_encoder_node", ros::init_options::AnonymousName);
    }

    if (enablePreview_ || enablePreviewRecon_ || enablePreviewPreProc_)  {
      nh_ = new ros::NodeHandle();
      if (enablePreview_) {
        ros_pub_ = nh_->advertise<sensor_msgs::PointCloud2>("//kddi_encoder//colorpoints", params_.ros_publish_max_queue_size);
      }
      if (enablePreviewPreProc_) {
        ros_pub_preproc_ = nh_->advertise<sensor_msgs::PointCloud2>("//kddi_encoder//preproc", params_.ros_publish_max_queue_size);
      }
      
      if (enablePreviewRecon_) {
        ros_pub_recon_ = nh_->advertise<sensor_msgs::PointCloud2>("//kddi_encoder//reconstruct", params_.ros_publish_max_queue_size);
        reconIdx_ = 0;
        if (params_.numOfFrameThreads < 2) {
          reconFrame_.resize(1);
        }
        else {
          reconFrame_.resize(params_.numOfFrameThreads);
        }
      }
    }
*/
  }

/** ROS関連は除外 ka-hagiwara
#if defined(ENABLE_DEBUG_ROS_OUTPUT)
  frame_count_ = 0;
  frame_count_preproc_ = 0;
  frame_count_recon_ = 0;
  ros_point_cloud_msg_.height = 1;
#endif // defined(ENABLE_DEBUG_ROS_OUTPUT)
*/
  return (true);
}

namespace {

/** ROS関連は除外 ka-hagiwara
inline int32_t dumpChannelIndex(const sensor_msgs::PointCloud2ConstPtr& cloud)
{
  for (size_t i = 0; i < cloud->fields.size(); ++i)
  {
    std::cout << "name[" << i << "]:" << cloud->fields[i];
  }

  return 0;
}
*/
}
/** ROS関連は除外 ka-hagiwara

void SequenceEncoder::dumpRosTopic(const sensor_msgs::PointCloud2ConstPtr& cloud)
{
  if (params_.dumpROSTopicPath.empty()) {
    return;
  }

  std::string dumpPath{expandNum(params_.dumpROSTopicPath, frameNum)};
  writeRosTopic(dumpPath, cloud.get());
}
*/

/** ROS関連は除外 ka-hagiwara
void SequenceEncoder::processPointCloud2Message(const sensor_msgs::PointCloud2ConstPtr& cloud)
{
  std::cout << "frameid:" << cloud->header.frame_id; // cepton_9779
  std::cout << "[#" << cloud->header.seq;
  std::cout << "] time:" << std::fixed <<  std::setprecision(3) << cloud->header.stamp.toSec();
  std::cout << "] count:" << cloud->width * cloud->height;
  std::cout << ", size:[" << cloud->width << "x" << cloud->height << "]" << std::endl;

  if (0 == frameNum) {
    clock_ros_initial_receive_ = std::chrono::system_clock::now();
  }

  outputDebugLog("receive pcc from ros.");

  if (!params_.dumpROSTopicPath.empty()) {
    dumpRosTopic(cloud);
  }

  if (enablePreview_)  {
    ros_pub_.publish(cloud);
  }

  outputDebugLog("  setPCCPointSet3 - enter.");
  PCCPointSet3 pointCloud;
  bool ret = setPCCPointSet3(cloud, pointCloud);
  outputDebugLog("  setPCCPointSet3 - leave.");

  {
    try
    {
      std::unique_lock<std::mutex> lock(mutex_timestamp_);
      timestamp_.push_back(cloud->header.stamp.toNSec());
    }
    catch(std::exception ex) {
      std::cerr << " !!! [error] " << __PRETTY_FUNCTION__ << ". "
        << ex.what() << std::endl;
      return;
    }
  }

  outputDebugLog("  compressOneFrame - enter.");
  int res = compressOneFrame(clock_user_, pointCloud);
  outputDebugLog("  compressOneFrame - leave.");

  if (0 == res) {
    frameNum++;
  }
  else {
    std::unique_lock<std::mutex> lock(mutex_timestamp_);
    timestamp_.pop_back();
    return;
  }

#if defined(OUTPUT_DETAIL_MESSAGE)
  dumpChannelIndex(cloud);

  int32_t xi = findChannelIndex(cloud.get(), _plyAttrNames.position[0]); // findChannelIndex(cloud, "x");
  int32_t yi = findChannelIndex(cloud.get(), _plyAttrNames.position[1]); // findChannelIndex(cloud, "y");
  int32_t zi = findChannelIndex(cloud.get(), _plyAttrNames.position[2]); // findChannelIndex(cloud, "z");
  int32_t rgbi = findChannelIndex(cloud.get(), "rgb");

  if ((xi < 0) || (yi < 0) || (zi < 0)) {
    // invalid data
    return;
  }

  const uint32_t xoff = cloud->fields[xi].offset;
  const uint32_t yoff = cloud->fields[yi].offset;
  const uint32_t zoff = cloud->fields[zi].offset;
  const uint32_t rgboff = (rgbi<0) ? -1 : cloud->fields[rgbi].offset;
  const uint32_t point_step = cloud->point_step;
  const size_t point_count = cloud->width * cloud->height;

  std::cout << "time:" << cloud->header.stamp;
  std::cout << ", frameid:" << cloud->header.frame_id; // cepton_9779
  std::cout << "seq:" << cloud->header.seq << ", ";

  std::cout << "[x:" << xi << ", y:" << yi << ", z:" << zi
       << "], ofs:[x:" << xoff << ", y:" << yoff << ", z:" << zoff
       << "], ofs:rgb:"<< rgboff << ", region[" << cloud->width << ", " << cloud->height
       << "], count:" << point_count << ", step:" << point_step
       << std::endl;

  const uint8_t *ptr = &cloud->data.front(), *ptr_end = &cloud->data.back();

#if 0
  int count=0;
  for (; (ptr < ptr_end) && (count<8); ptr += point_step)
  {
    float x = *reinterpret_cast<const float*>(ptr + xoff);
    float y = *reinterpret_cast<const float*>(ptr + yoff);
    float z = *reinterpret_cast<const float*>(ptr + zoff);

    if (rgboff < 0) {
      std::cout << "[x:" << x << ", y:" << y << ", z:" << z << "], ";
    }
    else {
      uint32_t rgb = *reinterpret_cast<const uint32_t*>(ptr + rgboff);
      std::cout << "[x:" << x << ", y:" << y << ", z:" << z << ", rgb<" << rgb << ">:(" << (rgb&0xFF) << ", "  << ((rgb>>8)&0xFF) << ", " << ((rgb>>16)&0xFF) << ")], ";
    }
    ++count;
  }
  std::cout << std::endl;
#endif  
#endif // if defined(OUTPUT_DETAIL_MESSAGE)

  outputDebugLog("receive pcc from ros - leave. ----------");
}
*/

/** ROS関連は除外 ka-hagiwara
bool SequenceEncoder::setPCCPointSet3(const sensor_msgs::PointCloud2ConstPtr& cloud, pcc::PCCPointSet3& pointCloud)
{
  int32_t xi = findChannelIndex(cloud.get(), _plyAttrNames.position[0]); // findChannelIndex(cloud, "x");
  int32_t yi = findChannelIndex(cloud.get(), _plyAttrNames.position[1]); // findChannelIndex(cloud, "y");
  int32_t zi = findChannelIndex(cloud.get(), _plyAttrNames.position[2]); // findChannelIndex(cloud, "z");
  int32_t rgbi = findChannelIndex(cloud.get(), "rgb");

  if ((xi < 0) || (yi < 0) || (zi < 0)) {
    // invalid data
    return (false);
  }

  const uint32_t xoff = cloud->fields[xi].offset;
  const uint32_t yoff = cloud->fields[yi].offset;
  const uint32_t zoff = cloud->fields[zi].offset;
  const uint32_t rgboff = (rgbi<0) ? -1 : cloud->fields[rgbi].offset;
  const uint32_t point_step = cloud->point_step;
  const size_t point_count = cloud->width * cloud->height;

  if (point_count * point_step != cloud->data.size())
  {
    return (false);
  }

  if (point_count == 0)
  {
    return (false);
  }
  else {
    bool withColors = (-1 != rgboff);
    bool withReflectances = false; // indexReflectance != PCC_UNDEFINED_INDEX;
    bool withFrameIndex = false; // (-1 < cloud->header.seq); // seq とは別ものかも。要確認。

    pointCloud.addRemoveAttributes(withColors, withReflectances);
    if (withFrameIndex)
      pointCloud.addFrameIndex();
    else
      pointCloud.removeFrameIndex();

    pointCloud.resize(point_count);

    const uint8_t *ptr = &cloud->data.front(), *ptr_end = &cloud->data.back();
    size_t pointCounter = 0;
    int i=0;
    for (; ptr < ptr_end; ptr += point_step) {
      float x = *reinterpret_cast<const float*>(ptr + xoff);
      float y = *reinterpret_cast<const float*>(ptr + yoff);
      float z = *reinterpret_cast<const float*>(ptr + zoff);

      auto& position = pointCloud[pointCounter];
      position[0] = x * params_.inputScale;
      position[1] = y * params_.inputScale;
      position[2] = z * params_.inputScale;

#if 0
if (i<10) {
std::cout << "src #" << i << " [x:" << x << ", y:" << y << ", z:" << z << std::endl;
i++;
}
#endif
      if (0 <= rgboff) {
        pointCloud.getColor(pointCounter)[PCC_COLOR_INDEX_BLUE] = ptr[rgboff + ROS_COLOR_INDEX_BLUE];
        pointCloud.getColor(pointCounter)[PCC_COLOR_INDEX_GREEN] = ptr[rgboff + ROS_COLOR_INDEX_GREEN];
        pointCloud.getColor(pointCounter)[PCC_COLOR_INDEX_RED] = ptr[rgboff + ROS_COLOR_INDEX_RED];
      }
      ++pointCounter;
    }
  }

    return (true);
}
*/

bool SequenceEncoder::initializeSocket()
{
  if (params_.ip_address.empty() || 0 >= params_.ip_port || 65535 < params_.ip_port) {
    // nop
    return (true);
  }

  int inetType = params_.ipv6 ? AF_INET6 : AF_INET;
  void* inetBuf = params_.ipv6 ? (void*)&peer6_.sin6_addr : (void*)&peer_.sin_addr;
  int inet_pton_res = inet_pton(inetType, params_.ip_address.c_str(), inetBuf);

  if (1 != inet_pton_res) {
      cerr << "[socket] invalid address." << std::endl;
      return (false);
  }

  if (params_.ipv6) {
    peer6_.sin6_family = AF_INET6;
    peer6_.sin6_port = htons(params_.ip_port);
  }
  else {
    peer_.sin_family = AF_INET;
    peer_.sin_port = htons(params_.ip_port);
#if 0
    if (0 == peer_.sin_addr.s_addr || 0xFFFFFFFF == peer_.sin_addr.s_addr) {
      cerr << "[socket] invalid address." << std::endl;
      return (false);
    }
#endif
  }

  pcc_sock_ = socket(inetType, SOCK_STREAM, 0);

  if (pcc_sock_ < 0) {
    std::cerr << "failed to create socket :" << pcc_sock_ << ", errno:" << errno << std::endl;
    return (false);
  }

  int r;

  if (params_.ipv6) {
    r = connect(pcc_sock_, (struct sockaddr*)&peer6_, sizeof(peer6_));
  }
  else {
    r = connect(pcc_sock_, (struct sockaddr*)&peer_, sizeof(peer_));
  }

  if (r < 0) {
    std::cerr << "failed to connect. addr:[" << params_.ip_address << "]"
      << ", errno:" << errno
      << ", ipv6:" << params_.ipv6
      << std::endl;
    return (false);
  }

  try{
    th_sender_.thread_ = std::make_shared<std::thread>(&SequenceEncoder::sendThread, this);
  }
  catch(std::exception ex) {
    std::cerr << "failed to create thread." << std::endl;
    std::cerr << " [detail]:" << ex.what();
    return (false);
  }

  return (true);
}

bool SequenceEncoder::sendThread()
{
  std::cout << __PRETTY_FUNCTION__ << " --enter." << std::endl;

  ThreadUtil<std::vector<char>>::DATA tcp_buff;

  while (-1 != pcc_sock_) {
    {
      std::unique_lock<std::mutex> lock(th_sender_.mutex_);
      if (th_sender_.queue_.empty()) {
        th_sender_.cond_.wait(lock);
        if (-1 == pcc_sock_) {
          break;
        }
      }

      if ((th_sender_.queue_.empty()) || 
          (th_sender_.queue_.front().data_.empty())) {
        break;
      }

      tcp_buff.data_ = std::move(th_sender_.queue_.front().data_);
      th_sender_.queue_.pop();
    }

    outputDebugLog("       [th] onOutputBuffer - enter.");
    if (!sendData(tcp_buff.data_.data(), tcp_buff.data_.size())) {
      return (false);
    }
  }

  std::cout << __PRETTY_FUNCTION__ << " --leave." << std::endl;

  return(true);
}

bool SequenceEncoder::fillSendBuff(char type, const char* buf, uint32_t length)
{
  uint32_t headersize = sizeof(type) + sizeof(length);
  uint32_t sendsize = headersize + length;
  char* sendBuff = NULL;

#if defined (ENABLE_SEND_THREAD)
  ThreadUtil<std::vector<char>>::DATA tcp_buff;
#endif // defined (ENABLE_SEND_THREAD)

  try {
#if defined (ENABLE_SEND_THREAD)
  tcp_buff.data_.resize(sendsize);
  sendBuff = &tcp_buff.data_[0];
#else // defined (ENABLE_SEND_THREAD)
  if (sendbuff_.size() < sendsize) {
    sendbuff_.resize(sendsize);
  }
  sendBuff = &sendbuff_[0];
#endif // defined (ENABLE_SEND_THREAD)
  }
  catch(std::exception ex) {
    std::cerr << " !!! [error] " << __PRETTY_FUNCTION__ << ". "
      << ex.what() << std::endl;
    return (false);
  }

  sendBuff[0] = char(type);
  for (int index = 1, shift=24; 0 <= shift; index++, shift-=8){
    sendBuff[index] = char(length >> shift);
  }

#if defined(OUTPUT_DETAIL_MESSAGE)
  std::cout << "send packet. type:" << (unsigned)sendBuff[0] << 
                ", length:" << length << std::endl;
#endif // defined(OUTPUT_DETAIL_MESSAGE)

  memcpy(&sendBuff[headersize], buf, length);

#if defined (ENABLE_SEND_THREAD)
  try
  {
    std::unique_lock<std::mutex> lock(th_sender_.mutex_);
    th_sender_.queue_.push(tcp_buff);
    th_sender_.cond_.notify_one();
  }
  catch(std::exception ex) {
    std::cerr << " !!! [error] " << __PRETTY_FUNCTION__ << ". "
      << ex.what() << std::endl;
    return (false);
  }
#else // defined (ENABLE_SEND_THREAD)
  if (!sendData(&sendBuff[0], sendsize)) {
    return (false);
  }
#endif // defined (ENABLE_SEND_THREAD)
  return (true);  
}

bool SequenceEncoder::fillSendBuff(const pcc::PayloadBuffer& buf)
{
  if (pcc::PayloadType::kGeometryBrick == buf.type) {
    uint64_t timestamp = 0LL;
    char buff[sizeof(timestamp)];
    {
      std::unique_lock<std::mutex> lock(mutex_timestamp_);
      if (!timestamp_.empty()) {
        timestamp = timestamp_.front();
        timestamp_.pop_front();
      }
    }
    for (int index = 0, shift=56; 0 <= shift; index++, shift-=8){
      buff[index] = char(timestamp >> shift);
    }

    if (!fillSendBuff (char(pcc::PayloadType::kGeometryBrick) | 0x80, &buff[0], uint32_t(sizeof(buff)))) {
      return (false);
    }
  }

  if (!fillSendBuff(char(buf.type), buf.data(), uint32_t(buf.size()))) {
    return (false);
  }

  return(true);
}

bool SequenceEncoder::sendData(void*data, size_t length)
{
  char* pdata = static_cast<char*>(data);
  size_t sendsize = params_.mtu_size < length ? params_.mtu_size : length;
  size_t totalsendsize = 0;

#if defined(OUTPUT_DETAIL_MESSAGE)
  cout << "[socket] send - enter. length:" << length << " -----" << std::endl;
#endif // defined(OUTPUT_DETAIL_MESSAGE)

  output_byte_total_ += length;
  output_byte_max_ = max(output_byte_max_, length);
  output_byte_min_ = min(output_byte_min_, length);

  while ((-1 != pcc_sock_) && (totalsendsize < length)) {
    if (-1 == send(pcc_sock_, &pdata[totalsendsize], sendsize, 0)) {
      return(false);
    }
#if defined(ENABLE_DUMP_BIN)
    dumpSendStream.write(&pdata[totalsendsize], sendsize);
#endif // defined(ENABLE_DUMP_BIN)

    totalsendsize += sendsize;
    if (length - totalsendsize < params_.mtu_size) {
      sendsize = length - totalsendsize;
    }
  }

#if defined(ENABLE_DUMP_BIN)
  dumpSendStream.flush();
#endif // defined(ENABLE_DUMP_BIN)

#if defined(OUTPUT_DETAIL_MESSAGE)
  cout << "[socket] send - leave. -----" << std::endl;
#endif // defined(OUTPUT_DETAIL_MESSAGE)

  return (true);
}

//----------------------------------------------------------------------------
std::vector<std::string>* SequenceEncoder::ret_files() {
  return &files_xy;
}
void SequenceEncoder::ret_files_init(int n){

  files_xy.resize(n);
}
void SequenceEncoder::ret_files_clear(){
  files_xy.clear();
}
int SequenceEncoder::ret_files_size(){
  return files_xy.size();
}

const char * SequenceEncoder::watchPath(void)
{
  const char *tmp;
  tmp=params_.plydir.c_str();
  return tmp;
}

int SequenceEncoder::getlist(DIR *dir)
{
    struct dirent *diread;
    candidateList.clear();

    while ((diread = readdir(dir)) != nullptr) {
      std::string tmpst= diread->d_name;
      if(tmpst.find(".ply") != std::string::npos) {
        candidateList.push_back(tmpst);
            #ifdef msd_debug
            cout << "tmpst :" << tmpst <<" size :" << elist.size() << endl;
            #endif
      }
    }
    closedir(dir);
    return candidateList.size();
}
//----------------------------------------------------------------------------

int SequenceEncoder::mvEncoded_(string& fileName,string& pathorg, string& encodedpath)
{
    cout << "fileName :" << fileName << " path " << pathorg << " encoded path :" << encodedpath << endl;
    cout << "src :" << pathorg+fileName << " dst :" << pathorg+encodedpath+fileName << endl;
    int ret = rename((pathorg+fileName).c_str(),(pathorg+encodedpath+fileName).c_str());
    
    cout << "ret :" << ret << endl;

    return 0;
}
//----------------------------------------------------------------------------

int SequenceEncoder::compress(Stopwatch* clock)
{
  //#define msd_debug
  DIR *dir,*encoded_dir;
  struct dirent *diread;  
  vector<string> elist,filestmp;

  //watch start
  //SequenceEncoder::startWatchFile();
  //thread th([this]() { this->startWatchFile(); });
  //th.join();
  //fileWatcher();
  //fileWatcher_();
  string pathply = params_.plydir;
  string pathencoded= pathply + "encoded_dir/";
  string renamepath = "encoded_dir/";
  
  if ((dir = opendir(pathply.c_str())) != nullptr) {   
    if( (encoded_dir = opendir(pathencoded.c_str())) == nullptr ) {
      
      if(mkdir(pathencoded.c_str(), 0777)==0){
        std::cout << " sucees " << std::endl; 
      }
      else{
        cout << "makedir fail :" << pathencoded << endl;
        closedir(dir);
        closedir(encoded_dir);
        return 0;
      }
    }
    else {
      closedir(encoded_dir);
    }
    elist.clear();
    while ((diread = readdir(dir)) != nullptr) {
      std::string tmpst= diread->d_name;
      if(tmpst.find(".ply") != std::string::npos) {
        elist.push_back(tmpst);
            #ifdef msd_debug
            cout << "tmpst :" << tmpst <<" size :" << elist.size() << endl;
            #endif
      }
    }
    closedir(dir);
  } else {
    perror("opendir");
    return EXIT_FAILURE;
  }
  //only first dir, sort
  std::sort(elist.begin(),elist.end());

  while(1) {
    while(params_.continousEncoding) {//for wait new ply file
      if( (elist.size()<=params_.MaxExecutedListSize) && (elist.size()>0) ) {
        break;
      }
      //std::cout << " ## elist.size == 0 " << std::endl;

      elist.clear();
      if ((dir = opendir(pathply.c_str())) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
          std::string tmpst= diread->d_name;
          if(tmpst.find(".ply") != std::string::npos) {
            elist.push_back(tmpst);
              #ifdef msd_debug
              cout << "tmpst :" << tmpst <<" size :" << elist.size() << endl;
              #endif
          }
        }
      }
      else {
        //std::cout << " ## new file not comming.. " << std::endl;        
      }
      closedir(dir);

      if(elist.size()>0)  {
        break;
      }
      else {
        //cout << "sleep.." <<std::endl;
        usleep(params_.sleepWaitNextPly * 1000);  //sleep and wake up from file add event
      }
    }
    if(elist.size()>0) 
      cout <<"###"<< elist.front() << " : current file " << elist.size()  <<endl;
    else{
      cout <<"### elist.size()==0"  <<endl;
      if(params_.continousEncoding)
        continue;
      else
        break;
    }    
    string cfile = elist.front();
    elist.erase(elist.begin());
        
    PCCPointSet3 pointCloud;
    string srcName  =  params_.plydir.c_str() + cfile;
    if (!ply::read(srcName, _plyAttrNames, params_.inputScale, pointCloud)) {
      cerr << "Error: can't open input file! :" << srcName << endl;
      return -1;
    }

    mvEncoded_(cfile,pathply,renamepath);

    if (compressOneFrame(clock, pointCloud)) {
      cerr << "Error: compressOneFrame!" << endl;
      return -1;
    }

    // 入力によってはエンコーダが即終了し、socket 通信先のデコーダの挙動に影響が出るため、
    // キューが一定数溜まったらスリープする。
    // todo: ファイル入力モード時の socket 通信の終了手順見直し
    while (interval_for_fileinput_) {
      //cout << "interval_for_fileinput_" << endl;
      try
      {
        std::unique_lock<std::mutex> lock(th_sender_.mutex_);
        if (th_sender_.queue_.size() < 8) {
          break;
        }
        th_sender_.cond_.notify_one();
      }
      catch(std::exception ex) {
        std::cerr << " !!! [error] " << __PRETTY_FUNCTION__ << ". "
        << ex.what() << std::endl;
        // nop
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_for_fileinput_));
    }
  }
  cout << "compress - end proc." << endl;
#ifdef DO_PARALLEL_CODING
  //cout << "DO_PARALLEL_CODING" << endl;
  // 並列エンコーダの終端処理
  // このメソッドを呼ばないと、最後までエンコードしたストリームが出力されません
  if (false == endOfParallelEncoding(clock)) {
    cout << "endOfParallelEncoding(clock))" << endl;
    return -1;
  }
#endif // DO_PARALLEL_CODING

  //exit from while loop
  if (bytestreamFile.is_open()) {
    std::cout << "Total bitstream size " << bytestreamFile.tellp() << " B\n";
    bytestreamFile.close();
  }

  return 0;
}
//----------------------------------------------------------------------------

int SequenceEncoder::compressOneFrame(Stopwatch* clock, pcc::PCCPointSet3& pointCloud)
{
  if (pointCloud.getPointCount() == 0) {
    cout << "Error: can't open input file!" << endl;
    return -1;
  }

  // Some evaluations wish to scan the points in azimuth order to simulate
  // real-time acquisition (since the input has lost its original order).
  // NB: because this is trying to emulate the input order, binning is disabled
  if (params_.sortInputByAzimuth)
    sortByAzimuth(
      pointCloud, 0, pointCloud.getPointCount(), 0., _angularOrigin);

  // Sanitise the input point cloud
  // todo(df): remove the following with generic handling of properties
  bool codeColour = encoderParams_.attributeIdxMap.count("color");
  if (!codeColour)
    pointCloud.removeColors();
  assert(codeColour == pointCloud.hasColors());

  bool codeReflectance = encoderParams_.attributeIdxMap.count("reflectance");
  if (!codeReflectance)
    pointCloud.removeReflectances();
  assert(codeReflectance == pointCloud.hasReflectances());

  clock->start();

  if (params_.convertColourspace)
    convertFromGbr(encoderParams_.sps.attributeSets, pointCloud);

  scaleAttributesForInput(encoderParams_.sps.attributeSets, pointCloud);

#ifdef DO_PARALLEL_CODING
  auto bytestreamLenFrameStart = bytestreamFile.tellp();

  input_point_total_+= pointCloud.getPointCount();
  input_point_max_ = max(input_point_max_, pointCloud.getPointCount());
  input_point_min_ = min(input_point_min_, pointCloud.getPointCount());

  if (nullptr != encoder) {
    // The reconstructed point cloud
    CloudFrame recon;
    auto* reconPtr = params_.reconstructedDataPath.empty() ? nullptr : &recon;
    outputDebugLog("    encoder.compress - enter.");

/** ROS関連は除外
#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
    if (enablePreviewPreProc_)  {
      publishPointCloud2(pointCloud, ros_pub_preproc_);
   }
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
*/

#if !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
    int ret = encoder->compress(pointCloud, &encoderParams_, this, reconPtr);
#else // if !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
    int ret = encoder->compress(pointCloud, &encoderParams_, this, &recon);
#endif // !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)

    if (ret) {
      cout << "Error: can't compress point cloud!" << endl;
      return -1;
    }
    outputDebugLog("    encoder.compress - leave.");

    if (bytestreamFile.is_open()) {
      auto bytestreamLenFrameEnd = bytestreamFile.tellp();
      int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;
      std::cout << "Total frame size " << frameLen << " B" << std::endl;
    }
    clock->stop();

/** ROS関連は除外 ka-hagiwara
#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
    if (enablePreviewRecon_)  {
      publishPointCloud2(recon, ros_pub_recon_);
    }
#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
*/

    if (reconPtr)
      writeOutputFrame(params_.reconstructedDataPath, {}, recon, recon.cloud);
  }
  else {
    // The reconstructed point cloud
    CloudFrame* recon = &reconFrame_[reconIdx_];
    reconIdx_ = (reconIdx_+1) % params_.numOfFrameThreads;
    auto** reconPtr = (params_.reconstructedDataPath.empty() && !enablePreviewRecon_)? nullptr : &recon;
    outputDebugLog("    encoder.compress[parallel] - enter.");

/** ROS関連は除外 ka-hagiwara
#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
    if (enablePreviewPreProc_)  {
      publishPointCloud2(pointCloud, ros_pub_preproc_);
    }
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
*/

    int ret = parallelEncoder->compress(&pointCloud, &encoderParams_, this, reconPtr);
    if (ret) {
      cout << "Error: can't compress point cloud!" << endl;
      return -1;
    }
    outputDebugLog("    encoder.compress - leave.");

    if (bytestreamFile.is_open()) {
      auto bytestreamLenFrameEnd = bytestreamFile.tellp();
      int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;

      if (0 != frameLen) {
        std::cout << "Total frame size " << frameLen << " B" << std::endl;
      }
    }

    clock->stop();

    if (nullptr != reconPtr && nullptr != recon) {
/** ROS関連は除外 ka-hagiwara
#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
      if (enablePreviewRecon_)  {
        publishPointCloud2(*recon, ros_pub_recon_);
      }
#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
*/

      writeOutputFrame(params_.reconstructedDataPath, {}, *recon, recon->cloud);
    }
  }
#else // !DO_PARALLEL_CODING
  // The reconstructed point cloud
  CloudFrame recon;
  auto* reconPtr = params_.reconstructedDataPath.empty() ? nullptr : &recon;

  auto bytestreamLenFrameStart = bytestreamFile.tellp();

  input_point_total_+= pointCloud.getPointCount();
  input_point_max_ = max(input_point_max_, pointCloud.getPointCount());
  input_point_min_ = min(input_point_min_, pointCloud.getPointCount());

  outputDebugLog("    encoder.compress - enter.");

#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
  if (enablePreviewPreProc_)  {
    publishPointCloud2(pointCloud, ros_pub_preproc_);
  }
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)

#if !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
  int ret = encoder.compress(pointCloud, &encoderParams_, this, reconPtr);
#else //if !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
  int ret = encoder.compress(pointCloud, &encoderParams_, this, &recon);
#endif // !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)

  if (ret) {
    cout << "Error: can't compress point cloud!" << endl;
    return -1;
  }
  outputDebugLog("    encoder.compress - leave.");

  if (bytestreamFile.is_open()) {
    auto bytestreamLenFrameEnd = bytestreamFile.tellp();
    int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;
    std::cout << "Total frame size " << frameLen << " B" << std::endl;
  }

  clock->stop();

#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
  if (enablePreviewRecon_)  {
    publishPointCloud2(recon, ros_pub_recon_);
  }
#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)

  if (reconPtr)
    writeOutputFrame(params_.reconstructedDataPath, {}, recon, recon.cloud);
#endif // !DO_PARALLEL_CODING

  return 0;
}

//----------------------------------------------------------------------------
void
SequenceEncoder::onOutputBuffer(const PayloadBuffer& buf)
{
  outputDebugLog("      onOutputBuffer - enter.");

  if (bytestreamFile.is_open()) {
    writeTlv(buf, bytestreamFile);
  }

  if (-1 != pcc_sock_) {
    if (!fillSendBuff(buf)) {
      outputDebugLog("      onOutputBuffer - error.");
    }
  }

  outputDebugLog("      onOutputBuffer - leave.");
}

//----------------------------------------------------------------------------
void
SequenceEncoder::onPostRecolour(const PCCPointSet3& cloud)
{
  if (params_.postRecolorPath.empty()) {
    return;
  }

  PCCPointSet3 tmpCloud(cloud);
  CloudFrame frame;
  frame.setParametersFrom(encoderParams_.sps, encoderParams_.outputFpBits);
  frame.cloud = cloud;
  frame.frameNum = frameNum - params_.firstFrameNum;

  writeOutputFrame(params_.postRecolorPath, {}, frame, tmpCloud);
}

void SequenceEncoder::print_measure_info(double proc_sec)
{
  std::cout << "process time: " << proc_sec << "[millsec]" << std::endl;
  int bitrate = (output_byte_total_ * 8)*1000. / proc_sec;
  std::cout << "bitrate: " << bitrate << "[bps], " << frameNum << "[frames]" << std::endl;

  std::cout << "packet total:" << output_byte_total_ <<
               "[Bytes], average:" << output_byte_total_ / frameNum <<
               ", min:" << output_byte_min_ <<
               ", max:" << output_byte_max_ << std::endl;

  std::cout << "point total:" << input_point_total_ <<
               "[points], average:" << input_point_total_ / frameNum <<
               ", min:" << input_point_min_ <<
               ", max:" << input_point_max_ << std::endl;
}
//============================================================================

inline void SequenceEncoder::outputDebugLog(const std::string& msg)
{
  if (params_.debugLogPath.empty()) {
    return;
  }
  
  if (!debugLog_.is_open()) {
    debugLog_.open(params_.debugLogPath, std::ofstream::out);
    if (!debugLog_.is_open()) {
      return;
    }
  }

  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  auto now_msec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  auto time = now - clock_ros_initial_receive_;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(time).count();
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time).count();;

  debugLog_ << std::put_time(std::localtime(&now_time_t), "%Y/%m/%d %T.") 
             << setw(3) <<setfill('0') << (now_msec % 1000)
             << " [" << setw(2) <<setfill('0') << minutes
             << ":" << setw(2) <<setfill('0') << (sec % 60)
             << "." << setw(3) <<setfill('0') << (msec % 1000) <<  "] "
             << msg << std::endl;

}


#ifdef DO_PARALLEL_CODING
bool SequenceEncoder::endOfParallelEncoding(Stopwatch* clock)
{
  if (nullptr != parallelEncoder) {
    int ret = 0;
    
    do {
      clock->start();
      // 終端処理
      auto bytestreamLenFrameStart = bytestreamFile.tellp();
      // The reconstructed point cloud
      CloudFrame* recon = nullptr;
      auto** reconPtr = params_.reconstructedDataPath.empty() ? nullptr : &recon;
      ret = parallelEncoder->compress(nullptr, nullptr, this, reconPtr);
      if (0 > ret) {
        cout << "Error: can't compress point cloud!" << endl;
        return(false);
      }

      if (bytestreamFile.is_open()) {
        auto bytestreamLenFrameEnd = bytestreamFile.tellp();
        int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;

        if (0 != frameLen) {
          std::cout << "Total frame size " << frameLen << " B" << std::endl;
        }
      }

      clock->stop();

      if (nullptr != reconPtr && nullptr != recon) {
        writeOutputFrame(params_.reconstructedDataPath, {}, *recon, recon->cloud);
      }
    } while (0 == ret);
  }

  return(true);
}
#endif // DO_PARALLEL_CODING

#if 0
----------
topic:"//kddi_cap//colorpoints"
frameid:"cepton_9779"
  input_sub = nh.subscribe("//kddi_cap//colorpoints", 10, &SequenceEncoder::processPointCloud2Message, this);
  name[0]:name: x, offset: 0, datatype: 7, count: 1
  name[1]:name: y, offset: 4, datatype: 7, count: 1
  name[2]:name: z, offset: 8, datatype: 7, count: 1
  name[3]:name: rgb, offset: 16, datatype: 7, count: 1
  time:1663047373.785158000, frameid:cepton_9779, seq:2397, [x:0, y:1, z:2], ofs:[x:0, y:4, z:8], ofs:rgb:16, region[31728, 1], count:31728, step:32
  [x:-2.37075, y:5.28036, z:2.16191], [x:-1.16186, y:5.60815, z:2.12215], [x:0.0762649, y:5.6987, z:2.08547], [x:1.30314, y:5.62942, z:2.04878], [x:0, y:0, z:0], [x:0.995952, y:1.28962, z:0.521375], [x:1.02356, y:0.852492, z:0.393686], [x:-2.37378, y:3.36014, z:0.538355], 
  rgb<4291941320>:(200, 211, 209)]

----------
topic:"//cepton//points"
frameid:"cepton_9779"
  input_sub = nh.subscribe("//cepton//points", 10, &SequenceEncoder::processPointCloud2Message, this);
  name[0]:timestamp,
  name[1]:image_x,
  name[2]:distance,
  name[3]:image_z,
  name[4]:intensity,
  name[5]:return_type,
  name[6]:flags,
  name[7]:x, name[8]:y, name[9]:z
  time:1663047374.868750000, frameid:cepton_9779, seq:0, [x:7, y:8, z:9], ofs:[x:28, y:32, z:36], 
  region[31968, 1], count:31968, step:40, [x:-2.40665, y:3.36656, z:0.552993], 

----------
topic:"//livox//lidar"
frameid:"livox_frame"
  input_sub = nh.subscribe("//livox//lidar", 10, &SequenceEncoder::processPointCloud2Message, this);
  name[0]:x, name[1]:y, name[2]:z
  name[3]:intensity
  name[4]:tag
  name[5]:line
  time:763.100497140, frameid:livox_frame, seq:2464, [x:0, y:1, z:2], ofs:[x:0, y:4, z:8], region[24000, 1], count:24000, step:18
  [x:1.585, y:-0.11, z:-0.183], [x:1.598, y:-0.114, z:-0.159], [x:1.617, y:-0.117, z:-0.136], [x:1.643, y:-0.122, z:-0.112], [x:1.645, y:-0.125, z:-0.086], [x:1.668, y:-0.129, z:-0.061], [x:1.584, y:-0.122, z:-0.183], [x:1.597, y:-0.125, z:-0.159], 

----------
topic:"//velodyne_points"
frameid:"velodyne"
  input_sub = nh.subscribe("//velodyne_points", 10, &SequenceEncoder::processPointCloud2Message, this);
  name[0]:x, name[1]:y, name[2]:z
  name[3]:intensity
  name[4]:ring
  name[5]:time
  time:1663047371.646978855, frameid:velodyne, seq:7, [x:0, y:1, z:2], ofs:[x:0, y:4, z:8], region[16659, 1], count:16659, step:22
  [x:2.19942, y:-1.07844, z:-0.656365], [x:2.23926, y:-1.09894, z:-0.575875], [x:2.25717, y:-1.10822, z:0.131782], [x:2.07775, y:-1.02013, z:-0.449928], [x:2.23001, y:-1.09537, z:0.217366], [x:2.07427, y:-1.01932, z:-0.366057], [x:2.20543, y:-1.08425, z:0.301749], [x:2.06096, y:-1.01367, z:-0.282006], 
#endif

/** ROS関連は除外 ka-hagiwara
#if defined(ENABLE_DEBUG_ROS_OUTPUT)

bool SequenceEncoder::initPointCloud2(const pcc::PCCPointSet3& pointCloud, pcc::AxisOrder geometryAxisOrder)
{
  if (0 < frame_count_) {
    return (false);
  }

  static const uint8_t XYZ_TYPE = sensor_msgs::PointField::FLOAT32;
  static const size_t XYZ_SIZE = 4;
  int fieldSize = 3;
  uint32_t point_step = XYZ_SIZE * fieldSize;

  ros_rgbidx_ = 0;
  ros_rgbofs_ = 0;
  ros_refcidx_ = 0;
  ros_refcofs_ = 0;
  ros_frameiidx_ = 0;
  ros_frameiofs_ = 0;

  if (pointCloud.hasColors()) {
    ros_rgbidx_ = fieldSize;
    fieldSize++;
    ros_rgbofs_ = point_step;
    point_step += 4;
  }
  if (pointCloud.hasReflectances()) {
    ros_refcidx_ = fieldSize;
    fieldSize++;
    ros_refcofs_ = point_step;
    point_step += 2;
    // fout << "property uint16 refc" << std::endl;
  }
  if (pointCloud.hasFrameIndex()) {
    ros_frameiidx_ = fieldSize;
    fieldSize++;
    ros_frameiofs_ = point_step;
    point_step += 2;
    // fout << "property uint8 frameindex" << std::endl;
  }

  ros_point_cloud_msg_.fields.resize(fieldSize);

  ros_point_cloud_msg_.point_step = point_step;
  ros_point_cloud_msg_.header.frame_id = "cepton_9779"; // params_.ros_frameid.c_str();
  ros_point_cloud_msg_.is_dense = false;
  ros_point_cloud_msg_.is_bigendian = false;

  ros_point_cloud_msg_.fields[0].datatype =
    ros_point_cloud_msg_.fields[1].datatype =
    ros_point_cloud_msg_.fields[2].datatype = XYZ_TYPE;
  ros_point_cloud_msg_.fields[0].count =
    ros_point_cloud_msg_.fields[1].count =
    ros_point_cloud_msg_.fields[2].count = 1;

  ros_point_cloud_msg_.fields[0].offset = 0;
  ros_point_cloud_msg_.fields[1].offset = ros_point_cloud_msg_.fields[0].offset + XYZ_SIZE;
  ros_point_cloud_msg_.fields[2].offset = ros_point_cloud_msg_.fields[1].offset + XYZ_SIZE;

  switch (geometryAxisOrder) {
    case pcc::AxisOrder::kZYX:
    case pcc::AxisOrder::kZYX_4:
      ros_point_cloud_msg_.fields[0].name = "z"; ros_point_cloud_msg_.fields[1].name = "y"; ros_point_cloud_msg_.fields[2].name = "x";
      break;
    case pcc::AxisOrder::kXZY:
      ros_point_cloud_msg_.fields[0].name = "x"; ros_point_cloud_msg_.fields[1].name = "z"; ros_point_cloud_msg_.fields[2].name = "y";
      break;
    case pcc::AxisOrder::kYZX:
      ros_point_cloud_msg_.fields[0].name = "y"; ros_point_cloud_msg_.fields[1].name = "z"; ros_point_cloud_msg_.fields[2].name = "x";
      break;
    case pcc::AxisOrder::kZXY:
      ros_point_cloud_msg_.fields[0].name = "z"; ros_point_cloud_msg_.fields[1].name = "x"; ros_point_cloud_msg_.fields[2].name = "y";
      break;
    case pcc::AxisOrder::kYXZ:
      ros_point_cloud_msg_.fields[0].name = "y"; ros_point_cloud_msg_.fields[1].name = "x"; ros_point_cloud_msg_.fields[2].name = "z";
      break;
    case pcc::AxisOrder::kXYZ:
    case pcc::AxisOrder::kXYZ_7:
    default:
      ros_point_cloud_msg_.fields[0].name = "x"; ros_point_cloud_msg_.fields[1].name = "y"; ros_point_cloud_msg_.fields[2].name = "z";
      break;
  }

  if (0 < ros_rgbofs_) {
    ros_point_cloud_msg_.fields[ros_rgbidx_].name = "rgb";
    ros_point_cloud_msg_.fields[ros_rgbidx_].offset = ros_rgbofs_;
    ros_point_cloud_msg_.fields[ros_rgbidx_].datatype = sensor_msgs::PointField::UINT32;
    ros_point_cloud_msg_.fields[ros_rgbidx_].count = 1;
  }

  if (0 < ros_refcofs_) {
    ros_point_cloud_msg_.fields[ros_refcidx_].name = "refc";
    ros_point_cloud_msg_.fields[ros_refcidx_].offset = ros_refcofs_;
    ros_point_cloud_msg_.fields[ros_refcidx_].datatype = sensor_msgs::PointField::UINT16;
    ros_point_cloud_msg_.fields[ros_refcidx_].count = 1;
  }

  if (0 < ros_frameiofs_) {
    ros_point_cloud_msg_.fields[ros_frameiidx_].name = "index";
    ros_point_cloud_msg_.fields[ros_frameiidx_].offset = ros_frameiofs_;
    ros_point_cloud_msg_.fields[ros_frameiidx_].datatype = sensor_msgs::PointField::UINT16;
    ros_point_cloud_msg_.fields[ros_frameiidx_].count = 1;
  }

  return(true);
}
#endif // defined(ENABLE_DEBUG_ROS_OUTPUT)

#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
bool SequenceEncoder::publishPointCloud2(const pcc::CloudFrame& frame, ros::Publisher& ros_publisher)
{
  if (!ros::ok()) {
    std::cerr << "[publishPointCloud2] ros:ok return false." << std::endl;
    return(false);
  }

  outputDebugLog("    setPointCloud2 -enter.");
  sensor_msgs::PointCloud2 ros_point_cloud = ros_point_cloud_msg_;
  setPointCloud2(frame, ros_point_cloud);
  outputDebugLog("    setPointCloud2 -leave.");

  outputDebugLog("    ros_pub_.publish -enter.");
  ros_publisher.publish(ros_point_cloud);
  outputDebugLog("    ros_pub_.publish -leave.");
  // ros::spinOnce();
  // loop_rate.sleep();
  return (true);
}

bool SequenceEncoder::setPointCloud2(const pcc::CloudFrame& frame, sensor_msgs::PointCloud2& ros_point_cloud)
{
  pcc::PCCPointSet3 pointCloud(frame.cloud);

  if (!params_.dumpUncompressedDataPath.empty()) {
    writeOutputFrame(
      params_.dumpUncompressedDataPath, params_.preInvScalePath, frame, pointCloud);
  }
  else {
    // dumpUncompressedDataPath で出力する場合、writeOutputFrame内で行われる処理。
    scaleAttributesForOutput(frame.attrDesc, pointCloud);

    if (params_.convertColourspace) {
      convertToGbr(frame.attrDesc, pointCloud);
    }
  }

  if (0 == frame_count_) {
    initPointCloud2(pointCloud, frame.geometry_axis_order);
    frame_count_++;  
  }

  outputDebugLog("      setPointCloud2 -start.");
  const size_t pointCount = pointCloud.getPointCount();
  ros_point_cloud.header.seq = frame_count_recon_;
  ros_point_cloud.header.stamp = ros::Time::now();
  ros_point_cloud.width = pointCount;
  size_t dataLen = pointCount * ros_point_cloud_msg_.point_step;
  ros_point_cloud.data.resize(dataLen);

  frame_count_recon_++;

  size_t xyzofs[] = { ros_point_cloud_msg_.fields[0].offset,
                      ros_point_cloud_msg_.fields[1].offset,
                      ros_point_cloud_msg_.fields[2].offset};

  double positionScale = outputScale(frame) / (1 << frame.outputFpBits);
  Vec3<double> positionOffset = outputOrigin(frame);

  for (size_t i = 0; i < pointCount; ++i) {
    Vec3<double> position = (pointCloud[i] + positionOffset) / params_.inputScale; //* positionScale + positionOffset;

    // uint8_t* pdst = &ros_point_cloud.data[0] + i * ros_point_cloud_msg_.point_step;
    uint8_t* pdst = &ros_point_cloud.data[i * ros_point_cloud_msg_.point_step];
    *(float*)(&pdst[xyzofs[0]]) = position[0];
    *(float*)(&pdst[xyzofs[1]]) = position[1];
    *(float*)(&pdst[xyzofs[2]]) = position[2];
#if 0
if (i<10) {
  if (i==0) {
    std::cout << "positionScale:" << positionScale << ", positionOffset:[" << positionOffset[0] <<  
    ", " << positionOffset[1] << "," << positionOffset[2] << "]" << 
    ", inputScale:" << params_.inputScale << std::endl;
  }
std::cout << "#" << i << " org:[x:" << pointCloud[i].x() << ", y:" << pointCloud[i].y() << ", z:" << pointCloud[i].z() << std::endl;
std::cout << "  cvt:[x:" << position[0] << ", y:" << position[1] << ", z:" << position[2] << std::endl;
}
#endif
    if (pointCloud.hasColors()) {
      const Vec3<attr_t>& c = pointCloud.getColor(i);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_BLUE]  = uint8_t(c[PCC_COLOR_INDEX_BLUE]);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_GREEN] = uint8_t(c[PCC_COLOR_INDEX_GREEN]);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_RED]   = uint8_t(c[PCC_COLOR_INDEX_RED]);
    }

    if (pointCloud.hasReflectances()) {
      *(uint16_t*)&pdst[ros_refcofs_] = pointCloud.getReflectance(i);
    }

    if (pointCloud.hasFrameIndex()) {
      *(uint16_t*)&pdst[ros_frameiofs_] = pointCloud.getFrameIndex(i);
    }
  }

  outputDebugLog("      setPointCloud2 -end.");

  return(true);
}

#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)

#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
bool SequenceEncoder::publishPointCloud2(const pcc::PCCPointSet3& pointCloud, ros::Publisher& ros_publisher)
{
  if (!ros::ok()) {
    std::cerr << "[publishPointCloud2] ros:ok return false." << std::endl;
    return(false);
  }

  outputDebugLog("    setPointCloud2 -enter.");
  sensor_msgs::PointCloud2 ros_point_cloud = ros_point_cloud_msg_;
  setPointCloud2(pointCloud, ros_point_cloud);
  outputDebugLog("    setPointCloud2 -leave.");

  outputDebugLog("    ros_pub_.publish -enter.");
  ros_publisher.publish(ros_point_cloud);
  outputDebugLog("    ros_pub_.publish -leave.");

  // ros::spinOnce();
  // loop_rate.sleep();
  return (true);
}

bool SequenceEncoder::setPointCloud2(const pcc::PCCPointSet3& pointCloud, sensor_msgs::PointCloud2& ros_point_cloud)
{
#if 0  
  pcc::PCCPointSet3 pointCloud(frame.cloud);

  if (!params_.dumpUncompressedDataPath.empty()) {
    writeOutputFrame(
      params_.dumpUncompressedDataPath, params_.preInvScalePath, frame, pointCloud);
  }
  else {
    // dumpUncompressedDataPath で出力する場合、writeOutputFrame内で行われる処理。
    scaleAttributesForOutput(frame.attrDesc, pointCloud);

    if (params_.convertColourspace) {
      convertToGbr(frame.attrDesc, pointCloud);
    }
  }
#endif

  if (0 == frame_count_) {
    pcc::AxisOrder geometryAxisOrder = pcc::AxisOrder::kXYZ;
    initPointCloud2(pointCloud, geometryAxisOrder);
    frame_count_++;
  }

  outputDebugLog("      setPointCloud2 -start.");
  const size_t pointCount = pointCloud.getPointCount();
  ros_point_cloud.header.seq = frame_count_preproc_;
  ros_point_cloud.header.stamp = ros::Time::now();
  ros_point_cloud.width = pointCount;
  size_t dataLen = pointCount * ros_point_cloud_msg_.point_step;
  ros_point_cloud.data.resize(dataLen);

  frame_count_preproc_++;

  size_t xyzofs[] = { ros_point_cloud_msg_.fields[0].offset,
                      ros_point_cloud_msg_.fields[1].offset,
                      ros_point_cloud_msg_.fields[2].offset};

//  double positionScale = outputScale(frame) / (1 << frame.outputFpBits);
//  Vec3<double> positionOffset = outputOrigin(frame);

  for (size_t i = 0; i < pointCount; ++i) {
    Vec3<double> position = pointCloud[i]  / params_.inputScale; //* positionScale + positionOffset;

    // uint8_t* pdst = &ros_point_cloud.data[0] + i * ros_point_cloud_msg_.point_step;
    uint8_t* pdst = &ros_point_cloud.data[i * ros_point_cloud_msg_.point_step];
    *(float*)(&pdst[xyzofs[0]]) = position[0];
    *(float*)(&pdst[xyzofs[1]]) = position[1];
    *(float*)(&pdst[xyzofs[2]]) = position[2];
    if (pointCloud.hasColors()) {
      const Vec3<attr_t>& c = pointCloud.getColor(i);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_BLUE]  = uint8_t(c[PCC_COLOR_INDEX_BLUE]);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_GREEN] = uint8_t(c[PCC_COLOR_INDEX_GREEN]);
      pdst[ros_rgbofs_ + ROS_COLOR_INDEX_RED]   = uint8_t(c[PCC_COLOR_INDEX_RED]);
    }

    if (pointCloud.hasReflectances()) {
      *(uint16_t*)&pdst[ros_refcofs_] = pointCloud.getReflectance(i);
    }

    if (pointCloud.hasFrameIndex()) {
      *(uint16_t*)&pdst[ros_frameiofs_] = pointCloud.getFrameIndex(i);
    }
  }

  outputDebugLog("      setPointCloud2 -end.");

  return(true);
}
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
*/
