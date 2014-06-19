#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QtGui>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include "cv.h"

class MainWindow;
class ImageWinEventInterface;
class InpaintingProcessor;

typedef std::vector<ImageWinEventInterface*> EventVec;

class ImageWin : public QLabel
{
	Q_OBJECT

	friend class InpaintingProcessor;

public:
	ImageWin(QWidget *parent = 0);
	~ImageWin();

public:
	void zoomIn();
	void zoomOut();
	void viewInActualPix();
	void viewInScreen();
	void scaleImage(double factor);

	void SetImage(QImage* img);
	void setPath(QString _img_filePath);
	double getscaleFactor() const;
	QImage getImage() const;
	QImage getSelectionImage() const;
	QString getPath() const;

	Qt::MouseButton getPressedMouseButton(){return mousepressed;}

	static void RegisterEventReceiver(ImageWinEventInterface* recv);

protected:
	virtual void mouseMoveEvent ( QMouseEvent* event );
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	void init_Param();
	
	Qt::MouseButton mousepressed;
	QPoint selectionstart;
	QPoint selectionend;

	double initial_scale;
	double scaleFactor;

	QString img_filePath;//图片路径

	static EventVec _eventvec;
};

#endif // IMAGEWIDGET_H
