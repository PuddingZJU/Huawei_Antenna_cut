#ifndef SMOOTH_H
#define SMOOTH_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

typedef float IntensityType;

void Smooth(IntensityType *pnImage, const size_t nImageWidth, const size_t nImageHeight, const size_t nTempW, const size_t nTempH, const size_t nMidW, const size_t nMidH, const float* pValue, const float fCoef);

void LoG(IntensityType *pnImage, const size_t nImageWidth, const size_t nImageHeight);

#endif