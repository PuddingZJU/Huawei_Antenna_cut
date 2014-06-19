#ifndef SEGMENT_INTERFACE_H
#define SEGMENT_INTERFACE_H

#include "cv.h"

#include <QtGui/QImage>


namespace NMH
{

	struct neighbor_idx
	{
		int _x;
		int _y;
	};

	enum MixtureType
	{
		MT_Linear,
		MT_Squared,
		MT_Cubical,

		MT_COUNT
	};

	class AbstractSegmentor
	{
	public:
		virtual ~AbstractSegmentor(){}
		virtual void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg) = 0;
	};

	struct CannyParam
	{
		int canny_min;
		int canny_max;

		CannyParam()
		{
			canny_min = 60;
			canny_max = 180;
		}
	};

	struct SmoothDataParam
	{
		double lamda;
		double kappa;

		SmoothDataParam()
		{
			lamda = 1.0;
			kappa = 4.0;
		}
	};

	struct LazyBrushParam
	{
		CannyParam canny_param;
		//TODO:
		SmoothDataParam sd_param;
		bool use_canny_input;
		bool use_canny_enhance;
		bool canny_erode;
		bool data_use_hist;

		LazyBrushParam()
		{
			use_canny_input = true;
			use_canny_enhance = true;
			canny_erode = false;
			data_use_hist = false;
		}
	};

	struct GrabCutParam
	{
		CannyParam canny_param;
		//TODO:
		SmoothDataParam sd_param;
	};

	struct GraphCutParam
	{
		//TODO:
		SmoothDataParam sd_param;
	};

	struct MixtureParam
	{
		CannyParam canny_param;
		//TODO:
		SmoothDataParam sd_param;

		double edge_weight;
		MixtureType mixture_type;

		MixtureParam()
		{
			edge_weight = 0.25;
			mixture_type = MT_Linear;
		}
	};

	class LazyBrushSegmentor: public AbstractSegmentor
	{
	public:
		LazyBrushSegmentor(const LazyBrushParam &param);
		virtual ~LazyBrushSegmentor();

		virtual void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg);

	private:
		LazyBrushParam param;
	};

	class GrabCutSegmentor: public AbstractSegmentor
	{
	public:
		GrabCutSegmentor(const GrabCutParam &param);
		virtual ~GrabCutSegmentor();

		virtual void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg);

	private:
		GrabCutParam param;
	};

	class MixtureSegmentor: public AbstractSegmentor
	{
	public:
		MixtureSegmentor(const MixtureParam &param);
		virtual ~MixtureSegmentor();

		virtual void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg);

	private:
		MixtureParam param;
	};

	class GraphCutSegmentor: public AbstractSegmentor
	{
	public:
		GraphCutSegmentor(const GraphCutParam &param);
		virtual ~GraphCutSegmentor();

		virtual void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg);

	private:
		GraphCutParam param;
	};

	enum MethodType
	{
		MT_LazyBrush,
		MT_GraphCut,
		MT_GrabCut,
		MT_Mixture,

		MT_Count
	};

	void segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, MethodType method_type, const void *method_param, cv::Mat &out_seg);

}
#endif