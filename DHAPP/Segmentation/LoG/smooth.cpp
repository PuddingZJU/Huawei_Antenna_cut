#include "smooth.h"
/**
*滤波模板函数
*/
void Smooth(IntensityType *pnImage, const size_t nImageWidth, const size_t nImageHeight, const size_t nTempW, const size_t nTempH, const size_t nMidW, const size_t nMidH, const float* pValue, const float fCoef)
{
	assert(nMidH < nTempH);
	assert(nMidW < nTempW);

	IntensityType* pnDstImage = new IntensityType[nImageHeight * nImageWidth];
	memcpy(pnDstImage, pnImage, sizeof(IntensityType)*nImageHeight*nImageWidth);

	IntensityType nMaxDepth = 0, nMinDepth = 0xFFFFu;
	//ii, kk用于Height的索引，jj, tt用于Width的索引
	for (size_t ii = nMidH; ii < nImageHeight - (nTempH - 1 - nMidH); ii++)
		for (size_t jj = nMidW; jj < nImageWidth - (nTempW - 1 - nMidW); jj++)
		{
			nMaxDepth = nMaxDepth > pnImage[ii * nImageWidth + jj]? nMaxDepth : pnImage[ii * nImageWidth + jj];
			nMinDepth = nMinDepth < pnImage[ii * nImageWidth + jj]? nMinDepth : pnImage[ii * nImageWidth + jj];

			float sum = 0.0f;
			//计算(ii, jj)点的信息
			for (size_t kk = 0; kk < nTempH; kk++)
			{
				for (size_t tt = 0; tt < nTempW; tt++)
				{
					size_t relW = jj - nMidW + tt;
					size_t relH = ii - nMidH + kk;

					sum += pnImage[relH * nImageWidth + relW]*pValue[kk * nTempW + tt];
				}
			}

			sum *= fCoef;
			pnDstImage[ii * nImageWidth + jj] = sum;
		}

#ifdef SHOW_IMAGE

		printf("before: nMaxDepth %u,\tnMinDepth %u\n", nMaxDepth, nMinDepth);

		cv::namedWindow("Depth_Src");
		cv::namedWindow("Depth_Dst");

		cv::Mat depthSrc(nImageHeight, nImageWidth, CV_16UC1);
		cv::Mat depthDst(nImageHeight, nImageWidth, CV_16UC1);
		memcpy(depthSrc.data, pnImage, sizeof(IntensityType)*nImageHeight*nImageWidth);
		memcpy(depthDst.data, pnDstImage, sizeof(IntensityType)*nImageHeight*nImageWidth);

		float scale = (float)0xFFFFu / (float)nMaxDepth;
		nMaxDepth = 0, nMinDepth = 0xFFFF;
		for (size_t ii = 0; ii < nImageHeight; ii++)
		{
			for (size_t jj = 0; jj < nImageWidth; jj++)
			{
				((IntensityType*)(depthSrc.data))[ii * nImageWidth + jj] *= scale;
				((IntensityType*)(depthDst.data))[ii * nImageWidth + jj] *= scale;

				nMaxDepth = nMaxDepth > ((IntensityType*)(depthDst.data))[ii * nImageWidth + jj]? nMaxDepth : ((IntensityType*)(depthDst.data))[ii * nImageWidth + jj];
				nMinDepth = nMinDepth < ((IntensityType*)(depthDst.data))[ii * nImageWidth + jj]? nMinDepth : ((IntensityType*)(depthDst.data))[ii * nImageWidth + jj];
			}
		}
		printf("after: nMaxDepth %u,\tnMinDepth %u\n", nMaxDepth, nMinDepth);

		cv::imshow("Depth_Src", depthSrc);
		cv::imshow("Depth_Dst", depthDst);

		cv::waitKey(0); //等待按键

#endif

		memcpy((void*)pnImage, (void*)pnDstImage, sizeof(IntensityType)*nImageHeight*nImageWidth);
		delete[] pnDstImage;
}

void GaussSmooth(IntensityType *pnImage, const size_t nImageWidth, const size_t nImageHeight)
{
	static float s_GaussSmooth[] = {1, 2, 1, 
															2, 18, 2, 
															1, 2, 1};

	static float s_GaussCoef = 1.0f/30.0f;
	Smooth(pnImage, nImageWidth, nImageHeight, 3, 3, 1, 1, s_GaussSmooth, s_GaussCoef);
}

void EdgeSobel(IntensityType* pnImage, const size_t nImageWidth, const size_t nImageHeight)
{
	static float s_SobelH[] = {-1, -1, -1,
													0, 0, 0, 
													1, 1, 1};

	static float s_SobelCoef = 1;
	Smooth(pnImage, nImageWidth, nImageHeight, 3, 3, 1, 1, s_SobelH, s_SobelCoef);
}

void LoG(IntensityType *pnImage, const size_t nImageWidth, const size_t nImageHeight)
{
	static float s_LoG[] = {-2, -4, -4, -4, -2,
											-4, 0, 8, 0, -4,
											-4, 8, 24, 9, -4,
											-4, 0, 8, 0, -4,
											-2, -4, -4, -4, -2};

	static float s_LoG2[] = {0, 0, 1, 0, 0,
		0, 1, 2, 1, 0,
		1, 2, -16, 2, 1,
		0, 1, 2, 1, 0,
		0, 0, 1, 0, 0};

	static float s_LoGCoef = 1;
	Smooth(pnImage, nImageWidth, nImageHeight, 5, 5, 2, 2, s_LoG2, s_LoGCoef);
}