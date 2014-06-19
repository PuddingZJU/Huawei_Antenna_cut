#include "ElementPick.h"

#include "ImageWin.h"
#include "Segmentation/CommonFunc.h"
#include "MainWindow.h"

#include <QPoint>

ElementPick::ElementPick(void)
{
	ImageWin::RegisterEventReceiver(this);
}

ElementPick::~ElementPick(void)
{
}

void ElementPick::setMask(ImageWin* imgwin, IplImage* img)
{
	std::map<ImageWin*, IplImage*>::iterator it = m_maskimg.find(imgwin);
	if (it != m_maskimg.end())
	{
		cvReleaseImage(&(it->second));
	}
	m_maskimg[imgwin] = img;
	m_selectedindex[imgwin] = -1;
	m_hoveredindex[imgwin] = -1;
}

void ElementPick::setContours(ImageWin* win, CvSeq* contours)
{
	m_contours[win].clear();
	for (CvSeq* contour = contours; contour!= 0; contour= contour->h_next)
	{
		m_contours[win].push_back(contour);
	}
}

bool ElementPick::getSelectedElement(QImage& outimage, QImage& maskimg)
{
	ImageWin* activewin = MainWindow::Instance().GetActiveSubWindow();
	if (activewin)
	{
		std::map<ImageWin*, int>::iterator it = m_selectedindex.find(activewin); 
		if (it != m_selectedindex.end())
		{
			int index = it->second;
			if (index >= 0)
			{
				CvSeq* contour = m_contours[activewin][index];

				cv::Mat mat;
				cv::Rect r0= cvBoundingRect(contour);
				mat.create(r0.height, r0.width, CV_8UC3);

				IplImage matimg = mat;
				cvZero(&matimg);

				cvDrawContours(
					&matimg,
					contour,
					cvScalarAll(255),
					cvScalarAll(255),
					0, CV_FILLED, 8, cvPoint(-r0.x, -r0.y));

				Mat2QImage(mat, maskimg);

				outimage = activewin->getImage().copy(r0.x, r0.y, r0.width, r0.height);

				return true;
			}
		}
	}
	return false;
}

bool ElementPick::procPaint(ImageWin* imgwin, QPaintEvent *event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != ELEMENTPICKMODE)
		return false;

	if (m_selectedindex[imgwin] < 0 && m_hoveredindex[imgwin] < 0)
		return false;

	std::map<ImageWin*, IplImage*>::iterator it = m_maskimg.find(imgwin);
	if (it == m_maskimg.end())
		return false;
	
	cv::Mat mat;
	IplImage* maskimg = it->second;
	mat.create(maskimg->height, maskimg->width, CV_8UC4);

	IplImage matimg = mat;
	cvZero(&matimg);

	if (m_hoveredindex[imgwin] >= 0)
	{
		CvSeq* contour = m_contours[imgwin][m_hoveredindex[imgwin]];
		cvDrawContours(
			&matimg,
			contour,
			cvScalar(255, 255, 255, 128),
			cvScalar(255, 255, 255, 128),
			0, CV_FILLED);
	}
	if (m_selectedindex[imgwin] >= 0)
	{
		CvSeq* contour = m_contours[imgwin][m_selectedindex[imgwin]];
		cvDrawContours(
			&matimg,
			contour,
			cvScalar(255, 255, 255, 128),
			cvScalar(255, 255, 255, 128),
			0, CV_FILLED);
		cvDrawContours(
			&matimg,
			contour,
			cvScalar(0, 0, 255, 255),
			cvScalar(0, 0, 255, 255),
			0, 2);
	}

	QImage highlightimg;
	Mat2QImage(mat, highlightimg);

	QPainter painter(imgwin);
	painter.drawImage(QRect(QPoint(0,0), imgwin->getscaleFactor() * highlightimg.size()), highlightimg);
	return false;
}


bool ElementPick::procMousePress(ImageWin* imgwin, QMouseEvent* event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != ELEMENTPICKMODE)
		return false;

	std::map<ImageWin*, IplImage*>::iterator it = m_maskimg.find(imgwin);
	if (it == m_maskimg.end())
		return false;

	IplImage* maskimg = it->second;
	QPoint point = event->pos();
	double scale = imgwin->getscaleFactor();
	CvScalar s = cvGet2D(maskimg, point.y() / scale, point.x() / scale);
	int index = s.val[0] - 128;
	if (index < 0 || index >= m_contours[imgwin].size())
		m_selectedindex[imgwin] = -1;
	else
		m_selectedindex[imgwin] = index;

	imgwin->update();
	return false;
}

bool ElementPick::procMouseMove(ImageWin* imgwin, QMouseEvent* event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != ELEMENTPICKMODE)
		return false;

	std::map<ImageWin*, IplImage*>::iterator it = m_maskimg.find(imgwin);
	if (it == m_maskimg.end())
		return false;

	IplImage* maskimg = it->second;
	QPoint point = event->pos();
	double scale = imgwin->getscaleFactor();
	CvScalar s = cvGet2D(maskimg, point.y() / scale, point.x() / scale);
	int index = s.val[0] - 128;
	int newindex;
	if (index < 0 || index >= m_contours[imgwin].size())
		newindex = -1;
	else
		newindex = index;

	if (newindex == m_hoveredindex[imgwin])	
		return false;

	imgwin->update();
	m_hoveredindex[imgwin] = newindex;
	return false;
}