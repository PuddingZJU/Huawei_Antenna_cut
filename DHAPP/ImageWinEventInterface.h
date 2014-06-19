#ifndef __ES_DHAPP_IMAGEWINEVENTINTERFACE_H
#define __ES_DHAPP_IMAGEWINEVENTINTERFACE_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>

class ImageWin;
class ImageWinEventInterface
{
protected:
	ImageWinEventInterface(void){}

public:
	virtual ~ImageWinEventInterface(void){}

	virtual bool procPaint(ImageWin* imgwin, QPaintEvent *event){return false;}

	virtual bool procMouseMove(ImageWin* imgwin, QMouseEvent* event){return false;}
	virtual bool procMousePress(ImageWin* imgwin, QMouseEvent* event){return false;}
	virtual bool procMouseRelease(ImageWin* imgwin, QMouseEvent* event){return false;}
	virtual bool procWheel(ImageWin* imgwin, QWheelEvent* event){return false;}
};

#endif