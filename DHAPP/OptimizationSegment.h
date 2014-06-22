#pragma once
#include <QObject>
#include <QImage>
#include <QtGui>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QDebug>
#include <QMap>

#include "cv.h"
#include "ImageWinEventInterface.h"
#include "OptionPageManager.h"
#include "SegmentationOptionPage.h"
#include <vector>
#include "SegCommand.h"
#include <nlopt.hpp>
#include <cmath>


typedef struct{
	double x,y,angle;
}Line;




