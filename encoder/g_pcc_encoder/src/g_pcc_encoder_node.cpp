#include <memory>

#include "version.h"
#include "program_options_lite.h"
#include "SequenceEncoder.hpp"       // encoder
// #include "SequenceDecoder.hpp"       // decoder
// #include "ros/ros.h"
// #include "sensor_msgs/PointCloud2.h"
// #include "std_msgs/String.h"

//#include "FileWatch.hpp"
using namespace std;
#include "g_pcc_encoder_node.hpp"

int encodeProc(SequenceEncoder& encoder, Stopwatch& clock_user, pcc::chrono::Stopwatch<std::chrono::steady_clock>& clock_wall)
{
  std::cout << "----- init -----" << std::endl;

  if (!encoder.init()) {
    std::cerr << "  !!! failed to initialize." << std::endl;
    return (1);
  }

  clock_wall.start();

  std::cout << "----- compress -----" << std::endl;

  return(encoder.compress(&clock_user));
}

//============================================================================
//static 
std::vector<std::string> SequenceEncoder::files_xy;
int
lib_main(int argc, char* argv[])
{
//  std::cout << "MPEG PCC tmc3 version " << ::pcc::version << std::endl;

#if 0
  for (int n=0; n<argc; n++) {
    std::cout << "  arg:" << argv[n] << std::endl;
  }
#endif

  pcc::chrono::Stopwatch<std::chrono::steady_clock> clock_wall;
  pcc::chrono::Stopwatch<pcc::chrono::utime_inc_children_clock> clock_user;
  SequenceEncoder::ret_files_init(1000);
  SequenceEncoder encoder;

  try {
    if (!encoder.ParseParameters(argc, argv)) {
      std::cerr << "  !!! failed to parse parameter." << std::endl;
      return (1);
    }
  }
  catch (df::program_options_lite::ParseFailure& e) {
    std::cerr << "Error parsing option \"" << e.arg << "\" with argument \""
              << e.val << "\"." << std::endl;
    return 1;
  }

  int ret = encodeProc(encoder, clock_user, clock_wall);

  clock_wall.stop();

  std::cout << "----- end proc. -----" << std::endl;

  using namespace std::chrono;
  auto total_wall = duration_cast<milliseconds>(clock_wall.count()).count();
  auto total_user = duration_cast<milliseconds>(clock_user.count()).count();
  std::cout << "Processing time (wall): " << total_wall / 1000.0 << " s\n";
  std::cout << "Processing time (user): " << total_user / 1000.0 << " s\n";

  encoder.print_measure_info(total_wall);

  return ret;
}

