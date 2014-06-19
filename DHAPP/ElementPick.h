#pragma once

#include "ImageWinEventInterface.h"

#include <map>
#include <vector>
#include "cv.h"

#include <QObject>

class ImageWin;
class ElementPick
	:public ImageWinEventInterface
{
public:
	ElementPick(void);
	~ElementPick(void);

	void setMask(ImageWin* imgwin, IplImage* img);
	void setContours(ImageWin* imgwin, CvSeq* contours);

	bool getSelectedElement(QImage& outimage, QImage& maskimg);

	virtual bool procPaint(ImageWin* imgwin, QPaintEvent *event);
	virtual bool procMousePress(ImageWin* imgwin, QMouseEvent* event);
	virtual bool procMouseMove(ImageWin* imgwin, QMouseEvent* event);

private:
	std::map<ImageWin*, int> m_selectedindex;
	std::map<ImageWin*, int> m_hoveredindex;
	std::map<ImageWin*, IplImage*> m_maskimg;
	std::map<ImageWin*, std::vector<CvSeq*> > m_contours;
};
