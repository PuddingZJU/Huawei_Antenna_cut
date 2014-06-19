#include "GMM.h"
#include "Macros.h"
#include "Log.h"

using namespace cv;
using namespace std;

#define PI 3.1415926
#define MAX_EM_ITERATION_NUM 5

void convert2mat(const Vec3b &color, cv::Mat &data)
{
	data.create(CHANNEL_NUM, 1, VALUE_TYPE);
	for (int ii = 0; ii < CHANNEL_NUM; ii++)
	{
		data.at<float>(ii, 0) = color[ii]/255.0f;
	}
}

Gaussian::Gaussian():	mean(CHANNEL_NUM, 1, VALUE_TYPE), covariance(CHANNEL_NUM, CHANNEL_NUM, VALUE_TYPE), inverse(CHANNEL_NUM, CHANNEL_NUM, VALUE_TYPE), eigen_values(1, CHANNEL_NUM, CV_32FC1), eigen_vectors(CHANNEL_NUM, CHANNEL_NUM, CV_32FC1)
{
	determinant = weight = 0.0f;	
}

GaussianModify::GaussianModify():	sum(CHANNEL_NUM, 1, CV_32FC1, Scalar::all(0)), product(CHANNEL_NUM, CHANNEL_NUM, CV_32FC1, Scalar::all(0)), count(0)
{
}

void GaussianModify::add(cv::Mat &color)
{
	mats.push_back(color);
	count++;
}

void GaussianModify::modify_gaussian(Gaussian &gaussian, int total_num)
{
	Mat covar;
	Mat mean;

	std::cout<<"modify_gaussian\t"<<mats.size()<<endl;

	if (mats.size() == 0) return;

	cv::calcCovarMatrix(&mats[0], mats.size(), covar, mean, CV_COVAR_NORMAL |CV_COVAR_SCALE| CV_COVAR_ROWS, CV_32F);

	gaussian.mean = mean;
	gaussian.covariance = covar;
	gaussian.inverse = gaussian.covariance.inv(DECOMP_SVD);
	gaussian.determinant = (float)cv::determinant(gaussian.covariance);
	if (total_num != 0)gaussian.weight = (float)count/total_num;
	else gaussian.weight = 0.0f;

#ifdef _DEBUG
	std::cout<<"mean vector as follow : "<<std::endl<<gaussian.mean<<std::endl<<mean<<std::endl;
	std::cout<<"covariance as follow : "<<std::endl<<gaussian.covariance<<std::endl<<covar<<std::endl;
#endif

	LOG_MAT(mean);
	LOG_MAT(covar);
	LOG_MAT(gaussian.inverse);

	SVD::compute(gaussian.covariance, gaussian.eigen_values, gaussian.eigen_vectors, gaussian.eigen_vectors);
}

GMM::GMM(int K)
{
	this->K = K;
	gaussian = new Gaussian[K];
}

GMM::~GMM()
{
	if (gaussian) delete []gaussian;
}

float GMM::probability(const cv::Mat &color)
{
	float sum = 0.0f;
	for (int ii = 0; ii < K; ii++)
	{
		sum += gaussian[ii].weight * probability(color, ii);
	}

	return sum;
}

float GMM::probability(const cv::Mat &color, int gaussian_idx)
{
	clock_t start_time = clock();

	if (!gaussian || gaussian[gaussian_idx].weight <= FLT_MIN) return 0.0f;

	float diff[CHANNEL_NUM];
	for (int ii = 0; ii < CHANNEL_NUM; ii++)
	{
		diff[ii] = color.at<float>(ii, 0) - gaussian[gaussian_idx].mean.at<float>(ii, 0);
	}

	const cv::Mat &inverse = gaussian[gaussian_idx].inverse;
	float tf = (diff[0]*inverse.at<float>(0, 0) + diff[1]*inverse.at<float>(1, 0) + diff[2]*inverse.at<float>(2, 0))*diff[0] + 
		(diff[0]*inverse.at<float>(0, 1) + diff[1]*inverse.at<float>(1, 1) + diff[2]*inverse.at<float>(2, 1))*diff[1] + 
		(diff[0]*inverse.at<float>(0, 2) + diff[1]*inverse.at<float>(1, 2) + diff[2]*inverse.at<float>(2, 2))*diff[2];

	{
		 	//Mat diff = (color - gaussian[gaussian_idx].mean);
		 	//Mat trans = diff.t();
		 	//Mat tmp = trans * gaussian[gaussian_idx].inverse;
		 	//Mat result = tmp * diff;
		 
		 	//float tf = result.at<float>(0, 0);
	}

	float numerator = exp(-0.5 * tf);
	float denominator = sqrt(pow(2.0f * PI, CHANNEL_NUM) * gaussian[gaussian_idx].determinant);
	//float denominator = gaussian[gaussian_idx].determinant;

	clock_t end_time = clock();

	//cout<<"float GMM::probability(const cv::Mat &color, int gaussian_idx) once use "<<(end_time - start_time)<<"ms"<<endl;

	if (denominator < std::numeric_limits<float>::epsilon()) return 0.0f;

	float retVal = numerator / denominator;
	return retVal;
}

int GMM::which_component(const Mat &color)
{
	float max_prob = -1;
	int max_idx = -1;
	for (int ii = 0; ii < K; ii++)
	{
		float _prob = probability(color, ii);
		if (_prob > max_prob)
		{
			max_prob = _prob;
			max_idx = ii;
		}
	}

	return max_idx;
}

ImageGMM::ImageGMM(int K):	fore_gmm(K), bkg_gmm(K)
{
	this->K = K;
	fg_modifies = new GaussianModify[K];
	bkg_modifies = new GaussianModify[K];
}

ImageGMM::~ImageGMM()
{
	if (fg_modifies) delete []fg_modifies;
	if (bkg_modifies) delete []bkg_modifies;
}

void ImageGMM::build(cv::Mat &img, cv::Mat &component, cv::Mat &segment)
{
	const int kMeansItCount = 10;
	const int kMeansType = KMEANS_PP_CENTERS;

	Mat bgdLabels, fgdLabels;
	static std::vector<Vec3f> bgdSamples, fgdSamples;

	static std::map<int, int> fgd_idx_map;
	static std::map<int, int> bgd_idx_map;

	///清空samples
	bgdSamples.clear();
	fgdSamples.clear();

	for( int y = 0; y < img.rows; y++ )
	{
		for(int x = 0; x < img.cols; x++ )
		{
			if( segment.at<char>(y, x) == SegmentationBackground)
			{
				bgdSamples.push_back( (Vec3f)img.at<Vec3b>(y, x) );
				bgd_idx_map[y * img.cols + x] = bgdSamples.size() - 1;
			}
			else
			{
				fgdSamples.push_back( (Vec3f)img.at<Vec3b>(y, x) );
				fgd_idx_map[y * img.cols + x] = fgdSamples.size() - 1;
			}
		}
	}

	CV_Assert( !bgdSamples.empty() && !fgdSamples.empty() );

	Mat _bgdSamples( (int)bgdSamples.size(), 3, CV_32FC1, &bgdSamples[0][0] );
	kmeans( _bgdSamples, K, bgdLabels,	TermCriteria( CV_TERMCRIT_ITER, kMeansItCount, 0.0), 0, kMeansType );

	Mat _fgdSamples( (int)fgdSamples.size(), 3, CV_32FC1, &fgdSamples[0][0] );
	kmeans( _fgdSamples, K, fgdLabels, TermCriteria( CV_TERMCRIT_ITER, kMeansItCount, 0.0), 0, kMeansType );

	for( int y = 0; y < img.rows; y++ )
	{
		for(int x = 0; x < img.cols; x++ )
		{
			if( segment.at<char>(y, x) == SegmentationBackground)
			{
				int ii = bgd_idx_map[y * img.cols + x];
				component.at<char>(y, x) = bgdLabels.at<int>(ii, 0);
			}
			else
			{
				int ii = fgd_idx_map[y * img.cols + x];
				component.at<char>(y, x) = fgdLabels.at<int>(ii, 0);
			}
		}
	}

	LOG_MAT(component);

	fg_modifies->clear();
	bkg_modifies->clear();

	int fg_sum = 0;
	int bkg_sum = 0;

	for (int y = 0; y < component.rows; y++)
	{
		for (int x = 0; x < component.cols; x++)
		{
			char _segm = segment.at<char>(y, x);
			char _comp = component.at<char>(y, x);
			Vec3b rgb = img.at<Vec3b>(y, x);

			Mat tmat(CHANNEL_NUM, 1, CV_32FC1);
			tmat.at<float>(0, 0) = rgb[0]/255.0f;
			tmat.at<float>(1, 0) = rgb[1]/255.0f;
			tmat.at<float>(2, 0) = rgb[2]/255.0f;

			if (_segm == SegmentationForeground)
			{
				fg_sum++;
				fg_modifies[_comp].add(tmat);
			}
			else
			{
				bkg_sum++;
				bkg_modifies[_comp].add(tmat);
			}
		}
	}

	for (int ii = 0; ii < K; ii++)
	{
		fg_modifies[ii].modify_gaussian(fore_gmm.gaussian[ii], fg_sum);
		bkg_modifies[ii].modify_gaussian(bkg_gmm.gaussian[ii], bkg_sum);
	}

   	//EM(img, segment, fore_gmm, SegmentationForeground);
   	//EM(img, segment, bkg_gmm, SegmentationBackground);
}

void ImageGMM::EM(cv::Mat &img, cv::Mat &segment, GMM &gmm, SegmentationValue seg)
{
	int pix_sum = 0;

	int array_length = K * img.cols * img.rows;
	/// 排列为[[gmm0, gmm1, ..., gmmk], ..., [gmm0, gmm1, ..., gmmk]]，即每组为一系列gmm的值
	double *prior = new double[array_length];
	double *posterior = new double[array_length];
	double *rnk = new double[array_length];

	for (int ii = 0; ii < array_length; ii++)
	{
		prior[ii] = posterior[ii] = rnk[ii] = 0.0;
	}

	double old_max_prob = -1.0e20;
	double prob_sum = 0.0;

	double max_prob = old_max_prob;
	int iteration_num = 0;

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			if (segment.at<char>(y, x) != seg) continue;
			pix_sum++;
		}
	}

	std::cout<<"pix_sum is : "<<pix_sum<<endl;

	do 
	{
		iteration_num++;
		prob_sum = 0.0;
		old_max_prob = max_prob;

		for (int y = 0; y < img.rows; y++)
		{
			for (int x = 0; x < img.cols; x++)
			{
				if (segment.at<char>(y, x) != seg) continue;

				Vec3b rgb = img.at<Vec3b>(y, x);

				Mat data(CHANNEL_NUM, 1, CV_32FC1);
				for (int ii = 0; ii < 3; ii++)
				{
					data.at<float>(ii, 0) = rgb[ii]/255.0f;
				}

				int pix_idx = y * img.cols + x;

				double rni_sum = 0.0;

				for (int ii = 0; ii < K; ii++)
				{
					/// 先验概率
					Mat _mean = gmm.gaussian[ii].mean;
					Mat _variance = gmm.gaussian[ii].covariance;

					prior[pix_idx*K + ii] = gmm.probability(data, ii);

					/// 后验概率
					posterior[pix_idx*K + ii] = prior[pix_idx*K + ii] * gmm.gaussian[ii].weight;

					rni_sum += posterior[pix_idx*K + ii];
				}

				double _debug_rnk_sum = 0.0;
				for (int ii = 0; ii < K; ii++)
				{
					rnk[pix_idx*K + ii] = posterior[pix_idx*K + ii]/rni_sum;
					_debug_rnk_sum += rnk[pix_idx*K + ii] ;
				}
				assert(abs(_debug_rnk_sum - 1.0) <= 1e-5);
				//assert(abs(_debug_rnk_sum - 1.0) <= std::numeric_limits<double>::epsilon());
			}
		}

		double _debug_weight_sum = 0.0;
		for (int ii = 0; ii < K; ii++)
		{
			Mat _sum(CHANNEL_NUM, 1, CV_32FC1, Scalar::all(0));
			double _weight_sum = 0.0;

			for (int y = 0; y < img.rows; y++)
			{
				for (int x = 0; x < img.cols; x++)
				{
					if (segment.at<char>(y, x) != seg) continue;

					Vec3b rgb = img.at<Vec3b>(y, x);

					Mat data(CHANNEL_NUM, 1, CV_32FC1);
					for (int jj = 0; jj < 3; jj++)
					{
						data.at<float>(jj, 0) = rgb[jj]/255.0f;
					}

					int pix_idx = y * img.cols + x;

					_weight_sum += rnk[pix_idx*K + ii];
					_sum += data * rnk[pix_idx * K + ii];
				}
			}

			/// 更新均值
			if (_weight_sum < std::numeric_limits<float>::epsilon()) gmm.gaussian[ii].mean.setTo(0.0f);
			else gmm.gaussian[ii].mean = _sum/_weight_sum;
			LOG_MAT(gmm.gaussian[ii].mean);

			Mat _covar(CHANNEL_NUM, CHANNEL_NUM, CV_32FC1, Scalar::all(0));

			for (int y = 0; y < img.rows; y++)
			{
				for (int x = 0; x < img.cols; x++)
				{
					if (segment.at<char>(y, x) != seg) continue;

					Vec3b rgb = img.at<Vec3b>(y, x);

					Mat data(CHANNEL_NUM, 1, CV_32FC1);
					for (int jj = 0; jj < 3; jj++)
					{
						data.at<float>(jj, 0) = rgb[jj]/255.0f;
					}

					int pix_idx = y * img.cols + x;

					_covar += (data - gmm.gaussian[ii].mean) * (data - gmm.gaussian[ii].mean).t() * rnk[pix_idx * K + ii];
				}
			}

			gmm.gaussian[ii].covariance = _covar/_weight_sum;
			gmm.gaussian[ii].weight = _weight_sum/(pix_sum);
			LOG_MAT(gmm.gaussian[ii].covariance);

			_debug_weight_sum += _weight_sum;
		}

		max_prob = 0.0;
		for (int y = 0; y < img.rows; y++)
		{
			for (int x = 0; x < img.cols; x++)
			{
				if (segment.at<char>(y, x) != seg) continue;

				Vec3b rgb = img.at<Vec3b>(y, x);

				Mat data(CHANNEL_NUM, 1, CV_32FC1);
				for (int jj = 0; jj < 3; jj++)
				{
					data.at<float>(jj, 0) = rgb[jj]/255.0f;
				}

				int pix_idx = y * img.cols + x;

				double tmp_prob = gmm.probability(data);
				prob_sum += tmp_prob;

				max_prob += log10(tmp_prob);
			}
		}
			
		std::cout<<max_prob<<endl;
	}while((max_prob - old_max_prob) > MIN_STOP_PROB_DIFF && iteration_num < MAX_EM_ITERATION_NUM);

	std::cout<<"iteration number is : "<<iteration_num<<std::endl;

	delete [] prior;
	delete [] posterior;
	delete [] rnk;
}

void ImageGMM::learn(cv::Mat &img, cv::Mat &component, cv::Mat &segment)
{
	int changed_num = 0;

	for (int y = 0; y < segment.rows; y++)
	{
		for (int x = 0; x < segment.cols; x++)
		{
			char _segm = segment.at<char>(y, x);
			Vec3b rgb = img.at<Vec3b>(y, x);

			Mat tmat(CHANNEL_NUM, 1, CV_32FC1);
			for (int ii = 0; ii < 3; ii++)
			{
				tmat.at<float>(ii, 0) = rgb[ii]/255.0f;
			}

			if (_segm == SegmentationForeground)
			{	
				char fg_comp = -1;
				float max_prob = -1.0f;
				for (int ii = 0; ii < this->K; ii++)
				{
					float _prob = fore_gmm.probability(tmat, ii);
					if (_prob > max_prob)
					{
						max_prob = _prob;
						fg_comp = ii;
					}
				}

				if (component.at<char>(y, x) != fg_comp) changed_num++;
				component.at<char>(y, x) = fg_comp;
			}
			else if (_segm == SegmentationBackground)
			{
				char bkg_comp = -1;	
				float max_prob = -1.0f;
				for (int ii = 0; ii < this->K; ii++)
				{
					float _prob = bkg_gmm.probability(tmat, ii);
					if (_prob > max_prob)
					{
						max_prob = _prob;
						bkg_comp = ii;
					}
				}

				if (component.at<char>(y, x) != bkg_comp) changed_num++;
				component.at<char>(y, x) = bkg_comp;
			}
		}
	}

	std::cout<<"gmm learn "<<changed_num<<" pixel's component has changed"<<std::endl;

	GaussianModify *fg_modifies = new GaussianModify[K];
	GaussianModify *bkg_modifies = new GaussianModify[K];

	int fg_sum = 0;
	int bkg_sum = 0;

	for (int y = 0; y < component.rows; y++)
	{
		for (int x = 0; x < component.cols; x++)
		{
			char _segm = segment.at<char>(y, x);
			char _comp = component.at<char>(y, x);
			Vec3b rgb = img.at<Vec3b>(y, x);

			Mat tmat(CHANNEL_NUM, 1, CV_32FC1);
			tmat.at<float>(0, 0) = rgb[0]/255.0f;
			tmat.at<float>(1, 0) = rgb[1]/255.0f;
			tmat.at<float>(2, 0) = rgb[2]/255.0f;

			if (_segm == SegmentationForeground)
			{
				fg_sum++;
				fg_modifies[_comp].add(tmat);
			}
			else
			{
				bkg_sum++;
				bkg_modifies[_comp].add(tmat);
			}
		}
	}

	for (int ii = 0; ii < K; ii++)
	{
		fg_modifies[ii].modify_gaussian(fore_gmm.gaussian[ii], fg_sum);
		bkg_modifies[ii].modify_gaussian(bkg_gmm.gaussian[ii], bkg_sum);
	}

	delete []fg_modifies;
	delete []bkg_modifies;
}