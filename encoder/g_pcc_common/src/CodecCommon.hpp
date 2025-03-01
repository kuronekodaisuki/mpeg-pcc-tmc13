#pragma once

#define _CRT_SECURE_NO_WARNINGS

// 並列エンコード/デコードを行うためのプリプロセッサマクロ
#define DO_PARALLEL_CODING

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <queue>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <exception>

////#include <ros/ros.h>
////#include <sensor_msgs/PointCloud2.h>

#include "TMC3Config.h"
#include "pcc_chrono.h"

#include "hls.h"

namespace pcc {
struct CloudFrame;
class PCCPointSet3;
}

namespace df{
  namespace program_options_lite{
    class OptionSpecific;
  }
}

struct Parameters;

typedef pcc::chrono::Stopwatch<pcc::chrono::utime_inc_children_clock>
  Stopwatch;


//============================================================================
enum class OutputSystem
{
  // Output after global scaling, don't convert to external system
  kConformance = 0,

  // Scale output to external coordinate system
  kExternal = 1,
};

//----------------------------------------------------------------------------
struct Parameters {
  bool isDecoder;

  // command line parsing should adjust dist2 values according to PQS
  bool positionQuantizationScaleAdjustsDist2;

  // Scale factor to apply when loading the ply before integer conversion.
  // Eg, If source point positions are in fractional metres converting to
  // millimetres will allow some fidelity to be preserved.
  double inputScale;

  // Length of the output point clouds unit vectors.
  double outputUnitLength;

  //  output mode for ply writing (binary or ascii)
  bool outputBinaryPly;

  // Fractional fixed-point bits retained in conformance output
  int outputFpBits;

  // Output coordinate system to use
  OutputSystem outputSystem;

  // when true, configure the encoder as if no attributes are specified
  bool disableAttributeCoding;

  // Frame number of first file in input sequence.
  int firstFrameNum;

  // Number of frames to process.
  int frameCount;

  std::string uncompressedDataPath;
  std::string compressedStreamPath;
  std::string reconstructedDataPath;

  std::string plydir;

  int sleepWaitNextPly;
  int continousEncoding;
  int MaxExecutedListSize;

  // Filename for saving recoloured point cloud (encoder).
  std::string postRecolorPath;

  // Filename for saving pre inverse scaled point cloud (decoder).
  std::string preInvScalePath;

  // perform attribute colourspace conversion on ply input/output.
  bool convertColourspace;

  // resort the input points by azimuth angle
  bool sortInputByAzimuth;

  bool print_help;

  std::string ip_address;
  int ip_port;
  bool ipv6;
  int mtu_size;

  int ros_subscribe_max_queue_size;
  int ros_publish_max_queue_size;
  std::string ros_subscribe_topic;
  std::string ros_publish_topic;
  std::string ros_frameid;

  std::string dumpUncompressedDataPath;
  std::string dumpROSTopicPath;
  std::string debugLogPath;
#ifdef DO_PARALLEL_CODING

  //! フレームスレッド数
  int numOfFrameThreads;
  //! スライスの並列処理の可否
  bool doSliceParallelProc;
#endif // DO_PARALLEL_CODING

  int file_repeat_num_;
};

template<typename T>
class ThreadUtil {
public: 
  ThreadUtil()
      : mutex_(), queue_(), cond_(), thread_() {}
  ~ThreadUtil(){}

  bool joinable() {  return (nullptr != thread_.get() && thread_->joinable());}

  void join(bool unlock_cond = true) {
    if (unlock_cond) { cond_.notify_one(); }
    thread_->join();
  }

  struct DATA {
    T data_;
  };

  std::mutex mutex_;
  std::queue<DATA> queue_;
  std::condition_variable cond_;
  std::shared_ptr<std::thread> thread_;
};

class SequenceCodec {
protected:
    SequenceCodec(){}
    virtual ~SequenceCodec(){}

public:
    static bool isDecoder(int argc, char* argv[]);

    df::program_options_lite::OptionSpecific&
     addCommonOption(df::program_options_lite::OptionSpecific& opt);

    // virtual bool ParseParameters(int argc, char* argv[]);

  // Perform conversions and write output point cloud
  //  \params cloud  a mutable copy of reconFrame.cloud
  void writeOutputFrame(
    const std::string& postInvScalePath,
    const std::string& preInvScalePath,
    const pcc::CloudFrame& reconFrame,
    pcc::PCCPointSet3& cloud);

  // determine the output ply scale factor
  double outputScale(const pcc::CloudFrame& cloud) const;

  // the output ply origin, scaled according to output coordinate system
  pcc::Vec3<double> outputOrigin(const pcc::CloudFrame& cloud) const;

  void scaleAttributesForInput(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

  void scaleAttributesForOutput(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

  std::array<const char*, 3>
    axisOrderToPropertyNames(pcc::AxisOrder order);

  void convertToGbr(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

  void convertFromGbr(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

protected:
  template<typename T>
  static std::istream& readUInt(std::istream& in, T& val);

  int closeSocket(int32_t& sock);

  //// int32_t findChannelIndex(const sensor_msgs::PointCloud2* cloud, const std::string& channel);
  //// void writeRosTopic(const std::string& fileName, const sensor_msgs::PointCloud2* cloud);

protected:
  // static const size_t IP_TRANSFER_BUFFER_PERIOD = 1300;

  static const size_t PCC_COLOR_INDEX_GREEN = 0;
  static const size_t PCC_COLOR_INDEX_BLUE = 1;
  static const size_t PCC_COLOR_INDEX_RED = 2;

  static const size_t ROS_COLOR_INDEX_BLUE = 0;
  static const size_t ROS_COLOR_INDEX_GREEN = 1;
  static const size_t ROS_COLOR_INDEX_RED = 2;

  Parameters params_;

  std::mutex mutex_timestamp_;
  std::deque<uint64_t> timestamp_;
};
