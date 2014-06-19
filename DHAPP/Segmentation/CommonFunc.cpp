#include <QDebug>

#include <math.h>
#include "cv.h"
#include "highgui.h"

#include "CommonFunc.h"

void ErrorFunc(char* error_msg)
{
	qDebug()<<error_msg;
}

void QImage2Mat(const QImage &img, cv::Mat &img_mat)
{
	int height = img.height();
	int width = img.width();
	
	img_mat.create(height, width, CV_8UC3);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			QRgb _value = img.pixel(x, y);
			uchar *ptr = img_mat.ptr<uchar>(y, x);
			ptr[0] = _value&0xFF;
			ptr[1] = (_value&0xFF00)>>8;
			ptr[2] = (_value&0xFF0000)>>16;
		}
	}
}

void Mat2QImage(const cv::Mat &img_mat, QImage &img)
{
	int height = img_mat.rows;
	int width = img_mat.cols;

	img = QImage(width, height, QImage::Format_ARGB32);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const uchar *ptr = img_mat.ptr<uchar>(y, x);
			if (img_mat.type() == CV_8UC1 || img_mat.type() == CV_8SC1)
			{
				QRgb _value = 0xFF000000|ptr[0];
				img.setPixel(x, y, _value);
			}
			else if (img_mat.type() == CV_8UC3 || img_mat.type() == CV_8SC3)
			{
				QRgb _value = 0xFF000000|(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16));
				img.setPixel(x, y, _value);
			}
			else if (img_mat.type() == CV_8UC4 || img_mat.type() == CV_8SC4)
			{
				QRgb _value = ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24);
				img.setPixel(x, y, _value);
			}
			else
			{
				CV_Error(CV_StsError, "invalid mat type");
			}
		}
	}
}

void CalcMaskAndHist(QImage &mask_image, uchar *_mask, int *_image_intensity, int *obj_hist, int &obj_seed_sum, int *bkg_hist, int &bkg_seed_sum)
{
	int _image_width = mask_image.width();
	int _image_height = mask_image.height();
	int _image_size = _image_width * _image_height;

	obj_seed_sum = bkg_seed_sum = 0;
	memset(obj_hist, 0, sizeof(int) * MAX_INTENSITY);
	memset(bkg_hist, 0, sizeof(int) * MAX_INTENSITY);

	memset(_mask, 2, sizeof(uchar)*_image_size);

	for (int y = 0; y < _image_height; y++)
	{
		for (int x = 0; x < _image_width; x++)
		{
			uchar *ptr = _mask + y * _image_width + x;
			QRgb pix = mask_image.pixel(x, y);
			if (pix == BACKGROUND_COLOR)
			{
				(*ptr) = 0;
				bkg_hist[_image_intensity[y * _image_width + x]]++;
				bkg_seed_sum++;
			}
			else if (pix == FOREGROUND_COLOR) 
			{
				(*ptr) = 1;
				obj_hist[_image_intensity[y * _image_width + x]]++;
				obj_seed_sum++;
			}
		}
	}
}


void CalcVariance(int *_image_intensity, int _image_width, int _image_height, double &_var)
{
	int _image_size = _image_height * _image_width;

	double _sum = 0.0;
	for (int ii = 0; ii < _image_width * _image_height; ii++)
	{
		_sum += _image_intensity[ii];
	}
	double _average = _sum/_image_size;

	/// 计算方差
	_sum = 0.0;
	for(int ii = 0; ii < _image_width * _image_height; ii++)
	{
		_sum += (_image_intensity[ii] - _average) * (_image_intensity[ii] - _average);
	}
	_var = 1.0/(_image_size - 1) * _sum;
}
