#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include <QColor>
#include <QPoint>
#include <QImage>


#define MASK_BACKGROUND 0
#define MASK_FOREGROUND 1
#define MASK_UNKNOW 2

#define BACKGROUND_COLOR 0xFF000000
#define FOREGROUND_COLOR  0xFF000001
#define UNDEFINED_COLOR		0xFF000002

#define FLOAT_ZERO	1.0e-10
#define MAX_INTENSITY 256


typedef double PenaltyType;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

void ErrorFunc(char *error_msg);
void QImage2Mat(const QImage &img, cv::Mat &img_mat);
void Mat2QImage(const cv::Mat &img_mat, QImage &img);

template<typename T>
void CalcIntensity(QImage &_image, T *_image_intensity)
{
	int _image_width = _image.width();
	int _image_height = _image.height();
	int _image_size = _image_width * _image_height;

	for (int ii = 0; ii < _image_size; ii++) _image_intensity[ii] = 0;

	for (int y = 0; y < _image_height; y++)
	{
		T *ptr = NULL;
		for (int x = 0; x < _image_width; x++)
		{
			ptr = _image_intensity + y * _image_width + x;
			QRgb pix = _image.pixel(x, y);
			int r, g, b;
			r = 0xFF&(pix>>16);
			g = 0xFF&(pix>>8);
			b = 0xFF&pix;
			*ptr = T(0.11*r + 0.59*g + 0.3*b);
		}
	}
}

template<typename T>
QImage IntensityToImage(const T *intensity, int width, int height, T max_intensity, T min_intensity)
{
	QImage image(width, height, QImage::Format_RGB32);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int gray = (intensity[y * width + x] - min_intensity)/(double)(max_intensity - min_intensity) * 0xFF;
			QRgb gray_pix = qRgb(gray, gray, gray);
			image.setPixel(x, y, gray_pix);
		}
	}

	return image;
}

void CalcMaskAndHist(QImage &mask_image, uchar *_mask, int *_image_intensity, int *obj_hist, int &obj_seed_sum, int *bkg_hist, int &bkg_seed_sum);
void CalcVariance(int *_image_intensity, int _image_width, int _image_height, double &_var);

#endif