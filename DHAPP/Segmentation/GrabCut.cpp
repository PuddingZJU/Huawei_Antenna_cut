#include "GrabCut.h"
#include "CommonFunc.h"
#include "SegmentInterface.h"

#include <ctime>

using namespace cv;
using namespace std;

static int canny_min = 60;
static int canny_max = 150;

#define MAX_RUN_TIMES 5

GrabCut::GrabCut(Mat &img)
{
	this->img = img;

	graph = NULL;
	image_gmm = NULL;

	lambda = 50.0;
	L = 8*lambda + 1;

	edge_lambda = 50.0;

	component.create(img.size(), CV_8UC1);
	segment.create(img.size(), CV_8UC1);
	trimap.create(img.size(), CV_8UC1);

	nlink.create(img.size(), CV_32FC(GC_NLINK_COUNT));
	tlink.create(img.size(), CV_32FC2);
}

GrabCut::~GrabCut()
{
	if(graph) delete graph;
	if (image_gmm) delete image_gmm;
}

void GrabCut::calc_beta()
{
	beta = 0;
	for( int y = 0; y < img.rows; y++ )
	{
		for( int x = 0; x < img.cols; x++ )
		{
			Vec3d color = img.at<Vec3b>(y,x);
			if( x>0 ) // left
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y,x-1);
				beta += diff.dot(diff);
			}
			if( y>0 && x>0 ) // upleft
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x-1);
				beta += diff.dot(diff);
			}
			if( y>0 ) // up
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x);
				beta += diff.dot(diff);
			}
			if( y>0 && x<img.cols-1) // upright
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x+1);
				beta += diff.dot(diff);
			}
		}
	}
	if( beta <= std::numeric_limits<double>::epsilon() )
		beta = 0;
	else
		beta = 1.f / (2 * beta/(4*img.cols*img.rows - 3*img.cols - 3*img.rows + 2) );
}

void GrabCut::calc_nlink(bool with_edge_component/* = false*/, double edge_weight/* = 0.0*/, void *canny_param /*= NULL*/)
{
	double gamma = lambda;
	const double gammaDivSqrt2 = gamma / std::sqrt(2.0f);

	float max_n_cap = -1.0f;

	for( int y = 0; y < img.rows; y++ )
	{
		for( int x = 0; x < img.cols; x++ )
		{
			float * ptr = nlink.ptr<float>(y, x);

			Vec3d color = img.at<Vec3b>(y,x);
			if( x-1>=0 ) // left
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y,x-1);
				ptr[GC_LEFT] = gamma * exp(-beta*diff.dot(diff));

				if (ptr[GC_LEFT] > max_n_cap) max_n_cap = ptr[GC_LEFT];
			}
			else
				ptr[GC_LEFT] = 0;

			if( x-1>=0 && y-1>=0 ) // upleft
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x-1);
				ptr[GC_UP_LEFT] = gammaDivSqrt2 * exp(-beta*diff.dot(diff));

				if (ptr[GC_UP_LEFT] > max_n_cap) max_n_cap = ptr[GC_UP_LEFT];
			}
			else
				ptr[GC_UP_LEFT] = 0;

			if( y-1>=0 ) // up
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x);
				ptr[GC_UP] = gamma * exp(-beta*diff.dot(diff));

				if (ptr[GC_UP] > max_n_cap) max_n_cap = ptr[GC_UP];
			}
			else
				ptr[GC_UP] = 0;

			if( x+1<img.cols-1 && y-1>=0 ) // upright
			{
				Vec3d diff = color - (Vec3d)img.at<Vec3b>(y-1,x+1);
				ptr[GC_UP_RIGHT] = gammaDivSqrt2 * exp(-beta*diff.dot(diff));

				if (ptr[GC_UP_RIGHT] > max_n_cap) max_n_cap = ptr[GC_UP_RIGHT];
			}
			else
				ptr[GC_UP_RIGHT] = 0;
		}
	}


	NMH::CannyParam *_canny_param = (NMH::CannyParam*)canny_param;
	if (_canny_param)
	{
		canny_min = _canny_param->canny_min;
		canny_max = _canny_param->canny_max;
	}

	if (canny_min > canny_max)
	{
		int tmp = canny_min;
		canny_min = canny_max;
		canny_max = tmp;
	}

	std::cout<<with_edge_component<<'\t'<<canny_min<<'\t'<<canny_max<<std::endl;

	if (with_edge_component)
	{
		Mat gray_img(img.size(), CV_8UC1);
		cv::cvtColor(img, gray_img, COLOR_RGB2GRAY);
		cv::GaussianBlur(gray_img, gray_img, Size(5, 5), 0, 0);

		Mat edge_img (img.size(), CV_8UC1);
		Mat edge_erode_img(img.size(), CV_8UC1);
		cv::Canny(gray_img, edge_img, canny_min, canny_max);

		//cv::namedWindow("Canny");
		//cv::imshow("Canny", edge_img);
		cv::waitKey(100);

		for( int y = 0; y < img.rows; y++ )
		{
			for( int x = 0; x < img.cols; x++ )
			{
				unsigned char edge_value = edge_img.at<unsigned char>(y, x);
				float * ptr = nlink.ptr<float>(y, x);

				if (edge_value != 0)
				{
					for (int ii = GC_LEFT; ii < GC_NLINK_COUNT; ii++)
					{
						ptr[ii] = ptr[ii] * (1.0 - edge_weight) + edge_lambda * edge_weight;
						if (ptr[ii] < 0) ptr[ii] = 0;
					}
				}
				else
				{
					for (int ii = GC_LEFT; ii < GC_NLINK_COUNT; ii++)
					{
						ptr[ii] = ptr[ii] * (1.0 - edge_weight);
						if (ptr[ii] < 0) ptr[ii] = 0;
					}
				}

				if (ptr[GC_UP_LEFT] < 0 || ptr[GC_UP_LEFT] < 0 || ptr[GC_UP] < 0 || ptr[GC_UP_RIGHT] < 0)
					std::cout<<"nlink cap <= 0"<<std::endl;
			}
		}

		L = (edge_lambda + lambda) * 8 + 2;
	}
}

void GrabCut::calc_tlink()
{
	static Mat data(CHANNEL_NUM, 1, VALUE_TYPE);

	time_t start_time = clock();

	tlink.create(img.size(), CV_32FC2);

	float min_t_cap = 1e10;
	float max_t_cap = -1.0f;

	for( int y = 0; y < img.rows; y++ )
	{
		for( int x = 0; x < img.cols; x++ )
		{
			char _segm = segment.at<char>(y, x);
			convert2mat(img.at<Vec3b>(y, x), data);

			float fore_cap = 0.0f;
			float bkg_cap = 0.0f;

			TrimapValue map_value = (TrimapValue)trimap.at<char>(y, x);
			if (map_value == TrimapUnknown)
			{
				fore_cap = -log(std::numeric_limits<float>::epsilon() + image_gmm->probability(data, SegmentationBackground));
				bkg_cap = -log(std::numeric_limits<float>::epsilon() + image_gmm->probability(data, SegmentationForeground));

				//fore_cap = std::numeric_limits<float>::epsilon() + image_gmm->probability(data, SegmentationBackground);
				//bkg_cap = std::numeric_limits<float>::epsilon() + image_gmm->probability(data, SegmentationForeground);

				if (min_t_cap > fore_cap) min_t_cap = fore_cap;
				if (min_t_cap > bkg_cap) min_t_cap = bkg_cap;
				if (max_t_cap < fore_cap) max_t_cap = fore_cap;
				if (max_t_cap < bkg_cap) max_t_cap = bkg_cap;
			}
			else if (map_value == TrimapBackground)
			{
				fore_cap = 0;
				bkg_cap = L;
			}
			else if (map_value == TrimapForeground)
			{
				fore_cap = L;
				bkg_cap = 0;
			}


			float *ptr = tlink.ptr<float>(y, x);
			ptr[SegmentationForeground] = fore_cap;
			ptr[SegmentationBackground] = bkg_cap;
		}
	}

	std::cout<<"min_t_cap = "<<min_t_cap<<'\t'<<"max_t_cap = "<<max_t_cap<<endl;
	time_t end_time = clock();

	cout<<"---------------------------"<<img.rows * img.cols <<"pixels call "<<"calc_tlink() use "<<end_time - start_time<<endl;
}

void GrabCut::init(int min_x, int max_x, int min_y, int max_y)
{
	trimap.setTo(Scalar(TrimapBackground));
	segment.setTo(Scalar(SegmentationBackground));

	for (int y = min_y; y <= max_y; y++)
	{
		for (int x = min_x; x <= max_x; x++)
		{
			trimap.at<char>(y, x) = TrimapUnknown;
			segment.at<char>(y, x) = SegmentationForeground;
		}
	}

	calc_beta();
	calc_nlink(false);
	build_gmm();
}

void GrabCut::set_hard_segment(std::vector<cv::Point> &points, TrimapValue map_value)
{
	for (int ii = 0; ii < points.size(); ii++)
	{
		trimap.at<char>(points[ii]) = map_value;
		for(int dx = -5; dx < 5; dx++)
		{
			for (int dy = -5; dy < 5; dy++)
			{
				int y = points[ii].y + dy;
				if (y < 0) y = 0;
				if (y >= segment.rows) y = segment.rows - 1;

				int x = points[ii].x + dx;
				if (x < 0) x = 0;
				if (x >= segment.cols) x = segment.rows - 1;

				if (map_value == TrimapBackground) segment.at<char>(y, x) = SegmentationBackground;
				else if (map_value == TrimapForeground) segment.at<char>(y, x) = SegmentationForeground;
			}
		}
	}
}

void GrabCut::build_gmm()
{
	if (!image_gmm) image_gmm = new ImageGMM(5);

	image_gmm->build(img, component, segment);
}

void GrabCut::run()
{
	int itera_num = MAX_RUN_TIMES;
	int changed_pix_num = 1;
	while (itera_num && changed_pix_num)
	{
		changed_pix_num = run_once();
		itera_num--;
	}

	std::cout<<"GrabCut has run "<<MAX_RUN_TIMES - itera_num<<" times"<<std::endl;
}

int GrabCut::run_once()
{
	std::cout<<"start run_once ..."<<std::endl;
	image_gmm->learn(img, component, segment);

	calc_tlink();

	if (graph)
	{
		delete graph;
		graph = NULL;
	}

	clock_t start_time = clock();

	graph = new MyGraph(img.rows * img.cols, 8 * img.rows * img.cols, ErrorFunc);
	graph->add_node(img.rows * img.cols);

	for( int y = 0; y < tlink.rows; y++ )
	{
		for( int x = 0; x < tlink.cols; x++ )
		{
			float fore_cap = tlink.ptr<float>(y, x)[SegmentationForeground];
			float bkg_cap = tlink.ptr<float>(y, x)[SegmentationBackground];

			int node_idx = y * tlink.cols + x;

			float left_cap = nlink.ptr<float>(y, x)[GC_LEFT];
			float up_left_cap = nlink.ptr<float>(y, x)[GC_UP_LEFT];
			float up_cap = nlink.ptr<float>(y, x)[GC_UP];
			float up_right_cap = nlink.ptr<float>(y, x)[GC_UP_RIGHT];

			graph->add_tweights(y * tlink.cols + x, fore_cap, bkg_cap);

			if(x > 0)
				graph->add_edge(node_idx, node_idx - 1, left_cap, left_cap);

			if (x > 0 && y >0)
				graph->add_edge(node_idx, node_idx - tlink.cols - 1, up_left_cap, up_left_cap);

			if (y > 0)
				graph->add_edge(node_idx, node_idx - tlink.cols, up_cap, up_cap);

			if (y > 0 && x < tlink.cols - 1)
				graph->add_edge(node_idx, node_idx - tlink.cols + 1, up_right_cap, up_right_cap);

		}
	}

	float max_flow = graph->maxflow();

	cv::Mat save_img = img.clone();
	save_img.setTo(Scalar::all(0));

	int changed_pix = 0;
	for( int y = 0; y < img.rows; y++ )
	{
		for( int x = 0; x < img.cols; x++ )
		{
			MyGraph::termtype type = graph->what_segment(y * img.cols + x, MyGraph::SINK);
			char map_value = trimap.at<char>(y, x);

			if (map_value == TrimapUnknown && type != segment.at<char>(y, x)) changed_pix++;

			if (type == MyGraph::SOURCE)
			{
				segment.at<char>(y, x) = SegmentationForeground;
				save_img.at<Vec3b>(y, x) = img.at<Vec3b>(y, x);
			}
			else
				segment.at<char>(y, x) = SegmentationBackground;
		}
	}

	clock_t end_time = clock();
	cout<<"-----------------------maxflow() once use "<<end_time - start_time<<"ms\n";

	// 	namedWindow("grabcut");
	// 	imshow("grabcut", save_img);
	std::cout<<"end run_once ..."<<std::endl;
	//	waitKey(100);

	std::cout<<changed_pix<<" pixels has changed"<<std::endl;

	return changed_pix;
}