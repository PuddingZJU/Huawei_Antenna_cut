#ifndef _PANORAMAINTERFACE_H__
#define _PANORAMAINTERFACE_H__
#include <vector>
#include <cxcore.hpp>

struct PanoramaParam
{
	bool useGPU;
	bool useDP;
	bool useGC;
	bool useLinear;
	bool useMulti;
	bool useSIFT;
};
cv::Mat StitchImage(std::vector<std::string> file_names, PanoramaParam pParam);

#endif