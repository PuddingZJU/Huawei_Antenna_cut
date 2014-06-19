#include "ImageWin.h"
#include "MainWindow.h"
#include "SegmentationParamPanel.h"

EventVec ImageWin::_eventvec;

ImageWin::ImageWin(QWidget *parent)
: QLabel(parent),
mousepressed(Qt::NoButton)
{
	resize(400, 300);
	init_Param();

	//图像组件
	this->setBackgroundRole(QPalette::Base);
	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	this->setScaledContents(true);

	this->setMouseTracking(true);
}

void ImageWin::init_Param()
{
	scaleFactor = 1.0;
}

ImageWin::~ImageWin()
{

}

void ImageWin::SetImage(QImage* img)
{
	if(img == NULL) return;
	if(img->isNull()) return;

	setPixmap(QPixmap::fromImage(*img));
	QSize _image_size = img->size();
	resize(scaleFactor * _image_size);

#ifdef _DEBUG
		const QPixmap * pixmap = this->pixmap();
		QSize pixmap_size = pixmap->size();
#endif
}

void ImageWin::zoomIn()
{
	scaleImage(1.25);
}

void ImageWin::zoomOut()
{
	scaleImage(0.8);
}

void ImageWin::viewInActualPix()
{
	scaleImage(1.0/scaleFactor);
}

void ImageWin::viewInScreen()
{
	//TODO:	
}

void ImageWin::scaleImage(double factor)
{
	Q_ASSERT(this->pixmap());
	scaleFactor *= factor;
	this->resize(scaleFactor * this->pixmap()->size());

	selectionstart *= factor;
	selectionend *= factor;
}

void ImageWin::RegisterEventReceiver(ImageWinEventInterface* recv)
{
	assert(recv);
	_eventvec.push_back(recv);
}

void ImageWin::mousePressEvent(QMouseEvent * event )
{
	mousepressed = event->button();

	QLabel::mousePressEvent(event);

	for (EventVec::const_iterator it = _eventvec.begin(); it != _eventvec.end(); it++)
	{
		if ((*it)->procMousePress(this, event))
			return;
	}

	if (MainWindow::Instance().GetOperationModeManager()->mode() == SELECTMODE)
	{
		if(event->button() == Qt::LeftButton)
		{
			selectionstart = event->pos();
		}		
	}
}
void ImageWin::mouseMoveEvent(QMouseEvent * event)
{
	QLabel::mouseMoveEvent(event);
	
	for (EventVec::const_iterator it = _eventvec.begin(); it != _eventvec.end(); it++)
	{
		if ((*it)->procMouseMove(this, event))
			return;
	}

	if (MainWindow::Instance().GetOperationModeManager()->mode() == SELECTMODE)
	{
		if(mousepressed == Qt::LeftButton)
		{
			selectionend = event->pos();
			update();
		}		
	}
}

void ImageWin::mouseReleaseEvent(QMouseEvent *event)
{
	mousepressed = Qt::NoButton;

	QLabel::mouseReleaseEvent(event);

	for (EventVec::const_iterator it = _eventvec.begin(); it != _eventvec.end(); it++)
	{
		if ((*it)->procMouseRelease(this, event))
			return;
	}

	if (MainWindow::Instance().GetOperationModeManager()->mode() == SELECTMODE)
	{
		if(event->button() == Qt::LeftButton)
		{
			selectionend = event->pos();
			update();
		}		
	}
}

void ImageWin::wheelEvent(QWheelEvent *event)
{
	QLabel::wheelEvent(event);

	for (EventVec::const_iterator it = _eventvec.begin(); it != _eventvec.end(); it++)
	{
		if ((*it)->procWheel(this, event))
			return;
	}

	QPoint cursor_pos = QCursor::pos();
	QPoint local_original_pos = this->mapFromGlobal(cursor_pos);

	int delta = event->delta();
	double _delta = 1.0 + delta/1000.0;
	scaleImage(_delta);
}

void ImageWin::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);

	for (EventVec::const_iterator it = _eventvec.begin(); it != _eventvec.end(); it++)
	{
		if ((*it)->procPaint(this, event))
			return;
	}

	if (MainWindow::Instance().GetOperationModeManager()->mode() == SELECTMODE)
	{
		if (!selectionstart.isNull() && !selectionend.isNull())
		{
			QRect rect(selectionstart, selectionend);
			rect = rect.normalized();
			if (rect.width() && rect.height())
			{
				QPainter painter(this);
				QPen pen(Qt::DashLine);
				pen.setWidth(1);
				pen.setColor(Qt::white);
				painter.setPen(pen);
				painter.drawRect(rect);
			}
		}
	}
}

void ImageWin::setPath(QString _img_filePath)
{
	this->img_filePath = _img_filePath;
}

double ImageWin::getscaleFactor() const
{
	return this->scaleFactor;
}

QImage  ImageWin::getImage() const
{
	return this->pixmap()->toImage();
}

QImage ImageWin::getSelectionImage() const
{
	QRect rect(selectionstart / scaleFactor, selectionend / scaleFactor);
	//QRect rect(selectionstart, selectionend);
	rect = rect.normalized();
	if (rect.width() && rect.height())
	{
		return pixmap()->copy(rect).toImage();		
	}	
	return pixmap()->toImage();
}

QString ImageWin::getPath() const
{
	return this->img_filePath;
}