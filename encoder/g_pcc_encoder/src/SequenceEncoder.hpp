#pragma once

#define PLYDIR "/home/msd/tmp/plydiv"
#define _CRT_SECURE_NO_WARNINGS

#define ENABLE_DEBUG_ROS_OUTPUT
#define ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT
#define ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT

#if !defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
// #   define ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT
#endif //!defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT) || defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
#  define ENABLE_DEBUG_ROS_OUTPUT
#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT) || defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)

#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <exception>

////#include <ros/ros.h>
////#include <sensor_msgs/PointCloud2.h>

#include "TMC3Config.h"

#include "pcc_chrono.h"
#include "ply.h"

#include "PCCTMC3Encoder.h"

#include "CodecCommon.hpp"
#include <vector>
#include <dirent.h>

#ifdef DO_PARALLEL_CODING
#include "ParallelEncoder.h"
#endif // DO_PARALLEL_CODING
using namespace std;
namespace df {
  namespace program_options_lite {
    struct ErrorReporter;
  }
}

typedef pcc::chrono::Stopwatch<pcc::chrono::utime_inc_children_clock>
  Stopwatch;

class SequenceEncoder
  : public SequenceCodec
  , pcc::PCCTMC3Encoder3::Callbacks {
public:
    SequenceEncoder();
    virtual ~SequenceEncoder();

    virtual bool ParseParameters(int argc, char* argv[]);

    bool init();
    void deinit();
    int compress(Stopwatch* clock);
    void print_measure_info(double proc_milsec);
    int startWatchFile();
    static std::string* watchpath();

    //static std::vector<std::string> files_xy; 
    static std::vector<std::string> files_xy;

    static void ret_files_init(int size);  
    int ret_files_size(void);
    void ret_files_clear(void);

    static std::vector<std::string>* ret_files(); 

    const char* watchPath(void);
    
    int getlist(DIR* path);
    std::vector<std::string> candidateList;
    int mvEncoded_(string& fileName, string& pathply, string& encoded_dir);
    
protected:
  void sanitizeEncoderOpts(df::program_options_lite::ErrorReporter& err);

  int compressOneFrame(Stopwatch* clock, pcc::PCCPointSet3& pointCloud);

  void onOutputBuffer(const pcc::PayloadBuffer& buf) override;
  void onPostRecolour(const pcc::PCCPointSet3& cloud) override;

  bool initializeSocket();
  bool fillSendBuff(const pcc::PayloadBuffer& buf);
  bool fillSendBuff(char type, const char* buf, uint32_t length);
  bool sendData(void*data, size_t length);
  bool sendThread();

////  void processPointCloud2Message(const sensor_msgs::PointCloud2ConstPtr& cloud);
////  bool setPCCPointSet3(const sensor_msgs::PointCloud2ConstPtr& cloud, pcc::PCCPointSet3& pointCloud);
////  void dumpRosTopic(const sensor_msgs::PointCloud2ConstPtr& cloud);

  inline void outputDebugLog(const std::string& msg);
#ifdef DO_PARALLEL_CODING

  //! 並列エンコーダの終端処理
  // このメソッドを呼ばないと、最後までエンコードしたストリームが出力されません
  bool endOfParallelEncoding(Stopwatch* clock);
#endif // DO_PARALLEL_CODING

private:
  struct SEND_BUFF{
    std::vector<char> tlv_;
  };

private:
  pcc::ply::PropertyNameMap _plyAttrNames;

  // The raw origin used for input sorting
  pcc::Vec3<int> _angularOrigin;

#ifdef DO_PARALLEL_CODING
  pcc::PCCTMC3Encoder3* encoder;
  ParallelEncoder* parallelEncoder;
#else // !DO_PARALLEL_CODING
  pcc::PCCTMC3Encoder3 encoder;
#endif // !DO_PARALLEL_CODING

  std::ofstream bytestreamFile;
  std::ofstream dumpSendStream; // ソケット出力データダンプ
  std::ofstream uncompressedDataDump;
  std::ofstream debugLog_;

  int frameNum;

  

private:
  pcc::EncoderParams encoderParams_;

  int pcc_sock_;
  sockaddr_in peer_;
  sockaddr_in6 peer6_;

  size_t input_point_total_;
  size_t input_point_max_;
  size_t input_point_min_;
  size_t output_byte_total_;
  size_t output_byte_max_;
  size_t output_byte_min_;

  int interval_for_fileinput_;

  bool enablePreview_;
  bool enablePreviewPreProc_;
  bool enablePreviewRecon_;

  std::vector<char> sendbuff_;

  ThreadUtil<std::vector<char>> th_sender_;

  pcc::chrono::Stopwatch<pcc::chrono::utime_inc_children_clock>* clock_user_;

  std::chrono::system_clock::time_point clock_ros_initial_receive_;

  int frame_count_;
  int frame_count_recon_;
  int frame_count_preproc_;
////  ros::Publisher ros_pub_;
////  ros::Publisher ros_pub_recon_;
////  ros::Publisher ros_pub_preproc_;
////  ros::NodeHandle* nh_;
  std::vector<pcc::CloudFrame> reconFrame_;
  int reconIdx_=0;

#if defined(ENABLE_DEBUG_ROS_OUTPUT)
  int32_t ros_rgbidx_;
  int32_t ros_rgbofs_;
  int32_t ros_refcidx_;
  int32_t ros_refcofs_;
  int32_t ros_frameiidx_;
  int32_t ros_frameiofs_;
////  sensor_msgs::PointCloud2 ros_point_cloud_msg_;
  bool initPointCloud2(const pcc::PCCPointSet3& pointCloud, pcc::AxisOrder geometryAxisOrder);
#endif // defined(ENABLE_DEBUG_ROS_OUTPUT)
#if defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
////  bool publishPointCloud2(const pcc::CloudFrame& frame, ros::Publisher& ros_publisher);
////  bool setPointCloud2(const pcc::CloudFrame& frame, sensor_msgs::PointCloud2& ros_point_cloud);
#endif // defined(ENABLE_RECONSTRUCT_DATA_ROS_OUTPUT)
#if defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)
////  bool publishPointCloud2(const pcc::PCCPointSet3& pointCloud, ros::Publisher& ros_publisher);
////  bool setPointCloud2(const pcc::PCCPointSet3& pointCloud, sensor_msgs::PointCloud2& ros_point_cloud);
#endif // defined(ENABLE_ENCODERINPUT_DATA_ROS_OUTPUT)

};

