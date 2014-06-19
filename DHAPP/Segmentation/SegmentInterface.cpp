#include <assert.h>

#include "cv.h"
#include <QtGui/QLabel>

#include "SegmentInterface.h"
#include "GrabCut.h"
#include "CommonFunc.h"
#include "Log/smooth.h"

//#define SHOW_INTERMEDIATE
//#define SHOW_INTERMEDIATE_OPENCV

namespace NMH {

	GrabCutSegmentor::GrabCutSegmentor(const GrabCutParam &param)
	{
		this->param = param;
	}

	GrabCutSegmentor::~GrabCutSegmentor()
	{

	}

	void GrabCutSegmentor::segment(const cv::Mat &img, const cv::Mat &mask, cv::Mat &out_seg)
	{
		assert(mask.size() == img.size());
		GrabCut grabcut(img.clone());

		cv::Mat &trimap = grabcut.get_trimap();
		cv::Mat &segment = grabcut.get_segment();
		trimap.setTo(cv::Scalar(TrimapUnknown));
		segment.setTo(cv::Scalar(SegmentationForeground));

		for (int y = 0; y < img.rows; y++)
		{
			for (int x = 0; x < img.cols; x++)
			{
				char mask_value = mask.at<char>(y, x);
				if (mask_value == MASK_BACKGROUND)
				{
					trimap.at<char>(y, x) = TrimapBackground;
					segment.at<char>(y, x) = SegmentationBackground;
				}
				else if (mask_value == MASK_FOREGROUND)
				{
					trimap.at<char>(y, x) = TrimapForeground;
					segment.at<char>(y, x) = SegmentationForeground;
				}
				else if (mask_value == MASK_UNKNOW)
				{
					trimap.at<char>(y, x) = TrimapUnknown;
					segment.at<char>(y, x) = SegmentationForeground;
				}
			}
		}

		grabcut.calc_beta();
		grabcut.calc_nlink(false);
		grabcut.build_gmm();

		grabcut.run();

		out_seg = segment;
	}

	GraphCutSegmentor::GraphCutSegmentor(const GraphCutParam &param)
	{
		this->param = param;
	}

	GraphCutSegmentor::~GraphCutSegmentor()
	{

	}

	void GraphCutSegmentor::segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg)
	{
		QImage _image, mask_image;
		Mat2QImage(img_mat, _image);
		Mat2QImage(mask_mat, mask_image);

		int _image_width = mask_image.width();
		int _image_height = mask_image.height();
		int _image_size = _image_width * _image_height;

		int *_image_intensity = new int[_image_size];
		CalcIntensity(_image, _image_intensity);

		int obj_hist[MAX_INTENSITY] = {0};
		int obj_seed_sum = 0;
		int bkg_hist[MAX_INTENSITY] = {0};
		int bkg_seed_sum = 0;

		/// mask_mat char array, 0 is backbround, 1 is foreground, 2 is undefined
		uchar *_mask = new uchar[_image_size];
		CalcMaskAndHist(mask_image, _mask, _image_intensity, obj_hist, obj_seed_sum, bkg_hist, bkg_seed_sum);

		double _var = 0.0;
		CalcVariance(_image_intensity, _image_width, _image_height, _var);

		Graph<double,double,double> graph(_image_size, 6 * _image_size, ErrorFunc);
		graph.add_node(_image_size);
		/// 赋值邻边capacity
		double _max_cap_boundary = 0.0;
		double K = 0.0;
		for (int y = 1; y < _image_height - 1; y++)
		{
			for (int x = 1; x < _image_width - 1; x++)
			{
				double cap_boundary[8] = {0.0};
				int node_idx = y * _image_width + x;

				neighbor_idx idx[8] = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}};
				double cap_sum = 0.0;

				for (int ii = 0; ii < sizeof(idx)/sizeof(idx[0]); ii++)
				{
					int node_idx2 = (y + idx[ii]._y) * _image_width + x + idx[ii]._x;

					cap_boundary[ii] = param.sd_param.kappa * exp(-0.5 * (_image_intensity[node_idx] - _image_intensity[node_idx2]) * (_image_intensity[node_idx] - _image_intensity[node_idx2])/_var) / (idx[ii]._x * idx[ii]._x + idx[ii]._y * idx[ii]._y);
					// TODO:
					graph.add_edge(node_idx, node_idx2, cap_boundary[ii], cap_boundary[ii]);
					cap_sum += cap_boundary[ii];
				}
				if (cap_sum > _max_cap_boundary) _max_cap_boundary = cap_sum;
			}
		}

		K = _max_cap_boundary + 1.0;

		double _min_cap_source = 1.0e10;
		double _max_cap_source = -1.0e10;
		double _min_cap_sink = 1.0e10;
		double _max_cap_sink = -1.0e10;
		/// 赋值terminal edge
		for (int y = 0; y < _image_height; y++)
		{
			for (int x = 0; x < _image_width; x++)
			{
				int node_id = y * _image_width + x;
				double cap_source = 0.0;
				double cap_sink = 0.0;
				if (_mask[node_id] == 0)
				{
					cap_source = 0;
					cap_sink = K;
				}
				else if (_mask[node_id] == 1)
				{
					cap_source = K;
					cap_sink = 0;
				}
				else
				{
					int inc = 0;
					double tmp_ratio = bkg_hist[_image_intensity[node_id]]/(double)bkg_seed_sum;
					/// 寻找最接近的总数不为零的亮度值
					while (tmp_ratio < FLOAT_ZERO && (++inc + _image_intensity[node_id]) < MAX_INTENSITY)
					{
						tmp_ratio = bkg_hist[_image_intensity[node_id] + inc]/(double)bkg_seed_sum;
					}
					if (tmp_ratio < FLOAT_ZERO)
					{
						inc = 0;
						while (tmp_ratio < FLOAT_ZERO && (--inc + _image_intensity[node_id]) >= 0)
						{
							tmp_ratio = bkg_hist[_image_intensity[node_id] + inc]/(double)bkg_seed_sum;
						}
					}
					cap_source = param.sd_param.lamda * -1.0 * log(tmp_ratio);

					inc = 0;
					tmp_ratio = obj_hist[_image_intensity[node_id]]/(double)obj_seed_sum;
					while (tmp_ratio < FLOAT_ZERO && (++inc + _image_intensity[node_id]) < MAX_INTENSITY)
					{
						tmp_ratio = obj_hist[_image_intensity[node_id] + inc]/(double)obj_seed_sum;
					}
					if (tmp_ratio < FLOAT_ZERO)
					{
						inc = 0;
						while (tmp_ratio < FLOAT_ZERO && (--inc + _image_intensity[node_id]) >= 0)
						{
							tmp_ratio = obj_hist[_image_intensity[node_id] + inc]/(double)obj_seed_sum;
						}
					}
					cap_sink = param.sd_param.lamda * -1.0 * log(tmp_ratio);

					if (cap_source < _min_cap_source) _min_cap_source = cap_source;
					if (cap_source > _max_cap_source) _max_cap_source = cap_source;
					if (cap_sink < _min_cap_sink) _min_cap_sink = cap_sink;
					if (cap_sink > _max_cap_sink) _max_cap_sink = cap_sink;
				}
				graph.add_tweights(node_id, cap_source, cap_sink);
			}
		}

		double max_flow = graph.maxflow();

		out_seg.create(img_mat.size(), CV_8UC1);
		out_seg.setTo(cv::Scalar(SegmentationForeground));
		for (int y = 0; y < _image_height; y++)
		{
			for (int x = 0; x < _image_width; x++)
			{
				int node_idx = y * _image_width + x;
				if (Graph<double,double,double>::SINK == graph.what_segment(node_idx, Graph<double,double,double>::SINK))
				{
					out_seg.at<char>(y, x) = SegmentationBackground;
				}
			}
		}

		delete []_mask;
		delete []_image_intensity;
	}


	LazyBrushSegmentor::LazyBrushSegmentor(const LazyBrushParam &param)
	{
		this->param = param;
	}

	LazyBrushSegmentor::~LazyBrushSegmentor()
	{

	}

	void LazyBrushSegmentor::segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg)
	{
		double lamda = param.sd_param.lamda;
		double kappa = param.sd_param.kappa;

		static QLabel *g_label = NULL, *g_label2 = NULL, *g_label3 = NULL;

		if (!g_label)
		{
			g_label = new QLabel();
			g_label->setWindowTitle(QString("painting stroke"));
		}

		if (!g_label2)
		{
			g_label2 = new QLabel();
			g_label2->setWindowTitle(QString("LoG result"));
		}

		if (!g_label3)
		{
			g_label3 = new QLabel();
			g_label3->setWindowTitle(QString("LoG result after Canny"));
		}

		QImage _image, mask_image;
		Mat2QImage(img_mat, _image);
		Mat2QImage(mask_mat, mask_image);

		int _image_width = _image.width();
		int _image_height = _image.height();
		int _image_size = _image_width * _image_height;

		float *LoG_result = new float[_image_size];
		CalcIntensity(_image, LoG_result);

		int *_image_intensity = new int[_image_size];
		CalcIntensity(_image, _image_intensity);

		int obj_hist[MAX_INTENSITY] = {0};
		int obj_seed_sum = 0;
		int bkg_hist[MAX_INTENSITY] = {0};
		int bkg_seed_sum = 0;

		/// mask_mat char array, 0 is backbround, 1 is foreground, 2 is undefined
		uchar *_mask = new uchar[_image_size];
		CalcMaskAndHist(mask_image, _mask, _image_intensity, obj_hist, obj_seed_sum, bkg_hist, bkg_seed_sum);

		double _var = 0.0;
		CalcVariance(_image_intensity, _image_width, _image_height, _var);

		int K = 2 * _image_size;
		float cof = 0.95f;

		/// 拉普拉斯高斯算子
		LoG(LoG_result, _image_width, _image_height);

		float max_log_value = -1.0f;
		float min_log_value = 1e10;
		for (int ii = 0; ii < _image_size; ii++)
		{
			if (LoG_result[ii] < 0) LoG_result[ii] = 0;
			if (LoG_result[ii] > max_log_value) max_log_value = LoG_result[ii];
			if (LoG_result[ii] < min_log_value) min_log_value = LoG_result[ii];
		}

#ifdef SHOW_INTERMEDIATE
		g_label2->setPixmap(QPixmap::fromImage(IntensityToImage(LoG_result, _image_width, _image_height, max_log_value, min_log_value)));
		g_label2->adjustSize();
		g_label2->show();
		cv::namedWindow("OpenCV");
		cvWaitKey(0);
#endif

		if (param.use_canny_input || param.use_canny_enhance)
		{
			/// OpenCV functions
			IplImage cv_img = img_mat;
			IplImage *cv_orig_img = &cv_img;

#ifdef SHOW_INTERMEDIATE_OPENCV
			cvNamedWindow("cv_orig_img");
			cvShowImage("cv_orig_img", cv_orig_img);
			cvWaitKey(0);
#endif
			IplImage *gray_img = cvCreateImage(cvGetSize(cv_orig_img), IPL_DEPTH_8U, 1);
			cvCvtColor(cv_orig_img, gray_img, CV_RGB2GRAY);
			cvSmooth(gray_img, gray_img);

			int canny_min = param.canny_param.canny_min;
			int canny_max = param.canny_param.canny_max;

			IplImage *edge_img = cvCreateImage(cvGetSize(cv_orig_img), IPL_DEPTH_8U, 1);
			IplImage *edge_erode_img = cvCreateImage(cvGetSize(cv_orig_img), IPL_DEPTH_8U, 1);
			cvCanny(gray_img, edge_img, canny_min, canny_max);

#ifdef SHOW_INTERMEDIATE_OPENCV
			cvNamedWindow("Canny Edge");
			cvShowImage("Canny Edge", edge_img);
			cvWaitKey(0);
#endif

			if (param.canny_erode)
			{
				cvSmooth(edge_img, edge_erode_img);
				cvErode(edge_erode_img, edge_erode_img);

#ifdef SHOW_INTERMEDIATE_OPENCV
				cvNamedWindow("Erode");
				cvShowImage("Erode", edge_erode_img);
				cvWaitKey(0);
#endif
			}

			/// 用Canny或者腐蚀后的Canny边缘强化LoG边界，并消除LoG区域内部边界
			for (int y = 0;y < _image_height; y++)
			{
				for (int x = 0; x < _image_width; x++)
				{
					unsigned char *ptr = NULL;

					if (param.canny_erode)
					{
						/// 用腐蚀后的Canny增强边缘
						ptr = (unsigned char *)(edge_erode_img->imageData + y * edge_erode_img->widthStep + x);
					}
					else
					{
						/// 用原始的Canny增强边缘
						ptr =  (unsigned char *)(edge_img->imageData + y * edge_img->widthStep + x);
					}

					/// 使用LoG边界同时用Canny增强
					if (!param.use_canny_input && param.use_canny_enhance)
					{
						if (*ptr) LoG_result[y * _image_width + x] = max_log_value;
					}
					/// 仅使用Canny边界作为输入
					else
					{
						LoG_result[y * _image_width + x] = *ptr;
					}

					if (LoG_result[y * _image_width + x] < 0)
					{
						LoG_result[y * _image_width + x] = 0;
					}
				}
			}

			max_log_value = -1.0f;
			min_log_value = 1e10;
			for (int ii = 0; ii < _image_size; ii++)
			{
				if (LoG_result[ii] > max_log_value) max_log_value = LoG_result[ii];
				if (LoG_result[ii] < min_log_value) min_log_value = LoG_result[ii];
			}

#ifdef SHOW_INTERMEDIATE
			g_label3->setPixmap(QPixmap::fromImage(IntensityToImage(LoG_result, _image_width, _image_height, max_log_value, min_log_value)));
			g_label3->adjustSize();
			g_label3->show();
			cvWaitKey(0);
#endif

			cvReleaseImage(&gray_img);
			cvReleaseImage(&edge_img);
			cvReleaseImage(&edge_erode_img);
		}

		for (int ii = 0; ii < _image_size; ii++)
		{
			if (LoG_result[ii] < 0.0) LoG_result[ii] = 1.0f;
			else
			{
				LoG_result[ii] = 1.0f - LoG_result[ii]/max_log_value;
			}
		}

		typedef Graph<float, float, float> MyGraph;
		MyGraph graph(_image_size, 6 *_image_size, ErrorFunc);
		graph.add_node(_image_size);
		int seed_num = 0;

		double _min_cap_source = 1.0e10;
		double _max_cap_source = -1.0e10;
		double _min_cap_sink = 1.0e10;
		double _max_cap_sink = -1.0e10;

		for (int y = 0; y < _image_height; y++)
		{
			for (int x = 0; x < _image_width; x++)
			{
				QRgb pix = mask_image.pixel(x, y);
				int node_idx = y * _image_width + x;
				double cap_source = 0.0;
				double cap_sink = 0.0;

				if (pix == BACKGROUND_COLOR)
				{
					cap_source = lamda * K;
					cap_sink = 0;
				}
				else if (pix == FOREGROUND_COLOR)
				{	
					cap_source = 0;
					cap_sink = lamda * K;
					seed_num++;
				}
				else
				{
					if (param.data_use_hist)
					{
						int inc = 0;
						double tmp_ratio = bkg_hist[_image_intensity[node_idx]]/(double)bkg_seed_sum;
						/// 寻找最接近的总数不为零的亮度值
						while (tmp_ratio < FLOAT_ZERO && (++inc + _image_intensity[node_idx]) < MAX_INTENSITY)
						{
							tmp_ratio = bkg_hist[_image_intensity[node_idx] + inc]/(double)bkg_seed_sum;
						}
						if (tmp_ratio < FLOAT_ZERO)
						{
							inc = 0;
							while (tmp_ratio < FLOAT_ZERO && (--inc + _image_intensity[node_idx]) >= 0)
							{
								tmp_ratio = bkg_hist[_image_intensity[node_idx] + inc]/(double)bkg_seed_sum;
							}
						}
						cap_source = param.sd_param.lamda * -1.0 * log(tmp_ratio) * K/1000;

						inc = 0;
						tmp_ratio = obj_hist[_image_intensity[node_idx]]/(double)obj_seed_sum;
						while (tmp_ratio < FLOAT_ZERO && (++inc + _image_intensity[node_idx]) < MAX_INTENSITY)
						{
							tmp_ratio = obj_hist[_image_intensity[node_idx] + inc]/(double)obj_seed_sum;
						}
						if (tmp_ratio < FLOAT_ZERO)
						{
							inc = 0;
							while (tmp_ratio < FLOAT_ZERO && (--inc + _image_intensity[node_idx]) >= 0)
							{
								tmp_ratio = obj_hist[_image_intensity[node_idx] + inc]/(double)obj_seed_sum;
							}
						}

						cap_sink = param.sd_param.lamda * -1.0 * log(tmp_ratio) * K/1000;
					}
					else
					{
						cap_sink = 0;
						cap_source = 0;
					}
				}

				if (cap_source < _min_cap_source) _min_cap_source = cap_source;
				if (cap_source > _max_cap_source) _max_cap_source = cap_source;
				if (cap_sink < _min_cap_sink) _min_cap_sink = cap_sink;
				if (cap_sink > _max_cap_sink) _max_cap_sink = cap_sink;

				graph.add_tweights(node_idx, cap_source, cap_sink);

				if (y != 0 && y != _image_height - 1 && x != 0 && x != _image_width - 1)
				{
					int left_idx = node_idx - 1;
					int right_idx = node_idx + 1;
					int top_idx = node_idx - _image_width;
					int bottom_idx = node_idx + _image_width;

					graph.add_edge(node_idx, left_idx, LoG_result[node_idx] * K + 1, LoG_result[left_idx]*K + 1);
					graph.add_edge(node_idx, right_idx, LoG_result[node_idx] * K + 1, LoG_result[right_idx]*K + 1);
					graph.add_edge(node_idx, top_idx, LoG_result[node_idx] * K + 1, LoG_result[top_idx]*K + 1);
					graph.add_edge(node_idx, bottom_idx, LoG_result[node_idx] * K + 1, LoG_result[bottom_idx]*K + 1);

				}
			}
		}

		float max_flow = graph.maxflow();

		out_seg.create(img_mat.size(), CV_8SC1);
		out_seg.setTo(cv::Scalar(SegmentationBackground));
		for (int y = 0; y < _image_height; y++)
		{
			for (int x = 0; x < _image_width; x++)
			{
				int node_idx = y * _image_width + x;
				if (MyGraph::SINK == graph.what_segment(node_idx, MyGraph::SINK))
				{
					out_seg.at<char>(y, x) = SegmentationForeground;
				}
			}
		}

		delete []LoG_result;
		delete []_image_intensity;
		delete []_mask;
	}

	MixtureSegmentor::MixtureSegmentor(const MixtureParam &param)
	{
		this->param = param;
	}

	MixtureSegmentor::~MixtureSegmentor()
	{

	}

	void MixtureSegmentor::segment(const cv::Mat &img_mat, const cv::Mat &mask_mat, cv::Mat &out_seg)
	{
		assert(mask_mat.size() == img_mat.size());
		GrabCut grabcut(img_mat.clone());

		cv::Mat &trimap = grabcut.get_trimap();
		cv::Mat &segment = grabcut.get_segment();
		trimap.setTo(cv::Scalar(TrimapUnknown));
		segment.setTo(cv::Scalar(SegmentationForeground));

		for (int y = 0; y < img_mat.rows; y++)
		{
			for (int x = 0; x < img_mat.cols; x++)
			{
				char mask_value = mask_mat.at<char>(y, x);
				if (mask_value == MASK_BACKGROUND)
				{
					trimap.at<char>(y, x) = TrimapBackground;
					segment.at<char>(y, x) = SegmentationBackground;
				}
				else if (mask_value == MASK_FOREGROUND)
				{
					trimap.at<char>(y, x) = TrimapForeground;
					segment.at<char>(y, x) = SegmentationForeground;
				}
				else if (mask_value == MASK_UNKNOW)
				{
					trimap.at<char>(y, x) = TrimapUnknown;
					segment.at<char>(y, x) = SegmentationForeground;
				}
			}
		}

		grabcut.calc_beta();
		grabcut.calc_nlink(true, param.edge_weight, &param.canny_param);
		grabcut.build_gmm();

		grabcut.run();

		out_seg = segment;
	}
}