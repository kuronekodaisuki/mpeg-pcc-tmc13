#pragma once
#include "pcc_chrono.h"
#include "Particle.h"
#include "ply.h"
#include "SequenceCodec.h"


using namespace pcc;

typedef pcc::chrono::Stopwatch<pcc::chrono::utime_inc_children_clock>
  Stopwatch;

class API SequenceEncoder: public SequenceCodec, pcc::PCCTMC3Encoder3::Callbacks 
{
public:
	// NB: params must outlive the lifetime of the decoder.
	SequenceEncoder(Parameters* params);

	int compress(Stopwatch* clock);

	int compress(std::vector<Particle>& particles);

protected:
	int compressOneFrame(Stopwatch* clock);
	int compressOneGOF(Stopwatch* clock);

	void onOutputBuffer(const pcc::PayloadBuffer& buf) override;
	void onPostRecolour(const pcc::PCCPointSet3& cloud) override;

private:
	ply::PropertyNameMap _plyAttrNames;

	// The raw origin used for input sorting
	pcc::Vec3<int> _angularOrigin;

	pcc::PCCTMC3Encoder3 encoder;

	std::ofstream bytestreamFile;

	int frameNum;

	int gofSizePlusOne;
	// Indicates the file number of current frame
	int currentFrame;
	// Indicates the file number of previous I/P frame
	int preIPFrame;
	bool codedGOF;
};